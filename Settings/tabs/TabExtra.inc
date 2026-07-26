;;_________________________________________________
;_________________EXTRA TAB METERS________________
; Slots 1–20. Empty names Hidden via ExtraHasN (RegExp).
; ExtraGamesCount = max used index; delete does not renumber.

[ExtraTitle]
Meter=String
Text="Non-Steam Games"
MeterStyle=OptionStyle
Group=TabExtra
Hidden=1
Y=36
X=8
Padding=0,0,0,0
SolidColor=0,0,0,1
W=160

[ExtraAddBtn]
Meter=String
Text="+ Add"
MeterStyle=NameStyle
Group=TabExtra
Hidden=1
Y=36
X=180
FontColor=#TextColor#
SolidColor=#Dark2#,1
Padding=4,2,4,2
LeftMouseUpAction=[!UpdateMeasureGroup ExtraHas][!EnableMeasure FindEmptyExtraSlot][!CommandMeasure FindEmptyExtraSlot "Reset"][!UpdateMeasure FindEmptyExtraSlot]
MouseOverAction=[!SetOption ExtraAddBtn FontColor "#SettingsHover#"][!UpdateMeter ExtraAddBtn][!Redraw]
MouseLeaveAction=[!SetOption ExtraAddBtn FontColor "#TextColor#"][!UpdateMeter ExtraAddBtn][!Redraw]

[ExtraScanBanner]
Meter=String
Text="Scan for Games to apply"
MeterStyle=NameStyle
Group=TabExtra
Hidden=(1-#ScanNeeded#)
Y=2R
X=8
W=160
FontColor=#SettingsHover#
Padding=0,2,0,2

[ExtraScanBtn]
Meter=String
Text="Scan"
MeterStyle=NameStyle
Group=TabExtra
Hidden=(1-#ScanNeeded#)
Y=r
X=180
SolidColor=#Dark2#,1
Padding=4,2,4,2
LeftMouseUpAction=[!WriteKeyValue Variables SkinPosX "#CURRENTCONFIGX#" "#@#SkinInfo.inc"][!WriteKeyValue Variables SkinPosY "#CURRENTCONFIGY#" "#@#SkinInfo.inc"]["#@#UpdateGames.pyw"][!SetVariable ScanNeeded 0][!UpdateMeasure ExtraTabRefresh][!Redraw]
MouseOverAction=[!SetOption ExtraScanBtn FontColor "#SettingsHover#"][!UpdateMeter ExtraScanBtn][!Redraw]
MouseLeaveAction=[!SetOption ExtraScanBtn FontColor "#TextColor#"][!UpdateMeter ExtraScanBtn][!Redraw]

[ExtraIconHint]
Meter=String
Text="Icon -> #@#img\EIcon\00N.jpg"
MeterStyle=NameStyle
Group=TabExtra
Hidden=1
Y=2R
X=8
W=250
FontSize=8
FontColor=#TextColor#,120
ClipString=1

[ExtraListAnchor]
Meter=String
Text=""
Group=TabExtra
Hidden=1
Y=2R
X=0
H=0
W=0

; --- Slot 1 ---
[ExtraHas1]
Measure=String
String=#Egame1#
DynamicVariables=1
RegExpSubstitute=1
Substitute="^\s*$":"1","^.+$":"0"
Group=ExtraHas

[ExtraRow1Name]
Meter=String
Text=#Egame1#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow1
Hidden=[ExtraHas1]
Y=2R
X=8
W=72
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 1][!SetOption InputExtraNameMeasure DefaultValue "#Egame1#"][!SetOption InputExtraNameMeasure Y "[ExtraRow1Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow1Path]
Meter=String
Text=#Egame1Path#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow1
Hidden=[ExtraHas1]
Y=r
X=84
W=70
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 1][!SetOption InputExtraPathMeasure DefaultValue "#Egame1Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow1Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow1Browse]
Meter=String
Text="[\xe8b7]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow1
Hidden=[ExtraHas1]
Y=r
X=158
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickTarget Egame1Path][!SetVariable ExtraEditIndex 1][!CommandMeasure FileChooseFile "ChooseFile 1"]
MouseOverAction=[!SetOption ExtraRow1Browse FontColor "#SettingsHover#"][!UpdateMeter ExtraRow1Browse][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow1Browse FontColor "#TextColor#"][!UpdateMeter ExtraRow1Browse][!Redraw]

[ExtraRow1Vis]
Meter=String
Text="[\xe7b3]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow1
Hidden=[ExtraHas1]
Y=r
X=178
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 1][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]
MouseOverAction=[!SetOption ExtraRow1Vis FontColor "#SettingsHover#"][!UpdateMeter ExtraRow1Vis][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow1Vis FontColor "#TextColor#"][!UpdateMeter ExtraRow1Vis][!Redraw]

[ExtraRow1Icon]
Meter=String
Text="[\xe8b9]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow1
Hidden=[ExtraHas1]
Y=r
X=198
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickSlot 1][!SetVariable _PickSlotPadded "001"][!SetVariable ExtraEditIndex 1][!CommandMeasure FileChooseImage "ChooseImage 1"]
MouseOverAction=[!SetOption ExtraRow1Icon FontColor "#SettingsHover#"][!UpdateMeter ExtraRow1Icon][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow1Icon FontColor "#TextColor#"][!UpdateMeter ExtraRow1Icon][!Redraw]

[ExtraRow1Clear]
Meter=String
Text="[\xe74d]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow1
Hidden=[ExtraHas1]
Y=r
X=218
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 1][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]
MouseOverAction=[!SetOption ExtraRow1Clear FontColor "#SettingsHover#"][!UpdateMeter ExtraRow1Clear][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow1Clear FontColor "#TextColor#"][!UpdateMeter ExtraRow1Clear][!Redraw]

; --- Slot 2 ---
[ExtraHas2]
Measure=String
String=#Egame2#
DynamicVariables=1
RegExpSubstitute=1
Substitute="^\s*$":"1","^.+$":"0"
Group=ExtraHas

[ExtraRow2Name]
Meter=String
Text=#Egame2#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow2
Hidden=[ExtraHas2]
Y=2R
X=8
W=72
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 2][!SetOption InputExtraNameMeasure DefaultValue "#Egame2#"][!SetOption InputExtraNameMeasure Y "[ExtraRow2Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow2Path]
Meter=String
Text=#Egame2Path#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow2
Hidden=[ExtraHas2]
Y=r
X=84
W=70
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 2][!SetOption InputExtraPathMeasure DefaultValue "#Egame2Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow2Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow2Browse]
Meter=String
Text="[\xe8b7]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow2
Hidden=[ExtraHas2]
Y=r
X=158
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickTarget Egame2Path][!SetVariable ExtraEditIndex 2][!CommandMeasure FileChooseFile "ChooseFile 1"]
MouseOverAction=[!SetOption ExtraRow2Browse FontColor "#SettingsHover#"][!UpdateMeter ExtraRow2Browse][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow2Browse FontColor "#TextColor#"][!UpdateMeter ExtraRow2Browse][!Redraw]

