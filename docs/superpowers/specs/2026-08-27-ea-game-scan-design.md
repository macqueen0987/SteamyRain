# EA App game scan (first-class tiles)

Date: 2026-08-27  
Status: approved for planning  
Scope: EA Desktop / EA App installed games as first-class tiles beside Steam and Xbox. Remote artwork (IGDB) is out of scope.

## Goal

SteamyRain already merges Steam and Xbox PC installs into one tile list using a shared record shape `{ stable_id, name, launch, image_path }`.

Add **EA App** discovery so installed EA titles appear as **first-class tiles in the same scroll** (no platform headers). Launch must go through the **EA/Origin launcher protocol**, not direct game `.exe` execution (anti-cheat / protected launchers).

## Decisions

| Topic | Choice |
|--------|--------|
| Tile status | First-class (same list as Steam/Xbox), not Extra injection |
| List layout | Single scroll; no platform grouping |
| Merge order | Steam → Xbox → EA |
| Launch | Launcher URI only; **no** direct `.exe` LeftMouseUpAction |
| Artwork | Local install / `__Installer` art; wide/splash preferred; placeholder if missing |
| Paths | Auto-detect ∪ Settings `EaDirs` (same UX as `XboxDirs`) |
| DLC | Skip InstallData `dlc-*` and install-tree `DLC/` packages |
| Architecture | New `ea_scan.py` source → extend merge in `UpdateGames.pyw` |

## Architecture

### Pipeline

1. `UpdateGames.pyw` runs sources:
   - `steam` — existing
   - `xbox` — existing
   - `ea` — new (`ea_scan.py`)
2. Each source returns:

   ```text
   { stable_id, name, launch, image_path }
   ```

3. Merge: `steam + xbox + ea` (preserve per-source scan order within each block).
4. Existing writers: `GamesInfo.inc`, `dynamicMeters*.inc` (unchanged contract).

### Stable IDs

- EA: `ea:<key>` where `<key>` is:
  1. Package id from InstallData base folder: `base-Origin.SFT.50.0000848` → `Origin.SFT.50.0000848`
  2. Else normalized install folder slug
- Hide / search use `stable_id` (same as Steam/Xbox).
- Search: numeric-only input does not apply to EA; full `ea:...` or name partial match (extend `SearchGames.lua` like Xbox).

### Launch (no exe)

Priority:

1. **EA launcher URI** from InstallData base package id:

   ```text
   [origin2://game/launch/?offerIds=Origin.SFT.50.0000848]
   ```

   (Rainmeter bang wraps the URI; exact query shape implemented to match EA App on Windows.)

2. If installer metadata exposes a **content id** (e.g. `installerdata.xml` `<contentID>`), optional secondary URI template may be tried before explorer fallback (implementation picks one working template verified against fixtures).

3. If no launcher key: `[explorer "<install_dir>"]` and Update status warns — **never** `[path\to\game.exe]`.

Rationale: EA titles use protected launchers / anti-cheat; mirroring Xbox “launch via platform” means platform protocol, not raw exe.

### Images (v1)

- Reuse Xbox wide-art ranking (`WideLogo`, `Splash`, `header`, `hero`, … over square `StoreLogo` / `SquareLogo`).
- Search roots: install dir + `__Installer` with **depth limit** (no full-tree `rglob` on multi-GB installs).
- Missing art → `@Resources/img/placeholder_game.jpg`.
- IGDB / remote cache → later spec.

### Settings

- Paths tab: **`EaDirs`** (comma-separated + Browse append/replace), mirroring `XboxDirs`.
- `SkinInfo.inc`: `EaDirs=` (empty default → auto-detect only).
- Auto-detect sources (union, dedupe):
  - `%ProgramData%\EA Desktop\machine.ini` → `machine.downloadinplacedir`
  - `%LocalAppData%\Electronic Arts\EA Desktop\user_*.ini` → `user.downloadinplacedir`
  - Defaults if unset: `C:\Program Files\EA Games`, common custom roots only when directory exists
- Non-empty `EaDirs` → auto ∪ user paths.

### Extra games

- Manual `NonSteamGames.inc` / Extra meters unchanged; append after merged Steam+Xbox+EA list.

## EA detection

### Installed-game truth source

Primary: **`%ProgramData%\EA Desktop\InstallData\<GameName>\`**

- **Base game:** child folder matching `base-*` (e.g. `base-Origin.SFT.50.0000848`).
- **DLC:** child folders matching `dlc-*` → **not** separate tiles.

Confirm install path: under a library root (`EaDirs` / auto-detect), folder `<GameName>` exists with `__Installer\installerdata.xml`.

### Display name

From `__Installer\installerdata.xml`:

- Prefer `<gameTitle locale="…">` matching `SkinInfo` `Locale` (`english` → `en_US`, `koreana` → `ko_KR`, etc.).
- Fallback: `en_US`, then first `gameTitle`, then InstallData folder name.

### Per-game acceptance

Emit a record when:

- InstallData has `base-*` for the title, **and**
- Matching install directory exists under a library root, **and**
- Not classified as DLC-only (no base, or only `dlc-*` / `DLC/` subtree).

### Exclusions

- InstallData entries with only `dlc-*` (no `base-*`).
- Install folders whose primary content is under `DLC\` only.
- Redistributables / runtime-only folders (name contains `runtime`, `redistributable` — same markers as Xbox scan).

## GamesInfo / meters

- Same as Xbox spec: `ID{n}` = `stable_id`; launch/image baked into meters from record; no `ID{n}Launch` in v1.
- `merge_game_records` becomes three-way (or generic `merge_game_records(*parts)`).

## Error handling

- Missing EA roots → Steam+Xbox-only success; one status line (`EA: no libraries found`).
- Roots but zero games → `EA: no games found`.
- Per-title parse failure → skip, continue scan; optional status with folder name.
- Parse/read errors must not abort entire scan (mirror Xbox try/except per game).
- Slow paths forbidden: no unbounded `rglob` over full game install trees.

## Testing

- Fixtures: fake `InstallData` (`base-*` + `dlc-*`), `installerdata.xml` with `gameTitle` + `contentID`, install dir with `__Installer`.
- Assert: base accepted, DLC skipped, `stable_id` prefix `ea:`, launch contains `origin2://` (not `.exe`), wide art preferred.
- `merge_game_records` order: steam, xbox, ea.
- Keep full `pytest` suite green.

## Out of scope

- IGDB / remote artwork
- Legacy Origin client-only installs (no EA Desktop InstallData)
- Direct exe / anti-cheat bypass
- Platform filter UI
- OFR↔SFT id remapping table (unless needed after live verification; SFT from InstallData is v1 default)

## Follow-ups

1. IGDB cache for missing wide banners
2. OFR id lookup if `Origin.SFT` URIs fail on some titles
3. Legacy Origin-only metadata source

## Success criteria

- Scan lists EA App games beside Steam/Xbox in one scroll
- Click uses launcher URI (not exe)
- DLC packages do not appear as tiles
- Hide state survives rescan via `ea:` stable ids
- Settings can add custom EA library folders
- Scan completes in seconds on large installs (Apex-scale)
