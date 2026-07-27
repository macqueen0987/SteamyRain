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
Command1=[!WriteKeyValue Variables Locale "$UserInput$" "#@#SkinInfo.inc"][!SetVariable Locale "$UserInput$"][!UpdateMeterGroup TabPaths][!UpdateMeasure LocaleCheckEnglish][!UpdateMeasure LocaleCheckKoreana][!Redraw]
DynamicVariables=1

; Keeps the Locale pills' highlight in sync with #Locale#. Any bang that
; changes Locale must also !UpdateMeasure both of these.
[LocaleCheckEnglish]
Measure=String
String=#Locale#
DynamicVariables=1
IfMatch=^english$
IfMatchAction=[!SetOption LocalePresetEnglish SolidColor "#SettingsHover#"][!SetOption LocalePresetEnglish FontColor "#Dark1#"][!UpdateMeter LocalePresetEnglish][!Redraw]
IfNotMatchAction=[!SetOption LocalePresetEnglish SolidColor "#TextBG#"][!SetOption LocalePresetEnglish FontColor "#TextColor#,180"][!UpdateMeter LocalePresetEnglish][!Redraw]

[LocaleCheckKoreana]
Measure=String
String=#Locale#
DynamicVariables=1
IfMatch=^koreana$
IfMatchAction=[!SetOption LocalePresetKoreana SolidColor "#SettingsHover#"][!SetOption LocalePresetKoreana FontColor "#Dark1#"][!UpdateMeter LocalePresetKoreana][!Redraw]
IfNotMatchAction=[!SetOption LocalePresetKoreana SolidColor "#TextBG#"][!SetOption LocalePresetKoreana FontColor "#TextColor#,180"][!UpdateMeter LocalePresetKoreana][!Redraw]

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
MeterStyle=FormField
Text=#SteamPath#
Y=(#FormRowY0#+0*#FormRowH#+6)
W=(#FormControlW#-28)
LeftMouseUpAction=[!CommandMeasure InputSteamPathMeasure "ExecuteBatch 1"]
Group=TabPaths
Hidden=1

[SteamPathBrowse]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b7]"
Group=TabPaths
Hidden=1
X=(#FormControlX#+#FormControlW#-24)
Y=(#FormRowY0#+0*#FormRowH#+4)
LeftMouseUpAction=[!SetVariable _PickTarget SteamPath][!CommandMeasure FileChooseFolder "ChooseFolder 1"]

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
MeterStyle=FormField
Text=#GameDirs#
Y=(#FormRowY0#+1*#FormRowH#+6)
W=(#FormControlW#-72)
LeftMouseUpAction=[!CommandMeasure InputGameDirsMeasure "ExecuteBatch 1"]
Group=TabPaths
Hidden=1

[GameDirsBrowse]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b7]"
Group=TabPaths
Hidden=1
X=(#FormControlX#+#FormControlW#-64)
Y=(#FormRowY0#+1*#FormRowH#+4)
LeftMouseUpAction=[!SetVariable _PickTarget GameDirs][!SetVariable _GameDirsMode append][!CommandMeasure FileChooseFolder "ChooseFolder 1"]

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
MeterStyle=FormField
Text=#RainMeterEXE#
Y=(#FormRowY0#+2*#FormRowH#+6)
W=(#FormControlW#-28)
LeftMouseUpAction=[!CommandMeasure InputRainMeterEXEMeasure "ExecuteBatch 1"]
Group=TabPaths
Hidden=1

[RainMeterEXEBrowse]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b7]"
Group=TabPaths
Hidden=1
X=(#FormControlX#+#FormControlW#-24)
Y=(#FormRowY0#+2*#FormRowH#+4)
LeftMouseUpAction=[!SetVariable _PickTarget RainMeterEXE][!CommandMeasure FileChooseFile "ChooseFile 1"]

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
MeterStyle=FormPill
Text="english"
Group=TabPaths
Hidden=1
Y=(#FormRowY0#+3*#FormRowH#+4)
LeftMouseUpAction=[!WriteKeyValue Variables Locale "english" "#@#SkinInfo.inc"][!SetVariable Locale "english"][!UpdateMeterGroup TabPaths][!UpdateMeasure LocaleCheckEnglish][!UpdateMeasure LocaleCheckKoreana][!Redraw]
MouseOverAction=[!SetOption LocalePresetEnglish SolidColor "#SettingsHover#"][!SetOption LocalePresetEnglish FontColor "#Dark1#"][!UpdateMeter LocalePresetEnglish][!Redraw]
MouseLeaveAction=[!UpdateMeasure LocaleCheckEnglish]

[LocalePresetKoreana]
Meter=String
MeterStyle=FormPill
Text="koreana"
Group=TabPaths
Hidden=1
X=(#FormControlX#+64)
Y=(#FormRowY0#+3*#FormRowH#+4)
LeftMouseUpAction=[!WriteKeyValue Variables Locale "koreana" "#@#SkinInfo.inc"][!SetVariable Locale "koreana"][!UpdateMeterGroup TabPaths][!UpdateMeasure LocaleCheckEnglish][!UpdateMeasure LocaleCheckKoreana][!Redraw]
MouseOverAction=[!SetOption LocalePresetKoreana SolidColor "#SettingsHover#"][!SetOption LocalePresetKoreana FontColor "#Dark1#"][!UpdateMeter LocalePresetKoreana][!Redraw]
MouseLeaveAction=[!UpdateMeasure LocaleCheckKoreana]

; --- Row 4: Locale value ---
[LocaleValue]
Meter=String
MeterStyle=FormField
Text=#Locale#
Y=(#FormRowY0#+4*#FormRowH#+6)
W=#FormControlW#
LeftMouseUpAction=[!CommandMeasure InputLocaleMeasure "ExecuteBatch 1"]
Group=TabPaths
Hidden=1