[ExtraRow2Vis]
Meter=String
Text="[\xe7b3]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow2
Hidden=[ExtraHas2]
Y=r
X=178
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 2][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]
MouseOverAction=[!SetOption ExtraRow2Vis FontColor "#SettingsHover#"][!UpdateMeter ExtraRow2Vis][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow2Vis FontColor "#TextColor#"][!UpdateMeter ExtraRow2Vis][!Redraw]

[ExtraRow2Icon]
Meter=String
Text="[\xe8b9]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow2
Hidden=[ExtraHas2]
Y=r
X=198
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickSlot 2][!SetVariable _PickSlotPadded "002"][!SetVariable ExtraEditIndex 2][!CommandMeasure FileChooseImage "ChooseImage 1"]
MouseOverAction=[!SetOption ExtraRow2Icon FontColor "#SettingsHover#"][!UpdateMeter ExtraRow2Icon][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow2Icon FontColor "#TextColor#"][!UpdateMeter ExtraRow2Icon][!Redraw]

[ExtraRow2Clear]
Meter=String
Text="[\xe74d]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow2
Hidden=[ExtraHas2]
Y=r
X=218
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 2][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]
MouseOverAction=[!SetOption ExtraRow2Clear FontColor "#SettingsHover#"][!UpdateMeter ExtraRow2Clear][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow2Clear FontColor "#TextColor#"][!UpdateMeter ExtraRow2Clear][!Redraw]

; --- Slot 3 ---
[ExtraHas3]
Measure=String
String=#Egame3#
DynamicVariables=1
RegExpSubstitute=1
Substitute="^\s*$":"1","^.+$":"0"
Group=ExtraHas

[ExtraRow3Name]
Meter=String
Text=#Egame3#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow3
Hidden=[ExtraHas3]
Y=2R
X=8
W=72
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 3][!SetOption InputExtraNameMeasure DefaultValue "#Egame3#"][!SetOption InputExtraNameMeasure Y "[ExtraRow3Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow3Path]
Meter=String
Text=#Egame3Path#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow3
Hidden=[ExtraHas3]
Y=r
X=84
W=70
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 3][!SetOption InputExtraPathMeasure DefaultValue "#Egame3Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow3Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow3Browse]
Meter=String
Text="[\xe8b7]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow3
Hidden=[ExtraHas3]
Y=r
X=158
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickTarget Egame3Path][!SetVariable ExtraEditIndex 3][!CommandMeasure FileChooseFile "ChooseFile 1"]
MouseOverAction=[!SetOption ExtraRow3Browse FontColor "#SettingsHover#"][!UpdateMeter ExtraRow3Browse][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow3Browse FontColor "#TextColor#"][!UpdateMeter ExtraRow3Browse][!Redraw]

[ExtraRow3Vis]
Meter=String
Text="[\xe7b3]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow3
Hidden=[ExtraHas3]
Y=r
X=178
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 3][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]
MouseOverAction=[!SetOption ExtraRow3Vis FontColor "#SettingsHover#"][!UpdateMeter ExtraRow3Vis][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow3Vis FontColor "#TextColor#"][!UpdateMeter ExtraRow3Vis][!Redraw]

[ExtraRow3Icon]
Meter=String
Text="[\xe8b9]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow3
Hidden=[ExtraHas3]
Y=r
X=198
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickSlot 3][!SetVariable _PickSlotPadded "003"][!SetVariable ExtraEditIndex 3][!CommandMeasure FileChooseImage "ChooseImage 1"]
MouseOverAction=[!SetOption ExtraRow3Icon FontColor "#SettingsHover#"][!UpdateMeter ExtraRow3Icon][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow3Icon FontColor "#TextColor#"][!UpdateMeter ExtraRow3Icon][!Redraw]

[ExtraRow3Clear]
Meter=String
Text="[\xe74d]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow3
Hidden=[ExtraHas3]
Y=r
X=218
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 3][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]
MouseOverAction=[!SetOption ExtraRow3Clear FontColor "#SettingsHover#"][!UpdateMeter ExtraRow3Clear][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow3Clear FontColor "#TextColor#"][!UpdateMeter ExtraRow3Clear][!Redraw]

; --- Slot 4 ---
[ExtraHas4]
Measure=String
String=#Egame4#
DynamicVariables=1
RegExpSubstitute=1
Substitute="^\s*$":"1","^.+$":"0"
Group=ExtraHas

[ExtraRow4Name]
Meter=String
Text=#Egame4#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow4
Hidden=[ExtraHas4]
Y=2R
X=8
W=72
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 4][!SetOption InputExtraNameMeasure DefaultValue "#Egame4#"][!SetOption InputExtraNameMeasure Y "[ExtraRow4Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow4Path]
Meter=String
Text=#Egame4Path#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow4
Hidden=[ExtraHas4]
Y=r
X=84
W=70
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 4][!SetOption InputExtraPathMeasure DefaultValue "#Egame4Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow4Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow4Browse]
Meter=String
Text="[\xe8b7]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow4
Hidden=[ExtraHas4]
Y=r
X=158
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickTarget Egame4Path][!SetVariable ExtraEditIndex 4][!CommandMeasure FileChooseFile "ChooseFile 1"]
MouseOverAction=[!SetOption ExtraRow4Browse FontColor "#SettingsHover#"][!UpdateMeter ExtraRow4Browse][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow4Browse FontColor "#TextColor#"][!UpdateMeter ExtraRow4Browse][!Redraw]

[ExtraRow4Vis]
Meter=String
Text="[\xe7b3]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow4
Hidden=[ExtraHas4]
Y=r
X=178
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 4][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]
MouseOverAction=[!SetOption ExtraRow4Vis FontColor "#SettingsHover#"][!UpdateMeter ExtraRow4Vis][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow4Vis FontColor "#TextColor#"][!UpdateMeter ExtraRow4Vis][!Redraw]

[ExtraRow4Icon]
Meter=String
Text="[\xe8b9]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow4
Hidden=[ExtraHas4]
Y=r
X=198
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickSlot 4][!SetVariable _PickSlotPadded "004"][!SetVariable ExtraEditIndex 4][!CommandMeasure FileChooseImage "ChooseImage 1"]
MouseOverAction=[!SetOption ExtraRow4Icon FontColor "#SettingsHover#"][!UpdateMeter ExtraRow4Icon][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow4Icon FontColor "#TextColor#"][!UpdateMeter ExtraRow4Icon][!Redraw]

[ExtraRow4Clear]
Meter=String
Text="[\xe74d]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow4
Hidden=[ExtraHas4]
Y=r
X=218
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 4][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]
MouseOverAction=[!SetOption ExtraRow4Clear FontColor "#SettingsHover#"][!UpdateMeter ExtraRow4Clear][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow4Clear FontColor "#TextColor#"][!UpdateMeter ExtraRow4Clear][!Redraw]

; --- Slot 5 ---
[ExtraHas5]
Measure=String
String=#Egame5#
DynamicVariables=1
RegExpSubstitute=1
Substitute="^\s*$":"1","^.+$":"0"
Group=ExtraHas

