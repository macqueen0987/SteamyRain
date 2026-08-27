# Settings UI Overhaul Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Make Paths, Non-Steam (Extra) games, and Hidden games fully manageable inside Rainmeter Settings tabs so users never hand-edit `SkinInfo.inc` / `NonSteamGames.inc` for those flows.

**Architecture:** Expand `Settings/Settings.ini` into a 4-tab shell (`ActiveTab=1..4`) that `@include`s per-tab `.inc` files. Layout keeps today’s meters; Paths/Extra use FileChoose + InputText + `!WriteKeyValue`; Hidden reuses list logic via a shared include. `UpdateGames.pyw` gains empty Extra-slot skipping so Extra UI can leave gaps without renumbering. Settings never launches Python; Extra changes show a “Scan for Games” prompt that uses the existing QuickSettings scan action.

**Tech Stack:** Rainmeter INI meters/measures, InputText plugin, FileChoose community plugin (`@Resources/Plugins/FileChoose.dll`), existing `!WriteKeyValue` / bang patterns, pytest for `UpdateGames.pyw` empty-slot behavior.

## Global Constraints

- Pure Rainmeter for settings UI — no Python GUI and Settings must not call `UpdateGames.pyw` except via the existing user-triggered “Scan for Games” button.
- FileChoose is required for Browse; if missing, Browse stays disabled and InputText manual entry remains.
- Extra slots: max 20 (`Egame1`…`Egame20`); delete clears slot (no renumber); UI hides empty slots.
- `GameDirs` separator is **comma** (matches existing `UpdateGames.pyw` `split(',')`) — not semicolon.
- Personal deploy files (`SkinInfo.inc`, `GamesInfo.inc`, `NonSteamGames.inc`, `img/*`, `dynamicMeters/*`) stay user-owned; do not overwrite on sync.
- Prefer existing visual language (Dark1/TextBG/SettingsHover, Segoe Fluent Icons, 225px-ish width).

## Locked decisions (from open design items)

1. **FileChoose path:** `@Resources/Plugins/FileChoose.dll` + `Plugin=#@#Plugins\FileChoose`. Ship dll + short `LICENSE-FileChoose.txt` / README note (forum plugin by author of FileChoose thread).
2. **Hidden:** Extract shared list UI into `@Resources/extraMeters/HiddenList.inc`. `Hidden/Hidden.ini` becomes a thin host; Settings Tab Hidden `@include`s the same file.
3. **Tabs:** Shell in `Settings.ini` + four includes under `Settings/tabs/` (`TabLayout.inc`, `TabPaths.inc`, `TabExtra.inc`, `TabHidden.inc`).

## File map

| File | Role |
|------|------|
| `Settings/Settings.ini` | Shell: vars, tab bar, ActiveTab show/hide, shared InputText/FileChoose measures, close/header |
| `Settings/tabs/TabLayout.inc` | Current layout/color meters (moved out of Settings.ini) |
| `Settings/tabs/TabPaths.inc` | Paths + Locale UI |
| `Settings/tabs/TabExtra.inc` | Extra games list UI (slots 1–20) |
| `Settings/tabs/TabHidden.inc` | Host for HiddenList + tab chrome |
| `@Resources/extraMeters/HiddenList.inc` | Shared hidden-games list (from Hidden.ini) |
| `@Resources/Plugins/FileChoose.dll` | File/folder/image picker |
| `@Resources/UpdateGames.pyw` | Skip empty Extra slots when building meters |
| `tests/test_update_games.py` | Empty-slot skip tests |
| `README.md` | FileChoose + Settings tabs note |
| `Hidden/Hidden.ini` | Thin wrapper around HiddenList.inc |

---

### Task 1: Tab shell + move Layout into include

**Files:**
- Modify: `Settings/Settings.ini`
- Create: `Settings/tabs/TabLayout.inc`
- Test: manual Rainmeter refresh (Layout-only behavior unchanged)

**Interfaces:**
- Consumes: existing Layout meters/measures currently in `Settings.ini`
- Produces: `ActiveTab` (1=Layout, 2=Paths, 3=Extra, 4=Hidden); groups `TabLayout`, `TabPaths`, `TabExtra`, `TabHidden`; tab bar meters `TabBtnLayout`…`TabBtnHidden`

- [ ] **Step 1: Create tabs folder and cut Layout body into TabLayout.inc**

