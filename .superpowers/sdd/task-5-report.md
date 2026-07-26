# Task 5 Report: Shared Hidden list + Hidden tab + QuickSettings link

**Status:** DONE  
**Branch:** `sync/local-advantages`  
**Commit:** `25aa42b` — `feat(settings): Hidden tab via shared HiddenList include`

## Summary

Extracted the Hidden games list into `@Resources/extraMeters/HiddenList.inc`, made `Hidden/Hidden.ini` a thin host (positioning + chrome), wired Settings tab 4 to the shared include, and pointed QuickSettings “Hidden Games” at Settings `ActiveTab=4` while deactivating the standalone Hidden config to avoid dual lists.

## What changed

| Path | Action |
|------|--------|
| `@Resources/extraMeters/HiddenList.inc` | **Created** — list measures, styles, Container/scroll, UnHide All, `@include20` dynamic rows |
| `Hidden/Hidden.ini` | Thin host: vars + ModeCheck/MoveWindow/SettingCheck/TilePixelsY + Header/Close/Logo/Title |
| `Settings/tabs/TabHidden.inc` | Includes HiddenList + title; no second Close |
| `Settings/Settings.ini` | Hidden list vars; TabSwitch tab 4 + DeactivateConfig; ModeCheck syncs `ActiveConfig` |
| `@Resources/extraMeters/QuickSettings.inc` | Hidden Games → Settings tab 4 + DeactivateConfig Hidden |
| `@Resources/UpdateGames.pyw` | Hidden meters use `HiddenNameStyle` (avoids Settings `NameStyle` clash) |

### Design notes

1. **`HiddenNameStyle`** — Shared list cannot redefine Settings’ `NameStyle` (Extra/Paths/Layout). Row titles use `HiddenNameStyle`; `UpdateGames.pyw` emits that for `is_hidden` meters. Local `dynamicHiddenMeters.inc` updated (gitignored; regenerated on Scan).
2. **Dual-open gate** — Tab 4 / QuickSettings: `[!DeactivateConfig "SteamyRain\Hidden" "Hidden.ini"]`.
3. **`HiddenWindow`** — Fires when `#hiddenWindow#=1` **or** `#ActiveTab#=4` so Settings unhide still refreshes the main skin.
4. **Standalone Hidden** — Still loadable (`NoGame.inc` / `hiddenWindow` flows). Host keeps Close → deactivate Hidden config.

## Verification

| Check | Result |
|-------|--------|
| `pytest tests -v` | **PASSED** — 7/7 |
| Structural: HiddenList include in Hidden.ini + TabHidden | Pass |
| Structural: QuickSettings → ActiveTab 4 + DeactivateConfig | Pass |
| Structural: ModeCheck sets ActiveConfig with ActiveConf | Pass |
| Extra/Paths tabs | Untouched |
| Manual Rainmeter verify | **Skipped** — interactive Rainmeter unavailable |

## Concerns

1. Manual Rainmeter verify not run — confirm unhide, UnHide All, QuickSettings → tab 4 without overlapping Hidden.ini, and standalone Hidden still opens via NoGame/`hiddenWindow`.
2. `dynamicHiddenMeters.inc` is gitignored; operators need a Scan (or the local file already patched) so rows use `HiddenNameStyle`.
3. Settings `ModeCheck` IfFalseAction now sets `ActiveConf`/`ActiveConfig` to `SteamyRain.ini` (Mode=1); improves main-skin targeting vs leaving List name stuck.
4. `HiddenGames` disabled off tab 4 so Lenght cannot `ShowMeter` empty-state chrome on Layout/Paths/Extra.

---

## Follow-up: HiddenListHidden on tab 4

**Finding:** Reviewer noted `HiddenListHidden=1` in Settings was never cleared on tab 4; `Container`/`NoGameText` use `Hidden=#HiddenListHidden#`, so `ShowMeterGroup TabHidden` alone could leave them hidden or re-hide on update.

**Fix:** `TabSwitch` now `[!SetVariable HiddenListHidden 0]` in `IfTrueAction4`, and `[!SetVariable HiddenListHidden 1]` in actions 1–3 when leaving tab 4. Standalone `Hidden/Hidden.ini` already loads with `HiddenListHidden=0` — unchanged.

**HiddenCheck:** Only `[!Refresh "SteamyRain\Hidden" "Hidden.ini"]` when `#hiddenWindow#=1`; tab 4 already `[!DeactivateConfig]` Hidden, so no Settings conflict — no gate added.

**Commit:** `fix(settings): clear HiddenListHidden on Hidden tab show`

---

## Follow-up: HiddenCheck resurrecting standalone Hidden on tab 4

**Finding:** `[HiddenCheck]` still `[!Refresh "SteamyRain\Hidden"]` whenever `#hiddenWindow#=1`. Tab 4 deactivated Hidden but left `hiddenWindow=1` (NoGame flow), so Settings updates could resurrect a second list alongside tab 4.

**Fix:** `TabSwitch` `IfTrueAction4` and QuickSettings Hidden Games now `[!WriteKeyValue Variables hiddenWindow "0" ...][!SetVariable hiddenWindow 0]` before `[!DeactivateConfig "SteamyRain\Hidden" "Hidden.ini"]`. `HiddenCheck` unchanged — no refresh once flag cleared.

**Commit:** `fix(settings): clear hiddenWindow when opening Hidden tab`
