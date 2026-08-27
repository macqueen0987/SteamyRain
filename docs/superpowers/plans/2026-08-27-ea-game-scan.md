# EA App First-Class Scan Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Scan EA Desktop installed games and show them as first-class tiles merged after Steam and Xbox, launching via EA launcher URI (never direct `.exe`).

**Architecture:** Add `@Resources/ea_scan.py` mirroring `xbox_scan.py`: discover library roots from INI + Settings `EaDirs`, match `%ProgramData%\EA Desktop\InstallData\<Game>\base-*` to install folders, emit unified records, extend `merge_game_records` and `main()`. Reuse Xbox wide-art ranking logic (copy constants/helpers into `ea_scan.py` — do not refactor `xbox_scan.py` in this plan).

**Tech Stack:** Python 3 (`UpdateGames.pyw`, `ea_scan.py`), Rainmeter `.inc`, pytest.

**Spec:** `docs/superpowers/specs/2026-08-27-ea-game-scan-design.md`

## Global Constraints

- First-class tiles in **one** scroll with Steam/Xbox; **no** platform headers or Extra injection for EA.
- Stable IDs: `ea:<packageId>` (e.g. `ea:Origin.SFT.50.0000848`); hide/search key off `stable_id`.
- Launch: **launcher URI only** — `[origin2://game/launch/?offerIds=<packageId>]`; **never** `["...\game.exe"]`.
- Explorer fallback: `[explorer "<install_dir>"]` + status warn when no package id.
- Merge order: **Steam → Xbox → EA**.
- DLC: skip InstallData `dlc-*` only entries; do not emit separate DLC tiles.
- Artwork: local only; wide/splash preferred; shallow depth-limited file walk; placeholder `@Resources/img/placeholder_game.jpg`.
- Settings: `EaDirs` comma list + Browse (mirror `XboxDirs` UX).
- No unbounded `rglob` on multi-GB install trees.
- Out of scope: IGDB, legacy Origin-only, OFR↔SFT remapping table, direct exe.
- Tests: `python -m pytest` from repo root.
- Project default execution: **Subagent-Driven Development** (do not ask Inline vs Subagent).

---

## File structure

| File | Responsibility |
|------|----------------|
| `@Resources/ea_scan.py` | EA root discovery, InstallData parse, per-game records, `scan_ea_libraries` |
| `@Resources/UpdateGames.pyw` | Import ea_scan; 3-way merge; `main()` wiring; `resolve_meter_image_name` for `ea:` |
| `@Resources/SkinInfo.inc` | Add `EaDirs=` |
| `Settings/tabs/TabPaths.inc` | `EaDirs` row after XboxDirs; shift rows below |
| `Settings/Settings.ini` | FileChoose routing for `EaDirs`; browse visibility |
| `tests/test_ea_scan.py` | EA fixtures and unit tests |
| `tests/test_update_games.py` | Extend merge test for EA |
| `README.md` | EA scan / `EaDirs` note |

---

### Task 1: EA library root discovery

**Files:**
- Create: `@Resources/ea_scan.py`
- Create: `tests/test_ea_scan.py`

**Interfaces:**
- Produces:
  - `read_ea_ini_value(ini_path: Path, key_suffix: str) -> str | None` — case-insensitive match lines like `machine.downloadinplacedir=C:\...` or `user.downloadinplacedir=E:\EA\`
  - `discover_ea_roots(user_dirs: list[str], programdata: Path | None = None, localappdata: Path | None = None, default_ea_games: str | None = None) -> list[str]` — user dirs first, then INI paths, then default `C:\Program Files\EA Games` if exists; dedupe case-insensitively; normalize trailing `\`

- [ ] **Step 1: Write failing tests**

```python
# tests/test_ea_scan.py
from __future__ import annotations
import importlib.util
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
EA_SCAN = ROOT / "@Resources" / "ea_scan.py"

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
    assert ea.read_ea_ini_value(ini, "downloadinplacedir") == r"E:\EA\"

