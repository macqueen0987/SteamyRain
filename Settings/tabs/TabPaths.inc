;_________________________________________________
;______________PATHS TAB (SettingsForm grid)______
; Rows: 0 SteamPath | 1 GameDirs | 2 RainMeterEXE | 3 Locale presets | 4 Locale value
; Browse X = FormControlX + FormControlW - 24

[InputSteamPathMeasure]
Measure=Plugin
Plugin=InputText
SolidColor=#InputBG#
FontColor=#TextColor#
FontSize=10
AntiAlias=1
X=#FormControlX#
Y=(#FormRowY0#+0*#FormRowH#+6)
H=16
W=(#FormControlW#-28)
DefaultValue=#SteamPath#
Command1=[!WriteKeyValue Variables SteamPath "$UserInput$" "#@#SkinInfo.inc"][!SetVariable SteamPath "$UserInput$"][!UpdateMeterGroup TabPaths][!Redraw]
DynamicVariables=1

[InputGameDirsMeasure]
Measure=Plugin
Plugin=InputText
SolidColor=#InputBG#
FontColor=#TextColor#
FontSize=10
AntiAlias=1
X=#FormControlX#
Y=(#FormRowY0#+1*#FormRowH#+6)
H=16
W=(#FormControlW#-72)
DefaultValue=#GameDirs#
Command1=[!WriteKeyValue Variables GameDirs "$UserInput$" "#@#SkinInfo.inc"][!SetVariable GameDirs "$UserInput$"][!UpdateMeterGroup TabPaths][!Redraw]
DynamicVariables=1

[InputRainMeterEXEMeasure]
Measure=Plugin
Plugin=InputText
SolidColor=#InputBG#
FontColor=#TextColor#
FontSize=10
AntiAlias=1
X=#FormControlX#
Y=(#FormRowY0#+2*#FormRowH#+6)
H=16
W=(#FormControlW#-28)
DefaultValue=#RainMeterEXE#
Command1=[!WriteKeyValue Variables RainMeterEXE "$UserInput$" "#@#SkinInfo.inc"][!SetVariable RainMeterEXE "$UserInput$"][!UpdateMeterGroup TabPaths][!Redraw]
DynamicVariables=1

[InputLocaleMeasure]
Measure=Plugin
Plugin=InputText
SolidColor=#InputBG#
FontColor=#TextColor#
FontSize=10
AntiAlias=1
X=#FormControlX#
Y=(#FormRowY0#+4*#FormRowH#+6)
H=16
W=#FormControlW#
DefaultValue=#Locale#
Command1=[!WriteKeyValue Variables Locale "$UserInput$" "#@#SkinInfo.inc"][!SetVariable Locale "$UserInput$"][!UpdateMeterGroup TabPaths][!Redraw]
DynamicVariables=1


;_________________________________________________
;_________________PATHS TAB METERS________________
[PathsAnchor]
Meter=String
SolidColor=0,0,0,1
W=1
H=1
X=#Pad#
Y=#FormRowY0#
Group=TabPaths
Hidden=1
DynamicVariables=1

; --- Row 0: Steam Path ---
[SteamPathLabel]
Meter=String
MeterStyle=FormLabel
Text="Steam Path"
Y=#FormRowY0#
Group=TabPaths
Hidden=1

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

; --- Row 1: Game Dirs ---
[GameDirsLabel]
Meter=String
MeterStyle=FormLabel
Text="Game Dirs"
Y=(#FormRowY0#+1*#FormRowH#)
Group=TabPaths
Hidden=1

[GameDirsValue]
Meter=String
MeterStyle=FormValue
Text=#GameDirs#
Y=(#FormRowY0#+1*#FormRowH#+6)
W=(#FormControlW#-72)
H=14
ClipString=1
SolidColor=#TextBG#
Padding=4,1,4,1
LeftMouseUpAction=[!CommandMeasure InputGameDirsMeasure "ExecuteBatch 1"]
Group=TabPaths
Hidden=1

[GameDirsBrowse]
Meter=String
MeterStyle=FormValue
Text="[\xe8b7]"
FontFace=Segoe Fluent Icons
FontSize=12
FontColor=#TextColor#
SolidColor=#Dark2#,1
Group=TabPaths
Hidden=1
X=(#FormControlX#+#FormControlW#-64)
Y=(#FormRowY0#+1*#FormRowH#+4)
LeftMouseUpAction=[!SetVariable _PickTarget GameDirs][!SetVariable _GameDirsMode append][!CommandMeasure FileChooseFolder "ChooseFolder 1"]
MouseOverAction=[!SetOption GameDirsBrowse FontColor "#SettingsHover#"][!UpdateMeter GameDirsBrowse][!Redraw]
MouseLeaveAction=[!SetOption GameDirsBrowse FontColor "#TextColor#"][!UpdateMeter GameDirsBrowse][!Redraw]

[GameDirsReplace]
Meter=String
MeterStyle=FormValue
Text="Repl"
FontSize=8
FontColor=#TextColor#
SolidColor=#Dark2#,1
Padding=2,2,2,2
Group=TabPaths
Hidden=1
X=(#FormControlX#+#FormControlW#-36)
Y=(#FormRowY0#+1*#FormRowH#+4)
LeftMouseUpAction=[!SetVariable _PickTarget GameDirs][!SetVariable _GameDirsMode replace][!CommandMeasure FileChooseFolder "ChooseFolder 1"]
MouseOverAction=[!SetOption GameDirsReplace FontColor "#SettingsHover#"][!UpdateMeter GameDirsReplace][!Redraw]
MouseLeaveAction=[!SetOption GameDirsReplace FontColor "#TextColor#"][!UpdateMeter GameDirsReplace][!Redraw]

; --- Row 2: Rainmeter EXE ---
[RainMeterEXELabel]
Meter=String
MeterStyle=FormLabel
Text="Rainmeter EXE"
Y=(#FormRowY0#+2*#FormRowH#)
Group=TabPaths
Hidden=1

[RainMeterEXEValue]
Meter=String
MeterStyle=FormValue
Text=#RainMeterEXE#
Y=(#FormRowY0#+2*#FormRowH#+6)
W=(#FormControlW#-28)
H=14
ClipString=1
SolidColor=#TextBG#
Padding=4,1,4,1
LeftMouseUpAction=[!CommandMeasure InputRainMeterEXEMeasure "ExecuteBatch 1"]
Group=TabPaths
Hidden=1

[RainMeterEXEBrowse]
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
Y=(#FormRowY0#+2*#FormRowH#+4)
LeftMouseUpAction=[!SetVariable _PickTarget RainMeterEXE][!CommandMeasure FileChooseFile "ChooseFile 1"]
MouseOverAction=[!SetOption RainMeterEXEBrowse FontColor "#SettingsHover#"][!UpdateMeter RainMeterEXEBrowse][!Redraw]
MouseLeaveAction=[!SetOption RainMeterEXEBrowse FontColor "#TextColor#"][!UpdateMeter RainMeterEXEBrowse][!Redraw]

; --- Row 3: Locale presets ---
[LocaleLabel]
Meter=String
MeterStyle=FormLabel
Text="Locale"
Y=(#FormRowY0#+3*#FormRowH#)
Group=TabPaths
Hidden=1

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

[LocalePresetKoreana]
Meter=String
MeterStyle=FormValue
Text="koreana"
FontSize=9
FontColor=#TextColor#
SolidColor=#TextBG#
Padding=4,2,4,2
Group=TabPaths
Hidden=1
X=(#FormControlX#+64)
Y=(#FormRowY0#+3*#FormRowH#+4)
LeftMouseUpAction=[!WriteKeyValue Variables Locale "koreana" "#@#SkinInfo.inc"][!SetVariable Locale "koreana"][!UpdateMeterGroup TabPaths][!Redraw]
MouseOverAction=[!SetOption LocalePresetKoreana FontColor "#SettingsHover#"][!UpdateMeter LocalePresetKoreana][!Redraw]
MouseLeaveAction=[!SetOption LocalePresetKoreana FontColor "#TextColor#"][!UpdateMeter LocalePresetKoreana][!Redraw]

; --- Row 4: Locale value ---
[LocaleValue]
Meter=String
MeterStyle=FormValue
Text=#Locale#
Y=(#FormRowY0#+4*#FormRowH#+6)
W=#FormControlW#
H=14
ClipString=1
SolidColor=#TextBG#
Padding=4,1,4,1
LeftMouseUpAction=[!CommandMeasure InputLocaleMeasure "ExecuteBatch 1"]
Group=TabPaths
Hidden=1