Move every Layout-only meter/measure currently under Settings (Tile Size through color Inputs, OptionStyle rows, TileSizeBar, toggles, Gap controls, color reset rows) into `Settings/tabs/TabLayout.inc`.

At top of `TabLayout.inc` keep no `[Variables]` — Variables stay in shell. Add `Group=TabLayout` to each Layout meter (and Hidden=`(#ActiveTab#<>1)` **or** control via shell measure — prefer shell measure so one place owns visibility).

- [ ] **Step 2: Rewrite Settings.ini as shell**

Replace body with:

```ini
[Rainmeter]
Update=-1
DynamicWindowSize=1

[Variables]
@include=#@#SkinInfo.inc
@include2=#@#GamesInfo.inc
@include3=#@#NonSteamGames.inc
ActiveConf="SteamyRainList.ini"
ActiveTab=1
DefaultVisTile=3.#Half#
DefaultGap=0
MaxGap=100
VisTileN=VisibleTiles
VisTile=#VisibleTiles#
YPLUS=([TilePixelsY]+20)
XPLUS=0
OHalfTiles=#Dark1#
OArrow=#Dark1#
OFade=#Dark1#
OHead=#Dark1#
ExtraSlotMax=20
ScanNeeded=0

; Keep ModeCheck / OptionsCheck / TileSizeCheck / MoveWindow / color InputText measures
; that Layout still needs — either leave in shell or move with TabLayout.
; Prefer: layout-specific measures in TabLayout.inc; shell keeps ModeCheck + MoveWindow + TabSwitch.

[TabSwitch]
Measure=Calc
Formula=#ActiveTab#
IfCondition=#ActiveTab#=1
IfTrueAction=[!ShowMeterGroup TabLayout][!HideMeterGroup TabPaths][!HideMeterGroup TabExtra][!HideMeterGroup TabHidden][!SetOption TabBtnLayout FontColor "#SettingsHover#"][!SetOption TabBtnPaths FontColor "#TextColor#,180"][!SetOption TabBtnExtra FontColor "#TextColor#,180"][!SetOption TabBtnHidden FontColor "#TextColor#,180"][!UpdateMeterGroup TabBar][!UpdateMeterGroup TabLayout][!UpdateMeterGroup TabPaths][!UpdateMeterGroup TabExtra][!UpdateMeterGroup TabHidden][!Redraw]
IfCondition2=#ActiveTab#=2
IfTrueAction2=[!HideMeterGroup TabLayout][!ShowMeterGroup TabPaths][!HideMeterGroup TabExtra][!HideMeterGroup TabHidden][!SetOption TabBtnLayout FontColor "#TextColor#,180"][!SetOption TabBtnPaths FontColor "#SettingsHover#"][!SetOption TabBtnExtra FontColor "#TextColor#,180"][!SetOption TabBtnHidden FontColor "#TextColor#,180"][!UpdateMeterGroup TabBar][!UpdateMeterGroup TabLayout][!UpdateMeterGroup TabPaths][!UpdateMeterGroup TabExtra][!UpdateMeterGroup TabHidden][!Redraw]
IfCondition3=#ActiveTab#=3
IfTrueAction3=[!HideMeterGroup TabLayout][!HideMeterGroup TabPaths][!ShowMeterGroup TabExtra][!HideMeterGroup TabHidden][!SetOption TabBtnLayout FontColor "#TextColor#,180"][!SetOption TabBtnPaths FontColor "#TextColor#,180"][!SetOption TabBtnExtra FontColor "#SettingsHover#"][!SetOption TabBtnHidden FontColor "#TextColor#,180"][!UpdateMeterGroup TabBar][!UpdateMeterGroup TabLayout][!UpdateMeterGroup TabPaths][!UpdateMeterGroup TabExtra][!UpdateMeterGroup TabHidden][!Redraw]
IfCondition4=#ActiveTab#=4
IfTrueAction4=[!HideMeterGroup TabLayout][!HideMeterGroup TabPaths][!HideMeterGroup TabExtra][!ShowMeterGroup TabHidden][!SetOption TabBtnLayout FontColor "#TextColor#,180"][!SetOption TabBtnPaths FontColor "#TextColor#,180"][!SetOption TabBtnExtra FontColor "#TextColor#,180"][!SetOption TabBtnHidden FontColor "#SettingsHover#"][!UpdateMeterGroup TabBar][!UpdateMeterGroup TabLayout][!UpdateMeterGroup TabPaths][!UpdateMeterGroup TabExtra][!UpdateMeterGroup TabHidden][!Redraw]
IfConditionMode=1
DynamicVariables=1
OnUpdateAction=[!EnableMeasure TabSwitch]
```

