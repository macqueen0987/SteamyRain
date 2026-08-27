# Settings Large Panel Relayout Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Rebuild SteamyRain Settings as a fixed **520×640** Rainmeter panel with a separated header/tab/content shell and relative layouts so Layout/Paths/Extra/Hidden are usable without overlapping controls.

**Architecture:** Keep bangs, FileChoose, ExtraHas, HiddenList sharing, and Scan flow. Replace the mini-panel absolute coordinates with shell variables (`WinW`/`WinH`/`ContentTop`/`ContentH`/`Pad`/`RowH`) and rewrite each tab include against those anchors. Extra/Hidden list bodies scroll inside Content; chrome (Add/Scan/Unhide All) stays fixed above the scroller.

**Tech Stack:** Rainmeter INI meters/measures, existing FileChoose + InputText, shared `HiddenList.inc`, pytest regression only (no Python behavior change expected).

## Global Constraints

- Pure Rainmeter + existing FileChoose; no Python settings GUI.
- Window: **W=520, H=640** fixed. Header H=28, tab bar Y=28 H=32, Content Y=60 H=580, Pad=16 → content width 488.
- Content area: **no absolute Y**. First control at `#ContentTop#` (or tab anchor); then `Y=(#RowH#)R` / `Y=2R`. `RowH≈28`, `SectionGap≈12`.
- `GameDirs` separator remains **comma**.
- Preserve visual language: Dark1, TextBG, SettingsHover, Segoe Fluent Icons.
- Do not renumber Extra slots; keep empty-slot skip in UpdateGames.
- Do not overwrite personal deploy files when syncing (`SkinInfo`, `GamesInfo`, `NonSteamGames`, `dynamicMeters`, EIcon/ELogo).
- Working branch: create/use a feature branch off `main` (currently ahead of origin); commit via `git commit -F` on Windows (no bash heredoc).
- Uncommitted DEBUG meter removal in `TabLayout.inc` must be included or superseded by the Layout rewrite.

## File map

| File | Responsibility |
|------|----------------|
| `Settings/Settings.ini` | Shell vars, background/header/tab bar, TabSwitch, shared FileChoose/Input sinks |
| `Settings/tabs/TabLayout.inc` | Layout relative rows |
| `Settings/tabs/TabPaths.inc` | Paths relative rows |
| `Settings/tabs/TabExtra.inc` | Extra chrome + scroll list |
| `Settings/tabs/TabHidden.inc` | Hidden chrome host |
| `@Resources/extraMeters/HiddenList.inc` | List width/height/scroll sized to Content |
| `README.md` | One-line note on Settings window size (optional in Task 5) |

---

### Task 1: Shell — 520×640 chrome + Content variables

**Files:**
- Modify: `Settings/Settings.ini`
- Test: manual Rainmeter refresh (structure); `pytest tests -v` (expect unchanged 7/7)

**Interfaces:**
- Consumes: existing `ActiveTab`, `TabSwitch`, FileChoose measures, tab includes
- Produces: variables `WinW=520`, `WinH=640`, `HeaderH=28`, `TabBarY=28`, `TabBarH=32`, `ContentTop=60`, `ContentH=580`, `Pad=16`, `ContentW=(#WinW#-#Pad#*2)`, `RowH=28`, `SectionGap=12`; Header/Close positioned with `WinW`; background Shape(s) fill `WinW×WinH`; tab buttons in a dedicated tab strip at `Y=#TabBarY#` (not overlapping Content)

- [ ] **Step 1: Branch**

```powershell
git checkout main
git checkout -b feat/settings-large-panel
```

If DEBUG-only local change exists on `TabLayout.inc`, keep it on the branch (will be replaced in Task 2).

- [ ] **Step 2: Add shell size variables to `[Variables]`**

Exact values:

```ini
WinW=520
WinH=640
HeaderH=28
TabBarY=28
TabBarH=32
ContentTop=60
ContentH=580
Pad=16
ContentW=(#WinW#-#Pad#*2)
RowH=28
SectionGap=12
```

Remove reliance on implicit 225/270 widths for Header/Close.

- [ ] **Step 3: Rebuild Header / Close / background**

Replace Header Shape with:

```ini
[Header]
Meter=Shape
Shape=Rectangle 0,0,#WinW#,#HeaderH#,6 | Fill Color #Dark1# | StrokeWidth 0
Shape2=Rectangle 0,(#HeaderH#/2),#WinW#,(#WinH#-#HeaderH#/2) | Fill Color #Dark1#,#Opacity# | StrokeWidth 0
DynamicVariables=1
Group=Mode
```

Close at `X=(#WinW#-19)` (adjust to match prior close hitbox). Logo stays in header row (`Y` within 0..HeaderH).

- [ ] **Step 4: Move tab bar below header**

Tab buttons `Y=#TabBarY#+6` (or vertically centered in tab strip), `X=#Pad#` then `X=8R`. Optional tab-strip background Shape `Y=#TabBarY#` `H=#TabBarH#` `W=#WinW#` with slightly different fill (`#TextBG#` or darker) so tabs never sit under Content labels.

Remove the hidden `[Title]` “Settings” anchor if ContentTop replaces it; tabs must not share Y with first Layout row.

- [ ] **Step 5: Verify TabSwitch still shows/hides groups**

No logic change required beyond redraw after size change. Confirm `HiddenListHidden` / `hiddenWindow` bangs from prior work remain.

- [ ] **Step 6: Manual structure check + pytest**

1. Sync or copy `Settings.ini` to Rainmeter Skins (exclude personal incs).
2. Open Settings — window ~520×640; tabs visible under header; no content covering tab labels.
3. Run:

```powershell
.\.venv\Scripts\Activate.ps1
pytest tests -v
```

Expected: 7 passed.

- [ ] **Step 7: Commit**

```powershell
git add Settings/Settings.ini
@"
feat(settings): expand shell to 520x640 with content anchors
"@ | Set-Content .git/COMMIT_MSG_TMP.txt -Encoding utf8
git commit -F .git/COMMIT_MSG_TMP.txt
Remove-Item .git/COMMIT_MSG_TMP.txt
```

---

### Task 2: Rewrite TabLayout — relative rows

**Files:**
- Modify: `Settings/tabs/TabLayout.inc` (full rewrite of meter positions; keep measure bangs)
- Test: manual Layout tab; pytest 7/7

**Interfaces:**
- Consumes: `#ContentTop#`, `#Pad#`, `#ContentW#`, `#RowH#`, `#SectionGap#`, existing Layout measures (`TileSizeCheck`, toggles, color InputText)
- Produces: Layout meters with **no absolute Y** for controls; InputText X/Y updated to follow row anchors (Rainmeter InputText needs numeric X/Y — set via bang on click from meter position, or place InputText measures with DynamicVariables matching row meters after first layout pass)

- [ ] **Step 1: Define Layout content anchor**

```ini
[LayoutAnchor]
Meter=String
SolidColor=0,0,0,1
W=1
H=1
X=#Pad#
Y=#ContentTop#
Group=TabLayout
DynamicVariables=1
```

First visible row uses `Y=0R` relative to this (or `Y=#ContentTop#` directly on first label).

- [ ] **Step 2: Rebuild rows in order**

1. Tile Size label (`OptionStyle` widened to ~200) + slider Shape whose X/Y are relative: e.g. slider track `X=(#Pad#+200)` `Y=r` with width `(#ContentW#-210)`.
2. Visible Games + stepper (`−` / value / `+`) on same baseline (`Y=r` / `X=...r`).
3. Section title “Display” then 2×2 grid:
   - Col width `(#ContentW#/2)`; each cell label + toggle Shape with `Y=r` aligned.
4. Gap between tiles + stepper (same pattern as Visible).
5. Section title “Colors” then five rows: label | reset icon | value string; wire existing InputText Command bangs; **update InputText measure X/Y** to new row positions (compute: ContentTop + N*RowH — document the formula used in comments next to each InputText).

Delete any remaining `[DEBUG]` meter.

- [ ] **Step 3: Kill leftover absolute coordinates**

Search `TabLayout.inc` for bare `Y=27`, `Y=47`, `Y=69`, `Y=90`, `Y=110`, `Y=130`, `Y=151`, etc. None may remain except formulas based on `#ContentTop#`/`#RowH#`.