[ExtraRow5Name]
Meter=String
Text=#Egame5#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow5
Hidden=[ExtraHas5]
Y=2R
X=8
W=72
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 5][!SetOption InputExtraNameMeasure DefaultValue "#Egame5#"][!SetOption InputExtraNameMeasure Y "[ExtraRow5Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow5Path]
Meter=String
Text=#Egame5Path#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow5
Hidden=[ExtraHas5]
Y=r
X=84
W=70
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 5][!SetOption InputExtraPathMeasure DefaultValue "#Egame5Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow5Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow5Browse]
Meter=String
Text="[\xe8b7]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow5
Hidden=[ExtraHas5]
Y=r
X=158
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickTarget Egame5Path][!SetVariable ExtraEditIndex 5][!CommandMeasure FileChooseFile "ChooseFile 1"]
MouseOverAction=[!SetOption ExtraRow5Browse FontColor "#SettingsHover#"][!UpdateMeter ExtraRow5Browse][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow5Browse FontColor "#TextColor#"][!UpdateMeter ExtraRow5Browse][!Redraw]

[ExtraRow5Vis]
Meter=String
Text="[\xe7b3]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow5
Hidden=[ExtraHas5]
Y=r
X=178
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 5][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]
MouseOverAction=[!SetOption ExtraRow5Vis FontColor "#SettingsHover#"][!UpdateMeter ExtraRow5Vis][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow5Vis FontColor "#TextColor#"][!UpdateMeter ExtraRow5Vis][!Redraw]

[ExtraRow5Icon]
Meter=String
Text="[\xe8b9]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow5
Hidden=[ExtraHas5]
Y=r
X=198
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickSlot 5][!SetVariable _PickSlotPadded "005"][!SetVariable ExtraEditIndex 5][!CommandMeasure FileChooseImage "ChooseImage 1"]
MouseOverAction=[!SetOption ExtraRow5Icon FontColor "#SettingsHover#"][!UpdateMeter ExtraRow5Icon][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow5Icon FontColor "#TextColor#"][!UpdateMeter ExtraRow5Icon][!Redraw]

[ExtraRow5Clear]
Meter=String
Text="[\xe74d]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow5
Hidden=[ExtraHas5]
Y=r
X=218
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 5][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]
MouseOverAction=[!SetOption ExtraRow5Clear FontColor "#SettingsHover#"][!UpdateMeter ExtraRow5Clear][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow5Clear FontColor "#TextColor#"][!UpdateMeter ExtraRow5Clear][!Redraw]

; --- Slot 6 ---
[ExtraHas6]
Measure=String
String=#Egame6#
DynamicVariables=1
RegExpSubstitute=1
Substitute="^\s*$":"1","^.+$":"0"
Group=ExtraHas

[ExtraRow6Name]
Meter=String
Text=#Egame6#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow6
Hidden=[ExtraHas6]
Y=2R
X=8
W=72
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 6][!SetOption InputExtraNameMeasure DefaultValue "#Egame6#"][!SetOption InputExtraNameMeasure Y "[ExtraRow6Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow6Path]
Meter=String
Text=#Egame6Path#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow6
Hidden=[ExtraHas6]
Y=r
X=84
W=70
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 6][!SetOption InputExtraPathMeasure DefaultValue "#Egame6Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow6Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow6Browse]
Meter=String
Text="[\xe8b7]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow6
Hidden=[ExtraHas6]
Y=r
X=158
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickTarget Egame6Path][!SetVariable ExtraEditIndex 6][!CommandMeasure FileChooseFile "ChooseFile 1"]
MouseOverAction=[!SetOption ExtraRow6Browse FontColor "#SettingsHover#"][!UpdateMeter ExtraRow6Browse][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow6Browse FontColor "#TextColor#"][!UpdateMeter ExtraRow6Browse][!Redraw]

[ExtraRow6Vis]
Meter=String
Text="[\xe7b3]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow6
Hidden=[ExtraHas6]
Y=r
X=178
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 6][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]
MouseOverAction=[!SetOption ExtraRow6Vis FontColor "#SettingsHover#"][!UpdateMeter ExtraRow6Vis][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow6Vis FontColor "#TextColor#"][!UpdateMeter ExtraRow6Vis][!Redraw]

[ExtraRow6Icon]
Meter=String
Text="[\xe8b9]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow6
Hidden=[ExtraHas6]
Y=r
X=198
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickSlot 6][!SetVariable _PickSlotPadded "006"][!SetVariable ExtraEditIndex 6][!CommandMeasure FileChooseImage "ChooseImage 1"]
MouseOverAction=[!SetOption ExtraRow6Icon FontColor "#SettingsHover#"][!UpdateMeter ExtraRow6Icon][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow6Icon FontColor "#TextColor#"][!UpdateMeter ExtraRow6Icon][!Redraw]

[ExtraRow6Clear]
Meter=String
Text="[\xe74d]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow6
Hidden=[ExtraHas6]
Y=r
X=218
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 6][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]
MouseOverAction=[!SetOption ExtraRow6Clear FontColor "#SettingsHover#"][!UpdateMeter ExtraRow6Clear][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow6Clear FontColor "#TextColor#"][!UpdateMeter ExtraRow6Clear][!Redraw]

; --- Slot 7 ---
[ExtraHas7]
Measure=String
String=#Egame7#
DynamicVariables=1
RegExpSubstitute=1
Substitute="^\s*$":"1","^.+$":"0"
Group=ExtraHas

[ExtraRow7Name]
Meter=String
Text=#Egame7#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow7
Hidden=[ExtraHas7]
Y=2R
X=8
W=72
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 7][!SetOption InputExtraNameMeasure DefaultValue "#Egame7#"][!SetOption InputExtraNameMeasure Y "[ExtraRow7Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow7Path]
Meter=String
Text=#Egame7Path#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow7
Hidden=[ExtraHas7]
Y=r
X=84
W=70
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 7][!SetOption InputExtraPathMeasure DefaultValue "#Egame7Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow7Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow7Browse]
Meter=String
Text="[\xe8b7]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow7
Hidden=[ExtraHas7]
Y=r
X=158
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickTarget Egame7Path][!SetVariable ExtraEditIndex 7][!CommandMeasure FileChooseFile "ChooseFile 1"]
MouseOverAction=[!SetOption ExtraRow7Browse FontColor "#SettingsHover#"][!UpdateMeter ExtraRow7Browse][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow7Browse FontColor "#TextColor#"][!UpdateMeter ExtraRow7Browse][!Redraw]

[ExtraRow7Vis]
Meter=String
Text="[\xe7b3]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow7
Hidden=[ExtraHas7]
Y=r
X=178
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 7][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]
MouseOverAction=[!SetOption ExtraRow7Vis FontColor "#SettingsHover#"][!UpdateMeter ExtraRow7Vis][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow7Vis FontColor "#TextColor#"][!UpdateMeter ExtraRow7Vis][!Redraw]

[ExtraRow7Icon]
Meter=String
Text="[\xe8b9]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow7
Hidden=[ExtraHas7]
Y=r
X=198
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickSlot 7][!SetVariable _PickSlotPadded "007"][!SetVariable ExtraEditIndex 7][!CommandMeasure FileChooseImage "ChooseImage 1"]
MouseOverAction=[!SetOption ExtraRow7Icon FontColor "#SettingsHover#"][!UpdateMeter ExtraRow7Icon][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow7Icon FontColor "#TextColor#"][!UpdateMeter ExtraRow7Icon][!Redraw]

