# SteamyRain
 Rainmeter skin SteamyRain

Thanks to [Nookz](https://forum.rainmeter.net/memberlist.php?mode=viewprofile&u=71269) from Rainmeter Forum who delevoped the skin in the first place.
You can read all about it here in his post and the comments: https://forum.rainmeter.net/viewtopic.php?t=43334

Thanks also to:
- [macqueen0987](https://forum.rainmeter.net/memberlist.php?mode=viewprofile&u=74947) who shared his changes for multiple gamedir folders and locale fixes https://forum.rainmeter.net/viewtopic.php?t=43334#p230808
---

## Requirements
You must have python installed!
It is using a python script to fetch the required information and images from your steam folder and build meters dynamically.

### FileChoose plugin
Settings Paths/Extra Browse buttons use the FileChoose plugin, bundled at
`@Resources/Plugins/FileChoose.dll`.

**It must be copied to `%APPDATA%\Rainmeter\Plugins\` to work.** Rainmeter
loads skin plugins with `LOAD_WITH_ALTERED_SEARCH_PATH`, so a plugin left in
`@Resources/Plugins` cannot resolve its own dependency on `Rainmeter.dll` and
fails to load with error 126 — Browse then silently does nothing:

```powershell
Copy-Item "@Resources\Plugins\FileChoose.dll" "$env:APPDATA\Rainmeter\Plugins\"
```

If Browse still does nothing, enable Rainmeter logging (Manage → Settings →
Debug) and check `%APPDATA%\Rainmeter\Rainmeter.log` for plugin load errors,
and confirm `HasFileChooseFlag=1` in Settings. Paths can always be typed via
click-to-edit (InputText) regardless.

---

## Setup
1. Make sure paths are correct: open **Settings → Paths** (or edit `@Resources\SkinInfo.inc` by hand) if your Steam folder, library folders, Xbox library folders, or Rainmeter install are not in their default locations. Xbox games are auto-detected from `.GamingRoot` markers and `C:\XboxGames`; add extra Xbox library folders in **Xbox Dirs** (comma-separated, same as Game Dirs) or leave empty for auto-only.
2. **Optional** Add your non-Steam games. *see section below for more information*
3. Open one of the SteamyRain.ini from the main folder and Click on 'Scan for Games'
4. Wait till it's done and you're good to go.
5. Adjust the settings to your liking.

## Settings
Open via QuickSettings → Settings, or middle-click the header icon.

The Settings window is **520×640**, with scrollable Extra and Hidden lists.

Tabs:
- **Layout** — tile size, visibility toggles, colors
- **Paths** — Steam / library folders / Xbox dirs / Rainmeter.exe / locale (Browse uses FileChoose)
- **Extra** — non-Steam games (then run Scan for Games)
- **Hidden** — unhide games

Settings tabs (Layout/Paths/Extra/Hidden) share their form controls (labels, fields, icon buttons, toggles, pills) from `Settings/styles/SettingsForm.inc` (`FormLabel`/`FormSection`/`FormField`/`FormIconBtn`/`FormPill`/`FormToggle`) — reuse these for new controls rather than hand-rolling new styles. Extra/Hidden's list-row and CTA-button chrome intentionally keep their own styles (see `docs/superpowers/specs/2026-07-27-settings-design-system-design.md`).

## Add Non-Steam Games
**Preferred:** Use **Settings → Extra** to add or edit non-Steam games, then run **Scan for Games**. The manual steps below are a fallback if you prefer editing `NonSteamGames.inc` directly.

This part of the process has to be done manually and will only take effect after using 'Scan for Games'

1. Open the @Resrouces\NonSteamGames.inc
2. Add each game using this format:  
```
Egame#="TheNameOfMyGame"
Egame#Path="ThePathToLaunchMyGame"
Egame#Vis=0
```
It does not have to be a game really. It could be any app or url. eg:  
```
Egame1="Photoshop"
Egame1Path="C:\ProgramData\Microsoft\Windows\Start Menu\Programs\Adobe Photoshop 2024.lnk"
Egame1Vis=0
Egame2="RainmeterForums"
Egame2Path="https://forum.rainmeter.net/index.php"
Egame2Vis=0
```
3. Modify the ExtraGamesCount and ExtraGamesCountPLUS variables at the top to reflect the amount of games you added.
In our example above we would set both of these to 2.
4. Inside @Resrouces\img\ELogo and @Resrouces\img\EIcon, add an icon and a logo for each of the games you added.
name them 00#.jpg. '#' reprensent the game number it is associated with.
I would recomend 32x32 for the icon and 460x215 for the logo
5. Run UpdateGames.pyw if you already followed the Setup steps until the end and already clicked on "Scan for Games". Everytime you add new entries to your NonSteamGames.inc file you have to run the python script again or put necessary infos in the *Meters.inc files manually.
6. Done!

---

## Shortcuts and Keybinds
Header Icon:  
RightClick = Open Steam  
MiddleClick = Open Skin settings  
DoubleClick = Minimise into Icon  
  
Icon Mode:  
DoubleClick = Return to full view  
  
Tiles Section:  
RightClick = Hide/Unhide Header  
MiddleClick = Switch from vertical tiling to horizontal tiling and vice versa  
ScrollUp/Down = Scroll games  
  
Settings Menu:  
RightClick = Reset default value  

## Search Function
It's a fairly simple search function. You can search by Name or by ID  
It will return partial matches for names and only identical matches for IDs.  
It will not account for spelling errors..  