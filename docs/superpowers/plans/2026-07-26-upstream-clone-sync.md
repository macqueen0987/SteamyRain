# Upstream Clone Sync Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Make `macqueen0987/SteamyRain` the working base, then port local advantages and user data onto it.

**Architecture:** Backup the current non-git local tree, clone upstream into `D:\Workspace\SteamyRain`, then selectively merge local scanner improvements and machine-specific config/data without overwriting upstream static skin structure.

**Tech Stack:** Rainmeter skins (INI/INC), Python 3 (`UpdateGames.pyw`), Git

## Global Constraints

- Do not push to GitHub.
- Preserve local `GamesInfo.inc`, `NonSteamGames.inc`, and `@Resources/img` user assets.
- Keep upstream Gap meters, Proton/Linux Runtime skip, image fallback, and multi-library accumulation.
- Keep local UTF-8 I/O, robust `CaseSensitiveConfigParser`, and Vis-hide preservation by appid.
- Map local `SkinInfo` values onto upstream key names (`GameDirs`, `TextBG`, etc.).

---

### Task 1: Backup local tree and clone upstream

**Files:**
- Create: `D:\Workspace\SteamyRain-local-backup\` (full copy of current tree)
- Replace: `D:\Workspace\SteamyRain\` with clone of `https://github.com/macqueen0987/SteamyRain`

- [ ] **Step 1: Copy current workspace to backup**

```powershell
Copy-Item -Recurse -Force "D:\Workspace\SteamyRain" "D:\Workspace\SteamyRain-local-backup"
```

Expected: backup folder exists with `@Resources\UpdateGames.pyw`

- [ ] **Step 2: Clear workspace and clone upstream**

```powershell
Remove-Item -Recurse -Force "D:\Workspace\SteamyRain\*"
git clone https://github.com/macqueen0987/SteamyRain.git "D:\Workspace\SteamyRain"
```

Expected: `git -C D:\Workspace\SteamyRain status` shows clean `main`

- [ ] **Step 3: Restore design/plan docs from backup**

```powershell
New-Item -ItemType Directory -Force "D:\Workspace\SteamyRain\docs\superpowers\specs"
New-Item -ItemType Directory -Force "D:\Workspace\SteamyRain\docs\superpowers\plans"
Copy-Item "D:\Workspace\SteamyRain-local-backup\docs\superpowers\specs\*" "D:\Workspace\SteamyRain\docs\superpowers\specs\"
Copy-Item "D:\Workspace\SteamyRain-local-backup\docs\superpowers\plans\*" "D:\Workspace\SteamyRain\docs\superpowers\plans\"
```

- [ ] **Step 4: Commit workspace bootstrap**

```bash
git add docs
git commit -m "docs: add local sync design and plan"
```

---

### Task 2: Merge UpdateGames.pyw advantages

**Files:**
- Modify: `@Resources/UpdateGames.pyw`
- Test: inline Python fixture under `$env:TEMP\steamy_scan_test`

**Interfaces:**
- Consumes: upstream `GameDirs`, image tails, Gap meters, Proton/Linux skip, multi-dir accumulate
- Produces: UTF-8 I/O, robust parser, Vis preservation by appid, `get_image_for_game` returning relative path with extension

- [ ] **Step 1: Replace CaseSensitiveConfigParser with local robust version**

Keep upstream module structure; set parser `__init__` to use `delimiters=('=')`, `comment_prefixes=(';')`, `inline_comment_prefixes=()`, `interpolation=None`, `strict=False`, `allow_no_value=True`.

- [ ] **Step 2: Force UTF-8 on all file reads/writes**

Apply `encoding='utf-8'` to appmanifest reads, meter writes, and `GamesInfo.inc` writes.

- [ ] **Step 3: Fix Vis-hide preservation to use appid**

When reading existing `GamesInfo.inc`, collect appids where `VisN=1` via matching `IDN`, then restore those hides after rewrite.

- [ ] **Step 4: Keep upstream scanner wins**

Retain: Proton/Linux Runtime name skip, multi-library list concatenation, per-file status updates, Gap meters, full JPG/PNG image candidate list + any-image fallback.

- [ ] **Step 5: Compile-check**

```powershell
python -m py_compile "@Resources\UpdateGames.pyw"
```

Expected: exit code 0

- [ ] **Step 6: Commit**

```bash
git add "@Resources/UpdateGames.pyw"
git commit -m "fix: merge local UTF-8, parser, and Vis preserve into scanner"
```

---

### Task 3: Port local user data onto upstream SkinInfo keys

**Files:**
- Modify: `@Resources/SkinInfo.inc` (values only; keep upstream key names)
- Copy: `@Resources/GamesInfo.inc`, `@Resources/NonSteamGames.inc`
- Copy: `@Resources/img/ELogo/*`, `@Resources/img/EIcon/*` if present in backup

- [ ] **Step 1: Map local values into upstream SkinInfo keys**

| Local key | Upstream key | Local value to keep |
|-----------|--------------|---------------------|
| SteamPath | SteamPath | `C:\Program Files (x86)\Steam` |
| RainMeterEXE | RainMeterEXE | `C:\Program Files\Rainmeter\Rainmeter.exe` |
| GameDir | GameDirs | `E:\SteamLibrary` |
| Locale | Locale | `koreana` |
| Mode/Header/HalfTiles/Visible* / SkinPos* / colors | same or TextBG↔Light1 mapping | keep local numbers/colors |

Color mapping if upstream uses old names:
- `Light1` → `TextBG`
- `Light2` → `TextColor`
- `Gray` → `InputBG`
- `Red` → `CloseBG`
- `Blue` → `SettingsHover`

Also set `GapBetweenTiles=10` from upstream default unless local had a preferred value (local had none → use 10).

- [ ] **Step 2: Copy game data and non-Steam entries from backup**

```powershell
Copy-Item "D:\Workspace\SteamyRain-local-backup\@Resources\GamesInfo.inc" "D:\Workspace\SteamyRain\@Resources\GamesInfo.inc"
Copy-Item "D:\Workspace\SteamyRain-local-backup\@Resources\NonSteamGames.inc" "D:\Workspace\SteamyRain\@Resources\NonSteamGames.inc"
```

- [ ] **Step 3: Copy non-Steam images if present**

```powershell
Copy-Item "D:\Workspace\SteamyRain-local-backup\@Resources\img\ELogo\*" "D:\Workspace\SteamyRain\@Resources\img\ELogo\" -ErrorAction SilentlyContinue
Copy-Item "D:\Workspace\SteamyRain-local-backup\@Resources\img\EIcon\*" "D:\Workspace\SteamyRain\@Resources\img\EIcon\" -ErrorAction SilentlyContinue
```

- [ ] **Step 4: Commit user-data port (exclude secrets; none expected)**

```bash
git add "@Resources/SkinInfo.inc" "@Resources/GamesInfo.inc" "@Resources/NonSteamGames.inc" "@Resources/img"
git commit -m "chore: restore local Steam paths, locale, and non-Steam games"
```

---

### Task 4: Verify

**Files:**
- Test: temporary fixture script (do not commit unless useful)

- [ ] **Step 1: py_compile again**

```powershell
python -m py_compile "@Resources\UpdateGames.pyw"
```

- [ ] **Step 2: Grep skin files for local-only color names that upstream no longer defines**

```powershell
rg "Light1|Light2|#Gray|#Red|#Blue" -g "*.ini" -g "*.inc"
```

Expected: no matches in active skin files (upstream names only)

- [ ] **Step 3: Confirm git status is clean except intentional untracked generated meters if any**

```bash
git status
```