[ExtraRow7Clear]
Meter=String
Text="[\xe74d]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow7
Hidden=[ExtraHas7]
Y=r
X=218
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 7][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]
MouseOverAction=[!SetOption ExtraRow7Clear FontColor "#SettingsHover#"][!UpdateMeter ExtraRow7Clear][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow7Clear FontColor "#TextColor#"][!UpdateMeter ExtraRow7Clear][!Redraw]

; --- Slot 8 ---
[ExtraHas8]
Measure=String
String=#Egame8#
DynamicVariables=1
RegExpSubstitute=1
Substitute="^\s*$":"1","^.+$":"0"
Group=ExtraHas

[ExtraRow8Name]
Meter=String
Text=#Egame8#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow8
Hidden=[ExtraHas8]
Y=2R
X=8
W=72
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 8][!SetOption InputExtraNameMeasure DefaultValue "#Egame8#"][!SetOption InputExtraNameMeasure Y "[ExtraRow8Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow8Path]
Meter=String
Text=#Egame8Path#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow8
Hidden=[ExtraHas8]
Y=r
X=84
W=70
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 8][!SetOption InputExtraPathMeasure DefaultValue "#Egame8Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow8Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow8Browse]
Meter=String
Text="[\xe8b7]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow8
Hidden=[ExtraHas8]
Y=r
X=158
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickTarget Egame8Path][!SetVariable ExtraEditIndex 8][!CommandMeasure FileChooseFile "ChooseFile 1"]
MouseOverAction=[!SetOption ExtraRow8Browse FontColor "#SettingsHover#"][!UpdateMeter ExtraRow8Browse][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow8Browse FontColor "#TextColor#"][!UpdateMeter ExtraRow8Browse][!Redraw]

[ExtraRow8Vis]
Meter=String
Text="[\xe7b3]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow8
Hidden=[ExtraHas8]
Y=r
X=178
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 8][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]
MouseOverAction=[!SetOption ExtraRow8Vis FontColor "#SettingsHover#"][!UpdateMeter ExtraRow8Vis][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow8Vis FontColor "#TextColor#"][!UpdateMeter ExtraRow8Vis][!Redraw]

[ExtraRow8Icon]
Meter=String
Text="[\xe8b9]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow8
Hidden=[ExtraHas8]
Y=r
X=198
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickSlot 8][!SetVariable _PickSlotPadded "008"][!SetVariable ExtraEditIndex 8][!CommandMeasure FileChooseImage "ChooseImage 1"]
MouseOverAction=[!SetOption ExtraRow8Icon FontColor "#SettingsHover#"][!UpdateMeter ExtraRow8Icon][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow8Icon FontColor "#TextColor#"][!UpdateMeter ExtraRow8Icon][!Redraw]

[ExtraRow8Clear]
Meter=String
Text="[\xe74d]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow8
Hidden=[ExtraHas8]
Y=r
X=218
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 8][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]
MouseOverAction=[!SetOption ExtraRow8Clear FontColor "#SettingsHover#"][!UpdateMeter ExtraRow8Clear][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow8Clear FontColor "#TextColor#"][!UpdateMeter ExtraRow8Clear][!Redraw]

; --- Slot 9 ---
[ExtraHas9]
Measure=String
String=#Egame9#
DynamicVariables=1
RegExpSubstitute=1
Substitute="^\s*$":"1","^.+$":"0"
Group=ExtraHas

[ExtraRow9Name]
Meter=String
Text=#Egame9#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow9
Hidden=[ExtraHas9]
Y=2R
X=8
W=72
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 9][!SetOption InputExtraNameMeasure DefaultValue "#Egame9#"][!SetOption InputExtraNameMeasure Y "[ExtraRow9Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow9Path]
Meter=String
Text=#Egame9Path#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow9
Hidden=[ExtraHas9]
Y=r
X=84
W=70
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 9][!SetOption InputExtraPathMeasure DefaultValue "#Egame9Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow9Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow9Browse]
Meter=String
Text="[\xe8b7]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow9
Hidden=[ExtraHas9]
Y=r
X=158
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickTarget Egame9Path][!SetVariable ExtraEditIndex 9][!CommandMeasure FileChooseFile "ChooseFile 1"]
MouseOverAction=[!SetOption ExtraRow9Browse FontColor "#SettingsHover#"][!UpdateMeter ExtraRow9Browse][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow9Browse FontColor "#TextColor#"][!UpdateMeter ExtraRow9Browse][!Redraw]

[ExtraRow9Vis]
Meter=String
Text="[\xe7b3]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow9
Hidden=[ExtraHas9]
Y=r
X=178
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 9][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]
MouseOverAction=[!SetOption ExtraRow9Vis FontColor "#SettingsHover#"][!UpdateMeter ExtraRow9Vis][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow9Vis FontColor "#TextColor#"][!UpdateMeter ExtraRow9Vis][!Redraw]

[ExtraRow9Icon]
Meter=String
Text="[\xe8b9]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow9
Hidden=[ExtraHas9]
Y=r
X=198
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickSlot 9][!SetVariable _PickSlotPadded "009"][!SetVariable ExtraEditIndex 9][!CommandMeasure FileChooseImage "ChooseImage 1"]
MouseOverAction=[!SetOption ExtraRow9Icon FontColor "#SettingsHover#"][!UpdateMeter ExtraRow9Icon][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow9Icon FontColor "#TextColor#"][!UpdateMeter ExtraRow9Icon][!Redraw]

[ExtraRow9Clear]
Meter=String
Text="[\xe74d]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow9
Hidden=[ExtraHas9]
Y=r
X=218
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 9][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]
MouseOverAction=[!SetOption ExtraRow9Clear FontColor "#SettingsHover#"][!UpdateMeter ExtraRow9Clear][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow9Clear FontColor "#TextColor#"][!UpdateMeter ExtraRow9Clear][!Redraw]

; --- Slot 10 ---
[ExtraHas10]
Measure=String
String=#Egame10#
DynamicVariables=1
RegExpSubstitute=1
Substitute="^\s*$":"1","^.+$":"0"
Group=ExtraHas

[ExtraRow10Name]
Meter=String
Text=#Egame10#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow10
Hidden=[ExtraHas10]
Y=2R
X=8
W=72
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 10][!SetOption InputExtraNameMeasure DefaultValue "#Egame10#"][!SetOption InputExtraNameMeasure Y "[ExtraRow10Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow10Path]
Meter=String
Text=#Egame10Path#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow10
Hidden=[ExtraHas10]
Y=r
X=84
W=70
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 10][!SetOption InputExtraPathMeasure DefaultValue "#Egame10Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow10Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow10Browse]
Meter=String
Text="[\xe8b7]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow10
Hidden=[ExtraHas10]
Y=r
X=158
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickTarget Egame10Path][!SetVariable ExtraEditIndex 10][!CommandMeasure FileChooseFile "ChooseFile 1"]
MouseOverAction=[!SetOption ExtraRow10Browse FontColor "#SettingsHover#"][!UpdateMeter ExtraRow10Browse][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow10Browse FontColor "#TextColor#"][!UpdateMeter ExtraRow10Browse][!Redraw]

