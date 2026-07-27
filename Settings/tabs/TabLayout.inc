;_________________________________________________
;______________LAYOUT TAB MEASURES________________
; Grid: labels X=#Pad# W=#LabelW#; controls X=#ControlX#
; Row Y = #ContentTop# + n*#RowH#
;   0 Tile Size | 1 Visible | 2 Display title | 3-4 toggles | 5 Gap
;   6 Colors title | 7..11 Color rows
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
IfTrueAction=[!SetVariable OArrow #Dark1#][!UpdateMeter NavArrowsButton][!ReDraw]
IfTrueAction2=[!SetVariable OFade #Dark1#][!UpdateMeter TileFadeButton][!ReDraw]
IfTrueAction3=[!SetVariable OHead #Dark1#][!UpdateMeter headerVisButton][!ReDraw]
IfTrueAction4=[!SetVariable OHalfTiles #Dark1#][!UpdateMeter HalfTilesButton][!ReDraw]
IfFalseAction=[!SetVariable OArrow #TextColor#][!UpdateMeter NavArrowsButton][!ReDraw]
IfFalseAction2=[!SetVariable OFade #TextColor#][!UpdateMeter TileFadeButton][!ReDraw]
IfFalseAction3=[!SetVariable OHead #TextColor#][!UpdateMeter headerVisButton][!ReDraw]
IfFalseAction4=[!SetVariable OHalfTiles #TextColor#][!UpdateMeter HalfTilesButton][!ReDraw]
IfConditionMode=1
DynamicVariables=1

[TileSizeCheck]
Measure=Calc
; Fill width = #ControlW#
Formula=(((#TilePixels#-170)*#ControlW#)/130)
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

; InputText at control column; Y = ContentTop + n*RowH (+4 for vertical center in row)
[InputColorMainBGMeasure]
Measure=Plugin
Plugin=InputText
SolidColor=#InputBG#
FontColor=#TextColor#
FontSize=10
AntiAlias=1
X=#ControlX#
Y=(#ContentTop#+7*#RowH#+4)
H=16
W=120
DefaultValue=#Dark1#
Command1=[!WriteKeyValue Variables Dark1 "$UserInput$" "#@#SkinInfo.inc"][!SetVariable Dark1 "$UserInput$"]
Command2=[!RefreshApp]
DynamicVariables=1

[InputColorInputBGMeasure]
Measure=Plugin
Plugin=InputText
SolidColor=#InputBG#
FontColor=#TextColor#
FontSize=10
AntiAlias=1
X=#ControlX#
Y=(#ContentTop#+8*#RowH#+4)
H=16
W=120
DefaultValue=#InputBG#
Command1=[!WriteKeyValue Variables InputBG "$UserInput$" "#@#SkinInfo.inc"][!SetVariable InputBG "$UserInput$"]
Command2=[!RefreshApp]
DynamicVariables=1

[InputColorTextColorMeasure]
Measure=Plugin
Plugin=InputText
SolidColor=#InputBG#
FontColor=#TextColor#
FontSize=10
AntiAlias=1
X=#ControlX#
Y=(#ContentTop#+9*#RowH#+4)
H=16
W=120
DefaultValue=#TextColor#
Command1=[!WriteKeyValue Variables TextColor "$UserInput$" "#@#SkinInfo.inc"][!SetVariable TextColor "$UserInput$"]
Command2=[!RefreshApp]
DynamicVariables=1

[InputColorTextBGMeasure]
Measure=Plugin
Plugin=InputText
SolidColor=#InputBG#
FontColor=#TextColor#
FontSize=10
AntiAlias=1
X=#ControlX#
Y=(#ContentTop#+10*#RowH#+4)
H=16
W=120
DefaultValue=#TextBG#
Command1=[!WriteKeyValue Variables TextBG "$UserInput$" "#@#SkinInfo.inc"][!SetVariable TextBG "$UserInput$"]
Command2=[!RefreshApp]
DynamicVariables=1

[InputColorOpacityMeasure]
Measure=Plugin
Plugin=InputText
SolidColor=#InputBG#
FontColor=#TextColor#
FontSize=10
AntiAlias=1
X=#ControlX#
Y=(#ContentTop#+11*#RowH#+4)
H=16
W=120
DefaultValue=#Opacity#
Command1=[!WriteKeyValue Variables Opacity "$UserInput$" "#@#SkinInfo.inc"][!SetVariable Opacity "$UserInput$"]
Command2=[!RefreshApp]
DynamicVariables=1


;_________________________________________________
;_______________LAYOUT TAB METERS_________________
[LayoutAnchor]
Meter=String
SolidColor=0,0,0,1
W=1
H=1
X=#Pad#
Y=#ContentTop#
Group=TabLayout
DynamicVariables=1

; --- Row 0: Tile Size ---
[TileSize]
Meter=String
MeterStyle=LayoutLabelStyle
Text="Tile Size"
Y=#ContentTop#

[TileSizeBar]
Meter=Shape
X=#ControlX#
Y=(#ContentTop#+8)
Shape=Rectangle 0,0,#ControlW#,10,2 | Fill Color #Dark1# | StrokeWidth 0
Shape2=Rectangle 0,0,[TileSizeCheck],10,2 | Fill Color #TextColor# | StrokeWidth 0
MiddleMouseUpAction=[!WriteKeyValue Variables TilePixels "225" "#@#SkinInfo.inc"][!SetVariable TilePixels "225"][!UpdateMeasure TileSizeCheck][!RefreshApp]
MouseScrollDownAction=[!WriteKeyValue Variables TilePixels "(Clamp((#TilePixels#-5),170,300))" "#@#SkinInfo.inc"][!SetVariable TilePixels "(Clamp((#TilePixels#-5),170,300))"][!UpdateMeasure TileSizeCheck][!RefreshApp]
MouseScrollUpAction=[!WriteKeyValue Variables TilePixels "(Clamp((#TilePixels#+5),170,300))" "#@#SkinInfo.inc"][!SetVariable TilePixels "(Clamp((#TilePixels#+5),170,300))"][!UpdateMeasure TileSizeCheck][!RefreshApp]
DynamicVariables=1
Group=TabLayout

; --- Row 1: Visible Games ---
[VisibleTiles]
Meter=String
MeterStyle=LayoutLabelStyle
Text="Visible Games"
Y=(#ContentTop#+1*#RowH#)

[VisibleLess]
Meter=String
MeterStyle=LayoutValueStyle
Text="[\xe938]"
FontFace=Segoe Fluent Icons
FontSize=11
SolidColor=#Dark2#,1
X=#ControlX#
Y=(#ContentTop#+1*#RowH#+4)
LeftMouseUpAction=[!Setvariable VisTile (Clamp((#VisTile#-1),1.#Half#,((#GameCountPLUS#+#ExtraGameCountPLUS#)-0.#Half#)))][!UpdateMeasure VisibleTilesCheck]
MouseOverAction=[!SetOption VisibleLess FontColor "#SettingsHover#"][!UpdateMeter VisibleLess][!ReDraw]
MouseLeaveAction=[!SetOption VisibleLess FontColor "#TextColor#,180"][!UpdateMeter VisibleLess][!ReDraw]
Group=TabLayout | Mode | Vis | HalfTile

[VisibleText]
Meter=String
MeterStyle=LayoutValueStyle
Text="[VisibleTilesCheck]"
StringAlign=Center
X=(#ControlX#+36)
Y=(#ContentTop#+1*#RowH#+4)
W=28
MiddleMouseUpAction=[!Setvariable VisTile "#DefaultVisTile#"][!UpdateMeasure VisibleTilesCheck]
MouseScrollDownAction=[!Setvariable VisTile (Clamp((#VisTile#-1),1.#Half#,((#GameCountPLUS#+#ExtraGameCountPLUS#)-0.#Half#)))][!UpdateMeasure VisibleTilesCheck]
MouseScrollUpAction=[!Setvariable VisTile (Clamp((#VisTile#+1),1.#Half#,((#GameCountPLUS#+#ExtraGameCountPLUS#)-0.#Half#)))][!UpdateMeasure VisibleTilesCheck]
Group=TabLayout | Mode | Vis | HalfTile

[VisibleMore]
Meter=String
MeterStyle=LayoutValueStyle
Text="[\xe937]"
FontFace=Segoe Fluent Icons
FontSize=11
SolidColor=#Dark2#,1
X=(#ControlX#+68)
Y=(#ContentTop#+1*#RowH#+4)
LeftMouseUpAction=[!Setvariable VisTile (Clamp((#VisTile#+1),1.#Half#,((#GameCountPLUS#+#ExtraGameCountPLUS#)-0.#Half#)))][!UpdateMeasure VisibleTilesCheck]
MouseOverAction=[!SetOption VisibleMore FontColor "#SettingsHover#"][!UpdateMeter VisibleMore][!ReDraw]
MouseLeaveAction=[!SetOption VisibleMore FontColor "#TextColor#,180"][!UpdateMeter VisibleMore][!ReDraw]
Group=TabLayout | Mode | Vis | HalfTile

; --- Row 2: Display section ---
[DisplaySection]
Meter=String
MeterStyle=LayoutSectionStyle
Text="Display"
Y=(#ContentTop#+2*#RowH#)

; --- Row 3: toggles top (two equal columns) ---
[NavArrows]
Meter=String
MeterStyle=LayoutLabelStyle
Text="Nav Arrows"
W=(#HalfW#-36)
Y=(#ContentTop#+3*#RowH#)

[NavArrowsButton]
Meter=Shape
MeterStyle=ToggleStyle
X=(#Pad#+#HalfW#-28)
Y=(#ContentTop#+3*#RowH#+6)
Shape=Rectangle 0,0,12,12 | Fill Color #OArrow# | Extend Button
Button=StrokeWidth 2 | Stroke Color #Dark2#
LeftMouseupAction=[!WriteKeyValue Variables Arrows (1-#Arrows#) "#@#SkinInfo.inc"][!SetVariable Arrows (1-#Arrows#) "SteamyRain" "#ActiveConf#"][!SetVariable Arrows (1-#Arrows#)][!EnableMeasure arrowCheck][!UpdateMeasure arrowCheck][!UpdateMeasure OptionsCheck]
DynamicVariables=1
Group=TabLayout

[TileFade]
Meter=String
MeterStyle=LayoutLabelStyle
Text="Fading Tiles"
X=(#Pad#+#HalfW#)
W=(#HalfW#-36)
Y=(#ContentTop#+3*#RowH#)

[TileFadeButton]
Meter=Shape
MeterStyle=ToggleStyle
X=(#Pad#+#ContentW#-28)
Y=(#ContentTop#+3*#RowH#+6)
Shape=Rectangle 0,0,12,12 | Fill Color #OFade# | Extend Button
Button=StrokeWidth 2 | Stroke Color #Dark2#
LeftMouseupAction=[!WriteKeyValue Variables Fade (1-#Fade#) "#@#SkinInfo.inc"][!SetVariable Fade (1-#Fade#)][!EnableMeasure fadeCheck][!UpdateMeasure fadeCheck][!UpdateMeasure OptionsCheck]
DynamicVariables=1
Group=TabLayout

; --- Row 4: toggles bottom ---
[HeaderVis]
Meter=String
MeterStyle=LayoutLabelStyle
Text="Header"
W=(#HalfW#-36)
Y=(#ContentTop#+4*#RowH#)

[HeaderVisButton]
Meter=Shape
MeterStyle=ToggleStyle
X=(#Pad#+#HalfW#-28)
Y=(#ContentTop#+4*#RowH#+6)
Shape=Rectangle 0,0,12,12 | Fill Color #OHead# | Extend Button
Button=StrokeWidth 2 | Stroke Color #Dark2#
LeftMouseupAction=[!WriteKeyValue Variables Header (1-#Header#) "#@#SkinInfo.inc"][!SetVariable Header (1-#Header#) "SteamyRain" "#ActiveConf#"][!SetVariable Header (1-#Header#)][!EnableMeasure headCheck][!UpdateMeasure headCheck][!UpdateMeasure OptionsCheck]
DynamicVariables=1
Group=TabLayout

[HalfTiles]
Meter=String
MeterStyle=LayoutLabelStyle
Text="Half Tiles"
X=(#Pad#+#HalfW#)
W=(#HalfW#-36)
Y=(#ContentTop#+4*#RowH#)

[HalfTilesButton]
Meter=Shape
MeterStyle=ToggleStyle
X=(#Pad#+#ContentW#-28)
Y=(#ContentTop#+4*#RowH#+6)
Shape=Rectangle 0,0,12,12 | Fill Color #OHalfTiles# | Extend Button
Button=StrokeWidth 2 | Stroke Color #Dark2#
LeftMouseupAction=[!SetVariable HalfTiles (1-#HalfTiles#)][!WriteKeyValue Variables HalfTiles (1-#HalfTiles#) "#@#SkinInfo.inc"][!EnableMeasure HalfTileCheck][!UpdateMeasure HalfTileCheck][!UpdateMeasure OptionsCheck]
DynamicVariables=1
Group=TabLayout | HalfTile

; --- Row 5: Gap ---
[GapBetweenTiles]
Meter=String
MeterStyle=LayoutLabelStyle
Text="Gap between tiles"
Y=(#ContentTop#+5*#RowH#)

[GapBetweenTilesLess]
Meter=String
MeterStyle=LayoutValueStyle
Text="[\xe938]"
FontFace=Segoe Fluent Icons
FontSize=11
SolidColor=#Dark2#,1
X=#ControlX#
Y=(#ContentTop#+5*#RowH#+4)
LeftMouseUpAction=[!Setvariable GapBetweenTiles (Clamp((#GapBetweenTiles#-1),0,#MaxGap#))][!UpdateMeasure GapBetweenTilesCheck]
MouseOverAction=[!SetOption GapBetweenTilesLess FontColor "#SettingsHover#"][!UpdateMeter GapBetweenTilesLess][!ReDraw]
MouseLeaveAction=[!SetOption GapBetweenTilesLess FontColor "#TextColor#,180"][!UpdateMeter GapBetweenTilesLess][!ReDraw]
Group=TabLayout | Mode | Vis | HalfTile

[GapBetweenTilesText]
Meter=String
MeterStyle=LayoutValueStyle
Text="[GapBetweenTilesCheck]"
StringAlign=Center
X=(#ControlX#+36)
Y=(#ContentTop#+5*#RowH#+4)
W=28
MiddleMouseUpAction=[!Setvariable GapBetweenTiles "#DefaultGap#"][!UpdateMeasure GapBetweenTilesCheck]
MouseScrollDownAction=[!Setvariable GapBetweenTiles (Clamp((#GapBetweenTiles#-1),0,#MaxGap#))][!UpdateMeasure GapBetweenTilesCheck]
MouseScrollUpAction=[!Setvariable GapBetweenTiles (Clamp((#GapBetweenTiles#+1),0,#MaxGap#))][!UpdateMeasure GapBetweenTilesCheck]
Group=TabLayout | Mode | Vis | HalfTile

[GapBetweenTilesMore]
Meter=String
MeterStyle=LayoutValueStyle
Text="[\xe937]"
FontFace=Segoe Fluent Icons
FontSize=11
SolidColor=#Dark2#,1
X=(#ControlX#+68)
Y=(#ContentTop#+5*#RowH#+4)
LeftMouseUpAction=[!Setvariable GapBetweenTiles (Clamp((#GapBetweenTiles#+1),0,#MaxGap#))][!UpdateMeasure GapBetweenTilesCheck]
MouseOverAction=[!SetOption GapBetweenTilesMore FontColor "#SettingsHover#"][!UpdateMeter GapBetweenTilesMore][!ReDraw]
MouseLeaveAction=[!SetOption GapBetweenTilesMore FontColor "#TextColor#,180"][!UpdateMeter GapBetweenTilesMore][!ReDraw]
Group=TabLayout | Mode | Vis | HalfTile

; --- Row 6: Colors section ---
[ColorsSection]
Meter=String
MeterStyle=LayoutSectionStyle
Text="Colors"
Y=(#ContentTop#+6*#RowH#)

; --- Rows 7..11: label | reset | value (same ControlX column) ---
[TextColorMainBG]
Meter=String
MeterStyle=LayoutLabelStyle
Text="Color Main BG"
Y=(#ContentTop#+7*#RowH#)

[ResetColorMainBG]
Meter=String
MeterStyle=LayoutValueStyle
Text="[\xe72c]"
FontFace=Segoe Fluent Icons
FontSize=10
X=#ControlX#
Y=(#ContentTop#+7*#RowH#+4)
LeftMouseUpAction=[!WriteKeyValue Variables Dark1 #Dark1_DEFAULT# "#@#SkinInfo.inc"][!Setvariable Dark1 #Dark1_DEFAULT#][!RefreshApp]

[InputColorMainBG]
Meter=String
MeterStyle=LayoutValueStyle
Text=#Dark1#
X=(#ControlX#+28)
Y=(#ContentTop#+7*#RowH#+4)
LeftMouseUpAction=!CommandMeasure "InputColorMainBGMeasure" "ExecuteBatch 1-2"

[TextColorInputBG]
Meter=String
MeterStyle=LayoutLabelStyle
Text="Color Input BG"
Y=(#ContentTop#+8*#RowH#)

[ResetColorInputBG]
Meter=String
MeterStyle=LayoutValueStyle
Text="[\xe72c]"
FontFace=Segoe Fluent Icons
FontSize=10
X=#ControlX#
Y=(#ContentTop#+8*#RowH#+4)
LeftMouseUpAction=[!WriteKeyValue Variables InputBG #InputBG_DEFAULT# "#@#SkinInfo.inc"][!Setvariable InputBG #InputBG_DEFAULT#][!RefreshApp]

[InputColorInputBG]
Meter=String
MeterStyle=LayoutValueStyle
Text=#InputBG#
X=(#ControlX#+28)
Y=(#ContentTop#+8*#RowH#+4)
LeftMouseUpAction=!CommandMeasure "InputColorInputBGMeasure" "ExecuteBatch 1-2"

[TextColorTextColor]
Meter=String
MeterStyle=LayoutLabelStyle
Text="Color Text"
Y=(#ContentTop#+9*#RowH#)

[ResetColorTextColor]
Meter=String
MeterStyle=LayoutValueStyle
Text="[\xe72c]"
FontFace=Segoe Fluent Icons
FontSize=10
X=#ControlX#
Y=(#ContentTop#+9*#RowH#+4)
LeftMouseUpAction=[!WriteKeyValue Variables TextColor #TextColor_DEFAULT# "#@#SkinInfo.inc"][!Setvariable TextColor #TextColor_DEFAULT#][!RefreshApp]

[InputColorTextColor]
Meter=String
MeterStyle=LayoutValueStyle
Text=#TextColor#
X=(#ControlX#+28)
Y=(#ContentTop#+9*#RowH#+4)
LeftMouseUpAction=!CommandMeasure "InputColorTextColorMeasure" "ExecuteBatch 1-2"

[TextColorTextBG]
Meter=String
MeterStyle=LayoutLabelStyle
Text="Color Text BG"
Y=(#ContentTop#+10*#RowH#)

[ResetColorTextBG]
Meter=String
MeterStyle=LayoutValueStyle
Text="[\xe72c]"
FontFace=Segoe Fluent Icons
FontSize=10
X=#ControlX#
Y=(#ContentTop#+10*#RowH#+4)
LeftMouseUpAction=[!WriteKeyValue Variables TextBG #TextBG_DEFAULT# "#@#SkinInfo.inc"][!Setvariable TextBG #TextBG_DEFAULT#][!RefreshApp]

[InputColorTextBG]
Meter=String
MeterStyle=LayoutValueStyle
Text=#TextBG#
X=(#ControlX#+28)
Y=(#ContentTop#+10*#RowH#+4)
LeftMouseUpAction=!CommandMeasure "InputColorTextBGMeasure" "ExecuteBatch 1-2"

[TextColorOpacity]
Meter=String
MeterStyle=LayoutLabelStyle
Text="Main BG Opacity"
Y=(#ContentTop#+11*#RowH#)

[ResetColorOpacity]
Meter=String
MeterStyle=LayoutValueStyle
Text="[\xe72c]"
FontFace=Segoe Fluent Icons
FontSize=10
X=#ControlX#
Y=(#ContentTop#+11*#RowH#+4)
LeftMouseUpAction=[!WriteKeyValue Variables Opacity #Opacity_DEFAULT# "#@#SkinInfo.inc"][!Setvariable Opacity #Opacity_DEFAULT#][!RefreshApp]

[InputColorOpacity]
Meter=String
MeterStyle=LayoutValueStyle
Text=#Opacity#
X=(#ControlX#+28)
Y=(#ContentTop#+11*#RowH#+4)
LeftMouseUpAction=!CommandMeasure "InputColorOpacityMeasure" "ExecuteBatch 1-2"
