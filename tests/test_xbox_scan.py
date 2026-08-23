from __future__ import annotations

import importlib.util
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
XBOX_SCAN = ROOT / "@Resources" / "xbox_scan.py"


def load_xbox():
    spec = importlib.util.spec_from_file_location("xbox_scan", XBOX_SCAN)
    mod = importlib.util.module_from_spec(spec)
    assert spec.loader is not None
    sys.modules["xbox_scan"] = mod
    spec.loader.exec_module(mod)
    return mod


def test_discover_xbox_roots_user_gamingroot_and_default(tmp_path: Path):
    xb = load_xbox()
    user = tmp_path / "CustomXbox"
    user.mkdir()
    drive = tmp_path / "D"
    drive.mkdir()
    lib = drive / "XboxGames"
    lib.mkdir()
    (drive / ".GamingRoot").write_text(str(lib), encoding="utf-8")
    default = tmp_path / "DefaultXbox"
    # pass default_path override for testability:
    roots = xb.discover_xbox_roots(
        [str(user), str(user)],  # dup
        gaming_root_dirs=[str(drive)],
        default_xbox_games=str(default),
    )
    default.mkdir()
    roots2 = xb.discover_xbox_roots(
        [str(user)],
        gaming_root_dirs=[str(drive)],
        default_xbox_games=str(default),
    )
    assert roots2[0] == str(user.resolve())
    assert any(Path(r).resolve() == lib.resolve() for r in roots2)
    assert any(Path(r).resolve() == default.resolve() for r in roots2)


def test_parse_xbox_game_dir_exe_and_image(tmp_path: Path):
    xb = load_xbox()
    game = tmp_path / "CoolGame"
    content = game / "Content"
    content.mkdir(parents=True)
    (content / "MicrosoftGame.config").write_text(
        """<?xml version="1.0"?>
<Game>
  <ShellVisuals DefaultDisplayName="Cool Game" />
  <ExecutableList><Executable Name="CoolGame.exe" Id="Game" /></ExecutableList>
  <Identity Name="CoolGameId" />
</Game>
""",
        encoding="utf-8",
    )
    (content / "CoolGame.exe").write_bytes(b"MZ")
    art = content / "art"
    art.mkdir()
    (art / "header.png").write_bytes(b"png")
    placeholder = str(tmp_path / "ph.jpg")
    Path(placeholder).write_bytes(b"\xff\xd8\xff\xd9")
    rec = xb.parse_xbox_game_dir(str(game), placeholder)
    assert rec["stable_id"] == "xbox:CoolGameId"
    assert rec["name"] == "Cool Game"
    assert "CoolGame.exe" in rec["launch"]
    assert rec["image_path"].endswith("header.png")


def test_parse_xbox_game_dir_falls_back_to_explorer_and_placeholder(tmp_path: Path):
    xb = load_xbox()
    game = tmp_path / "BareGame"
    content = game / "Content"
    content.mkdir(parents=True)
    (content / "appxmanifest.xml").write_text(
        '<Package><Identity Name="BareId" /><Applications><Application Id="App" /></Applications></Package>',
        encoding="utf-8",
    )
    placeholder = str(tmp_path / "ph.jpg")
    Path(placeholder).write_bytes(b"\xff\xd8\xff\xd9")
    rec = xb.parse_xbox_game_dir(str(game), placeholder)
    assert rec["stable_id"] == "xbox:BareId"
    assert "explorer" in rec["launch"].lower()
    assert rec["image_path"] == placeholder


def test_scan_xbox_libraries_skips_non_games(tmp_path: Path):
    xb = load_xbox()
    root = tmp_path / "XboxGames"
    good = root / "CoolGame" / "Content"
    good.mkdir(parents=True)
    (good / "MicrosoftGame.config").write_text(
        '<Game><ShellVisuals DefaultDisplayName="Cool Game" /><ExecutableList><Executable Name="g.exe"/></ExecutableList><Identity Name="Cool"/></Game>',
        encoding="utf-8",
    )
    (good / "g.exe").write_bytes(b"MZ")
    (root / "RandomEmpty").mkdir()
    ph = str(tmp_path / "ph.jpg")
    Path(ph).write_bytes(b"\xff\xd8\xff\xd9")
    records = xb.scan_xbox_libraries([str(root)], ph, lambda m: None)
    assert len(records) == 1
    assert records[0]["stable_id"] == "xbox:Cool"