[ExtraRow10Vis]
Meter=String
Text="[\xe7b3]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow10
Hidden=[ExtraHas10]
Y=r
X=178
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 10][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]
MouseOverAction=[!SetOption ExtraRow10Vis FontColor "#SettingsHover#"][!UpdateMeter ExtraRow10Vis][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow10Vis FontColor "#TextColor#"][!UpdateMeter ExtraRow10Vis][!Redraw]

[ExtraRow10Icon]
Meter=String
Text="[\xe8b9]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow10
Hidden=[ExtraHas10]
Y=r
X=198
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickSlot 10][!SetVariable _PickSlotPadded "010"][!SetVariable ExtraEditIndex 10][!CommandMeasure FileChooseImage "ChooseImage 1"]
MouseOverAction=[!SetOption ExtraRow10Icon FontColor "#SettingsHover#"][!UpdateMeter ExtraRow10Icon][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow10Icon FontColor "#TextColor#"][!UpdateMeter ExtraRow10Icon][!Redraw]

[ExtraRow10Clear]
Meter=String
Text="[\xe74d]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow10
Hidden=[ExtraHas10]
Y=r
X=218
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 10][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]
MouseOverAction=[!SetOption ExtraRow10Clear FontColor "#SettingsHover#"][!UpdateMeter ExtraRow10Clear][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow10Clear FontColor "#TextColor#"][!UpdateMeter ExtraRow10Clear][!Redraw]

; --- Slot 11 ---
[ExtraHas11]
Measure=String
String=#Egame11#
DynamicVariables=1
RegExpSubstitute=1
Substitute="^\s*$":"1","^.+$":"0"
Group=ExtraHas

[ExtraRow11Name]
Meter=String
Text=#Egame11#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow11
Hidden=[ExtraHas11]
Y=2R
X=8
W=72
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 11][!SetOption InputExtraNameMeasure DefaultValue "#Egame11#"][!SetOption InputExtraNameMeasure Y "[ExtraRow11Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow11Path]
Meter=String
Text=#Egame11Path#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow11
Hidden=[ExtraHas11]
Y=r
X=84
W=70
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 11][!SetOption InputExtraPathMeasure DefaultValue "#Egame11Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow11Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow11Browse]
Meter=String
Text="[\xe8b7]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow11
Hidden=[ExtraHas11]
Y=r
X=158
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickTarget Egame11Path][!SetVariable ExtraEditIndex 11][!CommandMeasure FileChooseFile "ChooseFile 1"]
MouseOverAction=[!SetOption ExtraRow11Browse FontColor "#SettingsHover#"][!UpdateMeter ExtraRow11Browse][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow11Browse FontColor "#TextColor#"][!UpdateMeter ExtraRow11Browse][!Redraw]

[ExtraRow11Vis]
Meter=String
Text="[\xe7b3]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow11
Hidden=[ExtraHas11]
Y=r
X=178
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 11][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]
MouseOverAction=[!SetOption ExtraRow11Vis FontColor "#SettingsHover#"][!UpdateMeter ExtraRow11Vis][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow11Vis FontColor "#TextColor#"][!UpdateMeter ExtraRow11Vis][!Redraw]

[ExtraRow11Icon]
Meter=String
Text="[\xe8b9]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow11
Hidden=[ExtraHas11]
Y=r
X=198
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickSlot 11][!SetVariable _PickSlotPadded "011"][!SetVariable ExtraEditIndex 11][!CommandMeasure FileChooseImage "ChooseImage 1"]
MouseOverAction=[!SetOption ExtraRow11Icon FontColor "#SettingsHover#"][!UpdateMeter ExtraRow11Icon][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow11Icon FontColor "#TextColor#"][!UpdateMeter ExtraRow11Icon][!Redraw]

[ExtraRow11Clear]
Meter=String
Text="[\xe74d]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow11
Hidden=[ExtraHas11]
Y=r
X=218
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 11][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]
MouseOverAction=[!SetOption ExtraRow11Clear FontColor "#SettingsHover#"][!UpdateMeter ExtraRow11Clear][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow11Clear FontColor "#TextColor#"][!UpdateMeter ExtraRow11Clear][!Redraw]

; --- Slot 12 ---
[ExtraHas12]
Measure=String
String=#Egame12#
DynamicVariables=1
RegExpSubstitute=1
Substitute="^\s*$":"1","^.+$":"0"
Group=ExtraHas

[ExtraRow12Name]
Meter=String
Text=#Egame12#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow12
Hidden=[ExtraHas12]
Y=2R
X=8
W=72
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 12][!SetOption InputExtraNameMeasure DefaultValue "#Egame12#"][!SetOption InputExtraNameMeasure Y "[ExtraRow12Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow12Path]
Meter=String
Text=#Egame12Path#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow12
Hidden=[ExtraHas12]
Y=r
X=84
W=70
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 12][!SetOption InputExtraPathMeasure DefaultValue "#Egame12Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow12Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow12Browse]
Meter=String
Text="[\xe8b7]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow12
Hidden=[ExtraHas12]
Y=r
X=158
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickTarget Egame12Path][!SetVariable ExtraEditIndex 12][!CommandMeasure FileChooseFile "ChooseFile 1"]
MouseOverAction=[!SetOption ExtraRow12Browse FontColor "#SettingsHover#"][!UpdateMeter ExtraRow12Browse][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow12Browse FontColor "#TextColor#"][!UpdateMeter ExtraRow12Browse][!Redraw]

[ExtraRow12Vis]
Meter=String
Text="[\xe7b3]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow12
Hidden=[ExtraHas12]
Y=r
X=178
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 12][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]
MouseOverAction=[!SetOption ExtraRow12Vis FontColor "#SettingsHover#"][!UpdateMeter ExtraRow12Vis][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow12Vis FontColor "#TextColor#"][!UpdateMeter ExtraRow12Vis][!Redraw]

[ExtraRow12Icon]
Meter=String
Text="[\xe8b9]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow12
Hidden=[ExtraHas12]
Y=r
X=198
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickSlot 12][!SetVariable _PickSlotPadded "012"][!SetVariable ExtraEditIndex 12][!CommandMeasure FileChooseImage "ChooseImage 1"]
MouseOverAction=[!SetOption ExtraRow12Icon FontColor "#SettingsHover#"][!UpdateMeter ExtraRow12Icon][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow12Icon FontColor "#TextColor#"][!UpdateMeter ExtraRow12Icon][!Redraw]

[ExtraRow12Clear]
Meter=String
Text="[\xe74d]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow12
Hidden=[ExtraHas12]
Y=r
X=218
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 12][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]
MouseOverAction=[!SetOption ExtraRow12Clear FontColor "#SettingsHover#"][!UpdateMeter ExtraRow12Clear][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow12Clear FontColor "#TextColor#"][!UpdateMeter ExtraRow12Clear][!Redraw]

; --- Slot 13 ---
[ExtraHas13]
Measure=String
String=#Egame13#
DynamicVariables=1
RegExpSubstitute=1
Substitute="^\s*$":"1","^.+$":"0"
Group=ExtraHas

