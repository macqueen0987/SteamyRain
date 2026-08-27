from __future__ import annotations

import os
import re
from pathlib import Path
from typing import Callable

_SKIP_FOLDER_MARKERS = ("runtime", "redistributable")
_IMAGE_EXTENSIONS = {".png", ".jpg", ".jpeg"}
_IMAGE_WIDE_TOKENS = (
    "wide",
    "header",
    "hero",
    "banner",
    "capsule",
    "library",
    "splash",
    "poster",
    "bravia",
)
_IMAGE_AVOID_TOKENS = ("square", "small", "store", "icon")
_IMAGE_MAX_DEPTH = 2

LOCALE_TO_EA = {
    "english": "en_US",
    "koreana": "ko_KR",
    "french": "fr_FR",
    "german": "de_DE",
    "italian": "it_IT",
    "spanish": "es_ES",
    "japanese": "ja_JP",
    "polish": "pl_PL",
    "portuguese": "pt_BR",
    "russian": "ru_RU",
    "chinese": "zh_CN",
    "tchinese": "zh_TW",
    "arabic": "ar_SA",
}

_RE_CONTENT_ID = re.compile(r"<contentID>\s*([^<]+)\s*</contentID>", re.IGNORECASE)
_RE_GAME_TITLE = re.compile(
    r'<gameTitle\s+locale="([^"]+)"[^>]*>([^<]+)</gameTitle>',
    re.IGNORECASE,
)


def read_ea_ini_value(ini_path: Path, key_suffix: str) -> str | None:
    if not ini_path.is_file():
        return None
    try:
        text = ini_path.read_text(encoding="utf-8", errors="replace")
    except OSError:
        return None
    suffix = key_suffix.casefold()
    for line in text.splitlines():
        if "=" not in line:
            continue
        key, _, value = line.partition("=")
        if key.strip().casefold().endswith(suffix):
            return value.strip().strip('"')
    return None


def discover_ea_roots(
    user_dirs: list[str],
    programdata: Path | None = None,
    localappdata: Path | None = None,
    default_ea_games: str | None = None,
) -> list[str]:
    if programdata is None:
        programdata = Path(os.environ.get("PROGRAMDATA", r"C:\ProgramData"))
    if localappdata is None:
        localappdata = Path(os.environ.get("LOCALAPPDATA", ""))
    if default_ea_games is None:
        default_ea_games = r"C:\Program Files\EA Games"

    roots: list[str] = []
    seen: set[str] = set()

    def add(candidate: str | None) -> None:
        if not candidate:
            return
        try:
            resolved = str(Path(candidate).resolve())
        except OSError:
            return
        key = resolved.casefold()
        if key in seen or not Path(resolved).is_dir():
            return
        seen.add(key)
        roots.append(resolved)

    for directory in user_dirs:
        add(directory.strip())

    machine = programdata / "EA Desktop" / "machine.ini"
    val = read_ea_ini_value(machine, "downloadinplacedir")
    if val:
        add(val)

    ea_local = localappdata / "Electronic Arts" / "EA Desktop"
    if ea_local.is_dir():
        for ini in ea_local.glob("user_*.ini"):
            val = read_ea_ini_value(ini, "downloadinplacedir")
            if val:
                add(val)

    add(default_ea_games)
    return roots


def package_id_from_base_folder(folder_name: str) -> str | None:
    lowered = folder_name.casefold()
    if not lowered.startswith("base-"):
        return None
    package_id = folder_name[5:].strip()
    return package_id or None


def list_installdata_games(install_data_root: Path) -> list[dict]:
    games: list[dict] = []
    if not install_data_root.is_dir():
        return games

    for entry in install_data_root.iterdir():
        if not entry.is_dir():
            continue
        package_id = None
        for child in entry.iterdir():
            if not child.is_dir():
                continue
            pid = package_id_from_base_folder(child.name)
            if pid:
                package_id = pid
                break
        if package_id is None:
            continue
        games.append(
            {
                "folder_name": entry.name,
                "package_id": package_id,
                "is_base": True,
            }
        )
    return games


def parse_installerdata_xml(path: Path, locale: str = "english") -> dict:
    result: dict = {"name": None, "content_id": None, "content_ids": []}
    if not path.is_file():
        return result
    try:
        text = path.read_text(encoding="utf-8", errors="replace")
    except OSError:
        return result

    content_ids = [match.group(1).strip() for match in _RE_CONTENT_ID.finditer(text)]
    result["content_ids"] = content_ids
    if content_ids:
        result["content_id"] = content_ids[0]

    titles: list[tuple[str, str]] = []
    for match in _RE_GAME_TITLE.finditer(text):
        titles.append((match.group(1).strip(), match.group(2).strip()))

    if not titles:
        return result

    preferred = LOCALE_TO_EA.get(locale.casefold(), "en_US")
    for loc, name in titles:
        if loc.casefold() == preferred.casefold():
            result["name"] = name
            return result
    for loc, name in titles:
        if loc.casefold() == "en_us":
            result["name"] = name
            return result
    result["name"] = titles[0][1]
    return result


