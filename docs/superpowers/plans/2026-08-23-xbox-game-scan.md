# Xbox PC First-Class Scan Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Scan installed Xbox PC / Game Pass PC games and show them as first-class tiles in the same SteamyRain list as Steam (local art only; Settings `XboxDirs` + auto-detect).

**Architecture:** Introduce a shared game record `{stable_id, name, launch, image_path}`. Refactor Steam scanning to emit `steam:<appid>` records with **baked** launch/image paths (Rainmeter `#ID#` substitution must not be used for launch/image once IDs are prefixed). Add `@Resources/xbox_scan.py` for roots + per-game parse. Merge Steam then Xbox in `UpdateGames.pyw`, preserve hide state across legacy numeric IDs, and add Paths UI for `XboxDirs`.

**Tech Stack:** Python 3 (UpdateGames.pyw + xbox_scan.py), Rainmeter `.inc`/Settings, pytest.

**Spec:** `docs/superpowers/specs/2026-08-23-xbox-game-scan-design.md`

## Global Constraints

- First-class tiles in **one** scroll with Steam; **no** platform headers or Extra injection for Xbox.
- Stable IDs: `steam:<appid>`, `xbox:<key>`; migrate legacy numeric GamesInfo IDs → `steam:<appid>` for Vis.
- Launch/image written into meters from the in-memory record (no `ID{n}Launch` in v1).
- Xbox art: local only; missing → `@Resources/img/placeholder_game.jpg`.
- Xbox roots: `XboxDirs` ∪ `.GamingRoot` ∪ `C:\XboxGames` (dedupe).
- No AUMID/WindowsApps ACL bypass; if no exe/AUMID → `explorer "<install_dir>"` + Update status warn.
- Out of scope: EA, IGDB/remote art, sort/filter by platform.
- Tests: `python -m pytest` from repo root (dev deps: `requirements-dev.txt`).
- Project default execution: **Subagent-Driven Development** (do not ask Inline vs Subagent).

---

## File structure

| File | Responsibility |
|------|----------------|
| `@Resources/xbox_scan.py` | Discover Xbox library roots; parse games → record list |
| `@Resources/UpdateGames.pyw` | Steam→records, merge, GamesInfo/meters writers, `main()` |
| `@Resources/img/placeholder_game.jpg` | Shared fallback tile art |
| `@Resources/SkinInfo.inc` | Add `XboxDirs=` |
| `Settings/tabs/TabPaths.inc` | Xbox Dirs row (after Game Dirs) |
| `Settings/Settings.ini` | FileChoose append/replace for `XboxDirs`; browse visibility |
| `@Resources/SearchGames.lua` | Match `steam:730` when user types `730` or full stable_id |
| `tests/test_xbox_scan.py` | Xbox fixtures |
| `tests/test_update_games.py` | Record/migration/meter/search-related updates |
| `README.md` | Xbox path / scan note |

---

### Task 1: Shared record helpers + Steam scan emits `steam:` records

**Files:**
- Modify: `@Resources/UpdateGames.pyw`
- Modify: `tests/test_update_games.py`
- Test: `tests/test_update_games.py`

**Interfaces:**
- Consumes: existing `process_appmanifest_files`
- Produces:
  - `normalize_stable_id(raw: str) -> str` — digit-only → `steam:{raw}`; else strip quotes/whitespace and return
  - `steam_launch(appid: str) -> str` — returns `[steam://rungameid/{appid}]`
  - `scan_steam_libraries(game_dirs: list[str], library_cache: str, status_fn) -> list[dict]` — each dict: `stable_id`, `name`, `launch`, `image_path` (may be `""` until image resolve task; for this task set `image_path` to `""` and `launch` via `steam_launch`)

- [ ] **Step 1: Write the failing tests**

Add to `tests/test_update_games.py`:

```python
def test_normalize_stable_id_migrates_numeric_and_keeps_prefixed(ug):
    assert ug.normalize_stable_id("730") == "steam:730"
    assert ug.normalize_stable_id('"730"') == "steam:730"
    assert ug.normalize_stable_id("steam:730") == "steam:730"
    assert ug.normalize_stable_id("xbox:Foo") == "xbox:Foo"


def test_scan_steam_libraries_returns_prefixed_records(ug, steam_library, monkeypatch):
    monkeypatch.setattr(ug, "update_rainmeter_status", lambda msg: None)
    records = ug.scan_steam_libraries([str(steam_library.parent)], r"C:\Steam\appcache\librarycache", ug.update_rainmeter_status)
    assert len(records) == 1
    assert records[0]["stable_id"] == "steam:111"
    assert records[0]["name"] == "Fixture Game"
    assert records[0]["launch"] == "[steam://rungameid/111]"
```