def test_discover_ea_roots_user_ini_and_default(tmp_path: Path):
    ea = load_ea()
    user = tmp_path / "CustomEA"
    user.mkdir()
    default = tmp_path / "DefaultEA"
    default.mkdir()
    pd = tmp_path / "ProgramData" / "EA Desktop"
    pd.mkdir(parents=True)
    (pd / "machine.ini").write_text(f"machine.downloadinplacedir={default}\n", encoding="utf-8")
    la = tmp_path / "Local" / "Electronic Arts" / "EA Desktop"
    la.mkdir(parents=True)
    (la / "user_1.ini").write_text(f"user.downloadinplacedir={user}\n", encoding="utf-8")
    roots = ea.discover_ea_roots(
        [str(user)],
        programdata=pd.parent.parent / "ProgramData",
        localappdata=la.parent.parent.parent / "Local",
        default_ea_games=str(default),
    )
    # Fix paths: pass programdata=tmp_path/"ProgramData", localappdata=tmp_path/"Local"
    roots = ea.discover_ea_roots(
        [str(user)],
        programdata=tmp_path / "ProgramData",
        localappdata=tmp_path / "Local",
        default_ea_games=str(default),
    )
    assert roots[0] == str(user.resolve())
    assert any(Path(r).resolve() == default.resolve() for r in roots)
```

Adjust test paths so `discover_ea_roots` looks at `programdata / "EA Desktop" / "machine.ini"` and `localappdata / "Electronic Arts" / "EA Desktop" / user_*.ini`.

- [ ] **Step 2: Run tests — expect FAIL**

Run: `python -m pytest tests/test_ea_scan.py::test_read_ea_ini_value_download_dir tests/test_ea_scan.py::test_discover_ea_roots_user_ini_and_default -v`

- [ ] **Step 3: Implement discovery in `ea_scan.py`**

```python
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

def discover_ea_roots(user_dirs, programdata=None, localappdata=None, default_ea_games=None):
    import os
    if programdata is None:
        programdata = Path(os.environ.get("PROGRAMDATA", r"C:\ProgramData"))
    if localappdata is None:
        localappdata = Path(os.environ.get("LOCALAPPDATA", ""))
    if default_ea_games is None:
        default_ea_games = r"C:\Program Files\EA Games"
    roots, seen = [], set()
    def add(candidate):
        if not candidate:
            return
        resolved = str(Path(candidate).resolve())
        key = resolved.casefold()
        if key in seen or not Path(resolved).is_dir():
            return
        seen.add(key)
        roots.append(resolved)
    for d in user_dirs:
        add(d.strip())
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
```

- [ ] **Step 4: Run tests — PASS**

- [ ] **Step 5: Commit**

```bash
git add @Resources/ea_scan.py tests/test_ea_scan.py
git commit -m "feat(ea): discover library roots from EaDirs and EA Desktop INI"
```

---

### Task 2: InstallData base package parsing

**Files:**
- Modify: `@Resources/ea_scan.py`
- Modify: `tests/test_ea_scan.py`

**Interfaces:**
- Consumes: `discover_ea_roots`
- Produces:
  - `INSTALL_DATA_DIRNAME = "InstallData"` (under `%ProgramData%/EA Desktop/`)
  - `list_installdata_games(install_data_root: Path) -> list[dict]` — each `{ "folder_name": str, "package_id": str | None, "is_base": bool }` where `package_id` from `base-Origin.SFT.50.NNNN` → `Origin.SFT.50.NNNN`; entries with only `dlc-*` omitted

- [ ] **Step 1: Failing tests**

```python
def test_list_installdata_games_base_only(tmp_path: Path):
    ea = load_ea()
    root = tmp_path / "InstallData"
    apex = root / "Apex"
    (apex / "base-Origin.SFT.50.0000848").mkdir(parents=True)
    (apex / "dlc-Origin.SFT.50.0000999").mkdir()
    bf = root / "BF6"
    (bf / "dlc-Origin.SFT.50.0001523").mkdir(parents=True)  # DLC-only → skip
    games = ea.list_installdata_games(root)
    assert len(games) == 1
    assert games[0]["folder_name"] == "Apex"
    assert games[0]["package_id"] == "Origin.SFT.50.0000848"

