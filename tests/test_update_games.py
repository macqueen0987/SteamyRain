from __future__ import annotations

import importlib.util
import sys
from pathlib import Path

import pytest


ROOT = Path(__file__).resolve().parents[1]
UPDATE_GAMES_PATH = ROOT / "@Resources" / "UpdateGames.pyw"


def load_update_games():
    spec = importlib.util.spec_from_file_location("update_games", UPDATE_GAMES_PATH)
    module = importlib.util.module_from_spec(spec)
    assert spec.loader is not None
    sys.modules["update_games"] = module
    spec.loader.exec_module(module)
    return module


@pytest.fixture
def ug():
    return load_update_games()


@pytest.fixture
def steam_library(tmp_path: Path):
    steamapps = tmp_path / "SteamLibrary" / "steamapps"
    steamapps.mkdir(parents=True)

    manifests = {
        "appmanifest_111.acf": '"name"\t\t"Fixture Game"',
        "appmanifest_222.acf": '"name"\t\t"Proton Experimental"',
        "appmanifest_228980.acf": '"name"\t\t"Steamworks Common Redistributables"',
        "appmanifest_333.acf": '"name"\t\t"Linux Runtime Soldier"',
        "appmanifest_bad.acf": '"name"\t\t"Not Numeric"',
    }
    for name, name_line in manifests.items():
        (steamapps / name).write_text(
            f'"AppState"\n{{\n\t"appid"\t\t"{name[12:-4]}"\n\t{name_line}\n}}\n',
            encoding="utf-8",
        )
    return steamapps


def test_normalize_stable_id_migrates_numeric_and_keeps_prefixed(ug):
    assert ug.normalize_stable_id("730") == "steam:730"
    assert ug.normalize_stable_id('"730"') == "steam:730"
    assert ug.normalize_stable_id("steam:730") == "steam:730"
    assert ug.normalize_stable_id("xbox:Foo") == "xbox:Foo"


def test_scan_steam_libraries_returns_prefixed_records(ug, steam_library, monkeypatch):
    monkeypatch.setattr(ug, "update_rainmeter_status", lambda msg: None)
    # scan_steam_libraries expects steamapps directories (same as main()'s loop)
    records = ug.scan_steam_libraries(
        [str(steam_library)],
        r"C:\Steam\appcache\librarycache",
        ug.update_rainmeter_status,
    )
    assert len(records) == 1
    assert records[0]["stable_id"] == "steam:111"
    assert records[0]["name"] == "Fixture Game"
    assert records[0]["launch"] == "[steam://rungameid/111]"


def test_process_appmanifest_keeps_real_games_and_skips_runtime(ug, steam_library, monkeypatch):
    monkeypatch.setattr(ug, "update_rainmeter_status", lambda msg: None)

    files = sorted(p.name for p in steam_library.glob("appmanifest_*.acf"))
    ids, info = ug.process_appmanifest_files(files, str(steam_library))

    assert ids == ["111"]
    assert info == [{"appid": "111", "name": "Fixture Game", "image": ""}]


def test_get_image_for_game_prefers_png_header_candidates(ug, tmp_path: Path, monkeypatch):
    monkeypatch.setattr(ug, "locale", "koreana", raising=False)

    app_dir = tmp_path / "111"
    app_dir.mkdir()
    (app_dir / "library_header.png").write_bytes(b"fake-png")

    assert ug.get_image_for_game(str(tmp_path), "111") == "library_header.png"


def test_get_image_for_game_falls_back_to_any_image(ug, tmp_path: Path, monkeypatch):
    monkeypatch.setattr(ug, "locale", "koreana", raising=False)

    app_dir = tmp_path / "111"
    nested = app_dir / "hashfolder"
    nested.mkdir(parents=True)
    (nested / "custom_art.jpg").write_bytes(b"fake-jpg")

    assert ug.get_image_for_game(str(tmp_path), "111") == "hashfolder/custom_art.jpg"


