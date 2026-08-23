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
