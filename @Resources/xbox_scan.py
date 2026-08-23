from __future__ import annotations

import os
import re
import string
from pathlib import Path
from typing import Callable

_SKIP_FOLDER_MARKERS = ("runtime", "redistributable")
_IMAGE_EXTENSIONS = {".png", ".jpg", ".jpeg"}
_IMAGE_PREFERENCE = ("header", "logo", "poster")
_RE_SHELL_DISPLAY = re.compile(
    r'<ShellVisuals[^>]*\bDefaultDisplayName="([^"]*)"',
    re.IGNORECASE,
)
_RE_DISPLAY_NAME = re.compile(r"<DisplayName[^>]*>([^<]+)</DisplayName>", re.IGNORECASE)
_RE_IDENTITY_NAME = re.compile(r'<Identity[^>]*\bName="([^"]*)"', re.IGNORECASE)
_RE_EXECUTABLE_NAME = re.compile(r'<Executable[^>]*\bName="([^"]*)"', re.IGNORECASE)
_RE_APPLICATION_ID = re.compile(r'<Application[^>]*\bId="([^"]*)"', re.IGNORECASE)
_RE_PACKAGE_FAMILY_NAME = re.compile(
    r'\bPackageFamilyName="([^"]*)"',
    re.IGNORECASE,
)


def _decode_gaming_root_text(raw: bytes) -> str | None:
    if raw.startswith(b"\xff\xfe") or raw.startswith(b"\xfe\xff"):
        try:
            text = raw.decode("utf-16")
        except UnicodeDecodeError:
            return None
    elif raw.startswith(b"\xef\xbb\xbf"):
        try:
            text = raw.decode("utf-8-sig")
        except UnicodeDecodeError:
            return None
    else:
        text = None
        for encoding in ("utf-16-le", "utf-8"):
            try:
                text = raw.decode(encoding)
                break
            except UnicodeDecodeError:
                continue
        if text is None:
            return None

    text = text.replace("\x00", "").lstrip("\ufeff")
    while text and not text[0].isprintable():
        text = text[1:]
    text = text.strip()
    return text or None


def read_gaming_root(file_path: str) -> str | None:
    path = Path(file_path)
    if not path.is_file():
        return None

    text = _decode_gaming_root_text(path.read_bytes())
    if text is None:
        return None

    library = Path(text)
    if not library.is_absolute():
        library = path.parent / text

    resolved = library.resolve()
    if resolved.is_dir():
        return str(resolved)
    return None


def discover_xbox_roots(
    user_dirs: list[str],
    gaming_root_dirs: list[str] | None = None,
    default_xbox_games: str | None = None,
) -> list[str]:
    if gaming_root_dirs is None:
        gaming_root_dirs = [
            f"{drive}:\\"
            for drive in string.ascii_uppercase
            if os.path.exists(f"{drive}:\\")
        ]
    if default_xbox_games is None:
        default_xbox_games = r"C:\XboxGames"

    roots: list[str] = []
    seen: set[str] = set()

    def add(candidate: str) -> None:
        if not candidate:
            return
        resolved = str(Path(candidate).resolve())
        key = resolved.casefold()
        if key in seen or not Path(resolved).exists():
            return
        seen.add(key)
        roots.append(resolved)

    for user_dir in user_dirs:
        add(user_dir)

    for drive_dir in gaming_root_dirs:
        gaming_root = read_gaming_root(str(Path(drive_dir) / ".GamingRoot"))
        if gaming_root:
            add(gaming_root)

    add(default_xbox_games)
    return roots


def _should_skip_folder_name(name: str) -> bool:
    lowered = name.casefold()
    return any(marker in lowered for marker in _SKIP_FOLDER_MARKERS)


def _content_dir(game_dir: Path) -> Path:
    return game_dir / "Content"


def _is_redistributable_path(path: Path) -> bool:
    lowered = str(path).casefold()
    return any(marker in lowered for marker in _SKIP_FOLDER_MARKERS)


def _read_text_file(path: Path) -> str | None:
    if not path.is_file():
        return None
    try:
        return path.read_text(encoding="utf-8")
    except (OSError, UnicodeDecodeError):
        return None


def _parse_microsoft_game_config(content_dir: Path) -> dict[str, str | None]:
    text = _read_text_file(content_dir / "MicrosoftGame.config")
    if not text:
        return {"name": None, "identity": None, "executable": None}

    display = _RE_SHELL_DISPLAY.search(text)
    if not display:
        display = _RE_DISPLAY_NAME.search(text)

    identity = _RE_IDENTITY_NAME.search(text)
    executable = _RE_EXECUTABLE_NAME.search(text)
    return {
        "name": display.group(1).strip() if display else None,
        "identity": identity.group(1).strip() if identity else None,
        "executable": executable.group(1).strip() if executable else None,
    }


