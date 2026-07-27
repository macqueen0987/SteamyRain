;_________________________________________________
;______________LAYOUT TAB (SettingsForm grid)_____
; Rows: Y=(#FormRowY0#+n*#FormRowH#)
; 0 TileSize | 1 Visible | 2 Display section
; 3-6 toggles | 7 Gap | 8 Colors section | 9-13 colors
[TilePixelsY]
Measure=Calc
Formula=((#TilePixels#*46.74)/100)
DynamicVariables=1

[OptionsCheck]
Measure=Calc
IfCondition=#Arrows#=1
IfCondition2=#Fade#=1
IfCondition3=#Header#=1
ifcondition4=#HalfTiles#=1
IfTrueAction=[!SetVariable OArrow #TextColor#][!UpdateMeter NavArrowsButton][!ReDraw]
IfTrueAction2=[!SetVariable OFade #TextColor#][!UpdateMeter TileFadeButton][!ReDraw]
IfTrueAction3=[!SetVariable OHead #TextColor#][!UpdateMeter HeaderVisButton][!ReDraw]
IfTrueAction4=[!SetVariable OHalfTiles #TextColor#][!UpdateMeter HalfTilesButton][!ReDraw]
IfFalseAction=[!SetVariable OArrow #Dark1#][!UpdateMeter NavArrowsButton][!ReDraw]
IfFalseAction2=[!SetVariable OFade #Dark1#][!UpdateMeter TileFadeButton][!ReDraw]
IfFalseAction3=[!SetVariable OHead #Dark1#][!UpdateMeter HeaderVisButton][!ReDraw]
IfFalseAction4=[!SetVariable OHalfTiles #Dark1#][!UpdateMeter HalfTilesButton][!ReDraw]
IfConditionMode=1
DynamicVariables=1

[TileSizeCheck]
Measure=Calc
; Track is Dark1; fill grows with TilePixels (never full opaque white brick feel at mid values)
Formula=(((#TilePixels#-170)*#FormControlW#)/130)
OnUpdateAction=[!SetVariable TilePixels "#TilePixels#" "SteamyRain" "#ActiveConf#"][!UpdateMeasure TilePixelsY "SteamyRain" "#ActiveConf#"][!UpdateMeter TileSizeBar][!ReDraw]
DynamicVariables=1

[arrowCheck]
Measure=Calc
OnUpdateAction=[!UpdateMeasure Offset "SteamyRain" "#ActiveConf#"]
DynamicVariables=1
Disabled=1

[fadeCheck]
Measure=Calc
IfCondition=#Fade#=1
IfTrueAction=[!WriteKeyValue Variables fadeMode OFF "#@#SkinInfo.inc"][!SetVariable FadeMode OFF "SteamyRain" "#ActiveConf#"][!UpdateMeter Container "SteamyRain" "#ActiveConf#"][!ReDraw "SteamyRain" "#ActiveConf#"]
IfFalseAction=[!WriteKeyValue Variables fadeMode ON "#@#SkinInfo.inc"][!SetVariable FadeMode ON "SteamyRain" "#ActiveConf#"][!UpdateMeter Container "SteamyRain" "#ActiveConf#"][!ReDraw "SteamyRain" "#ActiveConf#"]
DynamicVariables=1
Disabled=1

[headCheck]
Measure=Calc
OnUpdateAction=[!UpdateMeterGroup HeaderHide "SteamyRain" "#ActiveConf#"][!ReDraw "SteamyRain" "#ActiveConf#"]
DynamicVariables=1
Disabled=1

[VisibleTilesCheck]
Measure=Calc
Formula=(Floor(#VisTile#))
OnUpdateAction=[!UpdateMeterGroup Vis][!WriteKeyValue Variables #VisTileN# #VisTile# "#@#SkinInfo.inc"][!SetVariable #VisTileN# #VisTile# "SteamyRain" "#ActiveConf#"][!UpdateMeasure Lenght "SteamyRain" "#ActiveConf#"][!ReDraw]
DynamicVariables=1

[HalfTileCheck]
Measure=Calc
IfCondition=#HalfTiles#=1
IfTrueAction=[!SetVariable Half 0][!SetVariable VisTile (#VisTile#-0.5)][!WriteKeyValue Variables Half 0 "#@#SkinInfo.inc"][!UpdateMeterGroup HalfTile][!UpdateMeasure VisibleTilesCheck]
IfFalseAction=[!SetVariable Half 5][!SetVariable VisTile (#VisTile#+0.5)][!WriteKeyValue Variables Half 5 "#@#SkinInfo.inc"][!UpdateMeterGroup HalfTile][!UpdateMeasure VisibleTilesCheck]
DynamicVariables=1
Disabled=1

[GapBetweenTilesCheck]
Measure=Calc
Formula=(Floor(#GapBetweenTiles#))
OnUpdateAction=[!UpdateMeterGroup Vis][!WriteKeyValue Variables GapBetweenTiles #GapBetweenTiles# "#@#SkinInfo.inc"][!SetVariable GapBetweenTiles #GapBetweenTiles# "SteamyRain" "#ActiveConf#"][!UpdateMeasure Lenght "SteamyRain" "#ActiveConf#"][!ReDraw]
DynamicVariables=1

; InputText: FormControlX; Y = FormRowY0 + n*FormRowH + 6
[InputColorMainBGMeasure]
Measure=Plugin
Plugin=InputText
SolidColor=#InputBG#
FontColor=#TextColor#
FontSize=10
AntiAlias=1
X=(#FormControlX#+#FormControlW#-120)
Y=(#FormRowY0#+9*#FormRowH#+6)
H=16
W=120
DefaultValue=#Dark1#
Command1=[!WriteKeyValue Variables Dark1 "$UserInput$" "#@#SkinInfo.inc"][!SetVariable Dark1 "$UserInput$"]
Command2=[!Refresh "SteamyRain"]
DynamicVariables=1

[InputColorInputBGMeasure]
Measure=Plugin
Plugin=InputText
SolidColor=#InputBG#
FontColor=#TextColor#
FontSize=10
AntiAlias=1
X=(#FormControlX#+#FormControlW#-120)
Y=(#FormRowY0#+10*#FormRowH#+6)
H=16
W=120
DefaultValue=#InputBG#
Command1=[!WriteKeyValue Variables InputBG "$UserInput$" "#@#SkinInfo.inc"][!SetVariable InputBG "$UserInput$"]
Command2=[!Refresh "SteamyRain"]
DynamicVariables=1

[InputColorTextColorMeasure]
Measure=Plugin
Plugin=InputText
SolidColor=#InputBG#
FontColor=#TextColor#
FontSize=10
AntiAlias=1
X=(#FormControlX#+#FormControlW#-120)
Y=(#FormRowY0#+11*#FormRowH#+6)
H=16
W=120
DefaultValue=#TextColor#
Command1=[!WriteKeyValue Variables TextColor "$UserInput$" "#@#SkinInfo.inc"][!SetVariable TextColor "$UserInput$"]
Command2=[!Refresh "SteamyRain"]
DynamicVariables=1

[InputColorTextBGMeasure]
Measure=Plugin
Plugin=InputText
SolidColor=#InputBG#
FontColor=#TextColor#
FontSize=10
AntiAlias=1
X=(#FormControlX#+#FormControlW#-120)
Y=(#FormRowY0#+12*#FormRowH#+6)
H=16
W=120
DefaultValue=#TextBG#
Command1=[!WriteKeyValue Variables TextBG "$UserInput$" "#@#SkinInfo.inc"][!SetVariable TextBG "$UserInput$"]
Command2=[!Refresh "SteamyRain"]
DynamicVariables=1

[InputColorOpacityMeasure]
Measure=Plugin
Plugin=InputText
SolidColor=#InputBG#
FontColor=#TextColor#
FontSize=10
AntiAlias=1
X=(#FormControlX#+#FormControlW#-120)
Y=(#FormRowY0#+13*#FormRowH#+6)
H=16
W=120
DefaultValue=#Opacity#
Command1=[!WriteKeyValue Variables Opacity "$UserInput$" "#@#SkinInfo.inc"][!SetVariable Opacity "$UserInput$"]
Command2=[!Refresh "SteamyRain"]
DynamicVariables=1


;_________________________________________________
;_______________LAYOUT TAB METERS_________________
; --- Row 0: Tile Size ---
[TileSize]
Meter=String
MeterStyle=FormLabel
Text="Tile Size"
Y=#FormRowY0#
Group=TabLayout

[TileSizeBar]
Meter=Shape
X=#FormControlX#
Y=(#FormRowY0#+4)
; Shape1 is a transparent 20px band so the thin track is easy to hit; the
; track and fill are drawn 6px inside it, keeping their original visual Y.
Shape=Rectangle 0,0,#FormControlW#,20 | Fill Color 0,0,0,1 | StrokeWidth 0
Shape2=Rectangle 0,6,#FormControlW#,8,2 | Fill Color #Dark1# | StrokeWidth 0
Shape3=Rectangle 0,6,[TileSizeCheck],8,2 | Fill Color #TextColor#,200 | StrokeWidth 0
; Click anywhere on the track to jump to that value. $MouseX:%$ is 0-100
; across this meter; TilePixels spans 170-300, so percent * 1.3 + 170.
LeftMouseUpAction=[!WriteKeyValue Variables TilePixels "(Clamp((170+($MouseX:%$*1.3)),170,300))" "#@#SkinInfo.inc"][!SetVariable TilePixels "(Clamp((170+($MouseX:%$*1.3)),170,300))"][!UpdateMeasure TileSizeCheck][!Refresh "SteamyRain"]
MiddleMouseUpAction=[!WriteKeyValue Variables TilePixels "225" "#@#SkinInfo.inc"][!SetVariable TilePixels "225"][!UpdateMeasure TileSizeCheck][!Refresh "SteamyRain"]
MouseScrollDownAction=[!WriteKeyValue Variables TilePixels "(Clamp((#TilePixels#-5),170,300))" "#@#SkinInfo.inc"][!SetVariable TilePixels "(Clamp((#TilePixels#-5),170,300))"][!UpdateMeasure TileSizeCheck][!Refresh "SteamyRain"]
MouseScrollUpAction=[!WriteKeyValue Variables TilePixels "(Clamp((#TilePixels#+5),170,300))" "#@#SkinInfo.inc"][!SetVariable TilePixels "(Clamp((#TilePixels#+5),170,300))"][!UpdateMeasure TileSizeCheck][!Refresh "SteamyRain"]
DynamicVariables=1
Group=TabLayout

; --- Row 1: Visible Games ---
[VisibleTiles]
Meter=String
MeterStyle=FormLabel
Text="Visible Games"
Y=(#FormRowY0#+1*#FormRowH#)
Group=TabLayout

[VisibleLess]
Meter=String
MeterStyle=FormValue
Text="[\xe938]"
FontFace=Segoe Fluent Icons
FontSize=11
SolidColor=#Dark2#,1
X=(#FormControlX#+#FormControlW#-82)
Y=(#FormRowY0#+1*#FormRowH#+6)
LeftMouseUpAction=[!Setvariable VisTile (Clamp((#VisTile#-1),1.#Half#,((#GameCountPLUS#+#ExtraGameCountPLUS#)-0.#Half#)))][!UpdateMeasure VisibleTilesCheck]
MouseOverAction=[!SetOption VisibleLess FontColor "#SettingsHover#"][!UpdateMeter VisibleLess][!ReDraw]
MouseLeaveAction=[!SetOption VisibleLess FontColor "#TextColor#,180"][!UpdateMeter VisibleLess][!ReDraw]
Group=TabLayout | Mode | Vis | HalfTile

[VisibleText]
Meter=String
MeterStyle=FormValue
Text="[VisibleTilesCheck]"
StringAlign=Center
X=(#FormControlX#+#FormControlW#-46)
Y=(#FormRowY0#+1*#FormRowH#+6)
W=28
MiddleMouseUpAction=[!Setvariable VisTile "#DefaultVisTile#"][!UpdateMeasure VisibleTilesCheck]
MouseScrollDownAction=[!Setvariable VisTile (Clamp((#VisTile#-1),1.#Half#,((#GameCountPLUS#+#ExtraGameCountPLUS#)-0.#Half#)))][!UpdateMeasure VisibleTilesCheck]
MouseScrollUpAction=[!Setvariable VisTile (Clamp((#VisTile#+1),1.#Half#,((#GameCountPLUS#+#ExtraGameCountPLUS#)-0.#Half#)))][!UpdateMeasure VisibleTilesCheck]
Group=TabLayout | Mode | Vis | HalfTile

[VisibleMore]
Meter=String
MeterStyle=FormValue
Text="[\xe937]"
FontFace=Segoe Fluent Icons
FontSize=11
SolidColor=#Dark2#,1
X=(#FormControlX#+#FormControlW#-14)
Y=(#FormRowY0#+1*#FormRowH#+6)
LeftMouseUpAction=[!Setvariable VisTile (Clamp((#VisTile#+1),1.#Half#,((#GameCountPLUS#+#ExtraGameCountPLUS#)-0.#Half#)))][!UpdateMeasure VisibleTilesCheck]
MouseOverAction=[!SetOption VisibleMore FontColor "#SettingsHover#"][!UpdateMeter VisibleMore][!ReDraw]
MouseLeaveAction=[!SetOption VisibleMore FontColor "#TextColor#,180"][!UpdateMeter VisibleMore][!ReDraw]
Group=TabLayout | Mode | Vis | HalfTile

; --- Row 2: Display section ---
[DisplaySection]
Meter=String
MeterStyle=FormSection
Text="Display"
Y=(#FormRowY0#+2*#FormRowH#)
Group=TabLayout

; --- Rows 3-6: stacked toggles ---
[NavArrows]
Meter=String
MeterStyle=FormLabel
Text="Nav Arrows"
Y=(#FormRowY0#+3*#FormRowH#)
Group=TabLayout

[NavArrowsButton]
Meter=Shape
MeterStyle=FormToggle
X=(#FormControlX#+#FormControlW#-12)
Y=(#FormRowY0#+3*#FormRowH#+8)
Shape=Rectangle 0,0,12,12 | Fill Color #OArrow# | Extend Button
Button=StrokeWidth 2 | Stroke Color #Dark2#
LeftMouseupAction=[!WriteKeyValue Variables Arrows (1-#Arrows#) "#@#SkinInfo.inc"][!SetVariable Arrows (1-#Arrows#) "SteamyRain" "#ActiveConf#"][!SetVariable Arrows (1-#Arrows#)][!EnableMeasure arrowCheck][!UpdateMeasure arrowCheck][!UpdateMeasure OptionsCheck]
Group=TabLayout

[TileFade]
Meter=String
MeterStyle=FormLabel
Text="Fading Tiles"
Y=(#FormRowY0#+4*#FormRowH#)
Group=TabLayout

[TileFadeButton]
Meter=Shape
MeterStyle=FormToggle
X=(#FormControlX#+#FormControlW#-12)
Y=(#FormRowY0#+4*#FormRowH#+8)
Shape=Rectangle 0,0,12,12 | Fill Color #OFade# | Extend Button
Button=StrokeWidth 2 | Stroke Color #Dark2#
LeftMouseupAction=[!WriteKeyValue Variables Fade (1-#Fade#) "#@#SkinInfo.inc"][!SetVariable Fade (1-#Fade#)][!EnableMeasure fadeCheck][!UpdateMeasure fadeCheck][!UpdateMeasure OptionsCheck]
Group=TabLayout

[HeaderVis]
Meter=String
MeterStyle=FormLabel
Text="Header"
Y=(#FormRowY0#+5*#FormRowH#)
Group=TabLayout

[HeaderVisButton]
Meter=Shape
MeterStyle=FormToggle
X=(#FormControlX#+#FormControlW#-12)
Y=(#FormRowY0#+5*#FormRowH#+8)
Shape=Rectangle 0,0,12,12 | Fill Color #OHead# | Extend Button
Button=StrokeWidth 2 | Stroke Color #Dark2#
LeftMouseupAction=[!WriteKeyValue Variables Header (1-#Header#) "#@#SkinInfo.inc"][!SetVariable Header (1-#Header#) "SteamyRain" "#ActiveConf#"][!SetVariable Header (1-#Header#)][!EnableMeasure headCheck][!UpdateMeasure headCheck][!UpdateMeasure OptionsCheck]
Group=TabLayout

[HalfTiles]
Meter=String
MeterStyle=FormLabel
Text="Half Tiles"
Y=(#FormRowY0#+6*#FormRowH#)
Group=TabLayout

[HalfTilesButton]
Meter=Shape
MeterStyle=FormToggle
X=(#FormControlX#+#FormControlW#-12)
Y=(#FormRowY0#+6*#FormRowH#+8)
Shape=Rectangle 0,0,12,12 | Fill Color #OHalfTiles# | Extend Button
Button=StrokeWidth 2 | Stroke Color #Dark2#
LeftMouseupAction=[!SetVariable HalfTiles (1-#HalfTiles#)][!WriteKeyValue Variables HalfTiles (1-#HalfTiles#) "#@#SkinInfo.inc"][!EnableMeasure HalfTileCheck][!UpdateMeasure HalfTileCheck][!UpdateMeasure OptionsCheck]
Group=TabLayout | HalfTile

; --- Row 7: Gap ---
[GapBetweenTiles]
Meter=String
MeterStyle=FormLabel
Text="Gap between tiles"
Y=(#FormRowY0#+7*#FormRowH#)
Group=TabLayout

[GapBetweenTilesLess]
Meter=String
MeterStyle=FormValue
Text="[\xe938]"
FontFace=Segoe Fluent Icons
FontSize=11
SolidColor=#Dark2#,1
X=(#FormControlX#+#FormControlW#-82)
Y=(#FormRowY0#+7*#FormRowH#+6)
LeftMouseUpAction=[!Setvariable GapBetweenTiles (Clamp((#GapBetweenTiles#-1),0,#MaxGap#))][!UpdateMeasure GapBetweenTilesCheck]
MouseOverAction=[!SetOption GapBetweenTilesLess FontColor "#SettingsHover#"][!UpdateMeter GapBetweenTilesLess][!ReDraw]
MouseLeaveAction=[!SetOption GapBetweenTilesLess FontColor "#TextColor#,180"][!UpdateMeter GapBetweenTilesLess][!ReDraw]
Group=TabLayout | Mode | Vis | HalfTile

[GapBetweenTilesText]
Meter=String
MeterStyle=FormValue
Text="[GapBetweenTilesCheck]"
StringAlign=Center
X=(#FormControlX#+#FormControlW#-46)
Y=(#FormRowY0#+7*#FormRowH#+6)
W=28
MiddleMouseUpAction=[!Setvariable GapBetweenTiles "#DefaultGap#"][!UpdateMeasure GapBetweenTilesCheck]
MouseScrollDownAction=[!Setvariable GapBetweenTiles (Clamp((#GapBetweenTiles#-1),0,#MaxGap#))][!UpdateMeasure GapBetweenTilesCheck]
MouseScrollUpAction=[!Setvariable GapBetweenTiles (Clamp((#GapBetweenTiles#+1),0,#MaxGap#))][!UpdateMeasure GapBetweenTilesCheck]
Group=TabLayout | Mode | Vis | HalfTile

[GapBetweenTilesMore]
Meter=String
MeterStyle=FormValue
Text="[\xe937]"
FontFace=Segoe Fluent Icons
FontSize=11
SolidColor=#Dark2#,1
X=(#FormControlX#+#FormControlW#-14)
Y=(#FormRowY0#+7*#FormRowH#+6)
LeftMouseUpAction=[!Setvariable GapBetweenTiles (Clamp((#GapBetweenTiles#+1),0,#MaxGap#))][!UpdateMeasure GapBetweenTilesCheck]
MouseOverAction=[!SetOption GapBetweenTilesMore FontColor "#SettingsHover#"][!UpdateMeter GapBetweenTilesMore][!ReDraw]
MouseLeaveAction=[!SetOption GapBetweenTilesMore FontColor "#TextColor#,180"][!UpdateMeter GapBetweenTilesMore][!ReDraw]
Group=TabLayout | Mode | Vis | HalfTile

; --- Row 8: Colors section ---
[ColorsSection]
Meter=String
MeterStyle=FormSection
Text="Colors"
Y=(#FormRowY0#+8*#FormRowH#)
Group=TabLayout

; --- Rows 9-13: colors ---
[TextColorMainBG]
Meter=String
MeterStyle=FormLabel
Text="Color Main BG"
Y=(#FormRowY0#+9*#FormRowH#)
Group=TabLayout

[ResetColorMainBG]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe72c]"
X=(#FormControlX#+#FormControlW#-122)
Y=(#FormRowY0#+9*#FormRowH#+6)
LeftMouseUpAction=[!WriteKeyValue Variables Dark1 #Dark1_DEFAULT# "#@#SkinInfo.inc"][!Setvariable Dark1 #Dark1_DEFAULT#][!Refresh "SteamyRain"]
Group=TabLayout

[InputColorMainBG]
Meter=String
MeterStyle=FormField
Text=#Dark1#
X=(#FormControlX#+#FormControlW#-100)
Y=(#FormRowY0#+9*#FormRowH#+6)
W=100
LeftMouseUpAction=!CommandMeasure "InputColorMainBGMeasure" "ExecuteBatch 1-2"
Group=TabLayout

[TextColorInputBG]
Meter=String
MeterStyle=FormLabel
Text="Color Input BG"
Y=(#FormRowY0#+10*#FormRowH#)
Group=TabLayout

[ResetColorInputBG]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe72c]"
X=(#FormControlX#+#FormControlW#-122)
Y=(#FormRowY0#+10*#FormRowH#+6)
LeftMouseUpAction=[!WriteKeyValue Variables InputBG #InputBG_DEFAULT# "#@#SkinInfo.inc"][!Setvariable InputBG #InputBG_DEFAULT#][!Refresh "SteamyRain"]
Group=TabLayout

[InputColorInputBG]
Meter=String
MeterStyle=FormField
Text=#InputBG#
X=(#FormControlX#+#FormControlW#-100)
Y=(#FormRowY0#+10*#FormRowH#+6)
W=100
LeftMouseUpAction=!CommandMeasure "InputColorInputBGMeasure" "ExecuteBatch 1-2"
Group=TabLayout

[TextColorTextColor]
Meter=String
MeterStyle=FormLabel
Text="Color Text"
Y=(#FormRowY0#+11*#FormRowH#)
Group=TabLayout

[ResetColorTextColor]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe72c]"
X=(#FormControlX#+#FormControlW#-122)
Y=(#FormRowY0#+11*#FormRowH#+6)
LeftMouseUpAction=[!WriteKeyValue Variables TextColor #TextColor_DEFAULT# "#@#SkinInfo.inc"][!Setvariable TextColor #TextColor_DEFAULT#][!Refresh "SteamyRain"]
Group=TabLayout

[InputColorTextColor]
Meter=String
MeterStyle=FormField
Text=#TextColor#
X=(#FormControlX#+#FormControlW#-100)
Y=(#FormRowY0#+11*#FormRowH#+6)
W=100
LeftMouseUpAction=!CommandMeasure "InputColorTextColorMeasure" "ExecuteBatch 1-2"
Group=TabLayout

[TextColorTextBG]
Meter=String
MeterStyle=FormLabel
Text="Color Text BG"
Y=(#FormRowY0#+12*#FormRowH#)
Group=TabLayout

[ResetColorTextBG]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe72c]"
X=(#FormControlX#+#FormControlW#-122)
Y=(#FormRowY0#+12*#FormRowH#+6)
LeftMouseUpAction=[!WriteKeyValue Variables TextBG #TextBG_DEFAULT# "#@#SkinInfo.inc"][!Setvariable TextBG #TextBG_DEFAULT#][!Refresh "SteamyRain"]
Group=TabLayout

[InputColorTextBG]
Meter=String
MeterStyle=FormField
Text=#TextBG#
X=(#FormControlX#+#FormControlW#-100)
Y=(#FormRowY0#+12*#FormRowH#+6)
W=100
LeftMouseUpAction=!CommandMeasure "InputColorTextBGMeasure" "ExecuteBatch 1-2"
Group=TabLayout

[TextColorOpacity]
Meter=String
MeterStyle=FormLabel
Text="Main BG Opacity"
Y=(#FormRowY0#+13*#FormRowH#)
Group=TabLayout

[ResetColorOpacity]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe72c]"
X=(#FormControlX#+#FormControlW#-122)
Y=(#FormRowY0#+13*#FormRowH#+6)
LeftMouseUpAction=[!WriteKeyValue Variables Opacity #Opacity_DEFAULT# "#@#SkinInfo.inc"][!Setvariable Opacity #Opacity_DEFAULT#][!Refresh "SteamyRain"]
Group=TabLayout

[InputColorOpacity]
Meter=String
MeterStyle=FormField
Text=#Opacity#
X=(#FormControlX#+#FormControlW#-100)
Y=(#FormRowY0#+13*#FormRowH#+6)
W=100
LeftMouseUpAction=!CommandMeasure "InputColorOpacityMeasure" "ExecuteBatch 1-2"
Group=TabLayout
