# Task 6 Report: Polish, README, regression pass

**Status:** DONE  
**Branch:** `sync/local-advantages`  
**Commit:** `e9082ed` — `docs: document tabbed Settings and finish UI polish`

## Summary

Documented the tabbed Settings UI in README (new Settings section + Setup step 1 Paths tab), folded the empty `[HasFileChooseFlag]` INI section into `[Variables]` comments, and confirmed pytest regression green. No layout/window-size polish changes — existing tab padding and `DynamicWindowSize=1` were sufficient (YAGNI).

## What changed

| Path | Action |
|------|--------|
| `README.md` | Added **Settings** section (Layout/Paths/Extra/Hidden tabs); Setup step 1 now points to Settings → Paths as alternative to hand-editing `SkinInfo.inc` |
| `Settings/Settings.ini` | Moved `HasFileChooseFlag` override comments into `[Variables]`; removed empty `[HasFileChooseFlag]` section |

## Verification

| Check | Result |
|-------|--------|
| `pytest tests -v` | **PASSED** — 7/7 |
| README Settings section | Pass |
| Setup mentions Paths tab | Pass |
| HasFileChooseFlag section folded | Pass |
| Settings UI polish (padding/window) | **Skipped** — no issues observed; YAGNI |

## Operator checklist (manual Rainmeter — not run here)

- [ ] **Layout:** tile size, arrows, fade, header, half tiles, gap, all 5 colors + opacity + resets
- [ ] **Paths:** all four fields persist after Refresh Settings; Browse writes; InputText edits when Browse hidden
- [ ] **Extra:** add / edit / clear / scan; empty slots skipped on Scan
- [ ] **Hidden:** unhide one + unhide all; QuickSettings → tab 4; no dual Hidden.ini list
- [ ] **Mode:** horizontal/vertical positions Settings correctly (`ModeCheck` / `MoveWindow`)
- [ ] **Close:** Settings clears `Settings=0`

## Concerns

1. Manual Rainmeter regression not run — operator should walk the checklist above on a live install.
2. ~~`Add Non-Steam Games` README section still describes hand-editing `NonSteamGames.inc`; Extra tab in Settings is the preferred path but full doc refresh was out of scope for this task.~~ **Fixed:** Added preferred-path note at top of section (Settings → Extra, then Scan for Games); manual `NonSteamGames.inc` steps kept as fallback.
3. FileChoose Browse still depends on bundled `@Resources/Plugins/FileChoose.dll` (documented in Requirements).