Add header + four tab buttons (short labels: `Layout` / `Paths` / `Extra` / `Hidden`) in group `TabBar`, each:

```ini
LeftMouseUpAction=[!SetVariable ActiveTab 1][!UpdateMeasure TabSwitch]
```

(use 2/3/4 for other tabs). Widen header shape to fit four labels (~225–280px). Keep Close bang writing `Settings=0`.

At end of shell:

```ini
@include4=#CURRENTPATH#tabs\TabLayout.inc
@include5=#CURRENTPATH#tabs\TabPaths.inc
@include6=#CURRENTPATH#tabs\TabExtra.inc
@include7=#CURRENTPATH#tabs\TabHidden.inc
```

Create stub files for Paths/Extra/Hidden with a single placeholder String meter in the right group so Refresh does not fail:

```ini
[PathsStub]
Meter=String
Text="Paths (coming)"
Group=TabPaths
Hidden=1
Y=40
X=8
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
```

(Similar stubs for Extra/Hidden.)

- [ ] **Step 3: Manual verify Layout regression**

1. Copy/sync skin to Rainmeter Skins folder per `DEVNOTES.md`.
2. Open Settings (middle-click header / QuickSettings).
3. Confirm Layout controls still change tile size / toggles / colors and write `SkinInfo.inc`.
4. Click each tab button: only matching stub/content shows; Layout hides on Paths+.

- [ ] **Step 4: Commit**

```bash
git add Settings/Settings.ini Settings/tabs/TabLayout.inc Settings/tabs/TabPaths.inc Settings/tabs/TabExtra.inc Settings/tabs/TabHidden.inc
git commit -m "feat(settings): add tab shell and extract Layout tab"
```

---

### Task 2: Vendor FileChoose + shared picker measures

**Files:**
- Create: `@Resources/Plugins/FileChoose.dll` (download from Rainmeter forum FileChoose release)
- Create: `@Resources/Plugins/LICENSE-FileChoose.txt` (attribution: forum FileChoose plugin, version, author)
- Modify: `Settings/Settings.ini` (shared FileChoose + probe measures)
- Modify: `README.md` (Requirements: FileChoose shipped under `@Resources/Plugins`)

**Interfaces:**
- Consumes: FileChoose plugin API — bangs `ChooseFile N`, `ChooseFolder N`, `ChooseImage N`; macros `$Path$`, `$Name$`, `$Icon$`
- Produces: measures `FileChooseFolder`, `FileChooseFile`, `FileChooseImage` with Command slots written by later tabs; variable `HasFileChoose=1|0`

- [ ] **Step 1: Place plugin**