[ExtraRow13Name]
Meter=String
Text=#Egame13#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow13
Hidden=[ExtraHas13]
Y=2R
X=8
W=72
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 13][!SetOption InputExtraNameMeasure DefaultValue "#Egame13#"][!SetOption InputExtraNameMeasure Y "[ExtraRow13Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow13Path]
Meter=String
Text=#Egame13Path#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow13
Hidden=[ExtraHas13]
Y=r
X=84
W=70
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 13][!SetOption InputExtraPathMeasure DefaultValue "#Egame13Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow13Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow13Browse]
Meter=String
Text="[\xe8b7]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow13
Hidden=[ExtraHas13]
Y=r
X=158
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickTarget Egame13Path][!SetVariable ExtraEditIndex 13][!CommandMeasure FileChooseFile "ChooseFile 1"]
MouseOverAction=[!SetOption ExtraRow13Browse FontColor "#SettingsHover#"][!UpdateMeter ExtraRow13Browse][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow13Browse FontColor "#TextColor#"][!UpdateMeter ExtraRow13Browse][!Redraw]

[ExtraRow13Vis]
Meter=String
Text="[\xe7b3]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow13
Hidden=[ExtraHas13]
Y=r
X=178
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 13][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]
MouseOverAction=[!SetOption ExtraRow13Vis FontColor "#SettingsHover#"][!UpdateMeter ExtraRow13Vis][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow13Vis FontColor "#TextColor#"][!UpdateMeter ExtraRow13Vis][!Redraw]

[ExtraRow13Icon]
Meter=String
Text="[\xe8b9]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow13
Hidden=[ExtraHas13]
Y=r
X=198
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickSlot 13][!SetVariable _PickSlotPadded "013"][!SetVariable ExtraEditIndex 13][!CommandMeasure FileChooseImage "ChooseImage 1"]
MouseOverAction=[!SetOption ExtraRow13Icon FontColor "#SettingsHover#"][!UpdateMeter ExtraRow13Icon][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow13Icon FontColor "#TextColor#"][!UpdateMeter ExtraRow13Icon][!Redraw]

[ExtraRow13Clear]
Meter=String
Text="[\xe74d]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow13
Hidden=[ExtraHas13]
Y=r
X=218
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 13][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]
MouseOverAction=[!SetOption ExtraRow13Clear FontColor "#SettingsHover#"][!UpdateMeter ExtraRow13Clear][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow13Clear FontColor "#TextColor#"][!UpdateMeter ExtraRow13Clear][!Redraw]

; --- Slot 14 ---
[ExtraHas14]
Measure=String
String=#Egame14#
DynamicVariables=1
RegExpSubstitute=1
Substitute="^\s*$":"1","^.+$":"0"
Group=ExtraHas

[ExtraRow14Name]
Meter=String
Text=#Egame14#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow14
Hidden=[ExtraHas14]
Y=2R
X=8
W=72
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 14][!SetOption InputExtraNameMeasure DefaultValue "#Egame14#"][!SetOption InputExtraNameMeasure Y "[ExtraRow14Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow14Path]
Meter=String
Text=#Egame14Path#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow14
Hidden=[ExtraHas14]
Y=r
X=84
W=70
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 14][!SetOption InputExtraPathMeasure DefaultValue "#Egame14Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow14Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow14Browse]
Meter=String
Text="[\xe8b7]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow14
Hidden=[ExtraHas14]
Y=r
X=158
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickTarget Egame14Path][!SetVariable ExtraEditIndex 14][!CommandMeasure FileChooseFile "ChooseFile 1"]
MouseOverAction=[!SetOption ExtraRow14Browse FontColor "#SettingsHover#"][!UpdateMeter ExtraRow14Browse][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow14Browse FontColor "#TextColor#"][!UpdateMeter ExtraRow14Browse][!Redraw]

[ExtraRow14Vis]
Meter=String
Text="[\xe7b3]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow14
Hidden=[ExtraHas14]
Y=r
X=178
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 14][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]
MouseOverAction=[!SetOption ExtraRow14Vis FontColor "#SettingsHover#"][!UpdateMeter ExtraRow14Vis][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow14Vis FontColor "#TextColor#"][!UpdateMeter ExtraRow14Vis][!Redraw]

[ExtraRow14Icon]
Meter=String
Text="[\xe8b9]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow14
Hidden=[ExtraHas14]
Y=r
X=198
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickSlot 14][!SetVariable _PickSlotPadded "014"][!SetVariable ExtraEditIndex 14][!CommandMeasure FileChooseImage "ChooseImage 1"]
MouseOverAction=[!SetOption ExtraRow14Icon FontColor "#SettingsHover#"][!UpdateMeter ExtraRow14Icon][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow14Icon FontColor "#TextColor#"][!UpdateMeter ExtraRow14Icon][!Redraw]

[ExtraRow14Clear]
Meter=String
Text="[\xe74d]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow14
Hidden=[ExtraHas14]
Y=r
X=218
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 14][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]
MouseOverAction=[!SetOption ExtraRow14Clear FontColor "#SettingsHover#"][!UpdateMeter ExtraRow14Clear][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow14Clear FontColor "#TextColor#"][!UpdateMeter ExtraRow14Clear][!Redraw]

; --- Slot 15 ---
[ExtraHas15]
Measure=String
String=#Egame15#
DynamicVariables=1
RegExpSubstitute=1
Substitute="^\s*$":"1","^.+$":"0"
Group=ExtraHas

[ExtraRow15Name]
Meter=String
Text=#Egame15#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow15
Hidden=[ExtraHas15]
Y=2R
X=8
W=72
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 15][!SetOption InputExtraNameMeasure DefaultValue "#Egame15#"][!SetOption InputExtraNameMeasure Y "[ExtraRow15Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow15Path]
Meter=String
Text=#Egame15Path#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow15
Hidden=[ExtraHas15]
Y=r
X=84
W=70
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 15][!SetOption InputExtraPathMeasure DefaultValue "#Egame15Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow15Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow15Browse]
Meter=String
Text="[\xe8b7]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow15
Hidden=[ExtraHas15]
Y=r
X=158
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickTarget Egame15Path][!SetVariable ExtraEditIndex 15][!CommandMeasure FileChooseFile "ChooseFile 1"]
MouseOverAction=[!SetOption ExtraRow15Browse FontColor "#SettingsHover#"][!UpdateMeter ExtraRow15Browse][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow15Browse FontColor "#TextColor#"][!UpdateMeter ExtraRow15Browse][!Redraw]

[ExtraRow15Vis]
Meter=String
Text="[\xe7b3]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow15
Hidden=[ExtraHas15]
Y=r
X=178
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 15][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]
MouseOverAction=[!SetOption ExtraRow15Vis FontColor "#SettingsHover#"][!UpdateMeter ExtraRow15Vis][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow15Vis FontColor "#TextColor#"][!UpdateMeter ExtraRow15Vis][!Redraw]

[ExtraRow15Icon]
Meter=String
Text="[\xe8b9]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow15
Hidden=[ExtraHas15]
Y=r
X=198
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickSlot 15][!SetVariable _PickSlotPadded "015"][!SetVariable ExtraEditIndex 15][!CommandMeasure FileChooseImage "ChooseImage 1"]
MouseOverAction=[!SetOption ExtraRow15Icon FontColor "#SettingsHover#"][!UpdateMeter ExtraRow15Icon][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow15Icon FontColor "#TextColor#"][!UpdateMeter ExtraRow15Icon][!Redraw]