def build_ea_launch(
    content_ids: list[str] | None,
    install_dir: Path,
    package_id: str | None = None,
) -> str:
    if content_ids:
        offer_ids = ",".join(content_ids)
        return f"[origin2://game/launch/?offerIds={offer_ids}]"
    if package_id:
        return f"[origin2://game/launch/?offerIds={package_id}]"
    return f'[explorer "{install_dir}"]'


def _should_skip_folder_name(name: str) -> bool:
    lowered = name.casefold()
    return any(marker in lowered for marker in _SKIP_FOLDER_MARKERS)


def _iter_shallow_files(root: Path, max_depth: int = _IMAGE_MAX_DEPTH):
    if not root.is_dir():
        return
    root = root.resolve()
    for dirpath, dirnames, filenames in os.walk(root):
        current = Path(dirpath)
        try:
            depth = len(current.relative_to(root).parts)
        except ValueError:
            dirnames.clear()
            continue
        if depth >= max_depth:
            dirnames.clear()
        dirnames[:] = [d for d in dirnames if not _should_skip_folder_name(d)]
        for name in filenames:
            yield current / name


def _image_sort_key(path_or_name: str) -> tuple:
    lowered = Path(path_or_name).name.casefold()
    if any(token in lowered for token in _IMAGE_WIDE_TOKENS):
        bucket = 0
    elif any(token in lowered for token in _IMAGE_AVOID_TOKENS):
        bucket = 2
    else:
        bucket = 1
    return (bucket, lowered)


def _find_ea_game_image(
    install_dir: Path,
    installer_dir: Path,
    placeholder: str,
) -> str:
    search_roots = []
    if installer_dir.is_dir():
        search_roots.append(installer_dir)
    if install_dir.is_dir():
        search_roots.append(install_dir)

    candidates: list[tuple[tuple, int, str]] = []
    seen: set[str] = set()
    for root in search_roots:
        for path in _iter_shallow_files(root, max_depth=_IMAGE_MAX_DEPTH):
            if not path.is_file():
                continue
            if path.suffix.casefold() not in _IMAGE_EXTENSIONS:
                continue
            resolved = str(path.resolve())
            if resolved in seen:
                continue
            seen.add(resolved)
            try:
                depth = len(path.relative_to(root).parts)
            except ValueError:
                depth = 99
            candidates.append((_image_sort_key(path.name), depth, resolved))

    if not candidates:
        return placeholder

    candidates.sort(key=lambda item: (item[0], item[1]))
    return candidates[0][2]


def _find_install_dir(roots: list[str], folder_name: str) -> Path | None:
    for root in roots:
        candidate = Path(root) / folder_name
        if (candidate / "__Installer" / "installerdata.xml").is_file():
            return candidate.resolve()
    return None


def parse_ea_game(
    install_dir: Path,
    folder_name: str,
    package_id: str | None,
    locale: str,
    placeholder: str,
) -> dict | None:
    installer_dir = install_dir / "__Installer"
    metadata = parse_installerdata_xml(installer_dir / "installerdata.xml", locale)
    name = metadata.get("name") or folder_name
    stable_key = package_id or folder_name
    content_ids = metadata.get("content_ids") or []
    launch = build_ea_launch(content_ids, install_dir, package_id)
    image_path = _find_ea_game_image(install_dir, installer_dir, placeholder)
    return {
        "stable_id": f"ea:{stable_key}",
        "name": name,
        "launch": launch,
        "image_path": image_path,
    }


def scan_ea_libraries(
    roots: list[str],
    install_data_root: Path,
    locale: str,
    placeholder: str,
    status_fn: Callable[[str], None],
) -> list[dict]:
    records: list[dict] = []
    if not roots:
        return records

    for game in list_installdata_games(install_data_root):
        folder_name = game["folder_name"]
        if _should_skip_folder_name(folder_name):
            continue
        install_dir = _find_install_dir(roots, folder_name)
        if install_dir is None:
            continue
        try:
            record = parse_ea_game(
                install_dir,
                folder_name,
                game.get("package_id"),
                locale,
                placeholder,
            )
        except OSError as exc:
            status_fn(f"EA: skip {folder_name} ({exc})")
            continue
        if record:
            records.append(record)
    return records