Note: `scan_steam_libraries` receives dirs that already end with `steamapps` **or** library roots — match current `main()` which appends `\steamapps`. In the test, pass `[str(steam_library)]` (the `steamapps` path) and document that the function expects **steamapps directories**, same as today's loop.

Update `test_process_appmanifest_...` only if you change its return shape; prefer leaving it returning `(ids, info)` and wrapping in `scan_steam_libraries`.

- [ ] **Step 2: Run tests to verify they fail**

Run: `python -m pytest tests/test_update_games.py::test_normalize_stable_id_migrates_numeric_and_keeps_prefixed tests/test_update_games.py::test_scan_steam_libraries_returns_prefixed_records -v`

Expected: FAIL (functions missing)

- [ ] **Step 3: Minimal implementation in `UpdateGames.pyw`**

```python
def normalize_stable_id(raw: str) -> str:
    value = str(raw).strip().strip('"')
    if value.isdigit():
        return f"steam:{value}"
    return value


def steam_launch(appid: str) -> str:
    return f"[steam://rungameid/{appid}]"


def scan_steam_libraries(steamapps_dirs, library_cache, status_fn):
    records = []
    for game_dir in steamapps_dirs:
        status_fn(f"Processing files of {game_dir}")
        if not os.path.isdir(game_dir):
            status_fn(f"Missing library: {game_dir}")
            continue
        appmanifest_files = [f for f in os.listdir(game_dir) if f.startswith("appmanifest_")]
        processed_ids, games_info = process_appmanifest_files(appmanifest_files, game_dir)
        for app_id, info in zip(processed_ids, games_info):
            records.append({
                "stable_id": f"steam:{app_id}",
                "name": info["name"],
                "launch": steam_launch(app_id),
                "image_path": "",
            })
    return records
```

(Do not rewrite `main()` yet.)

- [ ] **Step 4: Run tests to verify they pass**

Run: `python -m pytest tests/test_update_games.py::test_normalize_stable_id_migrates_numeric_and_keeps_prefixed tests/test_update_games.py::test_scan_steam_libraries_returns_prefixed_records tests/test_update_games.py::test_process_appmanifest_keeps_real_games_and_skips_runtime -v`

Expected: PASS

- [ ] **Step 5: Commit**

```bash
git add @Resources/UpdateGames.pyw tests/test_update_games.py
git commit -m "feat(scan): add stable_id helpers and Steam record scan"
```

---

### Task 2: `write_game_info` uses records + migrates hidden IDs

**Files:**
- Modify: `@Resources/UpdateGames.pyw` (`write_game_info`)
- Modify: `tests/test_update_games.py`

**Interfaces:**
- Consumes: `normalize_stable_id`, record list
- Produces: `write_game_info(records: list[dict]) -> None` — writes `ID{n}={stable_id}`, names, Vis; preserves hide when existing ID (normalized) matches

- [ ] **Step 1: Write failing / update tests**

Replace `test_write_game_info_preserves_hidden_by_appid` to cover migration:

```python
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
```

- [ ] **Step 2: Run test — expect FAIL** (old signature / no prefix)

Run: `python -m pytest tests/test_update_games.py::test_write_game_info_preserves_hidden_across_steam_prefix_migration -v`

- [ ] **Step 3: Implement `write_game_info(records)`**

Rewrite to:
1. Read existing `GamesInfo.inc` if present.
2. Build `existing_hidden_ids: set[str]` from pairs `(normalize_stable_id(IDn), Visn==1)`.
3. Write `GameCount=len(records)`, `GameCountPLUS=count of non-hidden`, and for each record `ID{i}`, `ID{i}name`, `Vis{i}` using `stable_id` / `name`.

Remove the old `(processed_ids, games_info)` parameters.

- [ ] **Step 4: Run related tests**

Run: `python -m pytest tests/test_update_games.py -v`

Expected: PASS for write_game_info + prior Task 1 tests. Fix any callers in the same file still using the old signature (only tests/`main` — `main` still old until Task 5; if `main` breaks import-time only, leave until Task 5 but keep functions importable).

If `main()` still calls old `write_game_info`, temporarily keep a thin wrapper **or** update `main` call in Task 5 only and accept that running the skin mid-plan is broken — prefer updating the `main()` call site to `write_game_info(records)` with `records=[]` placeholder only if needed. Cleaner: change signature now and update `main()` to build records via `scan_steam_libraries` already (Steam-only) so the skin keeps working.