- [ ] **Step 4: Manual verify Layout**

Checklist:
- [ ] Tabs not covered
- [ ] Slider / steppers / toggles / color rows do not overlap
- [ ] Toggles still write SkinInfo; colors still refresh

- [ ] **Step 5: Commit**

```powershell
git add Settings/tabs/TabLayout.inc
@"
feat(settings): relayout Layout tab with relative rows
"@ | Set-Content .git/COMMIT_MSG_TMP.txt -Encoding utf8
git commit -F .git/COMMIT_MSG_TMP.txt
Remove-Item .git/COMMIT_MSG_TMP.txt
```

---

### Task 3: Rewrite TabPaths — relative rows

**Files:**
- Modify: `Settings/tabs/TabPaths.inc`
- Modify: `Settings/Settings.ini` only if PathPickSink / InputText coords need shell-side updates
- Test: manual Paths; pytest 7/7

**Interfaces:**
- Consumes: Content vars, FileChoose, `HasFileChooseFlag`, PathPickSink / GameDirs append-replace logic
- Produces: Paths rows at ContentTop+; Browse `Hidden=(1-#HasFileChooseFlag#)`

- [ ] **Step 1: Anchor + path rows**

Same pattern as Layout: label (ClipString value) | Browse icon.  
SteamPath, GameDirs (Append browse + Replace button), RainMeterEXE, Locale presets + input.

- [ ] **Step 2: Reposition InputText measures**

Update X/Y to match new rows (document formula). Keep `!WriteKeyValue` targets on `#@#SkinInfo.inc`.

- [ ] **Step 3: Manual verify**

Browse (if FileChoose present), append GameDirs with comma, Locale preset, flag=0 hides Browse.

- [ ] **Step 4: Commit**

```powershell
git add Settings/tabs/TabPaths.inc Settings/Settings.ini
@"
feat(settings): relayout Paths tab for large panel
"@ | Set-Content .git/COMMIT_MSG_TMP.txt -Encoding utf8
git commit -F .git/COMMIT_MSG_TMP.txt
Remove-Item .git/COMMIT_MSG_TMP.txt
```

---

### Task 4: Extra scroll region + chrome

**Files:**
- Modify: `Settings/tabs/TabExtra.inc`
- Modify: `Settings/Settings.ini` (ExtraTabRefresh / scroll measures if needed)
- Test: manual Extra scroll; pytest 7/7

**Interfaces:**
- Consumes: ExtraHasN, FindEmpty/Recount, FilePick/ImagePick, ScanNeeded
- Produces: Fixed chrome at ContentTop (Add, Scan banner); scroll container below chrome with height `ContentH - ChromeH`; list rows inside Container with Offset scroll (reuse Hidden/ActionTimer pattern or mouse wheel on Container)

- [ ] **Step 1: Split chrome vs list**

```ini
ExtraChromeH=56
ExtraListTop=(#ContentTop#+#ExtraChromeH#)
ExtraListH=(#ContentH#-#ExtraChromeH#)
```

Chrome meters: Add, ScanNeeded banner/button — **not** inside scroll Container.

- [ ] **Step 2: Scroll container**

Shape/Meter Container at `X=#Pad#` `Y=#ExtraListTop#` `W=#ContentW#` `H=#ExtraListH#`.  
MouseScrollUp/Down adjust `ExtraOffset` (new variable). Row meters use `Y=(...)r` inside container and `Container=ExtraListContainer`.

Keep slots 1–20, `Hidden=[ExtraHasN]` semantics (note: ExtraHas=1 means occupied — existing convention; do not invert).

- [ ] **Step 3: Widen row controls**

Name/Path fields use more of ContentW; Browse/Vis/Icon/Clear on the right with consistent X columns (e.g. Path ends at ContentW-120).

- [ ] **Step 4: Manual verify**

Add game, scroll list, clear gap slot, Scan banner → UpdateGames launch bang unchanged.

- [ ] **Step 5: Commit**

```powershell
git add Settings/tabs/TabExtra.inc Settings/Settings.ini
@"
feat(settings): Extra tab chrome plus inner scroll list
"@ | Set-Content .git/COMMIT_MSG_TMP.txt -Encoding utf8
git commit -F .git/COMMIT_MSG_TMP.txt
Remove-Item .git/COMMIT_MSG_TMP.txt
```