[ExtraRow15Clear]
Meter=String
Text="[\xe74d]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow15
Hidden=[ExtraHas15]
Y=r
X=218
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 15][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]
MouseOverAction=[!SetOption ExtraRow15Clear FontColor "#SettingsHover#"][!UpdateMeter ExtraRow15Clear][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow15Clear FontColor "#TextColor#"][!UpdateMeter ExtraRow15Clear][!Redraw]

; --- Slot 16 ---
[ExtraHas16]
Measure=String
String=#Egame16#
DynamicVariables=1
RegExpSubstitute=1
Substitute="^\s*$":"1","^.+$":"0"
Group=ExtraHas

[ExtraRow16Name]
Meter=String
Text=#Egame16#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow16
Hidden=[ExtraHas16]
Y=2R
X=8
W=72
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 16][!SetOption InputExtraNameMeasure DefaultValue "#Egame16#"][!SetOption InputExtraNameMeasure Y "[ExtraRow16Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow16Path]
Meter=String
Text=#Egame16Path#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow16
Hidden=[ExtraHas16]
Y=r
X=84
W=70
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 16][!SetOption InputExtraPathMeasure DefaultValue "#Egame16Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow16Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow16Browse]
Meter=String
Text="[\xe8b7]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow16
Hidden=[ExtraHas16]
Y=r
X=158
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickTarget Egame16Path][!SetVariable ExtraEditIndex 16][!CommandMeasure FileChooseFile "ChooseFile 1"]
MouseOverAction=[!SetOption ExtraRow16Browse FontColor "#SettingsHover#"][!UpdateMeter ExtraRow16Browse][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow16Browse FontColor "#TextColor#"][!UpdateMeter ExtraRow16Browse][!Redraw]

[ExtraRow16Vis]
Meter=String
Text="[\xe7b3]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow16
Hidden=[ExtraHas16]
Y=r
X=178
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 16][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]
MouseOverAction=[!SetOption ExtraRow16Vis FontColor "#SettingsHover#"][!UpdateMeter ExtraRow16Vis][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow16Vis FontColor "#TextColor#"][!UpdateMeter ExtraRow16Vis][!Redraw]

[ExtraRow16Icon]
Meter=String
Text="[\xe8b9]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow16
Hidden=[ExtraHas16]
Y=r
X=198
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickSlot 16][!SetVariable _PickSlotPadded "016"][!SetVariable ExtraEditIndex 16][!CommandMeasure FileChooseImage "ChooseImage 1"]
MouseOverAction=[!SetOption ExtraRow16Icon FontColor "#SettingsHover#"][!UpdateMeter ExtraRow16Icon][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow16Icon FontColor "#TextColor#"][!UpdateMeter ExtraRow16Icon][!Redraw]

[ExtraRow16Clear]
Meter=String
Text="[\xe74d]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow16
Hidden=[ExtraHas16]
Y=r
X=218
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 16][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]
MouseOverAction=[!SetOption ExtraRow16Clear FontColor "#SettingsHover#"][!UpdateMeter ExtraRow16Clear][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow16Clear FontColor "#TextColor#"][!UpdateMeter ExtraRow16Clear][!Redraw]

; --- Slot 17 ---
[ExtraHas17]
Measure=String
String=#Egame17#
DynamicVariables=1
RegExpSubstitute=1
Substitute="^\s*$":"1","^.+$":"0"
Group=ExtraHas

[ExtraRow17Name]
Meter=String
Text=#Egame17#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow17
Hidden=[ExtraHas17]
Y=2R
X=8
W=72
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 17][!SetOption InputExtraNameMeasure DefaultValue "#Egame17#"][!SetOption InputExtraNameMeasure Y "[ExtraRow17Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow17Path]
Meter=String
Text=#Egame17Path#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow17
Hidden=[ExtraHas17]
Y=r
X=84
W=70
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 17][!SetOption InputExtraPathMeasure DefaultValue "#Egame17Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow17Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow17Browse]
Meter=String
Text="[\xe8b7]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow17
Hidden=[ExtraHas17]
Y=r
X=158
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickTarget Egame17Path][!SetVariable ExtraEditIndex 17][!CommandMeasure FileChooseFile "ChooseFile 1"]
MouseOverAction=[!SetOption ExtraRow17Browse FontColor "#SettingsHover#"][!UpdateMeter ExtraRow17Browse][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow17Browse FontColor "#TextColor#"][!UpdateMeter ExtraRow17Browse][!Redraw]

[ExtraRow17Vis]
Meter=String
Text="[\xe7b3]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow17
Hidden=[ExtraHas17]
Y=r
X=178
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 17][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]
MouseOverAction=[!SetOption ExtraRow17Vis FontColor "#SettingsHover#"][!UpdateMeter ExtraRow17Vis][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow17Vis FontColor "#TextColor#"][!UpdateMeter ExtraRow17Vis][!Redraw]

[ExtraRow17Icon]
Meter=String
Text="[\xe8b9]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow17
Hidden=[ExtraHas17]
Y=r
X=198
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickSlot 17][!SetVariable _PickSlotPadded "017"][!SetVariable ExtraEditIndex 17][!CommandMeasure FileChooseImage "ChooseImage 1"]
MouseOverAction=[!SetOption ExtraRow17Icon FontColor "#SettingsHover#"][!UpdateMeter ExtraRow17Icon][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow17Icon FontColor "#TextColor#"][!UpdateMeter ExtraRow17Icon][!Redraw]

[ExtraRow17Clear]
Meter=String
Text="[\xe74d]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow17
Hidden=[ExtraHas17]
Y=r
X=218
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 17][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]
MouseOverAction=[!SetOption ExtraRow17Clear FontColor "#SettingsHover#"][!UpdateMeter ExtraRow17Clear][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow17Clear FontColor "#TextColor#"][!UpdateMeter ExtraRow17Clear][!Redraw]

; --- Slot 18 ---
[ExtraHas18]
Measure=String
String=#Egame18#
DynamicVariables=1
RegExpSubstitute=1
Substitute="^\s*$":"1","^.+$":"0"
Group=ExtraHas

[ExtraRow18Name]
Meter=String
Text=#Egame18#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow18
Hidden=[ExtraHas18]
Y=2R
X=8
W=72
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 18][!SetOption InputExtraNameMeasure DefaultValue "#Egame18#"][!SetOption InputExtraNameMeasure Y "[ExtraRow18Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow18Path]
Meter=String
Text=#Egame18Path#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow18
Hidden=[ExtraHas18]
Y=r
X=84
W=70
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 18][!SetOption InputExtraPathMeasure DefaultValue "#Egame18Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow18Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow18Browse]
Meter=String
Text="[\xe8b7]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow18
Hidden=[ExtraHas18]
Y=r
X=158
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickTarget Egame18Path][!SetVariable ExtraEditIndex 18][!CommandMeasure FileChooseFile "ChooseFile 1"]
MouseOverAction=[!SetOption ExtraRow18Browse FontColor "#SettingsHover#"][!UpdateMeter ExtraRow18Browse][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow18Browse FontColor "#TextColor#"][!UpdateMeter ExtraRow18Browse][!Redraw]