def _parse_appxmanifest(content_dir: Path) -> dict[str, str | None]:
    text = _read_text_file(content_dir / "appxmanifest.xml")
    if not text:
        return {"identity": None, "application_id": None, "package_family_name": None}

    identity = _RE_IDENTITY_NAME.search(text)
    application = _RE_APPLICATION_ID.search(text)
    pfn = _RE_PACKAGE_FAMILY_NAME.search(text)
    return {
        "identity": identity.group(1).strip() if identity else None,
        "application_id": application.group(1).strip() if application else None,
        "package_family_name": pfn.group(1).strip() if pfn else None,
    }


def _find_content_executable(content_dir: Path, relative_name: str | None) -> Path | None:
    if relative_name:
        candidate = (content_dir / relative_name).resolve()
        if candidate.is_file() and not _is_redistributable_path(candidate):
            return candidate

    for path in sorted(content_dir.rglob("*.exe")):
        if path.is_file() and not _is_redistributable_path(path):
            return path.resolve()
    return None


def _has_game_evidence(content_dir: Path) -> bool:
    if (content_dir / "MicrosoftGame.config").is_file():
        return True
    if (content_dir / "appxmanifest.xml").is_file():
        return True
    return _find_content_executable(content_dir, None) is not None


def _is_xbox_game_dir(game_dir: Path) -> bool:
    content_dir = _content_dir(game_dir)
    return content_dir.is_dir() and _has_game_evidence(content_dir)


def _sanitize_stable_id_key(name: str) -> str:
    sanitized = re.sub(r"[^A-Za-z0-9-]+", "", name)
    return sanitized or name


def _find_game_image(game_dir: Path, placeholder_image: str) -> str:
    candidates: list[tuple[int, str, str]] = []
    for path in game_dir.rglob("*"):
        if not path.is_file():
            continue
        if path.suffix.casefold() not in _IMAGE_EXTENSIONS:
            continue
        lowered = path.name.casefold()
        rank = 1
        for index, token in enumerate(_IMAGE_PREFERENCE):
            if token in lowered:
                rank = 0
                break
        candidates.append((rank, lowered, str(path.resolve())))

    if not candidates:
        return placeholder_image

    candidates.sort(key=lambda item: (item[0], item[1]))
    return candidates[0][2]


def _build_launch(
    game_dir: Path,
    content_dir: Path,
    executable_name: str | None,
    manifest: dict[str, str | None],
) -> str:
    exe = _find_content_executable(content_dir, executable_name)
    if exe is not None:
        return f'["{exe}"]'

    pfn = manifest.get("package_family_name")
    app_id = manifest.get("application_id")
    if pfn and app_id:
        aumid = f"{pfn}!{app_id}"
        return f"[shell:AppsFolder\\{aumid}]"

    resolved = str(game_dir.resolve())
    return f'[explorer "{resolved}"]'


def parse_xbox_game_dir(game_dir: str, placeholder_image: str) -> dict | None:
    path = Path(game_dir)
    if not path.is_dir() or _should_skip_folder_name(path.name):
        return None

    content_dir = _content_dir(path)
    if not _is_xbox_game_dir(path):
        return None

    config = _parse_microsoft_game_config(content_dir)
    manifest = _parse_appxmanifest(content_dir)

    name = config["name"] or path.name
    if not name.strip():
        return None

    identity = config["identity"] or manifest["identity"]
    stable_key = identity if identity else _sanitize_stable_id_key(path.name)

    launch = _build_launch(path, content_dir, config["executable"], manifest)
    image_path = _find_game_image(path, placeholder_image)

    return {
        "stable_id": f"xbox:{stable_key}",
        "name": name,
        "launch": launch,
        "image_path": image_path,
    }


def scan_xbox_libraries(
    roots: list[str],
    placeholder_image: str,
    status_fn: Callable[[str], None],
) -> list[dict]:
    records: list[dict] = []
    explorer_fallbacks = 0

    for root in roots:
        root_path = Path(root)
        if not root_path.is_dir():
            continue

        for child in sorted(root_path.iterdir()):
            if not child.is_dir():
                continue

            try:
                record = parse_xbox_game_dir(str(child), placeholder_image)
            except (OSError, UnicodeDecodeError) as exc:
                status_fn(f"Xbox: skipped {child.name} ({exc.__class__.__name__})")
                continue
            if record is None:
                continue

            if record["launch"].casefold().startswith("[explorer"):
                explorer_fallbacks += 1
            records.append(record)

    if explorer_fallbacks:
        noun = "game" if explorer_fallbacks == 1 else "games"
        status_fn(
            f"Xbox: {explorer_fallbacks} {noun} open folder only (no launcher found)"
        )

    return records
