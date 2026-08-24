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


def test_read_gaming_root_utf16_le_bom_control_byte(tmp_path: Path):
    xb = load_xbox()
    drive = tmp_path / "D"
    drive.mkdir()
    lib = drive / "XboxGames"
    lib.mkdir()
    payload = b"\xff\xfe" + b"\x01\x00" + "XboxGames".encode("utf-16-le")
    (drive / ".GamingRoot").write_bytes(payload)

    result = xb.read_gaming_root(str(drive / ".GamingRoot"))
    assert result == str(lib.resolve())


def test_read_gaming_root_rgbx_utf16_relative_path(tmp_path: Path):
    """Real Xbox PC .GamingRoot: RGBX magic + u32 version + UTF-16-LE relative dir."""
    xb = load_xbox()
    drive = tmp_path / "E"
    drive.mkdir()
    lib = drive / "Xbox"
    lib.mkdir()
    # Matches E:\\.GamingRoot on this machine: RGBX\\x01\\x00\\x00\\x00 + UTF-16 "Xbox\\0"
    payload = b"RGBX" + b"\x01\x00\x00\x00" + "Xbox".encode("utf-16-le") + b"\x00\x00"
    (drive / ".GamingRoot").write_bytes(payload)

    result = xb.read_gaming_root(str(drive / ".GamingRoot"))
    assert result == str(lib.resolve())


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
  <ShellVisuals DefaultDisplayName="Cool Game" StoreLogo="StoreLogo.png" />
  <ExecutableList><Executable Name="CoolGame.exe" Id="Game" /></ExecutableList>
  <Identity Name="CoolGameId" />
</Game>
""",
        encoding="utf-8",
    )
    (content / "CoolGame.exe").write_bytes(b"MZ")
    (content / "StoreLogo.png").write_bytes(b"png")
    art = content / "art"
    art.mkdir()
    (art / "header.png").write_bytes(b"png")
    placeholder = str(tmp_path / "ph.jpg")
    Path(placeholder).write_bytes(b"\xff\xd8\xff\xd9")
    rec = xb.parse_xbox_game_dir(str(game), placeholder)
    assert rec["stable_id"] == "xbox:CoolGameId"
    assert rec["name"] == "Cool Game"
    assert "CoolGame.exe" in rec["launch"]
    # Prefer ShellVisuals StoreLogo over deep art walk
    assert rec["image_path"].endswith("StoreLogo.png")


def test_find_game_image_does_not_walk_deep_trees(tmp_path: Path):
    xb = load_xbox()
    game = tmp_path / "Huge"
    content = game / "Content"
    deep = content / "a" / "b" / "c" / "d"
    deep.mkdir(parents=True)
    (deep / "hidden.png").write_bytes(b"png")
    (content / "logo.png").write_bytes(b"png")
    placeholder = str(tmp_path / "ph.jpg")
    Path(placeholder).write_bytes(b"\xff\xd8\xff\xd9")
    assert xb._find_game_image(game, placeholder).endswith("logo.png")
    # Depth > 2 must not be required; deep file alone should fall back to placeholder
    (content / "logo.png").unlink()
    assert xb._find_game_image(game, placeholder) == placeholder


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


def test_parse_xbox_game_dir_skips_dlc_package(tmp_path: Path):
    xb = load_xbox()
    game = tmp_path / "SpaceMarine2-DLC"
    content = game / "Content"
    content.mkdir(parents=True)
    (content / "MicrosoftGame.config").write_text(
        """<?xml version="1.0" encoding="utf-8"?>
<Game configVersion="1">
  <Identity Name="Publisher.Game-DLC21" Publisher="CN=Test" Version="1.0.0.0"/>
  <ShellVisuals DefaultDisplayName="Game - Chapter Voice Pack 1"/>
  <TargetDeviceFamilyForDLC>PC</TargetDeviceFamilyForDLC>
  <AllowedProducts><AllowedProduct>9NMAIN</AllowedProduct></AllowedProducts>
  <DesktopRegistration>
    <MainPackageDependency Name="Publisher.Game" />
  </DesktopRegistration>
</Game>
""",
        encoding="utf-8",
    )
    (content / "dummy.exe").write_bytes(b"MZ")
    placeholder = str(tmp_path / "ph.jpg")
    Path(placeholder).write_bytes(b"\xff\xd8\xff\xd9")
    assert xb.parse_xbox_game_dir(str(game), placeholder) is None


def test_scan_xbox_libraries_skips_dlc_keeps_main(tmp_path: Path):
    xb = load_xbox()
    root = tmp_path / "Xbox"
    main = root / "MainGame" / "Content"
    main.mkdir(parents=True)
    (main / "MicrosoftGame.config").write_text(
        '<Game><ShellVisuals DefaultDisplayName="Main Game" /><ExecutableList><Executable Name="g.exe"/></ExecutableList><Identity Name="Main"/></Game>',
        encoding="utf-8",
    )
    (main / "g.exe").write_bytes(b"MZ")
    dlc = root / "MainGame-DLC" / "Content"
    dlc.mkdir(parents=True)
    (dlc / "MicrosoftGame.config").write_text(
        '<Game><Identity Name="Main-DLC1"/><TargetDeviceFamilyForDLC>PC</TargetDeviceFamilyForDLC>'
        '<DesktopRegistration><MainPackageDependency Name="Main"/></DesktopRegistration></Game>',
        encoding="utf-8",
    )
    ph = str(tmp_path / "ph.jpg")
    Path(ph).write_bytes(b"\xff\xd8\xff\xd9")
    records = xb.scan_xbox_libraries([str(root)], ph, lambda m: None)
    assert len(records) == 1
    assert records[0]["stable_id"] == "xbox:Main"


def test_scan_xbox_libraries_skips_bad_game_sibling(tmp_path: Path, monkeypatch):
    xb = load_xbox()
    root = tmp_path / "XboxGames"
    good = root / "CoolGame" / "Content"
    good.mkdir(parents=True)
    (good / "MicrosoftGame.config").write_text(
        '<Game><ShellVisuals DefaultDisplayName="Cool Game" /><ExecutableList><Executable Name="g.exe"/></ExecutableList><Identity Name="Cool"/></Game>',
        encoding="utf-8",
    )
    (good / "g.exe").write_bytes(b"MZ")
    (root / "BadGame").mkdir()

    real_parse = xb.parse_xbox_game_dir

    def parse_with_failure(game_dir, placeholder_image):
        if Path(game_dir).name == "BadGame":
            raise PermissionError("denied")
        return real_parse(game_dir, placeholder_image)

    monkeypatch.setattr(xb, "parse_xbox_game_dir", parse_with_failure)

    messages: list[str] = []
    ph = str(tmp_path / "ph.jpg")
    Path(ph).write_bytes(b"\xff\xd8\xff\xd9")
    records = xb.scan_xbox_libraries([str(root)], ph, messages.append)

    assert len(records) == 1
    assert records[0]["stable_id"] == "xbox:Cool"
    assert any("BadGame" in msg and "PermissionError" in msg for msg in messages)
