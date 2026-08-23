# Xbox PC game scan (first-class tiles)

Date: 2026-08-23  
Status: approved for planning  
Scope: Xbox PC auto-scan into the same tile list as Steam. EA and remote artwork are out of scope.

## Goal

SteamyRain currently scans Steam libraries (`appmanifest_*.acf`) and builds Rainmeter meters that launch via `steam://rungameid/...`. Non-Steam titles are manual Extra slots.

We will add **Xbox PC / Game Pass PC** discovery so installed Xbox games appear as **first-class tiles in one combined list** with Steam (no platform headers). Images stay **local-only** for this release; IGDB (or similar) is a later spec.

## Decisions

| Topic | Choice |
|--------|--------|
| Tile status | First-class (same list as Steam), not Extra injection |
| Platforms now | Xbox PC only; EA later as another source |
| List layout | Single scroll; no platform grouping |
| Artwork | Local install/package art; placeholder if missing |
| Paths | Auto-detect ∪ Settings `XboxDirs` (Steam `GameDirs` pattern) |
| Architecture | Multi-source scanners → unified game records → existing writers |

## Architecture

### Pipeline

1. `UpdateGames.pyw` runs platform **sources**:
   - `steam` — existing appmanifest scan
   - `xbox` — auto-detect + `XboxDirs`
2. Each source returns the same record shape:

   ```text
   { stable_id, name, launch, image_path }
   ```

3. Records are **merged into one list** (order: Steam scan order, then Xbox scan order).
4. Existing writers produce `GamesInfo.inc` and `dynamicMeters*.inc`.

### Stable IDs

- Steam: `steam:<appid>` (e.g. `steam:730`)
- Xbox: `xbox:<key>` where `<key>` prefers a package/product-stable identifier, else a normalized install folder name
- `GamesInfo` `ID{n}` stores this string
- Hide / search key off `stable_id`
- **Migration:** on scan, map legacy numeric `ID{n}` / Vis entries to `steam:<appid>` so existing hide state is preserved

### Launch

- Steam: `steam://rungameid/<appid>` (unchanged behavior)
- Xbox: prefer AUMID → `shell:AppsFolder\<AUMID>`; else Content main exe / known launcher; if neither, still emit a tile, warn in Update status, and set launch to open the game install folder in Explorer (`explorer "<path>"`) so the tile is never a silent no-op

### Images (v1)

- Steam: `SteamPath/appcache/librarycache` (unchanged)
- Xbox: best local `.png`/`.jpg` under the install/package tree; else one shared placeholder under `@Resources`
- Remote/IGDB: **out of scope**

### Settings

- Paths tab: `XboxDirs` (comma-separated) + Browse, same UX as `GameDirs`
- Empty `XboxDirs` → auto-detect only
- Non-empty → auto-detect ∪ user paths (dedupe)

### Extra games

- Manual `NonSteamGames.inc` / Extra meters remain unchanged and still append after the unified Steam+Xbox list (existing Extra behavior).

## Xbox detection

### Roots (in order)

1. User `XboxDirs` from `SkinInfo.inc`
2. Per-drive `.GamingRoot` → Xbox library folder
3. Default `C:\XboxGames` if not already covered

### Per-game acceptance

Under each library, treat a child folder as a game when it has `Content\` plus at least one of:

- `MicrosoftGame.config`
- `appxmanifest.xml`
- Clear launcher/exe evidence used by the scanner

### Exclusions

- Runtimes, store stubs, nameless folders
- Unreadable `WindowsApps`-only installs (no ACL bypass); mention in Update status if relevant

## GamesInfo / meters

### GamesInfo.inc

- `ID{n}` = `stable_id`
- `ID{n}name`, `Vis{n}`, `GameCount`, `GameCountPLUS` — same roles as today
- Do **not** add `ID{n}Launch` in v1; launch and image come from the in-memory record when writing meters (YAGNI)

### `create_meter`

- Stop assuming every non-Extra tile uses `steam://rungameid/#ID#`
- Use the record’s `launch` for Name/Image `LeftMouseUpAction`
- Use the record’s `image_path` for `ImageName` (Steam cache rules for steam records; Xbox path or placeholder for xbox)
- Extra (`Egame*`) path unchanged

## Error handling

- Missing/empty Xbox roots → Steam-only success; one Update status line that Xbox found nothing / paths missing
- Per-game parse failure → skip game, continue scan; surface in status when useful
- Split Steam/Xbox scan into testable functions (or small modules) inside/near `UpdateGames.pyw`

## Testing

- Keep existing Steam tests; add coverage for `steam:` ID migration and Vis preservation
- Xbox fixtures: fake `.GamingRoot`, `MicrosoftGame.config`, sample image → assert detect, `stable_id`, launch, placeholder fallback
- Assert writers emit Xbox launch and image paths into meters

## Out of scope

- EA App / Origin scanner
- IGDB or any remote artwork download/cache
- Platform filter UI or section headers
- Elevating / ACL-bypassing `WindowsApps`
- Alphabetical merge sort option

## Follow-ups (later specs)

1. EA source implementing the same record shape
2. Optional remote art (e.g. IGDB) with local cache
3. Optional sort / filter by source

## Success criteria

- Scan for Games lists installed Xbox PC games beside Steam in one scroll
- Click launches via resolved Xbox launch string
- Hide state survives rescan for Steam (migrated IDs) and Xbox (`xbox:` IDs)
- Settings can add extra Xbox library folders
- No network required for artwork
