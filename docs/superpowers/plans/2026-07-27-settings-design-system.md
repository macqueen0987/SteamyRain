# Settings Shared Design System Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Make `Settings/styles/SettingsForm.inc` the single component system all four Settings tabs (Layout/Paths/Extra/Hidden) use, replacing the duplicated ad-hoc styling found in each tab, and fix the specific visual issues found along the way (orphan Locale box, inconsistent icon-button styling, unapplied section spacing, dead variables).

**Architecture:** Add three new shared `MeterStyle` blocks to `SettingsForm.inc` — `FormField` (click-to-edit value box), `FormIconBtn` (glyph button), `FormPill` (preset chip) — then migrate each tab's meters onto them, stripping the per-meter properties that become redundant. Rainmeter has no loops, so Extra tab's 20 repeated slots are migrated via a scoped PowerShell regex pass rather than by hand.

**Tech Stack:** Rainmeter INI meters/measures (no plugin/behavior changes), PowerShell for the mechanical Extra-slot transform, `pytest tests -v` for Python regression (unaffected but must stay green).

## Global Constraints

- Pure Rainmeter meters/measures/bangs only — no Python GUI, no new plugins.
- Color tokens (`Dark1`, `Dark2`, `TextColor`, `TextBG`, `InputBG`, `SettingsHover`, `Opacity`) are reused as-is, never redefined.
- `FormToggle` checkbox color rule is fixed policy: checked → `#TextColor#` (bright), unchecked → `#Dark1#` (recedes). Do not invert.
- Shell chrome (`Header`, `TabStrip`, `Close`, `Logo`) is out of scope — already shared/consistent.
- `HiddenList.inc` is shared by `Settings/tabs/TabHidden.inc` and standalone `Hidden/Hidden.ini` — any change there must be verified in both hosts.
- `HiddenNameStyle` (row hover-highlight), `VisStyle` (icon swaps glyph on hover), and CTA buttons (`+ Add`, `Scan`, `Unhide all`) encode interaction behavior the three new styles don't cover (background-swap-on-hover, icon-swap-on-hover, prominent CTA look) — left out of this pass to avoid regressing that behavior. Only `HiddenButtonStyle` (arrows) migrates, since its hover is a plain FontColor swap that matches `FormIconBtn` exactly.
- `GameDirsReplace` stays as its own text-label button (not `FormIconBtn`, which hardcodes `FontFace=Segoe Fluent Icons`) — it already uses the same `SolidColor=#Dark2#,1` background/hover pattern, so no change needed there.
- Windows dev machine: use PowerShell for scripted edits; commit messages via `git commit -F` (no bash heredoc) per existing project convention.
- After every task: sync `Settings/` (and `@Resources/`/`Hidden/` for Task 7) to `C:\Users\macqu_ddk09yx\Documents\Rainmeter\Skins\SteamyRain` via robocopy, then `!Refresh` the relevant Rainmeter config, before doing the manual visual check.

## File map

| File | Responsibility |
|------|-----------------|
| `Settings/styles/SettingsForm.inc` | Add `FormField`/`FormIconBtn`/`FormPill`, apply `FormSectionGap` |
| `Settings/Settings.ini` | Remove dead `RowH`/`SectionGap` variables |
| `Settings/tabs/TabLayout.inc` | Reset icons + color value displays → new styles |
| `Settings/tabs/TabPaths.inc` | Value boxes/browse icons → new styles; Locale pills get selection-state measures |
| `Settings/tabs/TabExtra.inc` | Slot 1 migrated by hand (template), slots 2–20 migrated by script |
| `@Resources/extraMeters/HiddenList.inc` | `HiddenButtonStyle` (arrows only) → `FormIconBtn` |
| `README.md` | One-line note on the shared design system (Task 8) |

---

### Task 1: Extend SettingsForm.inc with FormField/FormIconBtn/FormPill, apply FormSectionGap, remove dead variables

**Files:**
- Modify: `Settings/styles/SettingsForm.inc`
- Modify: `Settings/Settings.ini`
- Test: manual Rainmeter refresh (Layout tab should look unchanged except a touch more space above "Display"/"Colors"); `pytest tests -v`

**Interfaces:**
- Produces: `MeterStyle=FormField`, `MeterStyle=FormIconBtn`, `MeterStyle=FormPill` — consumed by every later task in this plan.

- [ ] **Step 1: Confirm `RowH`/`SectionGap` are truly unused before deleting**

```powershell
Select-String -Path "Settings\Settings.ini","Settings\tabs\*.inc","Settings\styles\*.inc","@Resources\extraMeters\*.inc" -Pattern '#RowH#|#SectionGap#'
```

