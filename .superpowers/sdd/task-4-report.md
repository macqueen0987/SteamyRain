# Task 4 Report: Extra tab + empty-slot scan skip

**Status:** DONE  
**Branch:** `sync/local-advantages`  
**Commit:** `efd8243` — `feat(settings): Extra games tab and skip empty NonSteam slots`

## Summary

Replaced the Extra stub with a 20-slot Non-Steam UI (add/edit/clear/vis/icon/scan), wired FilePickSink/ImagePickSink for Extra paths and icon copy, and taught `UpdateGames.pyw` to skip empty `EgameN` names via `iter_extra_game_indices` (TDD RED→GREEN).

## TDD evidence

| Step | Command | Result |
|------|---------|--------|
| RED | `pytest tests/test_update_games.py::test_iter_extra_game_indices_skips_empty_slots -v` | **FAILED** — `AttributeError: module 'update_games' has no attribute 'iter_extra_game_indices'` |
| GREEN | same + `pytest tests -v` | **PASSED** — 1/1 then **7/7** |

## What changed

| Path | Action |
|------|--------|
| `tests/test_update_games.py` | Added `test_iter_extra_game_indices_skips_empty_slots` |
| `@Resources/UpdateGames.pyw` | `iter_extra_game_indices`; `write_meters` iterates only non-empty Extra indices; `extra_games_vars` loaded in `main` |
| `Settings/tabs/TabExtra.inc` | Full Extra UI: header/Add, ScanNeeded banner+Scan, hint, slots 1–20 (name/path/browse/vis/icon/clear), `ExtraHasN` empty hide |
| `Settings/Settings.ini` | Extra vars; FilePickRouter; ImagePickSink copy bang; FindEmpty/Clear/Recount/ToggleVis/Bump; ExtraTabRefresh; InputText ×2; TabSwitch→ExtraTabRefresh |

### Extra UI behavior

1. **Add** — Loop `FindEmptyExtraSlot` finds first empty `EgameN`, writes `"New Game"` / empty path / Vis=0, bumps `ExtraGamesCount` if needed and `ExtraGameCountPLUS`, `ScanNeeded=1`.
2. **Rows 1–20** — `Hidden` via `ExtraHasN` String RegExp (`^\s*$`→1); `ExtraTabRefresh` re-applies after `ShowMeterGroup` (which overwrites Hidden).
3. **Edit** — name/path InputText; Browse → `FileChooseFile` → `FilePickRouter` → `NonSteamGames.inc`.
4. **Vis** — toggles `EgameNVis` + `ExtraGameCountPLUS` clamp (same idea as Hidden meters).
5. **Icon** — ChooseImage; `ImagePickSink` copies to `#@#img\EIcon\#_PickSlotPadded#.jpg` (`001`…`020`).
6. **Clear** — empties slot (no renumber); recount max occupied index; `ScanNeeded=1`.
7. **Scan** — QuickSettings-equivalent bang launching `#@#UpdateGames.pyw`; clears `ScanNeeded`. Settings does not auto-scan.

### Scan skip

- `ExtraGamesCount` remains max used index (UI).
- Empty middle names skipped at meter write time; section names keep original index (`EGame{i}`).

## Verification

| Check | Result |
|-------|--------|
| Helper + unit test | Pass (RED then GREEN) |
| Full `pytest tests -v` | Pass (7) |
| Stub removed; slots 1–20 + Add/Clear/Scan | Pass (structural) |
| FilePickSink Extra vs SkinInfo routing | Pass (bang strings) |
| ImagePickSink padded copy path | Pass (bang strings) |
| `@Resources/img/EIcon` and `ELogo` dirs | Already present |
| Manual Rainmeter verify | **Skipped** — interactive Rainmeter unavailable |

## Self-review

- Matches brief interfaces; Paths/GameDirs untouched.
- TDD followed for Python helper with captured RED failure mode.
- Prefer `ExtraHasN` + `ExtraTabRefresh` over literal `Hidden=(#EgameN#=)` because Calc formulas cannot test string emptiness; intent (hide empty slots) preserved.
- Commit message matches brief; use `git commit -F` on Windows.

## Concerns

1. Manual Rainmeter verify not run — operator should confirm Add/Browse/Clear/Vis/Icon/ScanNeeded banner and that scan skips cleared middle slots in live meters.
2. Nested bangs (`Egame#ExtraEditIndex#`, `#Egame[&FindEmptyExtraSlot]#`) match Hidden.ini patterns but need live confirmation.
3. `ShowMeterGroup TabExtra` forces chrome visible; banner/row Hidden depend on `ExtraTabRefresh` always running after tab show and mutations.
4. Icon copy uses `cmd /c copy`; fails silently if path has odd quoting — hint documents `00N.jpg` convention as fallback.
5. `ExtraGameCountPLUS` Vis toggle still WriteKeyValue Clamp formulas (pre-existing Hidden style); Add/Clear/Recount prefer numeric writes for Python `int(ExtraGamesCount)`.

## Review fix notes (Critical / Important)

**Commit message:** `fix(settings): use ExtraHasN for empty Extra slot detection`

### Critical — empty-slot Calc → ExtraHasN
- `[FindEmptyExtraSlotCheck]` now `IfCondition=[ExtraHas[&FindEmptyExtraSlot]]=1` (empty).
- `[ExtraRecountCheck]` now `IfCondition=[ExtraHas[&ExtraRecountMax]]=0` (occupied).
- Add path: `ExtraAddBtn` runs `[!UpdateMeasureGroup ExtraHas]` before the FindEmpty loop.
- Clear path: `ExtraClearWrite` runs `[!UpdateMeasureGroup ExtraHas]` before ExtraRecountMax.

### Important — ExtraBrowseGate Paths-style gate
- Replaced flag=0 force-hide `IfTrueAction` with `OnUpdateAction` Hidden formulas:
  `([ExtraHasN]+(1-#HasFileChooseFlag#))` on Browse + Icon (hide when empty **or** no FileChoose).

### Covering tests (re-run)
| Check | Result |
|-------|--------|
| `pytest tests/test_update_games.py -v` | **PASSED** — 7/7 |
| `pytest tests/ -v` | **PASSED** — 7/7 |
| Structural: `ExtraHas[&…]` in FindEmpty/Recount | **Pass** — Settings.ini L159 / L228 |