Minimal `main()` steam-only bridge in this task:

```python
# replace processed_ids/games_info accumulation with:
records = scan_steam_libraries(game_dirs, image_path, update_rainmeter_status)
write_game_info(records)
game_count = len(records)
# keep global processed_ids for get_image_for_game until Task 3:
processed_ids = [r["stable_id"].split(":", 1)[1] for r in records if r["stable_id"].startswith("steam:")]
```

(Image meters may still be wrong until Task 3 — acceptable for one commit if tests pass.)

- [ ] **Step 5: Commit**

```bash
git add @Resources/UpdateGames.pyw tests/test_update_games.py
git commit -m "feat(scan): write GamesInfo with steam: stable_ids and Vis migration"
```

---

### Task 3: Bake launch + image into `create_meter` / `write_meters`

**Files:**
- Modify: `@Resources/UpdateGames.pyw`
- Modify: `tests/test_update_games.py`
- Create: `@Resources/img/placeholder_game.jpg` (tiny valid JPEG; commit it)

**Interfaces:**
- Consumes: record `launch`, `image_path`
- Produces:
  - `resolve_steam_image(library_cache: str, appid: str) -> str` — absolute path to chosen file, or `""`
  - `create_meter(..., launch: str | None = None, image_name: str | None = None)` — for non-extra, use `launch` for both click actions; use `image_name` as full `ImageName` (not `#ID#\\...`)
  - `write_meters(..., records: list[dict], ...)` — iterate `records` for main tiles
  - `fill_steam_image_paths(records, library_cache) -> None` — mutates steam records' `image_path`

**Critical:** After `ID{n}=steam:730`, Rainmeter `#ID1#` must **not** appear inside `steam://` or librarycache paths.

- [ ] **Step 1: Add placeholder JPEG**

Create a minimal JPEG at `@Resources/img/placeholder_game.jpg` (e.g. 1×1 or small solid). Any valid `.jpg` bytes are fine.

Define constant in `UpdateGames.pyw`:

```python
PLACEHOLDER_IMAGE = os.path.join(os.path.dirname(os.path.abspath(__file__)), "img", "placeholder_game.jpg")
```

- [ ] **Step 2: Failing tests**

```python
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
    (tmp_path / "placeholder_game.jpg").write_bytes(
        b"\xff\xd8\xff\xd9"
    )  # minimal JPEG SOI/EOI; replace with real tiny jpg if Rainmeter needs fuller file — tests only check path
    cache = tmp_path / "cache"
    app = cache / "111"
    app.mkdir(parents=True)
    (app / "library_header.png").write_bytes(b"fake")
    records = [{"stable_id": "steam:111", "name": "G", "launch": "[steam://rungameid/111]", "image_path": ""}]
    ug.fill_steam_image_paths(records, str(cache))
    assert records[0]["image_path"].endswith("library_header.png")
```

Update/remove `test_create_meter_includes_gap_and_steam_launch` to the baked API (drop old `image_path` positional arg for library cache).

Refactor `get_image_for_game` to take `appid: str` instead of relying on global `processed_ids` + `id_key`, **or** keep it but only call from `fill_steam_image_paths` with appid. Prefer:

```python
def get_image_for_game(image_path, appid: str) -> str:
    # same search logic, use appid directly instead of processed_ids[idx]
```

Update existing get_image tests accordingly.

- [ ] **Step 3: Run tests — expect FAIL**

Run: `python -m pytest tests/test_update_games.py::test_create_meter_uses_baked_launch_and_image tests/test_update_games.py::test_fill_steam_image_paths_sets_absolute_or_placeholder -v`

- [ ] **Step 4: Implement**

- Change `create_meter` signature to remove Steam-only `image_path` library root for non-extra; add `launch`, `image_name`.
- Extra branch unchanged (`#{id_key}Path#`, `ELogo` paths).
- `write_meters`: for `i, record in enumerate(records, 1): create_meter(..., launch=record["launch"], image_name=record["image_path"] or PLACEHOLDER_IMAGE)`.
- `fill_steam_image_paths`: for each `steam:` record, set `image_path` to `os.path.join(cache, appid, filename)` or `PLACEHOLDER_IMAGE`.
- Wire `main()`: after steam scan, `fill_steam_image_paths(records, image_path)` then `write_game_info` / `write_meters`.

- [ ] **Step 5: Full unit tests**