[ExtraRow18Vis]
Meter=String
Text="[\xe7b3]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow18
Hidden=[ExtraHas18]
Y=r
X=178
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 18][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]
MouseOverAction=[!SetOption ExtraRow18Vis FontColor "#SettingsHover#"][!UpdateMeter ExtraRow18Vis][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow18Vis FontColor "#TextColor#"][!UpdateMeter ExtraRow18Vis][!Redraw]

[ExtraRow18Icon]
Meter=String
Text="[\xe8b9]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow18
Hidden=[ExtraHas18]
Y=r
X=198
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickSlot 18][!SetVariable _PickSlotPadded "018"][!SetVariable ExtraEditIndex 18][!CommandMeasure FileChooseImage "ChooseImage 1"]
MouseOverAction=[!SetOption ExtraRow18Icon FontColor "#SettingsHover#"][!UpdateMeter ExtraRow18Icon][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow18Icon FontColor "#TextColor#"][!UpdateMeter ExtraRow18Icon][!Redraw]

[ExtraRow18Clear]
Meter=String
Text="[\xe74d]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow18
Hidden=[ExtraHas18]
Y=r
X=218
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 18][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]
MouseOverAction=[!SetOption ExtraRow18Clear FontColor "#SettingsHover#"][!UpdateMeter ExtraRow18Clear][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow18Clear FontColor "#TextColor#"][!UpdateMeter ExtraRow18Clear][!Redraw]

; --- Slot 19 ---
[ExtraHas19]
Measure=String
String=#Egame19#
DynamicVariables=1
RegExpSubstitute=1
Substitute="^\s*$":"1","^.+$":"0"
Group=ExtraHas

[ExtraRow19Name]
Meter=String
Text=#Egame19#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow19
Hidden=[ExtraHas19]
Y=2R
X=8
W=72
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 19][!SetOption InputExtraNameMeasure DefaultValue "#Egame19#"][!SetOption InputExtraNameMeasure Y "[ExtraRow19Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow19Path]
Meter=String
Text=#Egame19Path#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow19
Hidden=[ExtraHas19]
Y=r
X=84
W=70
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 19][!SetOption InputExtraPathMeasure DefaultValue "#Egame19Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow19Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow19Browse]
Meter=String
Text="[\xe8b7]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow19
Hidden=[ExtraHas19]
Y=r
X=158
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickTarget Egame19Path][!SetVariable ExtraEditIndex 19][!CommandMeasure FileChooseFile "ChooseFile 1"]
MouseOverAction=[!SetOption ExtraRow19Browse FontColor "#SettingsHover#"][!UpdateMeter ExtraRow19Browse][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow19Browse FontColor "#TextColor#"][!UpdateMeter ExtraRow19Browse][!Redraw]

[ExtraRow19Vis]
Meter=String
Text="[\xe7b3]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow19
Hidden=[ExtraHas19]
Y=r
X=178
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 19][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]
MouseOverAction=[!SetOption ExtraRow19Vis FontColor "#SettingsHover#"][!UpdateMeter ExtraRow19Vis][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow19Vis FontColor "#TextColor#"][!UpdateMeter ExtraRow19Vis][!Redraw]

[ExtraRow19Icon]
Meter=String
Text="[\xe8b9]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow19
Hidden=[ExtraHas19]
Y=r
X=198
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickSlot 19][!SetVariable _PickSlotPadded "019"][!SetVariable ExtraEditIndex 19][!CommandMeasure FileChooseImage "ChooseImage 1"]
MouseOverAction=[!SetOption ExtraRow19Icon FontColor "#SettingsHover#"][!UpdateMeter ExtraRow19Icon][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow19Icon FontColor "#TextColor#"][!UpdateMeter ExtraRow19Icon][!Redraw]

[ExtraRow19Clear]
Meter=String
Text="[\xe74d]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow19
Hidden=[ExtraHas19]
Y=r
X=218
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 19][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]
MouseOverAction=[!SetOption ExtraRow19Clear FontColor "#SettingsHover#"][!UpdateMeter ExtraRow19Clear][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow19Clear FontColor "#TextColor#"][!UpdateMeter ExtraRow19Clear][!Redraw]

; --- Slot 20 ---
[ExtraHas20]
Measure=String
String=#Egame20#
DynamicVariables=1
RegExpSubstitute=1
Substitute="^\s*$":"1","^.+$":"0"
Group=ExtraHas

[ExtraRow20Name]
Meter=String
Text=#Egame20#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow20
Hidden=[ExtraHas20]
Y=2R
X=8
W=72
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 20][!SetOption InputExtraNameMeasure DefaultValue "#Egame20#"][!SetOption InputExtraNameMeasure Y "[ExtraRow20Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow20Path]
Meter=String
Text=#Egame20Path#
MeterStyle=NameStyle
Group=TabExtra | ExtraRow20
Hidden=[ExtraHas20]
Y=r
X=84
W=70
H=14
ClipString=1
SolidColor=#TextBG#
Padding=2,1,2,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 20][!SetOption InputExtraPathMeasure DefaultValue "#Egame20Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow20Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow20Browse]
Meter=String
Text="[\xe8b7]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow20
Hidden=[ExtraHas20]
Y=r
X=158
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickTarget Egame20Path][!SetVariable ExtraEditIndex 20][!CommandMeasure FileChooseFile "ChooseFile 1"]
MouseOverAction=[!SetOption ExtraRow20Browse FontColor "#SettingsHover#"][!UpdateMeter ExtraRow20Browse][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow20Browse FontColor "#TextColor#"][!UpdateMeter ExtraRow20Browse][!Redraw]

[ExtraRow20Vis]
Meter=String
Text="[\xe7b3]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow20
Hidden=[ExtraHas20]
Y=r
X=178
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 20][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]
MouseOverAction=[!SetOption ExtraRow20Vis FontColor "#SettingsHover#"][!UpdateMeter ExtraRow20Vis][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow20Vis FontColor "#TextColor#"][!UpdateMeter ExtraRow20Vis][!Redraw]

[ExtraRow20Icon]
Meter=String
Text="[\xe8b9]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow20
Hidden=[ExtraHas20]
Y=r
X=198
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable _PickSlot 20][!SetVariable _PickSlotPadded "020"][!SetVariable ExtraEditIndex 20][!CommandMeasure FileChooseImage "ChooseImage 1"]
MouseOverAction=[!SetOption ExtraRow20Icon FontColor "#SettingsHover#"][!UpdateMeter ExtraRow20Icon][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow20Icon FontColor "#TextColor#"][!UpdateMeter ExtraRow20Icon][!Redraw]

[ExtraRow20Clear]
Meter=String
Text="[\xe74d]"
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
AntiAlias=1
DynamicVariables=1
Group=TabExtra | ExtraRow20
Hidden=[ExtraHas20]
Y=r
X=218
SolidColor=#Dark2#,1
LeftMouseUpAction=[!SetVariable ExtraEditIndex 20][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]
MouseOverAction=[!SetOption ExtraRow20Clear FontColor "#SettingsHover#"][!UpdateMeter ExtraRow20Clear][!Redraw]
MouseLeaveAction=[!SetOption ExtraRow20Clear FontColor "#TextColor#"][!UpdateMeter ExtraRow20Clear][!Redraw]
