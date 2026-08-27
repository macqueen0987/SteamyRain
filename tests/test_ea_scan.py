from __future__ import annotations

import importlib.util
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
EA_SCAN = ROOT / "@Resources" / "ea_scan.py"

INSTALLER_XML = """<?xml version="1.0"?>
<install>
  <contentIDs><contentID>194908</contentID></contentIDs>
  <gameTitles>
    <gameTitle locale="en_US">Apex Legends</gameTitle>
    <gameTitle locale="ko_KR">Apex 레전드</gameTitle>
  </gameTitles>
</install>"""


def load_ea():
    spec = importlib.util.spec_from_file_location("ea_scan", EA_SCAN)
    mod = importlib.util.module_from_spec(spec)
    assert spec.loader is not None
    sys.modules["ea_scan"] = mod
    spec.loader.exec_module(mod)
    return mod


def test_read_ea_ini_value_download_dir(tmp_path: Path):
    ea = load_ea()
    ini = tmp_path / "machine.ini"
    ini.write_text("machine.downloadinplacedir=E:\\EA\\\n", encoding="utf-8")
    assert ea.read_ea_ini_value(ini, "downloadinplacedir") == "E:\\EA\\"


def test_discover_ea_roots_user_ini_and_default(tmp_path: Path):
    ea = load_ea()
    user = tmp_path / "CustomEA"
    user.mkdir()
    default = tmp_path / "DefaultEA"
    default.mkdir()
    programdata = tmp_path / "ProgramData"
    ea_desktop = programdata / "EA Desktop"
    ea_desktop.mkdir(parents=True)
    (ea_desktop / "machine.ini").write_text(
        f"machine.downloadinplacedir={default}\n",
        encoding="utf-8",
    )
    localappdata = tmp_path / "Local"
    user_ini_dir = localappdata / "Electronic Arts" / "EA Desktop"
    user_ini_dir.mkdir(parents=True)
    (user_ini_dir / "user_1.ini").write_text(
        f"user.downloadinplacedir={user}\n",
        encoding="utf-8",
    )
    roots = ea.discover_ea_roots(
        [str(user)],
        programdata=programdata,
        localappdata=localappdata,
        default_ea_games=str(default),
    )
    assert roots[0] == str(user.resolve())
    assert any(Path(r).resolve() == default.resolve() for r in roots)


def test_list_installdata_games_base_only(tmp_path: Path):
    ea = load_ea()
    root = tmp_path / "InstallData"
    apex = root / "Apex"
    (apex / "base-Origin.SFT.50.0000848").mkdir(parents=True)
    (apex / "dlc-Origin.SFT.50.0000999").mkdir()
    bf = root / "BF6"
    (bf / "dlc-Origin.SFT.50.0001523").mkdir(parents=True)
    games = ea.list_installdata_games(root)
    assert len(games) == 1
    assert games[0]["folder_name"] == "Apex"
    assert games[0]["package_id"] == "Origin.SFT.50.0000848"


def test_package_id_from_base_folder():
    ea = load_ea()
    assert ea.package_id_from_base_folder("base-Origin.SFT.50.0000848") == "Origin.SFT.50.0000848"
    assert ea.package_id_from_base_folder("dlc-Origin.SFT.50.0001523") is None


def test_parse_installerdata_xml_locale(tmp_path: Path):
    ea = load_ea()
    p = tmp_path / "installerdata.xml"
    p.write_text(INSTALLER_XML, encoding="utf-8")
    en = ea.parse_installerdata_xml(p, "english")
    assert en["name"] == "Apex Legends"
    assert en["content_id"] == "194908"
    assert en["content_ids"] == ["194908"]
    ko = ea.parse_installerdata_xml(p, "koreana")
    assert ko["name"] == "Apex 레전드"


def test_parse_installerdata_xml_multiple_content_ids(tmp_path: Path):
    ea = load_ea()
    p = tmp_path / "installerdata.xml"
    p.write_text(
        "<contentIDs><contentID>16425782</contentID><contentID>16425782_oa</contentID></contentIDs>",
        encoding="utf-8",
    )
    meta = ea.parse_installerdata_xml(p)
    assert meta["content_ids"] == ["16425782", "16425782_oa"]


def test_build_ea_launch_uses_content_ids_not_exe(tmp_path: Path):
    ea = load_ea()
    launch = ea.build_ea_launch(["194908"], tmp_path / "Apex")
    assert launch == "[origin2://game/launch/?offerIds=194908]"
    assert ".exe" not in launch.lower()


def test_build_ea_launch_joins_multiple_content_ids(tmp_path: Path):
    ea = load_ea()
    launch = ea.build_ea_launch(["16425782", "16425782_oa"], tmp_path / "F1 24")
    assert launch == "[origin2://game/launch/?offerIds=16425782,16425782_oa]"


def test_build_ea_launch_falls_back_to_package_id(tmp_path: Path):
    ea = load_ea()
    launch = ea.build_ea_launch([], tmp_path / "Apex", "Origin.SFT.50.0000848")
    assert "Origin.SFT.50.0000848" in launch


def test_parse_ea_game_record(tmp_path: Path):
    ea = load_ea()
    install = tmp_path / "Apex"
    inst = install / "__Installer"
    inst.mkdir(parents=True)
    (inst / "installerdata.xml").write_text(INSTALLER_XML, encoding="utf-8")
    rec = ea.parse_ea_game(
        install,
        "Apex",
        "Origin.SFT.50.0000848",
        "english",
        str(tmp_path / "ph.jpg"),
    )
    assert rec["stable_id"] == "ea:Origin.SFT.50.0000848"
    assert rec["name"] == "Apex Legends"
    assert rec["launch"] == "[origin2://game/launch/?offerIds=194908]"
    assert ".exe" not in rec["launch"].lower()


def test_find_ea_game_image_prefers_splash(tmp_path: Path):
    ea = load_ea()
    install = tmp_path / "Apex"
    inst = install / "__Installer"
    inst.mkdir(parents=True)
    (inst / "StoreLogo.png").write_bytes(b"x")
    (inst / "Splash.png").write_bytes(b"x")
    ph = str(tmp_path / "ph.jpg")
    Path(ph).write_bytes(b"\xff\xd8\xff\xd9")
    assert ea._find_ea_game_image(install, inst, ph).endswith("Splash.png")


def test_scan_ea_libraries_integration(tmp_path: Path):
    ea = load_ea()
    library = tmp_path / "EA"
    library.mkdir()
    install_data = tmp_path / "InstallData"
    game = "CoolGame"
    (install_data / game / "base-Origin.SFT.50.0001111").mkdir(parents=True)
    inst_root = library / game / "__Installer"
    inst_root.mkdir(parents=True)
    (inst_root / "installerdata.xml").write_text(
        '<contentIDs><contentID>1001111</contentID></contentIDs>'
        '<gameTitles><gameTitle locale="en_US">Cool Game</gameTitle></gameTitles>',
        encoding="utf-8",
    )
    (inst_root / "WideLogo.png").write_bytes(b"x")
    ph = str(tmp_path / "ph.jpg")
    Path(ph).write_bytes(b"\xff\xd8\xff\xd9")
    recs = ea.scan_ea_libraries(
        [str(library)],
        install_data,
        "english",
        ph,
        lambda m: None,
    )
    assert len(recs) == 1
    assert recs[0]["stable_id"] == "ea:Origin.SFT.50.0001111"
    assert recs[0]["launch"] == "[origin2://game/launch/?offerIds=1001111]"
    assert "WideLogo.png" in recs[0]["image_path"]