Run: `python -m pytest tests/test_update_games.py -v`

Expected: PASS

- [ ] **Step 6: Commit**

```bash
git add @Resources/UpdateGames.pyw @Resources/img/placeholder_game.jpg tests/test_update_games.py
git commit -m "feat(meters): bake launch and image paths for stable_id tiles"
```

---

### Task 4: Xbox library root discovery

**Files:**
- Create: `@Resources/xbox_scan.py`
- Create: `tests/test_xbox_scan.py`

**Interfaces:**
- Produces:
  - `read_gaming_root(file_path: str) -> str | None` — read `.GamingRoot` text, return absolute library path if it exists
  - `discover_xbox_roots(user_dirs: list[str], drive_letters: list[str] | None = None) -> list[str]` — order: normalized user dirs (existing only) → per-drive `.GamingRoot` → `C:\XboxGames` if exists; case-insensitive dedupe; preserve order

`.GamingRoot` format (common): UTF-16 or UTF-8 path string to the Xbox games folder. Implementation: try utf-16-le then utf-8; strip NULs/whitespace; if relative, resolve against the drive root that contained the file.

- [ ] **Step 1: Failing tests in `tests/test_xbox_scan.py`**

```python
from __future__ import annotations
import importlib.util
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
XBOX_SCAN = ROOT / "@Resources" / "xbox_scan.py"

def load_xbox():
    spec = importlib.util.spec_from_file_location("xbox_scan", XBOX_SCAN)
    mod = importlib.util.module_from_spec(spec)
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
```

Design `discover_xbox_roots` with optional `gaming_root_dirs` and `default_xbox_games` so tests never touch real `C:\`.

Production callers: `gaming_root_dirs=None` → `[f"{d}:\\" for d in string.ascii_uppercase if os.path.exists(f"{d}:\\")]`, `default_xbox_games=r"C:\XboxGames"`.

- [ ] **Step 2: Run — FAIL**

Run: `python -m pytest tests/test_xbox_scan.py::test_discover_xbox_roots_user_gamingroot_and_default -v`

- [ ] **Step 3: Implement `xbox_scan.py` discovery helpers only**

- [ ] **Step 4: PASS + commit**

```bash
git add @Resources/xbox_scan.py tests/test_xbox_scan.py
git commit -m "feat(xbox): discover library roots from XboxDirs, GamingRoot, default"
```

---

### Task 5: Xbox per-game parse → records

**Files:**
- Modify: `@Resources/xbox_scan.py`
- Modify: `tests/test_xbox_scan.py`

**Interfaces:**
- Produces:
  - `parse_xbox_game_dir(game_dir: str, placeholder_image: str) -> dict | None`
  - `scan_xbox_libraries(roots: list[str], placeholder_image: str, status_fn) -> list[dict]`

**Acceptance (spec):** child of root is a game if `Content/` exists and at least one of `MicrosoftGame.config`, `appxmanifest.xml`, or a non-redistributable `.exe` under `Content/`.

**Parsing rules (v1):**
1. Name: from `MicrosoftGame.config` (`ExecutableList` / `ShellVisuals` / `DisplayName` style tags — use regex/XML): prefer `<ShellVisuals ... DefaultDisplayName="...">` or `<DisplayName>`; else folder name.
2. `stable_id`: `xbox:` + Identity `Name` from config/manifest if present; else sanitized folder name (`re.sub(r'[^A-Za-z0-9]+', '', name)` or keep dashes).
3. Launch priority:
   - If config/manifest yields an executable relative path that exists under Content → `["{abs_exe}"]` (Rainmeter bang).
   - Else if `appxmanifest.xml` contains both a usable AUMID string attribute you can form **without** ACL tricks (only if Package Family Name appears literally in-file — rare), use `[shell:AppsFolder\{aumid}]`.
   - Else `[explorer "{game_dir}"]` and caller may status-warn.
4. Image: first `.png`/`.jpg` under game_dir (prefer names containing `header`, `logo`, `poster`); else `placeholder_image`.
5. Skip: empty name; folder names clearly `Runtime` / `Redistributable` (case-insensitive substring).

Sample fixture layout for tests:

```text
XboxGames/
  CoolGame/
    Content/
      MicrosoftGame.config   # see XML below
      CoolGame.exe
      art/header.png
```

Example `MicrosoftGame.config` body for the fixture:

```xml
<?xml version="1.0" encoding="utf-8"?>
<Game configVersion="1">
  <ShellVisuals DefaultDisplayName="Cool Game" />
  <ExecutableList>
    <Executable Name="CoolGame.exe" Id="Game" />
  </ExecutableList>
  <Identity Name="CoolGameId" />
