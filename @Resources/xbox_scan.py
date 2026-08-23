from __future__ import annotations

import os
import string
from pathlib import Path


def read_gaming_root(file_path: str) -> str | None:
    path = Path(file_path)
    if not path.is_file():
        return None

    raw = path.read_bytes()
    text: str | None = None
    for encoding in ("utf-16-le", "utf-8"):
        try:
            text = raw.decode(encoding)
            break
        except UnicodeDecodeError:
            continue
    if text is None:
        return None

    text = text.replace("\x00", "").strip()
    if not text:
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
