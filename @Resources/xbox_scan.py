from __future__ import annotations

import os
import re
import string
from pathlib import Path
from typing import Callable

_SKIP_FOLDER_MARKERS = ("runtime", "redistributable")
_IMAGE_EXTENSIONS = {".png", ".jpg", ".jpeg"}
# Prefer Steam-like landscape banners over square Store/Square logos.
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
_RE_DLC_MARKERS = re.compile(
    r"<TargetDeviceFamilyForDLC\b|<MainPackageDependency\b|<AllowedProducts\b",
    re.IGNORECASE,
)
_RE_SHELL_IMAGE_ATTRS = re.compile(
    r'\b(?:WideLogo|StoreLogo|Square150x150Logo|Square44x44Logo|Square480x480Logo|'
    r'SplashScreenImage|Logo)="([^"]+)"',
    re.IGNORECASE,
)
_RE_LOGO_ELEMENT = re.compile(r"<Logo[^>]*>([^<]+)</Logo>", re.IGNORECASE)


def _decode_gaming_root_text(raw: bytes) -> str | None:
    # Xbox PC writes: "RGBX" + u32le version + UTF-16-LE relative path + NUL
    if raw.startswith(b"RGBX") and len(raw) > 8:
        try:
            text = raw[8:].decode("utf-16-le")
        except UnicodeDecodeError:
            return None
        text = text.replace("\x00", "").strip()
        return text or None

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


def _is_dlc_package(content_dir: Path) -> bool:
    """Skip Xbox Store DLC / add-on packages (not launchable titles)."""
    text = _read_text_file(content_dir / "MicrosoftGame.config")
    if not text:
        return False
    if _RE_DLC_MARKERS.search(text):
        return True
    identity = _RE_IDENTITY_NAME.search(text)
    if identity and re.search(r"-?DLC", identity.group(1), re.IGNORECASE):
        return True
    return False


def _extract_logo_paths(text: str) -> list[str]:
    logos = [m.group(1).strip() for m in _RE_SHELL_IMAGE_ATTRS.finditer(text) if m.group(1).strip()]
    for m in _RE_LOGO_ELEMENT.finditer(text):
        value = m.group(1).strip()
        if value:
            logos.append(value)
    return logos


def _parse_microsoft_game_config(content_dir: Path) -> dict:
    text = _read_text_file(content_dir / "MicrosoftGame.config")
    if not text:
        return {"name": None, "identity": None, "executable": None, "logos": []}

    display = _RE_SHELL_DISPLAY.search(text)
    if not display:
        display = _RE_DISPLAY_NAME.search(text)

    identity = _RE_IDENTITY_NAME.search(text)
    executable = _RE_EXECUTABLE_NAME.search(text)
    return {
        "name": display.group(1).strip() if display else None,
        "identity": identity.group(1).strip() if identity else None,
        "executable": executable.group(1).strip() if executable else None,
        "logos": _extract_logo_paths(text),
    }


def _parse_appxmanifest(content_dir: Path) -> dict:
    text = _read_text_file(content_dir / "appxmanifest.xml")
    if not text:
        return {
            "identity": None,
            "application_id": None,
            "package_family_name": None,
            "logos": [],
        }

    identity = _RE_IDENTITY_NAME.search(text)
    application = _RE_APPLICATION_ID.search(text)
    pfn = _RE_PACKAGE_FAMILY_NAME.search(text)
    return {
        "identity": identity.group(1).strip() if identity else None,
        "application_id": application.group(1).strip() if application else None,
        "package_family_name": pfn.group(1).strip() if pfn else None,
        "logos": _extract_logo_paths(text),
    }


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


def _find_content_executable(content_dir: Path, relative_name: str | None) -> Path | None:
    if relative_name:
        candidate = (content_dir / relative_name).resolve()
        if candidate.is_file() and not _is_redistributable_path(candidate):
            return candidate

    # Shallow only — never rglob entire multi-GB Xbox installs.
    for path in _iter_shallow_files(content_dir, max_depth=_IMAGE_MAX_DEPTH):
        if path.suffix.casefold() == ".exe" and path.is_file() and not _is_redistributable_path(path):
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


def _image_sort_key(path_or_name: str) -> tuple:
    """Lower is better — landscape/wide banners before square store icons."""
    lowered = Path(path_or_name).name.casefold()
    if any(token in lowered for token in _IMAGE_WIDE_TOKENS):
        bucket = 0
    elif any(token in lowered for token in _IMAGE_AVOID_TOKENS):
        bucket = 2
    else:
        bucket = 1
    return (bucket, lowered)


def _resolve_logo_candidates(content_dir: Path, logos: list[str]) -> list[str]:
    found: list[str] = []
    for rel in logos:
        candidate = (content_dir / rel).resolve()
        if candidate.is_file() and candidate.suffix.casefold() in _IMAGE_EXTENSIONS:
            found.append(str(candidate))
    return found


def _find_game_image(
    game_dir: Path,
    placeholder_image: str,
    logos: list[str] | None = None,
) -> str:
    content = _content_dir(game_dir)
    if logos and content.is_dir():
        from_config = _resolve_logo_candidates(content, logos)
        if from_config:
            from_config.sort(key=_image_sort_key)
            return from_config[0]

    search_roots = []
    if content.is_dir():
        search_roots.append(content)
    search_roots.append(game_dir)

    candidates: list[tuple[tuple, int, str]] = []
    seen: set[str] = set()
    for root in search_roots:
        for path in _iter_shallow_files(root, max_depth=_IMAGE_MAX_DEPTH):
            if not path.is_file():
                continue
            if path.suffix.casefold() not in _IMAGE_EXTENSIONS:
                continue
            if _is_redistributable_path(path):
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
        return placeholder_image

    candidates.sort(key=lambda item: (item[0], item[1]))
    return candidates[0][2]


def _build_launch(
    game_dir: Path,
    content_dir: Path,
    executable_name: str | None,
    manifest: dict,
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
    if _is_dlc_package(content_dir):
        return None

    config = _parse_microsoft_game_config(content_dir)
    manifest = _parse_appxmanifest(content_dir)

    name = config["name"] or path.name
    if not str(name).strip():
        return None

    identity = config["identity"] or manifest["identity"]
    stable_key = identity if identity else _sanitize_stable_id_key(path.name)

    logos = list(config.get("logos") or []) + list(manifest.get("logos") or [])
    launch = _build_launch(path, content_dir, config["executable"], manifest)
    image_path = _find_game_image(path, placeholder_image, logos=logos)

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