</Game>
```

(Regex-based parse is fine; do not require a full XSD.)

- [ ] **Step 1: Failing tests**

```python
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
        "<Package><Identity Name=\"BareId\" /><Applications><Application Id=\"App\" /></Applications></Package>",
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
```

- [ ] **Step 2: Run — FAIL; implement; PASS**

Run: `python -m pytest tests/test_xbox_scan.py -v`

- [ ] **Step 3: Commit**

```bash
git add @Resources/xbox_scan.py tests/test_xbox_scan.py
git commit -m "feat(xbox): parse installed games into unified records"
```

---

### Task 6: Merge Xbox into `main()` + status lines

**Files:**
- Modify: `@Resources/UpdateGames.pyw`
- Modify: `tests/test_update_games.py`

**Interfaces:**
- Consumes: `discover_xbox_roots`, `scan_xbox_libraries`, `scan_steam_libraries`, `fill_steam_image_paths`
- Produces: `main()` builds `records = steam + xbox`, writes info/meters; status when Xbox roots empty or zero games

- [ ] **Step 1: Test merge helper (keep `main` thin)**

```python
def test_merge_steam_then_xbox_order(ug):
    steam = [{"stable_id": "steam:1", "name": "A", "launch": "[steam://rungameid/1]", "image_path": "a"}]
    xbox = [{"stable_id": "xbox:Z", "name": "B", "launch": '[explorer "x"]', "image_path": "b"}]
    assert ug.merge_game_records(steam, xbox) == steam + xbox
```

```python
def merge_game_records(steam_records, xbox_records):
    return list(steam_records) + list(xbox_records)
```

- [ ] **Step 2: Wire `main()`**

```python
from xbox_scan import discover_xbox_roots, scan_xbox_libraries
# When loading as script from @Resources, ensure sys.path includes script dir:
script_dir = os.path.dirname(os.path.abspath(__file__))
if script_dir not in sys.path:
    sys.path.insert(0, script_dir)
```

Flow:
1. Steam scan + `fill_steam_image_paths`
2. Parse `xboxdirs` from SkinInfo (comma-split, strip); `roots = discover_xbox_roots(user_dirs)`
3. If not roots: `update_rainmeter_status("Xbox: no libraries found")`
4. `xbox_records = scan_xbox_libraries(roots, PLACEHOLDER_IMAGE, update_rainmeter_status)`
5. If roots and not xbox_records: `update_rainmeter_status("Xbox: no games found")`
6. `records = merge_game_records(steam_records, xbox_records)`
7. `write_game_info(records)`; `write_meters(..., records=records, ...)`

Count explorer-fallback xbox launches and optionally one status line: `Xbox: N game(s) open folder only`.

- [ ] **Step 3: Integration-style unit test with tmp SkinInfo optional** — at minimum pytest for `merge_game_records` + import xbox from UpdateGames path.

Run: `python -m pytest tests/ -v`

- [ ] **Step 4: Commit**

```bash
git add @Resources/UpdateGames.pyw tests/test_update_games.py
git commit -m "feat(scan): merge Xbox records into Scan for Games pipeline"
```

---

### Task 7: Settings `XboxDirs` + SkinInfo + FileChoose

**Files:**
- Modify: `@Resources/SkinInfo.inc` — add `XboxDirs=` under PATHS (empty default)
- Modify: `Settings/tabs/TabPaths.inc` — insert Xbox Dirs as **row 2**; shift Rainmeter EXE → 3, Locale presets → 4, Locale value → 5
- Modify: `Settings/Settings.ini` — mirror `GameDirs` pick measures for `XboxDirs` (`_XboxDirsMode`, `PathPickXboxDirs`, append/replace); update `FileChooseBrowseVisibility` OnUpdateAction to include Xbox browse/replace meters
- Modify: `README.md` — Paths bullet mentions Xbox dirs / auto-detect

**Interfaces:**
- Consumes: existing GameDirs FileChoose pattern in `Settings.ini` (~lines 133–153, 183)
- Produces: user can set `XboxDirs` comma list; empty means auto-only

- [ ] **Step 1: Implement SkinInfo + TabPaths row shift**

Comment at top of TabPaths should read:

```text
; Rows: 0 SteamPath | 1 GameDirs | 2 XboxDirs | 3 RainMeterEXE | 4 Locale presets | 5 Locale value
```

Copy GameDirs InputText/meters/Browse/Repl for XboxDirs with renamed sections (`InputXboxDirsMeasure`, `XboxDirsLabel`, …). Y multipliers: Xbox = `2*#FormRowH#`, Rainmeter = `3*`, Locale label/pills = `4*`, Locale value = `5*`.