def test_write_game_info_preserves_hidden_across_steam_prefix_migration(ug, tmp_path: Path, monkeypatch):
    games_info = tmp_path / "GamesInfo.inc"
    games_info.write_text(
        "\n".join([
            "[Variables]",
            "GameCount=2",
            "GameCountPLUS=1",
            "ID1=111",
            'ID1name="Old Name"',
            "Vis1=0",
            "ID2=999",
            'ID2name="Hidden Game"',
            "Vis2=1",
            "",
        ]),
        encoding="utf-8",
    )
    monkeypatch.setattr(ug, "__file__", str(tmp_path / "UpdateGames.pyw"))

    ug.write_game_info([
        {"stable_id": "steam:999", "name": "Hidden Game"},
        {"stable_id": "steam:111", "name": "Fixture Game"},
    ])

    text = games_info.read_text(encoding="utf-8")
    assert "ID1=steam:999" in text
    assert "Vis1=1" in text
    assert "ID2=steam:111" in text
    assert "Vis2=0" in text
    assert "GameCountPLUS=1" in text


def test_create_meter_uses_baked_launch_and_image(ug, monkeypatch):
    monkeypatch.setattr(ug, "locale", "koreana", raising=False)
    meter = ug.create_meter(
        "ID1", 1, "Logo", False, False,
        launch="[steam://rungameid/111]",
        image_name=r"C:\Steam\appcache\librarycache\111\header.jpg",
    )
    assert meter["Image"]["LeftMouseUpAction"] == "[steam://rungameid/111]"
    assert meter["Name"]["LeftMouseUpAction"] == "[steam://rungameid/111]"
    assert meter["Image"]["ImageName"] == r"C:\Steam\appcache\librarycache\111\header.jpg"


def test_fill_steam_image_paths_sets_absolute_or_placeholder(ug, tmp_path: Path, monkeypatch):
    monkeypatch.setattr(ug, "locale", "koreana", raising=False)
    monkeypatch.setattr(ug, "PLACEHOLDER_IMAGE", str(tmp_path / "placeholder_game.jpg"), raising=False)
    (tmp_path / "placeholder_game.jpg").write_bytes(b"\xff\xd8\xff\xd9")
    cache = tmp_path / "cache"
    app = cache / "111"
    app.mkdir(parents=True)
    (app / "library_header.png").write_bytes(b"fake")
    records = [{"stable_id": "steam:111", "name": "G", "launch": "[steam://rungameid/111]", "image_path": ""}]
    ug.fill_steam_image_paths(records, str(cache))
    assert records[0]["image_path"].endswith("library_header.png")


def test_create_meter_includes_gap_and_steam_launch(ug, monkeypatch):
    monkeypatch.setattr(ug, "locale", "koreana", raising=False)
    meter = ug.create_meter(
        "ID1", 1, "Logo", False, False,
        launch="[steam://rungameid/111]",
        image_name=r"C:\Steam\appcache\librarycache\111\header.jpg",
    )

    assert meter["Gap"]["MeterStyle"] == "GapStyle"
    assert meter["Image"]["LeftMouseUpAction"] == "[steam://rungameid/111]"
    assert meter["Image"]["ImageName"] == r"C:\Steam\appcache\librarycache\111\header.jpg"


def test_case_sensitive_parser_keeps_key_case_and_hashes(ug, tmp_path: Path):
    cfg_path = tmp_path / "sample.inc"
    cfg_path.write_text(
        "[Variables]\nEgame1Path=\"C:\\Games\\App#1.exe\"\nPercent=100%\n",
        encoding="utf-8",
    )

    parser = ug.CaseSensitiveConfigParser()
    parser.read(cfg_path, encoding="utf-8")

    assert parser.get("Variables", "Egame1Path") == '"C:\\Games\\App#1.exe"'
    assert parser.get("Variables", "Percent") == "100%"
    assert "egame1path" not in parser["Variables"]


def test_iter_extra_game_indices_skips_empty_slots(ug):
    vars_ = {
        "Egame1": "EndField",
        "Egame2": "",
        "Egame3": "Nikke",
    }
    assert list(ug.iter_extra_game_indices(vars_, 3)) == [1, 3]


def test_merge_steam_then_xbox_order(ug):
    steam = [{"stable_id": "steam:1", "name": "A", "launch": "[steam://rungameid/1]", "image_path": "a"}]
    xbox = [{"stable_id": "xbox:Z", "name": "B", "launch": '[explorer "x"]', "image_path": "b"}]
    assert ug.merge_game_records(steam, xbox) == steam + xbox


def test_xbox_scan_importable_from_update_games_path(ug):
    import xbox_scan

    assert hasattr(xbox_scan, "discover_xbox_roots")
    assert hasattr(xbox_scan, "scan_xbox_libraries")