Download FileChoose.dll (current release from https://forum.rainmeter.net/viewtopic.php?f=128&t=33767). Save to `@Resources/Plugins/FileChoose.dll`. Add LICENSE/attribution text file noting third-party plugin, not Rainmeter core.

- [ ] **Step 2: Add shared measures in Settings.ini shell**

```ini
[HasFileChoose]
Measure=Plugin
Plugin=#@#Plugins\FileChoose
; If plugin fails to load, Rainmeter logs error; keep Browse actions gated by HasFileChooseFlag.
Disabled=1

[FileChooseFolder]
Measure=Plugin
Plugin=#@#Plugins\FileChoose
UseNewStyle=1
ReturnValue=Path
Command1=[!SetVariable _PickPath "$Path$"][!UpdateMeasure PathPickSink]
DynamicVariables=1

[FileChooseFile]
Measure=Plugin
Plugin=#@#Plugins\FileChoose
ReturnValue=Path
GetIcon=0
Command1=[!SetVariable _PickPath "$Path$"][!UpdateMeasure FilePickSink]
DynamicVariables=1

[FileChooseImage]
Measure=Plugin
Plugin=#@#Plugins\FileChoose
ReturnValue=Path
GetIcon=1
IconCache=#@#img\EIcon
IconSize=3
Command1=[!SetVariable _PickPath "$Path$"][!SetVariable _PickIcon "$Icon$"][!UpdateMeasure ImagePickSink]
DynamicVariables=1

[HasFileChooseFlag]
; Manual override: set to 1 after confirming plugin loads; Browse meters check this.
; Default 1 when dll is shipped; document setting to 0 to force InputText-only.
```

Add to `[Variables]`: `HasFileChooseFlag=1`, `_PickPath=`, `_PickIcon=`, `_PickTarget=`, `_GameDirsMode=append`.

Sink measures are stubs updated in Tasks 3–4 (`PathPickSink` / `FilePickSink` / `ImagePickSink`) — create empty Calc measures with `OnUpdateAction` no-ops in shell for now:

```ini
[PathPickSink]
Measure=Calc
Formula=1
DynamicVariables=1
Disabled=1

[FilePickSink]
Measure=Calc
Formula=1
DynamicVariables=1
Disabled=1

[ImagePickSink]
Measure=Calc
Formula=1
DynamicVariables=1
Disabled=1
```

- [ ] **Step 3: README note**

Under Requirements, add:

```markdown
### FileChoose plugin
Settings Paths/Extra Browse buttons use the bundled FileChoose plugin at
`@Resources/Plugins/FileChoose.dll`. If Browse does nothing, confirm the DLL
is present and set `HasFileChooseFlag=1` in Settings. Paths can always be
typed via click-to-edit (InputText).
```

- [ ] **Step 4: Manual verify plugin loads**

Refresh Settings; Rainmeter log should not show Plugin load failure. Temporarily add a debug meter that runs `[!CommandMeasure FileChooseFolder "ChooseFolder 1"]` — picking a folder should set `_PickPath` (log with `[!Log "#_PickPath#"]`).

- [ ] **Step 5: Commit**

```bash
git add @Resources/Plugins/FileChoose.dll @Resources/Plugins/LICENSE-FileChoose.txt Settings/Settings.ini README.md
git commit -m "feat(settings): vendor FileChoose plugin for path browse"
```

---

### Task 3: Paths tab

**Files:**
- Modify: `Settings/tabs/TabPaths.inc`
- Modify: `Settings/Settings.ini` (`PathPickSink` + InputText measures for path fields)

**Interfaces:**
- Consumes: `#SteamPath#`, `#GameDirs#`, `#RainMeterEXE#`, `#Locale#`, FileChooseFolder/FileChooseFile, `_GameDirsMode`
- Produces: writes those keys to `#@#SkinInfo.inc`; Locale presets `english` / `koreana`

- [ ] **Step 1: Build Paths UI in TabPaths.inc**

Rows (Y stack under tab bar ~Y=36):

1. SteamPath — label, clipped value, Browse folder, click value → InputText
2. GameDirs — label, clipped value, Browse(+ append), Replace button (sets `_GameDirsMode=replace` then ChooseFolder), click value → InputText
3. RainMeterEXE — label, clipped value, Browse file (filter via FileChoose default), InputText
4. Locale — presets `english` | `koreana` + InputText for custom

Example SteamPath Browse (only if `#HasFileChooseFlag#=1`):

```ini
[SteamPathBrowse]
Meter=String
Text="[\xe8b7]"
FontFace=Segoe Fluent Icons
Group=TabPaths
LeftMouseUpAction=[!SetVariable _PickTarget SteamPath][!CommandMeasure FileChooseFolder "ChooseFolder 1"]
```

`PathPickSink` OnUpdateAction (in shell):

```ini
OnUpdateAction=[!WriteKeyValue Variables "#_PickTarget#" "#_PickPath#" "#@#SkinInfo.inc"][!SetVariable "#_PickTarget#" "#_PickPath#"][!UpdateMeterGroup TabPaths][!Redraw]
```

For GameDirs when `_PickTarget=GameDirs`:

- if `_GameDirsMode=append` and `#GameDirs#` non-empty: write `#GameDirs#,#_PickPath#` (comma, strip trailing spaces)
- if replace or empty: write `#_PickPath#`
- then reset `_GameDirsMode=append`

Locale preset:

```ini
LeftMouseUpAction=[!WriteKeyValue Variables Locale "koreana" "#@#SkinInfo.inc"][!SetVariable Locale "koreana"][!UpdateMeterGroup TabPaths][!Redraw]
```

When `HasFileChooseFlag=0`, Browse meters `Hidden=1` or show disabled color without action.

- [ ] **Step 2: Manual verify Paths**

1. Change SteamPath via Browse → `@Resources/SkinInfo.inc` updates.
2. Append a second library folder to GameDirs → value is `path1,path2` (comma).
3. Replace GameDirs → single path.
4. Set Locale to `koreana` via preset.
5. With `HasFileChooseFlag=0`, Browse hidden; InputText still edits.

- [ ] **Step 3: Commit**

```bash
git add Settings/tabs/TabPaths.inc Settings/Settings.ini
git commit -m "feat(settings): add Paths tab with FileChoose browse"
```

---

### Task 4: Extra tab (Non-Steam slots) + empty-slot scan skip

**Files:**
- Modify: `Settings/tabs/TabExtra.inc`
- Modify: `Settings/Settings.ini` (FilePickSink / ImagePickSink, Extra helpers)
- Modify: `@Resources/UpdateGames.pyw` (skip empty Extra names)
- Modify: `tests/test_update_games.py`
- Optional: ensure `@Resources/img/EIcon` and `ELogo` dirs exist

**Interfaces:**
- Consumes: `EgameN`, `EgameNPath`, `EgameNVis`, `ExtraGamesCount`, `ExtraGameCountPLUS`, `#ExtraSlotMax#`
- Produces: UI to add/edit/clear slots 1–20; `ScanNeeded=1` after mutations; scan button runs existing UpdateGames launch bang from QuickSettings; `write_meters` skips Extra entries whose name is empty

- [ ] **Step 1: Write failing test for empty Extra slot skip**

Add to `tests/test_update_games.py` a unit that constructs a temporary NonSteamGames-like structure and asserts the meter writer skips empty names. Prefer extracting a small pure helper in `UpdateGames.pyw`:

```python
def iter_extra_game_indices(extra_vars: dict, extra_games_count: int):
    """Yield 1..extra_games_count indices that have a non-empty EgameN name."""
    for i in range(1, extra_games_count + 1):
        name = extra_vars.get(f"Egame{i}", "")
        if isinstance(name, str):
            name = name.strip().strip('"')
        if name:
            yield i
```

Test:

```python
def test_iter_extra_game_indices_skips_empty_slots():
    from UpdateGames import iter_extra_game_indices  # or import path used by tests
    vars_ = {
        "Egame1": "EndField",
        "Egame2": "",
        "Egame3": "Nikke",
    }
    assert list(iter_extra_game_indices(vars_, 3)) == [1, 3]
```

Adjust import to match how `tests/test_update_games.py` already loads the module.

- [ ] **Step 2: Run test — expect FAIL**

```powershell
.\.venv\Scripts\Activate.ps1
pytest tests/test_update_games.py::test_iter_extra_game_indices_skips_empty_slots -v
```

Expected: FAIL (helper missing or not used).

- [ ] **Step 3: Implement helper and use it in write_meters**

In `UpdateGames.pyw`, add `iter_extra_game_indices`. Change the Extra loop in `write_meters` from `for i in range(1, extra_games_count + 1)` to iterate only non-empty indices. Meter section names may keep `EGame{i}` with original index `i` (gaps OK — Hidden/dynamic meters already key by Extra index).

Also when reading Extra for count, keep `ExtraGamesCount` as max index (UI responsibility). Document that empty middle slots are skipped at scan time.

- [ ] **Step 4: Run test — expect PASS**

```powershell
pytest tests/test_update_games.py::test_iter_extra_game_indices_skips_empty_slots -v
pytest tests -v
```

Expected: all PASS.

- [ ] **Step 5: Build Extra tab UI**

Static 20 row template is heavy in Rainmeter; use a **scrollable working set** pattern:

Variables: `ExtraEditIndex=1` (selected slot), list shows occupied slots via String meters that reference `#Egame1#`… with `Hidden=(#EgameN#=)`.

Practical v1 layout:

1. Header row: `Add` button finds first empty slot `N` where `#EgameN#` is empty (Calc chain or sequential IfCondition measures `FindEmptySlot`), writes `EgameN="New Game"`, `EgameNPath=""`, `EgameNVis=0`, bumps `ExtraGamesCount` to `Max(ExtraGamesCount,N)`, sets `ScanNeeded=1`.
2. List: for N=1..20, a row group `ExtraRowN` Hidden when name empty:
   - Name (InputText on click)
   - Path (clipped + Browse file via FileChooseFile with `_PickTarget=EgameNPath`)
   - Vis toggle (same bang style as Hidden for `EgameNVis` / `ExtraGameCountPLUS`)
   - Icon button: ChooseImage / GetIcon → copy/move icon into `#@#img\EIcon\00N.jpg` (zero-pad 3 digits: `001`…`020`). FileChoose IconCache may write a temp icon; follow-up bang copies/renames into `00N.jpg` if plugin returns `$Icon$` path — if copy-via-bang is impossible, document “place file as `00N.jpg`” and use ChooseImage only to set a preview path variable until scan.
3. Clear button on row: write empty name/path, Vis=0, recompute `ExtraGamesCount` as highest N with non-empty name (Loop measure), `ScanNeeded=1`.
4. Banner when `ScanNeeded=1`: “Scan for Games to apply” + button duplicating QuickSettings scan:

```ini
LeftMouseUpAction=[!WriteKeyValue Variables SkinPosX "#CURRENTCONFIGX#" "#@#SkinInfo.inc"][!WriteKeyValue Variables SkinPosY "#CURRENTCONFIGY#" "#@#SkinInfo.inc"]["#@#UpdateGames.pyw"][!SetVariable ScanNeeded 0]
```

**Icon note (explicit):** If FileChoose cannot atomically write `00N.jpg`, Extra Icon button opens ChooseImage and shows chosen path; user-facing text: “Save/copy image to `@Resources/img/EIcon/00N.jpg`” OR use a tiny PowerShell one-liner bang only if already used elsewhere — **do not add new Python**. Prefer documenting filename convention + optional `Copy-Item` bang:

```ini
[!CommandMeasure FileChooseImage "ChooseImage 1"]
```

and in ImagePickSink:

```ini
OnUpdateAction=[!SetVariable "EIconPath#_PickSlot#" "#_PickPath#"]["cmd" "/c" "copy /Y \"#_PickPath#\" \"#@#img\EIcon\00#_PickSlotPadded#.jpg\""][!SetVariable ScanNeeded 1]
```

(Use padded slot variable set before ChooseImage.)

- [ ] **Step 6: Manual verify Extra**

1. Add game → `NonSteamGames.inc` gains next slot.
2. Set path via Browse; set name via InputText.
3. Clear middle slot → remains empty; Scan still builds meters for remaining games (pytest covers skip).
4. ScanNeeded banner appears then clears after scan.

- [ ] **Step 7: Commit**

```bash
git add Settings/tabs/TabExtra.inc Settings/Settings.ini @Resources/UpdateGames.pyw tests/test_update_games.py
git commit -m "feat(settings): Extra games tab and skip empty NonSteam slots"
```

---

### Task 5: Shared Hidden list + Hidden tab + QuickSettings link

**Files:**
- Create: `@Resources/extraMeters/HiddenList.inc` (extract from `Hidden/Hidden.ini`)
- Modify: `Hidden/Hidden.ini` (thin host)
- Modify: `Settings/tabs/TabHidden.inc`
- Modify: `@Resources/extraMeters/QuickSettings.inc` (optional: Hidden Games opens Settings tab 4)

**Interfaces:**
- Consumes: existing Hidden measures (`HiddenGames`, UnHideAll*, VisCheck, dynamic `@include` of `dynamicHiddenMeters.inc`)
- Produces: same unhide behavior from Settings tab 4; `hiddenWindow` flag behavior preserved when using standalone Hidden.ini

- [ ] **Step 1: Extract HiddenList.inc**

Move from `Hidden.ini` into `@Resources/extraMeters/HiddenList.inc`:

- GameStyle / NameStyle / VisStyle / list Container / scroll / UnHideAll button / `@include` dynamic hidden meters
- Keep window positioning, ModeCheck, MoveWindow, SettingCheck in `Hidden.ini` host

Hidden.ini becomes:

```ini
[Rainmeter]
...
[Variables]
...existing includes...
@include4=#@#extraMeters\HiddenList.inc
```

Ensure HiddenList meters do not depend on Settings-only vars.

- [ ] **Step 2: TabHidden.inc hosts list**

```ini
@include=#@#extraMeters\HiddenList.inc
```

Plus Settings-specific chrome (no second Close that deactivates Hidden config incorrectly). When running inside Settings, UnHide bangs must still write GamesInfo/NonSteamGames and refresh main skin — keep existing bangs.

If dual-include of the same meter names conflicts when both Settings and Hidden are active, **gate** QuickSettings so opening Settings Hidden tab deactivates `SteamyRain\Hidden` first:

```ini
[!DeactivateConfig "SteamyRain\Hidden" "Hidden.ini"][!SetVariable ActiveTab 4][!UpdateMeasure TabSwitch]
```

And QuickSettings “Hidden Games” can either keep old window **or** open Settings on tab 4 — prefer Settings tab 4 for one UX:

```ini
LeftMouseUpAction=[!WriteKeyValue Variables Settings "1" "#@#SkinInfo.inc"][!ActivateConfig "SteamyRain\Settings" "Settings.ini"][!SetVariable ActiveTab 4 "SteamyRain\Settings"][!UpdateMeasure TabSwitch "SteamyRain\Settings"][!DeactivateConfig "SteamyRain\Hidden" "Hidden.ini"][!HideMeterGroup QuickSettings]
```

Keep `Hidden.ini` loadable for backward compatibility (middle-click flows that still toggle `hiddenWindow`).

- [ ] **Step 3: Manual verify Hidden**

1. Hide a game from main skin → appears in Settings → Hidden tab → Unhide restores Vis + count.
2. UnHide All works for Steam + Extra.
3. Opening Hidden via QuickSettings lands on Settings tab 4 without two overlapping lists.

- [ ] **Step 4: Commit**

```bash
git add @Resources/extraMeters/HiddenList.inc Hidden/Hidden.ini Settings/tabs/TabHidden.inc @Resources/extraMeters/QuickSettings.inc
git commit -m "feat(settings): Hidden tab via shared HiddenList include"
```

---

### Task 6: Polish, README, regression pass

**Files:**
- Modify: `README.md` (Settings tabs overview; remove “must edit SkinInfo for paths” where obsolete)
- Modify: `Settings/Settings.ini` / tab incs (padding, DynamicWindowSize per tab height)
- Touch: `DEVNOTES.md` only if local deploy steps change (optional; gitignored)

**Interfaces:**
- Consumes: completed tabs
- Produces: documented user flow

- [ ] **Step 1: README Settings section**

Add short section:

```markdown
## Settings
Open via QuickSettings → Settings, or middle-click the header icon.
Tabs:
- Layout — tile size, visibility toggles, colors
- Paths — Steam / library folders / Rainmeter.exe / locale (Browse uses FileChoose)
- Extra — non-Steam games (then run Scan for Games)
- Hidden — unhide games
```

Update Setup step 1 to mention Paths tab as alternative to hand-editing.

- [ ] **Step 2: Full manual regression checklist**

- [ ] Layout: tile size, arrows, fade, header, half tiles, gap, all 5 colors + opacity + resets
- [ ] Paths: all four fields persist after Refresh Settings
- [ ] Extra: add / edit / clear / scan
- [ ] Hidden: unhide one + unhide all
- [ ] Mode horizontal/vertical still positions Settings correctly (`ModeCheck` / `MoveWindow`)
- [ ] Close Settings clears `Settings=0`
- [ ] `pytest tests -v` still green

- [ ] **Step 3: Commit**

```bash
git add README.md Settings/
git commit -m "docs: document tabbed Settings and finish UI polish"
```

---

## Spec coverage self-review

| Spec requirement | Task |
|------------------|------|
| Layout tab keeps existing controls | Task 1 |
| Paths UI + Browse + InputText | Task 3 |
| GameDirs multi-path | Task 3 (comma) |
| Locale presets | Task 3 |
| Extra slots ≤20, add/clear no renumber | Task 4 |
| Icons via FileChoose / filename convention | Task 4 |
| Scan for Games prompt (no auto Python) | Task 4 |
| Empty slot skip in UpdateGames | Task 4 |
| Hidden in Settings | Task 5 |
| FileChoose vendored + README | Task 2, 6 |
| Graceful no-plugin Browse disable | Task 2–3 |
| pytest for NonSteam empty slots | Task 4 |

## Placeholder / consistency notes

- Spec said semicolon for GameDirs; **plan uses comma** to match `UpdateGames.pyw`.
- Icon file copy uses `cmd /c copy` bang if FileChoose GetIcon path is insufficient — still no Python.
- Exact FileChoose option names (`UseNewStyle`, `IconCache`) follow forum plugin docs; adjust if dll version differs after download in Task 2.