- [ ] **Step 2: Settings.ini pick routing**

Duplicate GameDirs IfMatch block for `^XboxDirs$` and append/replace measures. Initialize `_XboxDirsMode=append` next to `_GameDirsMode`.

- [ ] **Step 3: Manual check list (no pytest for Rainmeter UI)**

Document in commit message; agent verifies by reading the `.inc` for correct `#XboxDirs#` variable writes to `#@#SkinInfo.inc`.

- [ ] **Step 4: README**

Under Setup/Paths: note Xbox library folders + auto `.GamingRoot` / `C:\XboxGames`.

- [ ] **Step 5: Commit**

```bash
git add @Resources/SkinInfo.inc Settings/tabs/TabPaths.inc Settings/Settings.ini README.md
git commit -m "feat(settings): add XboxDirs path field with browse/append"
```

---

### Task 8: Search matches prefixed Steam IDs + Xbox IDs

**Files:**
- Modify: `@Resources/SearchGames.lua`
- Optional: `tests/` — Lua untested in-repo; add a short comment test plan in commit body only if no harness

**Behavior:**
- If `tonumber(searchInput)` then match when `ID == searchInput` **or** `ID == "steam:"..searchInput`
- Else if input contains `:` match exact stable_id (case-sensitive)
- Else existing partial name match

```lua
function GetGameInfoByID(gameID)
    for i, ID in pairs(gameIDs) do
        if ID == gameID or ID == ("steam:" .. gameID) then
            SKIN:Bang('!ShowMeterGroup', 'G' .. i)
            gameCount = gameCount + 1
            result = gameCount
            return result
        end
    end
    -- keep existing fallthrough / loop style from file
end
```

Also in `Update()` when `tonumber` fails but input looks like `steam:` / `xbox:`, call ID match with full string:

```lua
if tonumber(searchInput) then
    result = GetGameInfoByID(searchInput)
elseif string.find(searchInput, ":", 1, true) then
    result = GetGameInfoByID(searchInput)
else
    result = GetGameInfoByName(searchInput)
end
```

- [ ] **Step 1: Edit Lua**
- [ ] **Step 2: Commit**

```bash
git add @Resources/SearchGames.lua
git commit -m "fix(search): match steam: prefixed and xbox stable IDs"
```

---

### Task 9: Verification + README success notes

**Files:**
- Modify: `README.md` if anything missing
- Test: full pytest

- [ ] **Step 1: Run full suite**

Run: `python -m pytest tests/ -v`

Expected: all PASS

- [ ] **Step 2: Spec checklist (manual)**

- [ ] Steam-only machine: scan still works; Vis survives one rescan after ID migration  
- [ ] Xbox fixture roots via `XboxDirs` in SkinInfo point at a real or test library  
- [ ] Placeholder shows when no art  
- [ ] Extra games still appear after merged list  

- [ ] **Step 3: Final commit only if README/docs tweaks remain**

```bash
git add -A
git status
# commit only if needed
```

---

## Self-review (plan vs spec)

| Spec requirement | Task |
|------------------|------|
| Multi-source records, merge Steam then Xbox | 1, 5, 6 |
| `steam:` / `xbox:` IDs + Vis migration | 1, 2 |
| Baked launch/image; placeholder | 3, 5 |
| Xbox roots: XboxDirs ∪ GamingRoot ∪ default | 4, 7 |
| Per-game Content + config/manifest/exe | 5 |
| Explorer fallback + status | 5, 6 |
| Settings XboxDirs | 7 |
| Extra unchanged | 3 (extra branch), 6 |
| Search by ID with prefixes | 8 |
| No EA / IGDB / ACL bypass | Global Constraints |
| Tests for steam migration + xbox fixtures | 2, 4, 5 |

**Placeholder scan:** none intentional.  
**Type consistency:** record keys always `stable_id`, `name`, `launch`, `image_path`; `write_game_info(records)`; `create_meter(..., launch=, image_name=)`.

---

## Execution handoff

Plan saved to `docs/superpowers/plans/2026-08-23-xbox-game-scan.md`.

Per project rules, implementation uses **Subagent-Driven Development** (one fresh implementer subagent per task, review between tasks). Say **ㄱ** / implement when you want execution to start.