Expected: no matches (only the definitions themselves, which the pattern above deliberately doesn't match since it requires the `#...#` reference form).

- [ ] **Step 2: Remove the dead variables from `Settings.ini`**

In `[Variables]`, delete these two lines (currently right after `ContentW`):

```ini
RowH=28
SectionGap=12
```

- [ ] **Step 3: Add the three new styles and apply FormSectionGap in `SettingsForm.inc`**

Update the header comment block (top of file) by adding after the existing `Toggle:` line:

```ini
;   Field:   MeterStyle=FormField (click-to-edit value box)
;   IconBtn: MeterStyle=FormIconBtn (glyph button, hover built-in)
;   Pill:    MeterStyle=FormPill (preset chip; pair with a String-measure
;            selection check that flips SolidColor/FontColor on match)
```

Change `[FormSection]`'s `Padding` line from:

```ini
Padding=8,6,8,6
```

to:

```ini
Padding=8,(6+#FormSectionGap#),8,6
```

Append these three new styles after `[FormToggle]`:

```ini
[FormField]
DynamicVariables=1
AntiAlias=1
FontSize=10
FontColor=#TextColor#,180
SolidColor=#TextBG#
Padding=4,1,4,1
H=14
ClipString=1

[FormIconBtn]
DynamicVariables=1
AntiAlias=1
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
SolidColor=#Dark2#,1
MouseActionCursor=0
MouseOverAction=[!SetOption #CURRENTSECTION# FontColor "#SettingsHover#"][!UpdateMeter #CURRENTSECTION#][!Redraw]
MouseLeaveAction=[!SetOption #CURRENTSECTION# FontColor "#TextColor#"][!UpdateMeter #CURRENTSECTION#][!Redraw]

[FormPill]
DynamicVariables=1
AntiAlias=1
FontSize=9
FontColor=#TextColor#
SolidColor=#TextBG#
Padding=4,2,4,2
```

- [ ] **Step 4: Sync, refresh, manual check**

```powershell
robocopy "D:\Workspace\SteamyRain\Settings" "C:\Users\macqu_ddk09yx\Documents\Rainmeter\Skins\SteamyRain\Settings" /E /IS /IT /FFT
& "C:\Program Files\Rainmeter\Rainmeter.exe" "!Refresh" "SteamyRain\Settings" "Settings.ini"
```

Open Settings → Layout tab. Expect: no broken layout, "Display" and "Colors" section headers sit slightly lower in their row (a few px gap above them) than before. Nothing should error in Rainmeter's log (`!Log` or About > Log).

- [ ] **Step 5: pytest regression**

```powershell
.\.venv\Scripts\Activate.ps1
pytest tests -v
```

Expected: same pass count as before this change (no Python touched).

- [ ] **Step 6: Commit**

```powershell
@'
feat(settings): add FormField/FormIconBtn/FormPill styles, apply section gap
'@ | Set-Content .git/COMMIT_MSG_TMP.txt -Encoding utf8
git add Settings/styles/SettingsForm.inc Settings/Settings.ini
git commit -F .git/COMMIT_MSG_TMP.txt
Remove-Item .git/COMMIT_MSG_TMP.txt
```

---

### Task 2: Migrate Layout tab reset icons and color value displays

**Files:**
- Modify: `Settings/tabs/TabLayout.inc`
- Test: manual Layout tab; `pytest tests -v`

**Interfaces:**
- Consumes: `FormIconBtn`, `FormField` from Task 1.

- [ ] **Step 1: Migrate the 5 reset icons to FormIconBtn**

For each of `ResetColorMainBG`, `ResetColorInputBG`, `ResetColorTextColor`, `ResetColorTextBG`, `ResetColorOpacity`: change `MeterStyle=FormValue` to `MeterStyle=FormIconBtn`, and delete that meter's `FontFace=Segoe Fluent Icons` and `FontSize=10` lines (now inherited). Example (`ResetColorMainBG`, apply the same edit shape to the other four):

Before:
```ini
[ResetColorMainBG]
Meter=String
MeterStyle=FormValue
Text="[\xe72c]"
FontFace=Segoe Fluent Icons
FontSize=10
Y=(#FormRowY0#+9*#FormRowH#+6)
LeftMouseUpAction=[!WriteKeyValue Variables Dark1 #Dark1_DEFAULT# "#@#SkinInfo.inc"][!Setvariable Dark1 #Dark1_DEFAULT#][!RefreshApp]
Group=TabLayout
```

After:
```ini
[ResetColorMainBG]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe72c]"
Y=(#FormRowY0#+9*#FormRowH#+6)
LeftMouseUpAction=[!WriteKeyValue Variables Dark1 #Dark1_DEFAULT# "#@#SkinInfo.inc"][!Setvariable Dark1 #Dark1_DEFAULT#][!RefreshApp]
Group=TabLayout
```

This means reset icons now show a small `Dark2` chip background and gain hover feedback (previously bare glyph, no hover) — intentional, matches the Browse icon look.

- [ ] **Step 2: Migrate the 5 color value displays to FormField**

For each of `InputColorMainBG`, `InputColorInputBG`, `InputColorTextColor`, `InputColorTextBG`, `InputColorOpacity`: change `MeterStyle=FormValue` to `MeterStyle=FormField`, add `W=100`. Example (`InputColorMainBG`):

Before:
```ini
[InputColorMainBG]
Meter=String
MeterStyle=FormValue
Text=#Dark1#
X=(#FormControlX#+28)
Y=(#FormRowY0#+9*#FormRowH#+6)
LeftMouseUpAction=!CommandMeasure "InputColorMainBGMeasure" "ExecuteBatch 1-2"
Group=TabLayout
```

After:
```ini
[InputColorMainBG]
Meter=String
MeterStyle=FormField
Text=#Dark1#
X=(#FormControlX#+28)
Y=(#FormRowY0#+9*#FormRowH#+6)
W=100
LeftMouseUpAction=!CommandMeasure "InputColorMainBGMeasure" "ExecuteBatch 1-2"
Group=TabLayout
```

Apply the same shape to `InputColorInputBG` (row 10), `InputColorTextColor` (row 11), `InputColorTextBG` (row 12), `InputColorOpacity` (row 13) — only `Text=`/`Y=`/`LeftMouseUpAction=` differ per meter; each already has its own correct values, just add `MeterStyle=FormField` (replacing `FormValue`) and `W=100`.

- [ ] **Step 3: Sync, refresh, manual check**

```powershell
robocopy "D:\Workspace\SteamyRain\Settings" "C:\Users\macqu_ddk09yx\Documents\Rainmeter\Skins\SteamyRain\Settings" /E /IS /IT /FFT
& "C:\Program Files\Rainmeter\Rainmeter.exe" "!Refresh" "SteamyRain\Settings" "Settings.ini"
```

Open Layout tab, Colors section. Expect: each color value now shows in a small boxed field (matches Paths tab's boxed values), reset icons show a chip with hover highlight. Click a color value to confirm the InputText popup still opens and writes correctly; click a reset icon to confirm it still resets.

- [ ] **Step 4: pytest regression**

```powershell
pytest tests -v
```

- [ ] **Step 5: Commit**

```powershell
@'
feat(settings): migrate Layout color rows to FormField/FormIconBtn
'@ | Set-Content .git/COMMIT_MSG_TMP.txt -Encoding utf8
git add Settings/tabs/TabLayout.inc
git commit -F .git/COMMIT_MSG_TMP.txt
Remove-Item .git/COMMIT_MSG_TMP.txt
```

---

### Task 3: Migrate Paths tab value boxes and browse icons

**Files:**
- Modify: `Settings/tabs/TabPaths.inc`
- Test: manual Paths tab; `pytest tests -v`

**Interfaces:**
- Consumes: `FormField`, `FormIconBtn` from Task 1.

- [ ] **Step 1: Migrate the 4 value boxes to FormField**

`SteamPathValue`, `GameDirsValue`, `RainMeterEXEValue`, `LocaleValue`: change `MeterStyle=FormValue` to `MeterStyle=FormField`, delete `H=14`, `ClipString=1`, `SolidColor=#TextBG#`, `Padding=4,1,4,1` (all now inherited). Keep each meter's own `W=` (they differ: `(#FormControlW#-28)`, `(#FormControlW#-72)`, `(#FormControlW#-28)`, `#FormControlW#`).

Before (`SteamPathValue`):
```ini
[SteamPathValue]
Meter=String
MeterStyle=FormValue
Text=#SteamPath#
Y=(#FormRowY0#+0*#FormRowH#+6)
W=(#FormControlW#-28)
H=14
ClipString=1
SolidColor=#TextBG#
Padding=4,1,4,1
LeftMouseUpAction=[!CommandMeasure InputSteamPathMeasure "ExecuteBatch 1"]
Group=TabPaths
Hidden=1
```

After:
```ini
[SteamPathValue]
Meter=String
MeterStyle=FormField
Text=#SteamPath#
Y=(#FormRowY0#+0*#FormRowH#+6)
W=(#FormControlW#-28)
LeftMouseUpAction=[!CommandMeasure InputSteamPathMeasure "ExecuteBatch 1"]
Group=TabPaths
Hidden=1
```

Apply the same shape (drop `H=14`/`ClipString=1`/`SolidColor=#TextBG#`/`Padding=4,1,4,1`, swap `MeterStyle`) to `GameDirsValue`, `RainMeterEXEValue`, `LocaleValue` — each keeps its own `Y=`/`W=`/`LeftMouseUpAction=`.

- [ ] **Step 2: Migrate the 3 browse icons to FormIconBtn**

`SteamPathBrowse`, `GameDirsBrowse`, `RainMeterEXEBrowse`: change `MeterStyle=FormValue` to `MeterStyle=FormIconBtn`, delete `FontFace=Segoe Fluent Icons`, `FontSize=12`, `FontColor=#TextColor#`, `SolidColor=#Dark2#,1`, `MouseOverAction=...`, `MouseLeaveAction=...` (all inherited now).

Before (`SteamPathBrowse`):
```ini
[SteamPathBrowse]
Meter=String
MeterStyle=FormValue
Text="[\xe8b7]"
FontFace=Segoe Fluent Icons
FontSize=12
FontColor=#TextColor#
SolidColor=#Dark2#,1
Group=TabPaths
Hidden=1
X=(#FormControlX#+#FormControlW#-24)
Y=(#FormRowY0#+0*#FormRowH#+4)
LeftMouseUpAction=[!SetVariable _PickTarget SteamPath][!CommandMeasure FileChooseFolder "ChooseFolder 1"]
MouseOverAction=[!SetOption SteamPathBrowse FontColor "#SettingsHover#"][!UpdateMeter SteamPathBrowse][!Redraw]
MouseLeaveAction=[!SetOption SteamPathBrowse FontColor "#TextColor#"][!UpdateMeter SteamPathBrowse][!Redraw]
```

After:
```ini
[SteamPathBrowse]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b7]"
Group=TabPaths
Hidden=1
X=(#FormControlX#+#FormControlW#-24)
Y=(#FormRowY0#+0*#FormRowH#+4)
LeftMouseUpAction=[!SetVariable _PickTarget SteamPath][!CommandMeasure FileChooseFolder "ChooseFolder 1"]
```

Apply the same shape to `GameDirsBrowse` and `RainMeterEXEBrowse` — keep each one's own `X=`/`Y=`/`LeftMouseUpAction=`.

`GameDirsReplace` is **not** touched in this task (see Global Constraints — it's a text label, not an icon glyph).

- [ ] **Step 3: Sync, refresh, manual check**

```powershell
robocopy "D:\Workspace\SteamyRain\Settings" "C:\Users\macqu_ddk09yx\Documents\Rainmeter\Skins\SteamyRain\Settings" /E /IS /IT /FFT
& "C:\Program Files\Rainmeter\Rainmeter.exe" "!Refresh" "SteamyRain\Settings" "Settings.ini"
```

Open Paths tab. Expect: value boxes render identically to before (same box look, now shared). Click each value box to confirm InputText edit still opens; click each browse icon to confirm FileChoose still opens and hover highlight still works.

- [ ] **Step 4: pytest regression**

```powershell
pytest tests -v
```

- [ ] **Step 5: Commit**

```powershell
@'
feat(settings): migrate Paths value boxes and browse icons to shared styles
'@ | Set-Content .git/COMMIT_MSG_TMP.txt -Encoding utf8
git add Settings/tabs/TabPaths.inc
git commit -F .git/COMMIT_MSG_TMP.txt
Remove-Item .git/COMMIT_MSG_TMP.txt
```

---

### Task 4: Locale preset pills — FormPill + selection-state measures

**Files:**
- Modify: `Settings/tabs/TabPaths.inc`
- Test: manual Paths tab; `pytest tests -v`

**Interfaces:**
- Consumes: `FormPill` from Task 1.
- Produces: measures `LocaleCheckEnglish`, `LocaleCheckKoreana` — not consumed elsewhere, but any future code that changes `#Locale#` must also `!UpdateMeasure` both of these to keep the pill highlight correct (documented inline as a comment).

- [ ] **Step 1: Add the two selection-state measures**

Add right after `[InputLocaleMeasure]` (in the measures block, before the `;___METERS___` divider):

```ini
; Keeps the Locale pills' highlight in sync with #Locale#. Any bang that
; changes Locale must also !UpdateMeasure both of these.
[LocaleCheckEnglish]
Measure=String
String=#Locale#
DynamicVariables=1
IfMatch=^english$
IfMatchAction=[!SetOption LocalePresetEnglish SolidColor "#SettingsHover#"][!SetOption LocalePresetEnglish FontColor "#Dark1#"][!UpdateMeter LocalePresetEnglish][!Redraw]
IfNotMatchAction=[!SetOption LocalePresetEnglish SolidColor "#TextBG#"][!SetOption LocalePresetEnglish FontColor "#TextColor#"][!UpdateMeter LocalePresetEnglish][!Redraw]

[LocaleCheckKoreana]
Measure=String
String=#Locale#
DynamicVariables=1
IfMatch=^koreana$
IfMatchAction=[!SetOption LocalePresetKoreana SolidColor "#SettingsHover#"][!SetOption LocalePresetKoreana FontColor "#Dark1#"][!UpdateMeter LocalePresetKoreana][!Redraw]
IfNotMatchAction=[!SetOption LocalePresetKoreana SolidColor "#TextBG#"][!SetOption LocalePresetKoreana FontColor "#TextColor#"][!UpdateMeter LocalePresetKoreana][!Redraw]
```

- [ ] **Step 2: Migrate the two pill meters to FormPill and wire the refresh**

Before:
```ini
[LocalePresetEnglish]
Meter=String
MeterStyle=FormValue
Text="english"
FontSize=9
FontColor=#TextColor#
SolidColor=#TextBG#
Padding=4,2,4,2
Group=TabPaths
Hidden=1
Y=(#FormRowY0#+3*#FormRowH#+4)
LeftMouseUpAction=[!WriteKeyValue Variables Locale "english" "#@#SkinInfo.inc"][!SetVariable Locale "english"][!UpdateMeterGroup TabPaths][!Redraw]
MouseOverAction=[!SetOption LocalePresetEnglish FontColor "#SettingsHover#"][!UpdateMeter LocalePresetEnglish][!Redraw]
MouseLeaveAction=[!SetOption LocalePresetEnglish FontColor "#TextColor#"][!UpdateMeter LocalePresetEnglish][!Redraw]
```

After:
```ini
[LocalePresetEnglish]
Meter=String
MeterStyle=FormPill
Text="english"
Group=TabPaths
Hidden=1
Y=(#FormRowY0#+3*#FormRowH#+4)
LeftMouseUpAction=[!WriteKeyValue Variables Locale "english" "#@#SkinInfo.inc"][!SetVariable Locale "english"][!UpdateMeterGroup TabPaths][!UpdateMeasure LocaleCheckEnglish][!UpdateMeasure LocaleCheckKoreana][!Redraw]
```

Same shape for `LocalePresetKoreana` (keeps its own `X=(#FormControlX#+64)`):

```ini
[LocalePresetKoreana]
Meter=String
MeterStyle=FormPill
Text="koreana"
Group=TabPaths
Hidden=1
X=(#FormControlX#+64)
Y=(#FormRowY0#+3*#FormRowH#+4)
LeftMouseUpAction=[!WriteKeyValue Variables Locale "koreana" "#@#SkinInfo.inc"][!SetVariable Locale "koreana"][!UpdateMeterGroup TabPaths][!UpdateMeasure LocaleCheckEnglish][!UpdateMeasure LocaleCheckKoreana][!Redraw]
```

- [ ] **Step 3: Also refresh the pills when Locale is typed manually**

In `[InputLocaleMeasure]`'s `Command1`, insert the same two `!UpdateMeasure` calls before `[!SetVariable Locale ...]`'s trailing `[!Redraw]` — wait, insert them right before the final `[!Redraw]`... actually there's no `[!Redraw]` in `Command1` today, so append at the end:

Before:
```ini
Command1=[!WriteKeyValue Variables Locale "$UserInput$" "#@#SkinInfo.inc"][!SetVariable Locale "$UserInput$"][!UpdateMeterGroup TabPaths][!Redraw]
```

After:
```ini
Command1=[!WriteKeyValue Variables Locale "$UserInput$" "#@#SkinInfo.inc"][!SetVariable Locale "$UserInput$"][!UpdateMeterGroup TabPaths][!UpdateMeasure LocaleCheckEnglish][!UpdateMeasure LocaleCheckKoreana][!Redraw]
```

- [ ] **Step 4: Sync, refresh, manual check**

```powershell
robocopy "D:\Workspace\SteamyRain\Settings" "C:\Users\macqu_ddk09yx\Documents\Rainmeter\Skins\SteamyRain\Settings" /E /IS /IT /FFT
& "C:\Program Files\Rainmeter\Rainmeter.exe" "!Refresh" "SteamyRain\Settings" "Settings.ini"
```

Open Paths tab. Expect: whichever preset matches the current `Locale` value shows highlighted (bright background, dark text); the other shows dim. Click the other preset — highlight should flip. Type a custom locale into the value field below and confirm both presets go dim (neither matches).

- [ ] **Step 5: pytest regression**

```powershell
pytest tests -v
```

- [ ] **Step 6: Commit**

```powershell
@'
feat(settings): give Locale presets a real selected-state indicator (FormPill)
'@ | Set-Content .git/COMMIT_MSG_TMP.txt -Encoding utf8
git add Settings/tabs/TabPaths.inc
git commit -F .git/COMMIT_MSG_TMP.txt
Remove-Item .git/COMMIT_MSG_TMP.txt
```

---

### Task 5: Extra tab — migrate slot 1 (template for the script in Task 6)

**Files:**
- Modify: `Settings/tabs/TabExtra.inc`
- Test: manual Extra tab, slot 1 only; `pytest tests -v`

**Interfaces:**
- Consumes: `FormField`, `FormIconBtn` from Task 1.
- Produces: the exact before/after shape Task 6's script must reproduce for slots 2–20.

- [ ] **Step 1: Migrate ExtraRow1Name and ExtraRow1Path to FormField**

Before:
```ini
[ExtraRow1Name]
Meter=String
Text=#Egame1#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow1
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas1]
Y=(((1-1)*#ExtraRowH#+16)-#ExtraOffset#)
X=0
W=110
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 1][!SetOption InputExtraNameMeasure DefaultValue "#Egame1#"][!SetOption InputExtraNameMeasure Y "[ExtraRow1Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]
```

After:
```ini
[ExtraRow1Name]
Meter=String
Text=#Egame1#
MeterStyle=FormField
Group=TabExtra | ExtraRow1
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas1]
Y=(((1-1)*#ExtraRowH#+16)-#ExtraOffset#)
X=0
W=110
LeftMouseUpAction=[!SetVariable ExtraEditIndex 1][!SetOption InputExtraNameMeasure DefaultValue "#Egame1#"][!SetOption InputExtraNameMeasure Y "[ExtraRow1Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]
```

Same shape for `ExtraRow1Path` (drop `H=14`/`ClipString=1`/`SolidColor=#TextBG#`/`Padding=2,1,2,1`, swap `MeterStyle` to `FormField`, keep its own `W=(#ContentW#-234)`, `X=114`, `LeftMouseUpAction=`).

- [ ] **Step 2: Migrate ExtraRow1Browse/Vis/Icon/Clear to FormIconBtn**

Before (`ExtraRow1Browse`):
```ini
[ExtraRow1Browse]
Meter=String
Text="[\xe8b7]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow1
Container=ExtraListContainer
Hidden=[ExtraHas1]
Y=r
X=(#ContentW#-112)
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickTarget Egame1Path][!SetVariable ExtraEditIndex 1][!CommandMeasure FileChooseFile "ChooseFile 1"]
MouseOverAction=[!SetOption ExtraRow1Browse FontColor "#SettingsHover#"][!UpdateMeter ExtraRow1Browse][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow1Browse FontColor "#TextColor#"][!UpdateMeter ExtraRow1Browse][!Redraw]
```

After:
```ini
[ExtraRow1Browse]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b7]"
DynamicVariables=1
Group=TabExtra | ExtraRow1
Container=ExtraListContainer
Hidden=[ExtraHas1]
Y=r
X=(#ContentW#-112)
LeftMouseUpAction=[!SetVariable _PickTarget Egame1Path][!SetVariable ExtraEditIndex 1][!CommandMeasure FileChooseFile "ChooseFile 1"]
```

Apply the same shape to `ExtraRow1Vis`, `ExtraRow1Icon`, `ExtraRow1Clear` — each keeps its own `Text=`/`X=`/`LeftMouseUpAction=`, drops `FontFace`/`FontSize`/`FontColor`/`AntiAlias`/`SolidColor`/`MouseOverAction`/`MouseLeaveAction`, adds `MeterStyle=FormIconBtn`.

- [ ] **Step 3: Sync, refresh, manual check on slot 1 only**

```powershell
robocopy "D:\Workspace\SteamyRain\Settings" "C:\Users\macqu_ddk09yx\Documents\Rainmeter\Skins\SteamyRain\Settings" /E /IS /IT /FFT
& "C:\Program Files\Rainmeter\Rainmeter.exe" "!Refresh" "SteamyRain\Settings" "Settings.ini"
```

Open Extra tab. If slot 1 has an entry, confirm: name/path fields look like boxed values (matches Paths tab), the 4 icon buttons render with hover highlight, and clicking each (edit name, edit path, browse, toggle vis, pick icon, clear) still works. If slot 1 is empty, temporarily add a game via "+ Add" to test, then clear it back out.

- [ ] **Step 4: pytest regression**

```powershell
pytest tests -v
```

- [ ] **Step 5: Commit**

```powershell
@'
feat(settings): migrate Extra slot 1 to FormField/FormIconBtn (template)
'@ | Set-Content .git/COMMIT_MSG_TMP.txt -Encoding utf8
git add Settings/tabs/TabExtra.inc
git commit -F .git/COMMIT_MSG_TMP.txt
Remove-Item .git/COMMIT_MSG_TMP.txt
```

---

### Task 6: Extra tab — apply the same migration to slots 2–20 via script

**Files:**
- Modify: `Settings/tabs/TabExtra.inc`
- Test: manual Extra tab across multiple slots; `pytest tests -v`

**Interfaces:**
- Consumes: the exact before/after shape from Task 5.

- [ ] **Step 1: Run the migration script**

Save as `scripts/migrate-extra-slots.ps1` (temporary, may be deleted after use) and run it:

```powershell
$path = "Settings/tabs/TabExtra.inc"
$raw = Get-Content $path -Raw

$sectionPattern = '(?ms)^\[(?<name>[^\]]+)\]\r?\n(?<body>.*?)(?=^\[|\z)'
$matches = [regex]::Matches($raw, $sectionPattern)

$sb = New-Object System.Text.StringBuilder
foreach ($m in $matches) {
    $name = $m.Groups['name'].Value
    $body = $m.Groups['body'].Value

    if ($name -match '^ExtraRow([2-9]|1[0-9]|20)(Browse|Vis|Icon|Clear)$') {
        $lines = [regex]::Split($body, "\r?\n")
        $keep = $lines | Where-Object {
            $_ -notmatch '^(FontFace=Segoe Fluent Icons|FontSize=11|FontColor=#TextColor#|AntiAlias=1|SolidColor=#Dark2#,1|MouseOverAction=\[!SetOption|MouseLeaveAction=\[!SetOption)'
        }
        $out = New-Object System.Collections.Generic.List[string]
        foreach ($l in $keep) {
            $out.Add($l)
            if ($l -eq 'Meter=String') { $out.Add('MeterStyle=FormIconBtn') }
        }
        $body = ($out -join "`r`n")
    }
    elseif ($name -match '^ExtraRow([2-9]|1[0-9]|20)(Name|Path)$') {
        $lines = [regex]::Split($body, "\r?\n")
        $mapped = $lines | ForEach-Object { if ($_ -eq 'MeterStyle=NameStyle') { 'MeterStyle=FormField' } else { $_ } }
        $keep = $mapped | Where-Object {
            $_ -notmatch '^(ClipString=1|SolidColor=#TextBG#|Padding=2,1,2,1|H=14)$'
        }
        $body = ($keep -join "`r`n")
    }

    [void]$sb.Append("[$name]`r`n$body")
}

Set-Content -Path $path -Value $sb.ToString() -NoNewline -Encoding UTF8
```

- [ ] **Step 2: Verify the transform landed on all 19 slots and touched nothing else**

```powershell
(Select-String -Path "Settings\tabs\TabExtra.inc" -Pattern '^MeterStyle=FormIconBtn$').Count
(Select-String -Path "Settings\tabs\TabExtra.inc" -Pattern '^MeterStyle=FormField$').Count
```

Expected: `FormIconBtn` count = 80 (20 slots × 4 icon meters, including slot 1 from Task 5), `FormField` count = 40 (20 slots × 2 field meters, including slot 1).

```powershell
git diff --stat Settings/tabs/TabExtra.inc
```

Read through `git diff Settings/tabs/TabExtra.inc` — confirm the `ExtraAddBtn`/`ExtraScanBtn`/`ExtraScanBanner`/`ExtraTitle` chrome meters (out of scope) are untouched, and no `; --- Slot N ---` comments were dropped.

- [ ] **Step 3: Delete the temporary script**

```powershell
Remove-Item scripts/migrate-extra-slots.ps1
```

- [ ] **Step 4: Sync, refresh, manual check across several slots**

```powershell
robocopy "D:\Workspace\SteamyRain\Settings" "C:\Users\macqu_ddk09yx\Documents\Rainmeter\Skins\SteamyRain\Settings" /E /IS /IT /FFT
& "C:\Program Files\Rainmeter\Rainmeter.exe" "!Refresh" "SteamyRain\Settings" "Settings.ini"
```

Open Extra tab, scroll the list. Spot-check at least: an early slot (2), a middle slot (~10), and the last used slot — confirm name/path fields and all 4 icon buttons render and their actions (edit, browse, vis toggle, icon pick, clear) still work. Add a new game (via "+ Add", which fills the first empty slot) and confirm the newly-created row also renders correctly with the new styles.

- [ ] **Step 5: pytest regression**

```powershell
pytest tests -v
```

- [ ] **Step 6: Commit**

```powershell
@'
feat(settings): migrate Extra slots 2-20 to FormField/FormIconBtn
'@ | Set-Content .git/COMMIT_MSG_TMP.txt -Encoding utf8
git add Settings/tabs/TabExtra.inc
git commit -F .git/COMMIT_MSG_TMP.txt
Remove-Item .git/COMMIT_MSG_TMP.txt
```

---

### Task 7: Hidden tab — migrate arrow buttons to FormIconBtn

**Files:**
- Modify: `@Resources/extraMeters/HiddenList.inc`
- Test: manual Settings → Hidden tab AND standalone `Hidden.ini`; `pytest tests -v`

**Interfaces:**
- Consumes: `FormIconBtn` from Task 1.
- Non-goal (see Global Constraints): `HiddenNameStyle` and `VisStyle` are NOT touched in this task.

- [ ] **Step 1: Migrate ArrowLess and ArrowMore off HiddenButtonStyle**

Before (`ArrowLess`):
```ini
[ArrowLess]
Meter=String
Text="[\xe935]"
FontColor=#TextBG#
LeftMouseUpAction=[!CommandMeasure MoveTimer "Execute 2"][!UpdateMeter ArrowLess][!UpdateMeasureGroup MoveTimer][!ReDraw]
MouseOverAction=[!SetOption ArrowLess FontColor "#TextColor#"][!UpdateMeter ArrowLess][!ReDraw]
MouseLeaveAction=[!SetOption ArrowLess FontColor "#TextBG#"][!UpdateMeter ArrowLess][!ReDraw]
InlineSetting=Shadow | 0 | -1 | 2 | #Dark2#
MeterStyle=HiddenButtonStyle
Group=Incr | Mode | Arrows | TabHidden
Hidden=1
X=(#HiddenListX#+#HiddenListW#/2)
Y=(#HiddenListY#-5)
```

`HiddenButtonStyle` currently provides `SolidColor=#Dark2#,1 / FontColor=#TextColor# / FontFace=Segoe Fluent Icons / FontWeight=400 / FontSize=12 / AntiAlias=1 / MouseActionCursor=0` — but the meter itself overrides `FontColor` to `#TextBG#` at rest and swaps to `#TextColor#` on hover (inverse of the base style, an intentional "arrow starts dim, brightens on hover" look). `FormIconBtn` does the opposite (starts `#TextColor#`, brightens further to `#SettingsHover#`) — keep that per-meter override pattern, just swap the base style:

After:
```ini
[ArrowLess]
Meter=String
Text="[\xe935]"
FontColor=#TextBG#
LeftMouseUpAction=[!CommandMeasure MoveTimer "Execute 2"][!UpdateMeter ArrowLess][!UpdateMeasureGroup MoveTimer][!ReDraw]
MouseOverAction=[!SetOption ArrowLess FontColor "#TextColor#"][!UpdateMeter ArrowLess][!ReDraw]
MouseLeaveAction=[!SetOption ArrowLess FontColor "#TextBG#"][!UpdateMeter ArrowLess][!ReDraw]
InlineSetting=Shadow | 0 | -1 | 2 | #Dark2#
MeterStyle=FormIconBtn
Group=Incr | Mode | Arrows | TabHidden
Hidden=1
X=(#HiddenListX#+#HiddenListW#/2)
Y=(#HiddenListY#-5)
```

Same edit for `ArrowMore` — only its `MeterStyle=HiddenButtonStyle` line changes to `MeterStyle=FormIconBtn`; every other line (including its own `MouseOverAction`/`MouseLeaveAction` override) stays.

Note: because both meters set their own `MouseOverAction`/`MouseLeaveAction`, they don't inherit `FormIconBtn`'s hover bang — that's expected and correct here, since they need the inverse dim→bright behavior, not `FormIconBtn`'s bright→brighter one. The migration only reuses `FormIconBtn`'s base look (`SolidColor`/`FontFace`/`FontSize`/`MouseActionCursor`), not its hover.

- [ ] **Step 2: Sync both hosts, refresh both, manual check**

```powershell
robocopy "D:\Workspace\SteamyRain\@Resources" "C:\Users\macqu_ddk09yx\Documents\Rainmeter\Skins\SteamyRain\@Resources" /E /IS /IT /FFT /XD dynamicMeters ELogo EIcon
robocopy "D:\Workspace\SteamyRain\Hidden" "C:\Users\macqu_ddk09yx\Documents\Rainmeter\Skins\SteamyRain\Hidden" /E /IS /IT /FFT
robocopy "D:\Workspace\SteamyRain\Settings" "C:\Users\macqu_ddk09yx\Documents\Rainmeter\Skins\SteamyRain\Settings" /E /IS /IT /FFT
& "C:\Program Files\Rainmeter\Rainmeter.exe" "!Refresh" "SteamyRain\Settings" "Settings.ini"
& "C:\Program Files\Rainmeter\Rainmeter.exe" "!Refresh" "SteamyRain\Hidden" "Hidden.ini"
```

Check both: Settings → Hidden tab (with enough hidden games that the arrows appear) and the standalone Hidden window. Confirm arrow buttons still look and behave the same (dim at rest, bright on hover, scroll the list on click).

- [ ] **Step 3: pytest regression**

```powershell
pytest tests -v
```

- [ ] **Step 4: Commit**

```powershell
@'
feat(settings): migrate Hidden list arrow buttons to FormIconBtn base style
'@ | Set-Content .git/COMMIT_MSG_TMP.txt -Encoding utf8
git add "@Resources/extraMeters/HiddenList.inc"
git commit -F .git/COMMIT_MSG_TMP.txt
Remove-Item .git/COMMIT_MSG_TMP.txt
```

---

### Task 8: Full regression pass + README note

**Files:**
- Modify: `README.md`
- Test: full manual walkthrough of all 4 tabs; `pytest tests -v`

- [ ] **Step 1: Full sync**

```powershell
$src = "D:\Workspace\SteamyRain"
$dst = "C:\Users\macqu_ddk09yx\Documents\Rainmeter\Skins\SteamyRain"
robocopy $src $dst /E /IS /IT /FFT `
  /XD .git .venv venv __pycache__ .pytest_cache .superpowers docs node_modules .cursor "@Resources\dynamicMeters" "@Resources\img\ELogo" "@Resources\img\EIcon" `
  /XF DEVNOTES.md SkinInfo.inc GamesInfo.inc NonSteamGames.inc
& "C:\Program Files\Rainmeter\Rainmeter.exe" "!Refresh" "SteamyRain\Settings" "Settings.ini"
```

- [ ] **Step 2: Manual checklist**

- [ ] Layout: toggles show correct checked/unchecked color, color fields boxed and editable, reset icons work
- [ ] Paths: value fields boxed and editable, browse icons work, Locale pill highlight matches current value, typing a custom locale dims both pills
- [ ] Extra: scroll through full list, add a game, edit name/path, browse, toggle visibility, pick icon, clear a slot
- [ ] Hidden: arrows scroll, Unhide All works, standalone `Hidden.ini` still opens correctly from QuickSettings
- [ ] No meter overlap or missing hover feedback on any tab

- [ ] **Step 3: pytest regression**

```powershell
pytest tests -v
```

Expected: 7 passed (or whatever the current baseline count is — must match pre-plan baseline).

- [ ] **Step 4: Add README note**

Under the Settings section, add one line noting the shared design system, e.g.:

```markdown
Settings tabs (Layout/Paths/Extra/Hidden) share one component system defined in `Settings/styles/SettingsForm.inc` (`FormLabel`/`FormSection`/`FormField`/`FormIconBtn`/`FormPill`/`FormToggle`) — new controls should reuse these styles rather than hand-rolling new ones.
```

- [ ] **Step 5: Commit**

```powershell
@'
docs: note shared Settings design system in README
'@ | Set-Content .git/COMMIT_MSG_TMP.txt -Encoding utf8
git add README.md
git commit -F .git/COMMIT_MSG_TMP.txt
Remove-Item .git/COMMIT_MSG_TMP.txt
```

---

## Spec coverage self-review

| Spec item | Task |
|-----------|------|
| FormField/FormIconBtn/FormPill added, FormSectionGap applied, dead vars removed | Task 1 |
| Layout tab color rows on shared styles | Task 2 |
| Paths tab value/browse on shared styles | Task 3 |
| Locale pill selected-state (new behavior, not in original spec table but required to make FormPill meaningful) | Task 4 |
| Extra tab slot 1 template + slots 2–20 mechanical migration | Task 5–6 |
| Checkbox on/off color rule documented | Global Constraints (already fixed in code pre-plan) |
| Paths empty space stays intentional whitespace | No task — explicitly a non-change, documented in spec |
| Hidden tab scope correction: `HiddenNameStyle`/`VisStyle`/CTA buttons excluded (behavior mismatch found during planning) | Global Constraints + Task 7 |
| README note | Task 8 |