def test_package_id_from_base_folder():
    ea = load_ea()
    assert ea.package_id_from_base_folder("base-Origin.SFT.50.0000848") == "Origin.SFT.50.0000848"
    assert ea.package_id_from_base_folder("dlc-Origin.SFT.50.0001523") is None
```

- [ ] **Step 2: Run — FAIL; implement `package_id_from_base_folder`, `list_installdata_games`; PASS; commit**

```bash
git commit -m "feat(ea): parse InstallData base packages and skip DLC-only titles"
```

---

### Task 3: Installer metadata, launch URI, stable_id (no exe)

**Files:**
- Modify: `@Resources/ea_scan.py`
- Modify: `tests/test_ea_scan.py`

**Interfaces:**
- Produces:
  - `LOCALE_TO_EA = {"english": "en_US", "koreana": "ko_KR", "french": "fr_FR", "german": "de_DE", "italian": "it_IT", "spanish": "es_ES", "japanese": "ja_JP", "polish": "pl_PL", "portuguese": "pt_BR", "russian": "ru_RU", "chinese": "zh_CN", "tchinese": "zh_TW", "arabic": "ar_SA"}`
  - `parse_installerdata_xml(path: Path, locale: str = "english") -> dict` — `{ "name": str|None, "content_id": str|None }` from `<gameTitle locale="en_US">` and `<contentID>`
  - `build_ea_launch(package_id: str | None, install_dir: Path) -> str` — if package_id: `[origin2://game/launch/?offerIds={package_id}]` else `[explorer "{install_dir}"]`
  - `parse_ea_game(install_dir: Path, folder_name: str, package_id: str | None, locale: str, placeholder: str) -> dict | None`

- [ ] **Step 1: Failing tests**

```python
INSTALLER_XML = """<?xml version="1.0"?>
<install>
  <contentIDs><contentID>194908</contentID></contentIDs>
  <gameTitles>
    <gameTitle locale="en_US">Apex Legends</gameTitle>
    <gameTitle locale="ko_KR">Apex 레전드</gameTitle>
  </gameTitles>
</install>"""

def test_parse_installerdata_xml_locale(tmp_path: Path):
    ea = load_ea()
    p = tmp_path / "installerdata.xml"
    p.write_text(INSTALLER_XML, encoding="utf-8")
    en = ea.parse_installerdata_xml(p, "english")
    assert en["name"] == "Apex Legends"
    assert en["content_id"] == "194908"
    ko = ea.parse_installerdata_xml(p, "koreana")
    assert ko["name"] == "Apex 레전드"

def test_build_ea_launch_origin2_not_exe(tmp_path: Path):
    ea = load_ea()
    launch = ea.build_ea_launch("Origin.SFT.50.0000848", tmp_path / "Apex")
    assert launch.startswith("[origin2://")
    assert ".exe" not in launch.lower()
    assert "Origin.SFT.50.0000848" in launch

def test_parse_ea_game_record(tmp_path: Path):
    ea = load_ea()
    install = tmp_path / "Apex"
    inst = install / "__Installer"
    inst.mkdir(parents=True)
    (inst / "installerdata.xml").write_text(INSTALLER_XML, encoding="utf-8")
    rec = ea.parse_ea_game(install, "Apex", "Origin.SFT.50.0000848", "english", str(tmp_path / "ph.jpg"))
    assert rec["stable_id"] == "ea:Origin.SFT.50.0000848"
    assert rec["name"] == "Apex Legends"
    assert "origin2://" in rec["launch"]
    assert ".exe" not in rec["launch"].lower()
```

- [ ] **Step 2: Implement; run tests; commit**

```bash
git commit -m "feat(ea): build launcher URI records from InstallData and installerdata.xml"
```

---

### Task 4: Wide art + `scan_ea_libraries`

**Files:**
- Modify: `@Resources/ea_scan.py`
- Modify: `tests/test_ea_scan.py`

**Interfaces:**
- Produces:
  - `_find_ea_game_image(install_dir: Path, installer_dir: Path, placeholder: str) -> str` — shallow walk (max depth 2) on `install_dir` and `installer_dir`; rank wide tokens (`wide`, `splash`, `header`, `hero`, `banner`, `poster`) over square (`square`, `store`, `small`); **no rglob**
  - `scan_ea_libraries(roots: list[str], install_data_root: Path, locale: str, placeholder: str, status_fn) -> list[dict]` — for each InstallData game with matching `roots/<folder_name>/__Installer/installerdata.xml`, call `parse_ea_game`; try/except per game; status on skip

- [ ] **Step 1: Failing tests**

```python
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
        '<gameTitles><gameTitle locale="en_US">Cool Game</gameTitle></gameTitles>',
        encoding="utf-8",
    )
    (inst_root / "WideLogo.png").write_bytes(b"x")
    ph = str(tmp_path / "ph.jpg")
    Path(ph).write_bytes(b"\xff\xd8\xff\xd9")
    recs = ea.scan_ea_libraries([str(library)], install_data, "english", ph, lambda m: None)
    assert len(recs) == 1
    assert recs[0]["stable_id"] == "ea:Origin.SFT.50.0001111"
    assert "WideLogo.png" in recs[0]["image_path"]
```

- [ ] **Step 2: Implement `scan_ea_libraries`; PASS; commit**

```bash
git commit -m "feat(ea): scan installed games with wide art and shallow image search"
```

---

### Task 5: Wire EA into `UpdateGames.pyw`

**Files:**
- Modify: `@Resources/UpdateGames.pyw`
- Modify: `tests/test_update_games.py`

**Interfaces:**
- Consumes: `discover_ea_roots`, `scan_ea_libraries`, default install data path `Path(os.environ["PROGRAMDATA"]) / "EA Desktop" / "InstallData"`
- Produces:
  - `merge_game_records(steam_records, xbox_records, ea_records=None)` → steam + xbox + (ea or [])
  - `main()` block after Xbox scan:

```python
from ea_scan import discover_ea_roots, scan_ea_libraries
# sys.path already includes script_dir from xbox import

eadirs_raw = variables.get('eadirs', '')
ea_user = [d.strip() for d in eadirs_raw.split(',') if d.strip()]
ea_roots = discover_ea_roots(ea_user)
install_data = Path(os.environ.get("PROGRAMDATA", r"C:\ProgramData")) / "EA Desktop" / "InstallData"
if not ea_roots:
    update_rainmeter_status("EA: no libraries found")
ea_records = scan_ea_libraries(ea_roots, install_data, locale, PLACEHOLDER_IMAGE, update_rainmeter_status)
if ea_roots and not ea_records:
    update_rainmeter_status("EA: no games found")
records = merge_game_records(steam_records, xbox_records, ea_records)
```

- Update `resolve_meter_image_name`: treat `stable_id.startswith("ea:")` like `xbox:` (use baked logo path for Icon mode).

- [ ] **Step 1: Update merge test**

```python
def test_merge_game_records_steam_xbox_ea_order(ug):
    steam = [{"stable_id": "steam:1", "name": "A", "launch": "[steam://rungameid/1]", "image_path": "a"}]
    xbox = [{"stable_id": "xbox:Z", "name": "B", "launch": "[explorer \"x\"]", "image_path": "b"}]
    ea = [{"stable_id": "ea:Origin.SFT.50.1", "name": "C", "launch": "[origin2://game/launch/?offerIds=Origin.SFT.50.1]", "image_path": "c"}]
    assert ug.merge_game_records(steam, xbox, ea) == steam + xbox + ea
```

- [ ] **Step 2: Implement; `python -m pytest tests/ -v`; commit**

```bash
git commit -m "feat(scan): merge EA records into Scan for Games pipeline"
```

---

### Task 6: Settings `EaDirs` + SkinInfo

**Files:**
- Modify: `@Resources/SkinInfo.inc` — add `EaDirs=` under PATHS (after `XboxDirs`)
- Modify: `Settings/tabs/TabPaths.inc` — row comment becomes `0 Steam | 1 GameDirs | 2 XboxDirs | 3 EaDirs | 4 Rainmeter | 5 Locale presets | 6 Locale value`; duplicate XboxDirs block for EaDirs; shift Y multipliers for rows below
- Modify: `Settings/Settings.ini` — `_EaDirsMode=append`; chain `PathPickRouterEa` after Xbox router; append/replace measures; add EaDirs browse meters to `PathsBrowseGate`

- [ ] **Step 1: Implement TabPaths + Settings.ini (mirror XboxDirs exactly)**
- [ ] **Step 2: Verify `#@#SkinInfo.inc` writes use variable `EaDirs`**
- [ ] **Step 3: Commit**

```bash
git commit -m "feat(settings): add EaDirs path field with browse/append"
```

---

### Task 7: Search + README + verification

**Files:**
- Modify: `README.md` — Setup/Paths: EA auto-detect + `EaDirs`; merge order; search by `ea:...` stable id
- Verify: `@Resources/SearchGames.lua` already routes `:` input to exact ID match — **no code change** unless test shows gap; if so, add comment in commit body only

- [ ] **Step 1: Full pytest**

Run: `python -m pytest tests/ -v`  
Expected: all PASS

- [ ] **Step 2: Live probe (optional manual)**

```python
from ea_scan import discover_ea_roots, scan_ea_libraries
from pathlib import Path
import os
roots = discover_ea_roots([])
ph = r"@Resources/img/placeholder_game.jpg"
idroot = Path(os.environ["PROGRAMDATA"]) / "EA Desktop" / "InstallData"
recs = scan_ea_libraries(roots, idroot, "koreana", ph, print)
print(len(recs), [r["name"] for r in recs])
```

Expect Apex, Battlefield 6, F1 24 on this machine; no Voice Pack / dlc-only rows; scan < 5s.

- [ ] **Step 3: README update; commit if not empty**

```bash
git commit -m "docs(readme): note EA App scan and EaDirs settings"
```

---

## Self-review (plan vs spec)

| Spec requirement | Task |
|------------------|------|
| `ea_scan.py` source | 1–4 |
| InstallData `base-*` / skip `dlc-*` | 2, 4 |
| Match install dir + `__Installer` | 4 |
| Launcher URI, no exe | 3, 5 |
| Explorer fallback | 3 |
| `ea:` stable_id | 3 |
| Merge Steam→Xbox→EA | 5 |
| `EaDirs` Settings | 6 |
| Wide art, shallow walk | 4 |
| Search `ea:` ids | 7 (existing colon branch) |
| Per-game error isolation | 4 (`try/except`) |
| Status messages | 5 |
| Extra unchanged | 5 (no Extra changes) |
| IGDB / Origin-only / OFR map | Global Constraints out of scope |

**Placeholder scan:** none intentional.  
**Type consistency:** record keys always `stable_id`, `name`, `launch`, `image_path`; `scan_ea_libraries(roots, install_data_root, locale, placeholder, status_fn)`.

---

## Execution handoff

Plan saved to `docs/superpowers/plans/2026-08-27-ea-game-scan.md`.

Per project rules, implementation uses **Subagent-Driven Development**. Say **ㄱ** to start execution.
