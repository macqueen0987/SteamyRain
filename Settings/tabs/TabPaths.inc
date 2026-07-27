;_________________________________________________
;______________PATHS TAB MEASURES_________________
; Row map (Y = #ContentTop# + n*#RowH#):
;   0 SteamPath | 1 GameDirs | 2 RainMeterEXE | 3 Locale presets | 4 Locale value
; Value column X = (#Pad#+130); Browse at (#Pad#+#ContentW#-24)
; InputText X/Y: value col (#Pad#+130), row Y = #ContentTop# + n*#RowH#

[InputSteamPathMeasure]
Measure=Plugin
Plugin=InputText
SolidColor=#InputBG#
FontColor=#TextColor#
FontSize=10
AntiAlias=1
; X = #Pad#+130
X=(#Pad#+130)
; Y = #ContentTop# + 0*#RowH#
Y=(#ContentTop#+0*#RowH#)
H=15
W=(#ContentW#-160)
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
; X = #Pad#+130
X=(#Pad#+130)
; Y = #ContentTop# + 1*#RowH#
Y=(#ContentTop#+1*#RowH#)
H=15
W=(#ContentW#-210)
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
; X = #Pad#+130
X=(#Pad#+130)
; Y = #ContentTop# + 2*#RowH#
Y=(#ContentTop#+2*#RowH#)
H=15
W=(#ContentW#-160)
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
; X = #Pad#+130
X=(#Pad#+130)
; Y = #ContentTop# + 4*#RowH#
Y=(#ContentTop#+4*#RowH#)
H=15
W=(#ContentW#-130)
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
Y=#ContentTop#
Group=TabPaths
Hidden=1
DynamicVariables=1

; --- Row 0: Steam Path ---
[SteamPathLabel]
Meter=String
MeterStyle=OptionStyle
Text="Steam Path"
X=#Pad#
Y=#ContentTop#
W=120
Group=TabPaths
Hidden=1

[SteamPathValue]
Meter=String
Text=#SteamPath#
MeterStyle=NameStyle
X=(#Pad#+130)
Y=#ContentTop#
W=(#ContentW#-160)
H=14
ClipString=1
SolidColor=#TextBG#
Padding=4,1,4,1
LeftMouseUpAction=[!CommandMeasure InputSteamPathMeasure "ExecuteBatch 1"]
Group=TabPaths
Hidden=1
DynamicVariables=1

[SteamPathBrowse]
Meter=String
Text="[\xe8b7]"
FontFace=Segoe Fluent Icons
FontSize=12
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabPaths
; Start hidden; PathsBrowseGate sets Hidden=(1-#HasFileChooseFlag#) after ShowMeterGroup
Hidden=1
X=(#Pad#+#ContentW#-20)
Y=(#ContentTop#+2)
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickTarget SteamPath][!CommandMeasure FileChooseFolder "ChooseFolder 1"]
MouseOverAction=[!SetOption SteamPathBrowse FontColor "#SettingsHover#"][!UpdateMeter SteamPathBrowse][!Redraw]
MouseLeaveAction=[!SetOption SteamPathBrowse FontColor "#TextColor#"][!UpdateMeter SteamPathBrowse][!Redraw]

; --- Row 1: Game Dirs (append Browse + Replace) ---
[GameDirsLabel]
Meter=String
MeterStyle=OptionStyle
Text="Game Dirs"
X=#Pad#
Y=(#ContentTop#+1*#RowH#)
W=120
Group=TabPaths
Hidden=1

[GameDirsValue]
Meter=String
Text=#GameDirs#
MeterStyle=NameStyle
X=(#Pad#+130)
Y=(#ContentTop#+1*#RowH#)
W=(#ContentW#-210)
H=14
ClipString=1
SolidColor=#TextBG#
Padding=4,1,4,1
LeftMouseUpAction=[!CommandMeasure InputGameDirsMeasure "ExecuteBatch 1"]
Group=TabPaths
Hidden=1
DynamicVariables=1

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
X=(#Pad#+#ContentW#-70)
Y=(#ContentTop#+1*#RowH#+2)
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
X=(#Pad#+#ContentW#-40)
Y=(#ContentTop#+1*#RowH#+2)
SolidColor=#Dark2#,1
Padding=2,2,2,2
LeftMouseUpAction=[!SetVariable _PickTarget GameDirs][!SetVariable _GameDirsMode replace][!CommandMeasure FileChooseFolder "ChooseFolder 1"]
MouseOverAction=[!SetOption GameDirsReplace FontColor "#SettingsHover#"][!UpdateMeter GameDirsReplace][!Redraw]
MouseLeaveAction=[!SetOption GameDirsReplace FontColor "#TextColor#"][!UpdateMeter GameDirsReplace][!Redraw]

; --- Row 2: Rainmeter EXE ---
[RainMeterEXELabel]
Meter=String
MeterStyle=OptionStyle
Text="Rainmeter EXE"
X=#Pad#
Y=(#ContentTop#+2*#RowH#)
W=120
Group=TabPaths
Hidden=1

[RainMeterEXEValue]
Meter=String
Text=#RainMeterEXE#
MeterStyle=NameStyle
X=(#Pad#+130)
Y=(#ContentTop#+2*#RowH#)
W=(#ContentW#-160)
H=14
ClipString=1
SolidColor=#TextBG#
Padding=4,1,4,1
LeftMouseUpAction=[!CommandMeasure InputRainMeterEXEMeasure "ExecuteBatch 1"]
Group=TabPaths
Hidden=1
DynamicVariables=1

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
X=(#Pad#+#ContentW#-20)
Y=(#ContentTop#+2*#RowH#+2)
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickTarget RainMeterEXE][!CommandMeasure FileChooseFile "ChooseFile 1"]
MouseOverAction=[!SetOption RainMeterEXEBrowse FontColor "#SettingsHover#"][!UpdateMeter RainMeterEXEBrowse][!Redraw]
MouseLeaveAction=[!SetOption RainMeterEXEBrowse FontColor "#TextColor#"][!UpdateMeter RainMeterEXEBrowse][!Redraw]

; --- Row 3: Locale presets ---
[LocaleLabel]
Meter=String
MeterStyle=OptionStyle
Text="Locale"
X=#Pad#
Y=(#ContentTop#+3*#RowH#)
W=120
Group=TabPaths
Hidden=1

[LocalePresetEnglish]
Meter=String
Text="english"
FontSize=9
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabPaths
Hidden=1
X=(#Pad#+130)
Y=(#ContentTop#+3*#RowH#)
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
X=5R
Y=r
SolidColor=#TextBG#
Padding=4,2,4,2
LeftMouseUpAction=[!WriteKeyValue Variables Locale "koreana" "#@#SkinInfo.inc"][!SetVariable Locale "koreana"][!UpdateMeterGroup TabPaths][!Redraw]
MouseOverAction=[!SetOption LocalePresetKoreana FontColor "#SettingsHover#"][!UpdateMeter LocalePresetKoreana][!Redraw]
MouseLeaveAction=[!SetOption LocalePresetKoreana FontColor "#TextColor#"][!UpdateMeter LocalePresetKoreana][!Redraw]

; --- Row 4: Locale value ---
[LocaleValue]
Meter=String
Text=#Locale#
MeterStyle=NameStyle
X=(#Pad#+130)
Y=(#ContentTop#+4*#RowH#)
W=(#ContentW#-130)
H=14
ClipString=1
SolidColor=#TextBG#
Padding=4,1,4,1
LeftMouseUpAction=[!CommandMeasure InputLocaleMeasure "ExecuteBatch 1"]
Group=TabPaths
Hidden=1
DynamicVariables=1
