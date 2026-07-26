;_________________________________________________
;_________________PATHS TAB METERS________________
[SteamPathLabel]
Meter=String
Text="Steam Path"
MeterStyle=OptionStyle
Group=TabPaths
Hidden=1
Y=36
X=8
Padding=0,0,0,0
SolidColor=0,0,0,1
W=250

[SteamPathValue]
Meter=String
Text=#SteamPath#
MeterStyle=NameStyle
Group=TabPaths
Hidden=1
Y=2R
X=8
W=200
H=14
ClipString=1
SolidColor=#TextBG#
Padding=4,1,4,1
LeftMouseUpAction=[!CommandMeasure InputSteamPathMeasure "ExecuteBatch 1"]

[SteamPathBrowse]
Meter=String
Text="[\xe8b7]"
FontFace=Segoe Fluent Icons
FontSize=12
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabPaths
Hidden=1
Y=r
X=220
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickTarget SteamPath][!CommandMeasure FileChooseFolder "ChooseFolder 1"]
MouseOverAction=[!SetOption SteamPathBrowse FontColor "#SettingsHover#"][!UpdateMeter SteamPathBrowse][!Redraw]
MouseLeaveAction=[!SetOption SteamPathBrowse FontColor "#TextColor#"][!UpdateMeter SteamPathBrowse][!Redraw]

[GameDirsLabel]
Meter=String
Text="Game Dirs"
MeterStyle=OptionStyle
Group=TabPaths
Hidden=1
Y=8R
X=8
Padding=0,0,0,0
SolidColor=0,0,0,1
W=250

[GameDirsValue]
Meter=String
Text=#GameDirs#
MeterStyle=NameStyle
Group=TabPaths
Hidden=1
Y=2R
X=8
W=170
H=14
ClipString=1
SolidColor=#TextBG#
Padding=4,1,4,1
LeftMouseUpAction=[!CommandMeasure InputGameDirsMeasure "ExecuteBatch 1"]

[GameDirsBrowse]
Meter=String
Text="[\xe8b7]"
FontFace=Segoe Fluent Icons
FontSize=12
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabPaths
Hidden=1
Y=r
X=190
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickTarget GameDirs][!SetVariable _GameDirsMode append][!CommandMeasure FileChooseFolder "ChooseFolder 1"]
MouseOverAction=[!SetOption GameDirsBrowse FontColor "#SettingsHover#"][!UpdateMeter GameDirsBrowse][!Redraw]
MouseLeaveAction=[!SetOption GameDirsBrowse FontColor "#TextColor#"][!UpdateMeter GameDirsBrowse][!Redraw]

[GameDirsReplace]
Meter=String
Text="Repl"
FontSize=8
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabPaths
Hidden=1
Y=r
X=220
SolidColor=#Dark2#,1
Padding=2,2,2,2
LeftMouseUpAction=[!SetVariable _PickTarget GameDirs][!SetVariable _GameDirsMode replace][!CommandMeasure FileChooseFolder "ChooseFolder 1"]
MouseOverAction=[!SetOption GameDirsReplace FontColor "#SettingsHover#"][!UpdateMeter GameDirsReplace][!Redraw]
MouseLeaveAction=[!SetOption GameDirsReplace FontColor "#TextColor#"][!UpdateMeter GameDirsReplace][!Redraw]

[RainMeterEXELabel]
Meter=String
Text="Rainmeter EXE"
MeterStyle=OptionStyle
Group=TabPaths
Hidden=1
Y=8R
X=8
Padding=0,0,0,0
SolidColor=0,0,0,1
W=250

[RainMeterEXEValue]
Meter=String
Text=#RainMeterEXE#
MeterStyle=NameStyle
Group=TabPaths
Hidden=1
Y=2R
X=8
W=200
H=14
ClipString=1
SolidColor=#TextBG#
Padding=4,1,4,1
LeftMouseUpAction=[!CommandMeasure InputRainMeterEXEMeasure "ExecuteBatch 1"]

[RainMeterEXEBrowse]
Meter=String
Text="[\xe8b7]"
FontFace=Segoe Fluent Icons
FontSize=12
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabPaths
Hidden=1
Y=r
X=220
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickTarget RainMeterEXE][!CommandMeasure FileChooseFile "ChooseFile 1"]
MouseOverAction=[!SetOption RainMeterEXEBrowse FontColor "#SettingsHover#"][!UpdateMeter RainMeterEXEBrowse][!Redraw]
MouseLeaveAction=[!SetOption RainMeterEXEBrowse FontColor "#TextColor#"][!UpdateMeter RainMeterEXEBrowse][!Redraw]

[LocaleLabel]
Meter=String
Text="Locale"
MeterStyle=OptionStyle
Group=TabPaths
Hidden=1
Y=8R
X=8
Padding=0,0,0,0
SolidColor=0,0,0,1
W=250

[LocalePresetEnglish]
Meter=String
Text="english"
FontSize=9
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabPaths
Hidden=1
Y=2R
X=8
SolidColor=#TextBG#
Padding=4,2,4,2
LeftMouseUpAction=[!WriteKeyValue Variables Locale "english" "#@#SkinInfo.inc"][!SetVariable Locale "english"][!UpdateMeterGroup TabPaths][!Redraw]
MouseOverAction=[!SetOption LocalePresetEnglish FontColor "#SettingsHover#"][!UpdateMeter LocalePresetEnglish][!Redraw]
MouseLeaveAction=[!SetOption LocalePresetEnglish FontColor "#TextColor#"][!UpdateMeter LocalePresetEnglish][!Redraw]

[LocalePresetKoreana]
Meter=String
Text="koreana"
FontSize=9
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabPaths
Hidden=1
Y=r
X=5R
SolidColor=#TextBG#
Padding=4,2,4,2
LeftMouseUpAction=[!WriteKeyValue Variables Locale "koreana" "#@#SkinInfo.inc"][!SetVariable Locale "koreana"][!UpdateMeterGroup TabPaths][!Redraw]
MouseOverAction=[!SetOption LocalePresetKoreana FontColor "#SettingsHover#"][!UpdateMeter LocalePresetKoreana][!Redraw]
MouseLeaveAction=[!SetOption LocalePresetKoreana FontColor "#TextColor#"][!UpdateMeter LocalePresetKoreana][!Redraw]

[LocaleValue]
Meter=String
Text=#Locale#
MeterStyle=NameStyle
Group=TabPaths
Hidden=1
Y=4R
X=8
W=200
H=14
ClipString=1
SolidColor=#TextBG#
Padding=4,1,4,1
LeftMouseUpAction=[!CommandMeasure InputLocaleMeasure "ExecuteBatch 1"]