---

### Task 5: HiddenList fit Content + TabHidden chrome + docs/sync

**Files:**
- Modify: `@Resources/extraMeters/HiddenList.inc`
- Modify: `Settings/tabs/TabHidden.inc`
- Modify: `Hidden/Hidden.ini` if shared vars need defaults for standalone width
- Modify: `README.md` (Settings window size note)
- Test: Settings Hidden tab + standalone Hidden.ini; pytest 7/7

**Interfaces:**
- Consumes: ContentW/ContentH or dedicated `HiddenListW`/`VisibleTilesS` equivalents
- Produces: List width ≈ ContentW; EntrySize readable (~24–28); container height ≈ ContentH − UnhideAll chrome; standalone Hidden.ini still usable (fallback vars if ContentTop undefined)

- [ ] **Step 1: Parameterize HiddenList dimensions**

At top of HiddenList (or TabHidden before include):

```ini
HiddenListW=#ContentW#
HiddenListH=(#ContentH#-40)
HiddenEntrySize=24
```

Standalone `Hidden.ini` sets the same literals if Content* vars absent:

```ini
HiddenListW=225
HiddenListH=180
HiddenEntrySize=21
```

(or scale standalone later; minimum: Settings path looks correct).

- [ ] **Step 2: TabHidden chrome**

Unhide All at ContentTop; include HiddenList below. Ensure `HiddenListHidden=0` on tab 4 still works.

- [ ] **Step 3: README**

Under Settings section, note window is 520×640 with scrollable Extra/Hidden lists.

- [ ] **Step 4: Full regression + deploy sync**

```powershell
pytest tests -v
```

Sync workspace → `Documents\Rainmeter\Skins\SteamyRain` with excludes:

```powershell
$src = "D:\Workspace\SteamyRain"
$dst = "C:\Users\macqu_ddk09yx\Documents\Rainmeter\Skins\SteamyRain"
robocopy $src $dst /E /IS /IT /FFT `
  /XD .git .venv venv __pycache__ .pytest_cache .superpowers docs node_modules .cursor "@Resources\dynamicMeters" "@Resources\img\ELogo" "@Resources\img\EIcon" `
  /XF DEVNOTES.md SkinInfo.inc GamesInfo.inc NonSteamGames.inc
& "C:\Program Files\Rainmeter\Rainmeter.exe" "!Refresh" "SteamyRain\Settings" "Settings.ini"
```

Manual checklist:
- [ ] Layout no overlap
- [ ] Paths persist
- [ ] Extra scroll + scan
- [ ] Hidden unhide
- [ ] Close clears Settings=0
- [ ] Mode positioning still OK

- [ ] **Step 5: Commit**

```powershell
git add "@Resources/extraMeters/HiddenList.inc" Settings/tabs/TabHidden.inc Hidden/Hidden.ini README.md
@"
feat(settings): size Hidden list for large panel and document
"@ | Set-Content .git/COMMIT_MSG_TMP.txt -Encoding utf8
git commit -F .git/COMMIT_MSG_TMP.txt
Remove-Item .git/COMMIT_MSG_TMP.txt
```

---

## Spec coverage self-review

| Spec item | Task |
|-----------|------|
| 520×640 shell, header/tab/content split | Task 1 |
| No absolute Y in Content; RowH/Pad vars | Task 1–3 |
| Layout rows + Display grid + Colors | Task 2 |
| Paths rows + Browse gate | Task 3 |
| Extra chrome + inner scroll | Task 4 |
| HiddenList sized; Unhide All; QuickSettings behavior kept | Task 5 |
| README / deploy sync / success criteria | Task 5 |
| No Python GUI; comma GameDirs; no Extra renumber | Global / unchanged code |

## Notes for implementers

- Rainmeter `InputText` plugin positions are absolute; derive from `ContentTop + index*RowH` and keep comments in sync when inserting rows.
- `ShowMeterGroup` unhides meters — never put debug meters in `TabLayout`/`TabExtra` groups.
- Prefer editing deploy copy only via robocopy sync at end of Task 5 (or after each task if iterating live).
