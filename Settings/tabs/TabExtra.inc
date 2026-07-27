;;_________________________________________________
;_________________EXTRA TAB METERS________________
; Slots 1–20. Empty names Hidden via ExtraHasN (RegExp).
; ExtraGamesCount = max used index; delete does not renumber.
; Fixed chrome at ContentTop; list scrolls in ExtraListContainer.

[ExtraTitle]
Meter=String
Text="Non-Steam Games"
MeterStyle=OptionStyle
Group=TabExtra
Hidden=1
Y=#ContentTop#
X=#Pad#
Padding=0,0,0,0
SolidColor=0,0,0,1
W=160

[ExtraAddBtn]
Meter=String
Text="+ Add"
MeterStyle=NameStyle
Group=TabExtra
Hidden=1
Y=#ContentTop#
X=(#Pad#+180)
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
Y=(#ContentTop#+28)
X=#Pad#
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
X=(#Pad#+180)
SolidColor=#Dark2#,1
Padding=4,2,4,2
LeftMouseUpAction=[!WriteKeyValue Variables SkinPosX "#CURRENTCONFIGX#" "#@#SkinInfo.inc"][!WriteKeyValue Variables SkinPosY "#CURRENTCONFIGY#" "#@#SkinInfo.inc"]["#@#UpdateGames.pyw"][!SetVariable ScanNeeded 0][!UpdateMeasure ExtraTabRefresh][!Redraw]
MouseOverAction=[!SetOption ExtraScanBtn FontColor "#SettingsHover#"][!UpdateMeter ExtraScanBtn][!Redraw]
MouseLeaveAction=[!SetOption ExtraScanBtn FontColor "#TextColor#"][!UpdateMeter ExtraScanBtn][!Redraw]

; Scroll clip region below chrome (ExtraListTop / ExtraListH)
[ExtraListContainer]
Meter=Shape
X=#Pad#
Y=#ExtraListTop#
Shape=Rectangle 0,0,#ContentW#,#ExtraListH# | Fill Color #Dark1# | StrokeWidth 0
DynamicVariables=1
Group=TabExtra
Hidden=1
MouseScrollUpAction=[!SetVariable ExtraOffset "(Clamp((#ExtraOffset#-#ExtraRowH#),0,#ExtraScrollMax#))"][!UpdateMeterGroup TabExtra][!Redraw]
MouseScrollDownAction=[!SetVariable ExtraOffset "(Clamp((#ExtraOffset#+#ExtraRowH#),0,#ExtraScrollMax#))"][!UpdateMeterGroup TabExtra][!Redraw]

[ExtraIconHint]
Meter=String
Text="Icon -> #@#img\EIcon\00N.jpg"
MeterStyle=NameStyle
Group=TabExtra
Hidden=1
Container=ExtraListContainer
Y=(0-#ExtraOffset#)
X=0
W=(#ContentW#-8)
FontSize=8
FontColor=#TextColor#,120
ClipString=1
DynamicVariables=1

[ExtraListAnchor]
Meter=String
Text=""
Group=TabExtra
Hidden=1
Container=ExtraListContainer
Y=2R
X=0
H=0
W=0
DynamicVariables=1

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
MeterStyle=FormField
Group=TabExtra | ExtraRow1
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas1]
Y=(((1-1)*#ExtraRowH#+16)-#ExtraOffset#)
X=0
W=110
LeftMouseUpAction=[!SetVariable ExtraEditIndex 1][!SetOption InputExtraNameMeasure DefaultValue "#Egame1#"][!SetOption InputExtraNameMeasure Y "[ExtraRow1Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow1Path]
Meter=String
Text=#Egame1Path#
MeterStyle=FormField
Group=TabExtra | ExtraRow1
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas1]
Y=r
X=114
W=(#ContentW#-234)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 1][!SetOption InputExtraPathMeasure DefaultValue "#Egame1Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow1Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow1Browse]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b7]"
DynamicVariables=1
Group=TabExtra | ExtraRow1
Container=ExtraListContainer
Hidden=[ExtraHas1]
Y=r
X=(#ContentW#-112)
LeftMouseUpAction=[!SetVariable _PickTarget Egame1Path][!SetVariable ExtraEditIndex 1][!CommandMeasure FileChooseFile "ChooseFile 1"]

[ExtraRow1Vis]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe7b3]"
DynamicVariables=1
Group=TabExtra | ExtraRow1
Container=ExtraListContainer
Hidden=[ExtraHas1]
Y=r
X=(#ContentW#-84)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 1][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]

[ExtraRow1Icon]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b9]"
DynamicVariables=1
Group=TabExtra | ExtraRow1
Container=ExtraListContainer
Hidden=[ExtraHas1]
Y=r
X=(#ContentW#-56)
LeftMouseUpAction=[!SetVariable _PickSlot 1][!SetVariable _PickSlotPadded "001"][!SetVariable ExtraEditIndex 1][!CommandMeasure FileChooseImage "ChooseImage 1"]

[ExtraRow1Clear]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe74d]"
DynamicVariables=1
Group=TabExtra | ExtraRow1
Container=ExtraListContainer
Hidden=[ExtraHas1]
Y=r
X=(#ContentW#-28)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 1][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]

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
MeterStyle=FormField
Group=TabExtra | ExtraRow2
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas2]
Y=(((2-1)*#ExtraRowH#+16)-#ExtraOffset#)
X=0
W=110
LeftMouseUpAction=[!SetVariable ExtraEditIndex 2][!SetOption InputExtraNameMeasure DefaultValue "#Egame2#"][!SetOption InputExtraNameMeasure Y "[ExtraRow2Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow2Path]
Meter=String
Text=#Egame2Path#
MeterStyle=FormField
Group=TabExtra | ExtraRow2
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas2]
Y=r
X=114
W=(#ContentW#-234)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 2][!SetOption InputExtraPathMeasure DefaultValue "#Egame2Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow2Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow2Browse]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b7]"
DynamicVariables=1
Group=TabExtra | ExtraRow2
Container=ExtraListContainer
Hidden=[ExtraHas2]
Y=r
X=(#ContentW#-112)
LeftMouseUpAction=[!SetVariable _PickTarget Egame2Path][!SetVariable ExtraEditIndex 2][!CommandMeasure FileChooseFile "ChooseFile 1"]

[ExtraRow2Vis]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe7b3]"
DynamicVariables=1
Group=TabExtra | ExtraRow2
Container=ExtraListContainer
Hidden=[ExtraHas2]
Y=r
X=(#ContentW#-84)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 2][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]

[ExtraRow2Icon]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b9]"
DynamicVariables=1
Group=TabExtra | ExtraRow2
Container=ExtraListContainer
Hidden=[ExtraHas2]
Y=r
X=(#ContentW#-56)
LeftMouseUpAction=[!SetVariable _PickSlot 2][!SetVariable _PickSlotPadded "002"][!SetVariable ExtraEditIndex 2][!CommandMeasure FileChooseImage "ChooseImage 1"]

[ExtraRow2Clear]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe74d]"
DynamicVariables=1
Group=TabExtra | ExtraRow2
Container=ExtraListContainer
Hidden=[ExtraHas2]
Y=r
X=(#ContentW#-28)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 2][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]

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
MeterStyle=FormField
Group=TabExtra | ExtraRow3
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas3]
Y=(((3-1)*#ExtraRowH#+16)-#ExtraOffset#)
X=0
W=110
LeftMouseUpAction=[!SetVariable ExtraEditIndex 3][!SetOption InputExtraNameMeasure DefaultValue "#Egame3#"][!SetOption InputExtraNameMeasure Y "[ExtraRow3Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow3Path]
Meter=String
Text=#Egame3Path#
MeterStyle=FormField
Group=TabExtra | ExtraRow3
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas3]
Y=r
X=114
W=(#ContentW#-234)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 3][!SetOption InputExtraPathMeasure DefaultValue "#Egame3Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow3Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow3Browse]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b7]"
DynamicVariables=1
Group=TabExtra | ExtraRow3
Container=ExtraListContainer
Hidden=[ExtraHas3]
Y=r
X=(#ContentW#-112)
LeftMouseUpAction=[!SetVariable _PickTarget Egame3Path][!SetVariable ExtraEditIndex 3][!CommandMeasure FileChooseFile "ChooseFile 1"]

[ExtraRow3Vis]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe7b3]"
DynamicVariables=1
Group=TabExtra | ExtraRow3
Container=ExtraListContainer
Hidden=[ExtraHas3]
Y=r
X=(#ContentW#-84)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 3][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]

[ExtraRow3Icon]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b9]"
DynamicVariables=1
Group=TabExtra | ExtraRow3
Container=ExtraListContainer
Hidden=[ExtraHas3]
Y=r
X=(#ContentW#-56)
LeftMouseUpAction=[!SetVariable _PickSlot 3][!SetVariable _PickSlotPadded "003"][!SetVariable ExtraEditIndex 3][!CommandMeasure FileChooseImage "ChooseImage 1"]

[ExtraRow3Clear]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe74d]"
DynamicVariables=1
Group=TabExtra | ExtraRow3
Container=ExtraListContainer
Hidden=[ExtraHas3]
Y=r
X=(#ContentW#-28)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 3][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]

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
MeterStyle=FormField
Group=TabExtra | ExtraRow4
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas4]
Y=(((4-1)*#ExtraRowH#+16)-#ExtraOffset#)
X=0
W=110
LeftMouseUpAction=[!SetVariable ExtraEditIndex 4][!SetOption InputExtraNameMeasure DefaultValue "#Egame4#"][!SetOption InputExtraNameMeasure Y "[ExtraRow4Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow4Path]
Meter=String
Text=#Egame4Path#
MeterStyle=FormField
Group=TabExtra | ExtraRow4
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas4]
Y=r
X=114
W=(#ContentW#-234)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 4][!SetOption InputExtraPathMeasure DefaultValue "#Egame4Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow4Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow4Browse]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b7]"
DynamicVariables=1
Group=TabExtra | ExtraRow4
Container=ExtraListContainer
Hidden=[ExtraHas4]
Y=r
X=(#ContentW#-112)
LeftMouseUpAction=[!SetVariable _PickTarget Egame4Path][!SetVariable ExtraEditIndex 4][!CommandMeasure FileChooseFile "ChooseFile 1"]

[ExtraRow4Vis]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe7b3]"
DynamicVariables=1
Group=TabExtra | ExtraRow4
Container=ExtraListContainer
Hidden=[ExtraHas4]
Y=r
X=(#ContentW#-84)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 4][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]

[ExtraRow4Icon]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b9]"
DynamicVariables=1
Group=TabExtra | ExtraRow4
Container=ExtraListContainer
Hidden=[ExtraHas4]
Y=r
X=(#ContentW#-56)
LeftMouseUpAction=[!SetVariable _PickSlot 4][!SetVariable _PickSlotPadded "004"][!SetVariable ExtraEditIndex 4][!CommandMeasure FileChooseImage "ChooseImage 1"]

[ExtraRow4Clear]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe74d]"
DynamicVariables=1
Group=TabExtra | ExtraRow4
Container=ExtraListContainer
Hidden=[ExtraHas4]
Y=r
X=(#ContentW#-28)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 4][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]

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
MeterStyle=FormField
Group=TabExtra | ExtraRow5
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas5]
Y=(((5-1)*#ExtraRowH#+16)-#ExtraOffset#)
X=0
W=110
LeftMouseUpAction=[!SetVariable ExtraEditIndex 5][!SetOption InputExtraNameMeasure DefaultValue "#Egame5#"][!SetOption InputExtraNameMeasure Y "[ExtraRow5Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow5Path]
Meter=String
Text=#Egame5Path#
MeterStyle=FormField
Group=TabExtra | ExtraRow5
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas5]
Y=r
X=114
W=(#ContentW#-234)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 5][!SetOption InputExtraPathMeasure DefaultValue "#Egame5Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow5Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow5Browse]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b7]"
DynamicVariables=1
Group=TabExtra | ExtraRow5
Container=ExtraListContainer
Hidden=[ExtraHas5]
Y=r
X=(#ContentW#-112)
LeftMouseUpAction=[!SetVariable _PickTarget Egame5Path][!SetVariable ExtraEditIndex 5][!CommandMeasure FileChooseFile "ChooseFile 1"]

[ExtraRow5Vis]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe7b3]"
DynamicVariables=1
Group=TabExtra | ExtraRow5
Container=ExtraListContainer
Hidden=[ExtraHas5]
Y=r
X=(#ContentW#-84)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 5][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]

[ExtraRow5Icon]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b9]"
DynamicVariables=1
Group=TabExtra | ExtraRow5
Container=ExtraListContainer
Hidden=[ExtraHas5]
Y=r
X=(#ContentW#-56)
LeftMouseUpAction=[!SetVariable _PickSlot 5][!SetVariable _PickSlotPadded "005"][!SetVariable ExtraEditIndex 5][!CommandMeasure FileChooseImage "ChooseImage 1"]

[ExtraRow5Clear]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe74d]"
DynamicVariables=1
Group=TabExtra | ExtraRow5
Container=ExtraListContainer
Hidden=[ExtraHas5]
Y=r
X=(#ContentW#-28)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 5][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]

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
MeterStyle=FormField
Group=TabExtra | ExtraRow6
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas6]
Y=(((6-1)*#ExtraRowH#+16)-#ExtraOffset#)
X=0
W=110
LeftMouseUpAction=[!SetVariable ExtraEditIndex 6][!SetOption InputExtraNameMeasure DefaultValue "#Egame6#"][!SetOption InputExtraNameMeasure Y "[ExtraRow6Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow6Path]
Meter=String
Text=#Egame6Path#
MeterStyle=FormField
Group=TabExtra | ExtraRow6
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas6]
Y=r
X=114
W=(#ContentW#-234)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 6][!SetOption InputExtraPathMeasure DefaultValue "#Egame6Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow6Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow6Browse]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b7]"
DynamicVariables=1
Group=TabExtra | ExtraRow6
Container=ExtraListContainer
Hidden=[ExtraHas6]
Y=r
X=(#ContentW#-112)
LeftMouseUpAction=[!SetVariable _PickTarget Egame6Path][!SetVariable ExtraEditIndex 6][!CommandMeasure FileChooseFile "ChooseFile 1"]

[ExtraRow6Vis]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe7b3]"
DynamicVariables=1
Group=TabExtra | ExtraRow6
Container=ExtraListContainer
Hidden=[ExtraHas6]
Y=r
X=(#ContentW#-84)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 6][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]

[ExtraRow6Icon]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b9]"
DynamicVariables=1
Group=TabExtra | ExtraRow6
Container=ExtraListContainer
Hidden=[ExtraHas6]
Y=r
X=(#ContentW#-56)
LeftMouseUpAction=[!SetVariable _PickSlot 6][!SetVariable _PickSlotPadded "006"][!SetVariable ExtraEditIndex 6][!CommandMeasure FileChooseImage "ChooseImage 1"]

[ExtraRow6Clear]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe74d]"
DynamicVariables=1
Group=TabExtra | ExtraRow6
Container=ExtraListContainer
Hidden=[ExtraHas6]
Y=r
X=(#ContentW#-28)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 6][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]

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
MeterStyle=FormField
Group=TabExtra | ExtraRow7
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas7]
Y=(((7-1)*#ExtraRowH#+16)-#ExtraOffset#)
X=0
W=110
LeftMouseUpAction=[!SetVariable ExtraEditIndex 7][!SetOption InputExtraNameMeasure DefaultValue "#Egame7#"][!SetOption InputExtraNameMeasure Y "[ExtraRow7Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow7Path]
Meter=String
Text=#Egame7Path#
MeterStyle=FormField
Group=TabExtra | ExtraRow7
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas7]
Y=r
X=114
W=(#ContentW#-234)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 7][!SetOption InputExtraPathMeasure DefaultValue "#Egame7Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow7Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow7Browse]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b7]"
DynamicVariables=1
Group=TabExtra | ExtraRow7
Container=ExtraListContainer
Hidden=[ExtraHas7]
Y=r
X=(#ContentW#-112)
LeftMouseUpAction=[!SetVariable _PickTarget Egame7Path][!SetVariable ExtraEditIndex 7][!CommandMeasure FileChooseFile "ChooseFile 1"]

[ExtraRow7Vis]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe7b3]"
DynamicVariables=1
Group=TabExtra | ExtraRow7
Container=ExtraListContainer
Hidden=[ExtraHas7]
Y=r
X=(#ContentW#-84)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 7][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]

[ExtraRow7Icon]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b9]"
DynamicVariables=1
Group=TabExtra | ExtraRow7
Container=ExtraListContainer
Hidden=[ExtraHas7]
Y=r
X=(#ContentW#-56)
LeftMouseUpAction=[!SetVariable _PickSlot 7][!SetVariable _PickSlotPadded "007"][!SetVariable ExtraEditIndex 7][!CommandMeasure FileChooseImage "ChooseImage 1"]

[ExtraRow7Clear]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe74d]"
DynamicVariables=1
Group=TabExtra | ExtraRow7
Container=ExtraListContainer
Hidden=[ExtraHas7]
Y=r
X=(#ContentW#-28)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 7][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]

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
MeterStyle=FormField
Group=TabExtra | ExtraRow8
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas8]
Y=(((8-1)*#ExtraRowH#+16)-#ExtraOffset#)
X=0
W=110
LeftMouseUpAction=[!SetVariable ExtraEditIndex 8][!SetOption InputExtraNameMeasure DefaultValue "#Egame8#"][!SetOption InputExtraNameMeasure Y "[ExtraRow8Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow8Path]
Meter=String
Text=#Egame8Path#
MeterStyle=FormField
Group=TabExtra | ExtraRow8
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas8]
Y=r
X=114
W=(#ContentW#-234)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 8][!SetOption InputExtraPathMeasure DefaultValue "#Egame8Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow8Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow8Browse]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b7]"
DynamicVariables=1
Group=TabExtra | ExtraRow8
Container=ExtraListContainer
Hidden=[ExtraHas8]
Y=r
X=(#ContentW#-112)
LeftMouseUpAction=[!SetVariable _PickTarget Egame8Path][!SetVariable ExtraEditIndex 8][!CommandMeasure FileChooseFile "ChooseFile 1"]

[ExtraRow8Vis]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe7b3]"
DynamicVariables=1
Group=TabExtra | ExtraRow8
Container=ExtraListContainer
Hidden=[ExtraHas8]
Y=r
X=(#ContentW#-84)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 8][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]

[ExtraRow8Icon]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b9]"
DynamicVariables=1
Group=TabExtra | ExtraRow8
Container=ExtraListContainer
Hidden=[ExtraHas8]
Y=r
X=(#ContentW#-56)
LeftMouseUpAction=[!SetVariable _PickSlot 8][!SetVariable _PickSlotPadded "008"][!SetVariable ExtraEditIndex 8][!CommandMeasure FileChooseImage "ChooseImage 1"]

[ExtraRow8Clear]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe74d]"
DynamicVariables=1
Group=TabExtra | ExtraRow8
Container=ExtraListContainer
Hidden=[ExtraHas8]
Y=r
X=(#ContentW#-28)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 8][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]

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
MeterStyle=FormField
Group=TabExtra | ExtraRow9
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas9]
Y=(((9-1)*#ExtraRowH#+16)-#ExtraOffset#)
X=0
W=110
LeftMouseUpAction=[!SetVariable ExtraEditIndex 9][!SetOption InputExtraNameMeasure DefaultValue "#Egame9#"][!SetOption InputExtraNameMeasure Y "[ExtraRow9Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow9Path]
Meter=String
Text=#Egame9Path#
MeterStyle=FormField
Group=TabExtra | ExtraRow9
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas9]
Y=r
X=114
W=(#ContentW#-234)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 9][!SetOption InputExtraPathMeasure DefaultValue "#Egame9Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow9Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow9Browse]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b7]"
DynamicVariables=1
Group=TabExtra | ExtraRow9
Container=ExtraListContainer
Hidden=[ExtraHas9]
Y=r
X=(#ContentW#-112)
LeftMouseUpAction=[!SetVariable _PickTarget Egame9Path][!SetVariable ExtraEditIndex 9][!CommandMeasure FileChooseFile "ChooseFile 1"]

[ExtraRow9Vis]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe7b3]"
DynamicVariables=1
Group=TabExtra | ExtraRow9
Container=ExtraListContainer
Hidden=[ExtraHas9]
Y=r
X=(#ContentW#-84)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 9][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]

[ExtraRow9Icon]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b9]"
DynamicVariables=1
Group=TabExtra | ExtraRow9
Container=ExtraListContainer
Hidden=[ExtraHas9]
Y=r
X=(#ContentW#-56)
LeftMouseUpAction=[!SetVariable _PickSlot 9][!SetVariable _PickSlotPadded "009"][!SetVariable ExtraEditIndex 9][!CommandMeasure FileChooseImage "ChooseImage 1"]

[ExtraRow9Clear]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe74d]"
DynamicVariables=1
Group=TabExtra | ExtraRow9
Container=ExtraListContainer
Hidden=[ExtraHas9]
Y=r
X=(#ContentW#-28)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 9][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]

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
MeterStyle=FormField
Group=TabExtra | ExtraRow10
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas10]
Y=(((10-1)*#ExtraRowH#+16)-#ExtraOffset#)
X=0
W=110
LeftMouseUpAction=[!SetVariable ExtraEditIndex 10][!SetOption InputExtraNameMeasure DefaultValue "#Egame10#"][!SetOption InputExtraNameMeasure Y "[ExtraRow10Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow10Path]
Meter=String
Text=#Egame10Path#
MeterStyle=FormField
Group=TabExtra | ExtraRow10
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas10]
Y=r
X=114
W=(#ContentW#-234)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 10][!SetOption InputExtraPathMeasure DefaultValue "#Egame10Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow10Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow10Browse]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b7]"
DynamicVariables=1
Group=TabExtra | ExtraRow10
Container=ExtraListContainer
Hidden=[ExtraHas10]
Y=r
X=(#ContentW#-112)
LeftMouseUpAction=[!SetVariable _PickTarget Egame10Path][!SetVariable ExtraEditIndex 10][!CommandMeasure FileChooseFile "ChooseFile 1"]

[ExtraRow10Vis]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe7b3]"
DynamicVariables=1
Group=TabExtra | ExtraRow10
Container=ExtraListContainer
Hidden=[ExtraHas10]
Y=r
X=(#ContentW#-84)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 10][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]

[ExtraRow10Icon]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b9]"
DynamicVariables=1
Group=TabExtra | ExtraRow10
Container=ExtraListContainer
Hidden=[ExtraHas10]
Y=r
X=(#ContentW#-56)
LeftMouseUpAction=[!SetVariable _PickSlot 10][!SetVariable _PickSlotPadded "010"][!SetVariable ExtraEditIndex 10][!CommandMeasure FileChooseImage "ChooseImage 1"]

[ExtraRow10Clear]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe74d]"
DynamicVariables=1
Group=TabExtra | ExtraRow10
Container=ExtraListContainer
Hidden=[ExtraHas10]
Y=r
X=(#ContentW#-28)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 10][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]

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
MeterStyle=FormField
Group=TabExtra | ExtraRow11
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas11]
Y=(((11-1)*#ExtraRowH#+16)-#ExtraOffset#)
X=0
W=110
LeftMouseUpAction=[!SetVariable ExtraEditIndex 11][!SetOption InputExtraNameMeasure DefaultValue "#Egame11#"][!SetOption InputExtraNameMeasure Y "[ExtraRow11Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow11Path]
Meter=String
Text=#Egame11Path#
MeterStyle=FormField
Group=TabExtra | ExtraRow11
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas11]
Y=r
X=114
W=(#ContentW#-234)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 11][!SetOption InputExtraPathMeasure DefaultValue "#Egame11Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow11Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow11Browse]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b7]"
DynamicVariables=1
Group=TabExtra | ExtraRow11
Container=ExtraListContainer
Hidden=[ExtraHas11]
Y=r
X=(#ContentW#-112)
LeftMouseUpAction=[!SetVariable _PickTarget Egame11Path][!SetVariable ExtraEditIndex 11][!CommandMeasure FileChooseFile "ChooseFile 1"]

[ExtraRow11Vis]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe7b3]"
DynamicVariables=1
Group=TabExtra | ExtraRow11
Container=ExtraListContainer
Hidden=[ExtraHas11]
Y=r
X=(#ContentW#-84)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 11][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]

[ExtraRow11Icon]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b9]"
DynamicVariables=1
Group=TabExtra | ExtraRow11
Container=ExtraListContainer
Hidden=[ExtraHas11]
Y=r
X=(#ContentW#-56)
LeftMouseUpAction=[!SetVariable _PickSlot 11][!SetVariable _PickSlotPadded "011"][!SetVariable ExtraEditIndex 11][!CommandMeasure FileChooseImage "ChooseImage 1"]

[ExtraRow11Clear]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe74d]"
DynamicVariables=1
Group=TabExtra | ExtraRow11
Container=ExtraListContainer
Hidden=[ExtraHas11]
Y=r
X=(#ContentW#-28)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 11][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]

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
MeterStyle=FormField
Group=TabExtra | ExtraRow12
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas12]
Y=(((12-1)*#ExtraRowH#+16)-#ExtraOffset#)
X=0
W=110
LeftMouseUpAction=[!SetVariable ExtraEditIndex 12][!SetOption InputExtraNameMeasure DefaultValue "#Egame12#"][!SetOption InputExtraNameMeasure Y "[ExtraRow12Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow12Path]
Meter=String
Text=#Egame12Path#
MeterStyle=FormField
Group=TabExtra | ExtraRow12
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas12]
Y=r
X=114
W=(#ContentW#-234)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 12][!SetOption InputExtraPathMeasure DefaultValue "#Egame12Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow12Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow12Browse]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b7]"
DynamicVariables=1
Group=TabExtra | ExtraRow12
Container=ExtraListContainer
Hidden=[ExtraHas12]
Y=r
X=(#ContentW#-112)
LeftMouseUpAction=[!SetVariable _PickTarget Egame12Path][!SetVariable ExtraEditIndex 12][!CommandMeasure FileChooseFile "ChooseFile 1"]

[ExtraRow12Vis]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe7b3]"
DynamicVariables=1
Group=TabExtra | ExtraRow12
Container=ExtraListContainer
Hidden=[ExtraHas12]
Y=r
X=(#ContentW#-84)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 12][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]

[ExtraRow12Icon]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b9]"
DynamicVariables=1
Group=TabExtra | ExtraRow12
Container=ExtraListContainer
Hidden=[ExtraHas12]
Y=r
X=(#ContentW#-56)
LeftMouseUpAction=[!SetVariable _PickSlot 12][!SetVariable _PickSlotPadded "012"][!SetVariable ExtraEditIndex 12][!CommandMeasure FileChooseImage "ChooseImage 1"]

[ExtraRow12Clear]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe74d]"
DynamicVariables=1
Group=TabExtra | ExtraRow12
Container=ExtraListContainer
Hidden=[ExtraHas12]
Y=r
X=(#ContentW#-28)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 12][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]

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
MeterStyle=FormField
Group=TabExtra | ExtraRow13
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas13]
Y=(((13-1)*#ExtraRowH#+16)-#ExtraOffset#)
X=0
W=110
LeftMouseUpAction=[!SetVariable ExtraEditIndex 13][!SetOption InputExtraNameMeasure DefaultValue "#Egame13#"][!SetOption InputExtraNameMeasure Y "[ExtraRow13Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow13Path]
Meter=String
Text=#Egame13Path#
MeterStyle=FormField
Group=TabExtra | ExtraRow13
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas13]
Y=r
X=114
W=(#ContentW#-234)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 13][!SetOption InputExtraPathMeasure DefaultValue "#Egame13Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow13Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow13Browse]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b7]"
DynamicVariables=1
Group=TabExtra | ExtraRow13
Container=ExtraListContainer
Hidden=[ExtraHas13]
Y=r
X=(#ContentW#-112)
LeftMouseUpAction=[!SetVariable _PickTarget Egame13Path][!SetVariable ExtraEditIndex 13][!CommandMeasure FileChooseFile "ChooseFile 1"]

[ExtraRow13Vis]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe7b3]"
DynamicVariables=1
Group=TabExtra | ExtraRow13
Container=ExtraListContainer
Hidden=[ExtraHas13]
Y=r
X=(#ContentW#-84)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 13][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]

[ExtraRow13Icon]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b9]"
DynamicVariables=1
Group=TabExtra | ExtraRow13
Container=ExtraListContainer
Hidden=[ExtraHas13]
Y=r
X=(#ContentW#-56)
LeftMouseUpAction=[!SetVariable _PickSlot 13][!SetVariable _PickSlotPadded "013"][!SetVariable ExtraEditIndex 13][!CommandMeasure FileChooseImage "ChooseImage 1"]

[ExtraRow13Clear]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe74d]"
DynamicVariables=1
Group=TabExtra | ExtraRow13
Container=ExtraListContainer
Hidden=[ExtraHas13]
Y=r
X=(#ContentW#-28)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 13][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]

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
MeterStyle=FormField
Group=TabExtra | ExtraRow14
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas14]
Y=(((14-1)*#ExtraRowH#+16)-#ExtraOffset#)
X=0
W=110
LeftMouseUpAction=[!SetVariable ExtraEditIndex 14][!SetOption InputExtraNameMeasure DefaultValue "#Egame14#"][!SetOption InputExtraNameMeasure Y "[ExtraRow14Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow14Path]
Meter=String
Text=#Egame14Path#
MeterStyle=FormField
Group=TabExtra | ExtraRow14
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas14]
Y=r
X=114
W=(#ContentW#-234)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 14][!SetOption InputExtraPathMeasure DefaultValue "#Egame14Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow14Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow14Browse]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b7]"
DynamicVariables=1
Group=TabExtra | ExtraRow14
Container=ExtraListContainer
Hidden=[ExtraHas14]
Y=r
X=(#ContentW#-112)
LeftMouseUpAction=[!SetVariable _PickTarget Egame14Path][!SetVariable ExtraEditIndex 14][!CommandMeasure FileChooseFile "ChooseFile 1"]

[ExtraRow14Vis]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe7b3]"
DynamicVariables=1
Group=TabExtra | ExtraRow14
Container=ExtraListContainer
Hidden=[ExtraHas14]
Y=r
X=(#ContentW#-84)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 14][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]

[ExtraRow14Icon]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b9]"
DynamicVariables=1
Group=TabExtra | ExtraRow14
Container=ExtraListContainer
Hidden=[ExtraHas14]
Y=r
X=(#ContentW#-56)
LeftMouseUpAction=[!SetVariable _PickSlot 14][!SetVariable _PickSlotPadded "014"][!SetVariable ExtraEditIndex 14][!CommandMeasure FileChooseImage "ChooseImage 1"]

[ExtraRow14Clear]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe74d]"
DynamicVariables=1
Group=TabExtra | ExtraRow14
Container=ExtraListContainer
Hidden=[ExtraHas14]
Y=r
X=(#ContentW#-28)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 14][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]

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
MeterStyle=FormField
Group=TabExtra | ExtraRow15
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas15]
Y=(((15-1)*#ExtraRowH#+16)-#ExtraOffset#)
X=0
W=110
LeftMouseUpAction=[!SetVariable ExtraEditIndex 15][!SetOption InputExtraNameMeasure DefaultValue "#Egame15#"][!SetOption InputExtraNameMeasure Y "[ExtraRow15Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow15Path]
Meter=String
Text=#Egame15Path#
MeterStyle=FormField
Group=TabExtra | ExtraRow15
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas15]
Y=r
X=114
W=(#ContentW#-234)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 15][!SetOption InputExtraPathMeasure DefaultValue "#Egame15Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow15Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow15Browse]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b7]"
DynamicVariables=1
Group=TabExtra | ExtraRow15
Container=ExtraListContainer
Hidden=[ExtraHas15]
Y=r
X=(#ContentW#-112)
LeftMouseUpAction=[!SetVariable _PickTarget Egame15Path][!SetVariable ExtraEditIndex 15][!CommandMeasure FileChooseFile "ChooseFile 1"]

[ExtraRow15Vis]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe7b3]"
DynamicVariables=1
Group=TabExtra | ExtraRow15
Container=ExtraListContainer
Hidden=[ExtraHas15]
Y=r
X=(#ContentW#-84)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 15][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]

[ExtraRow15Icon]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b9]"
DynamicVariables=1
Group=TabExtra | ExtraRow15
Container=ExtraListContainer
Hidden=[ExtraHas15]
Y=r
X=(#ContentW#-56)
LeftMouseUpAction=[!SetVariable _PickSlot 15][!SetVariable _PickSlotPadded "015"][!SetVariable ExtraEditIndex 15][!CommandMeasure FileChooseImage "ChooseImage 1"]

[ExtraRow15Clear]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe74d]"
DynamicVariables=1
Group=TabExtra | ExtraRow15
Container=ExtraListContainer
Hidden=[ExtraHas15]
Y=r
X=(#ContentW#-28)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 15][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]

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
MeterStyle=FormField
Group=TabExtra | ExtraRow16
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas16]
Y=(((16-1)*#ExtraRowH#+16)-#ExtraOffset#)
X=0
W=110
LeftMouseUpAction=[!SetVariable ExtraEditIndex 16][!SetOption InputExtraNameMeasure DefaultValue "#Egame16#"][!SetOption InputExtraNameMeasure Y "[ExtraRow16Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow16Path]
Meter=String
Text=#Egame16Path#
MeterStyle=FormField
Group=TabExtra | ExtraRow16
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas16]
Y=r
X=114
W=(#ContentW#-234)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 16][!SetOption InputExtraPathMeasure DefaultValue "#Egame16Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow16Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow16Browse]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b7]"
DynamicVariables=1
Group=TabExtra | ExtraRow16
Container=ExtraListContainer
Hidden=[ExtraHas16]
Y=r
X=(#ContentW#-112)
LeftMouseUpAction=[!SetVariable _PickTarget Egame16Path][!SetVariable ExtraEditIndex 16][!CommandMeasure FileChooseFile "ChooseFile 1"]

[ExtraRow16Vis]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe7b3]"
DynamicVariables=1
Group=TabExtra | ExtraRow16
Container=ExtraListContainer
Hidden=[ExtraHas16]
Y=r
X=(#ContentW#-84)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 16][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]

[ExtraRow16Icon]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b9]"
DynamicVariables=1
Group=TabExtra | ExtraRow16
Container=ExtraListContainer
Hidden=[ExtraHas16]
Y=r
X=(#ContentW#-56)
LeftMouseUpAction=[!SetVariable _PickSlot 16][!SetVariable _PickSlotPadded "016"][!SetVariable ExtraEditIndex 16][!CommandMeasure FileChooseImage "ChooseImage 1"]

[ExtraRow16Clear]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe74d]"
DynamicVariables=1
Group=TabExtra | ExtraRow16
Container=ExtraListContainer
Hidden=[ExtraHas16]
Y=r
X=(#ContentW#-28)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 16][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]

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
MeterStyle=FormField
Group=TabExtra | ExtraRow17
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas17]
Y=(((17-1)*#ExtraRowH#+16)-#ExtraOffset#)
X=0
W=110
LeftMouseUpAction=[!SetVariable ExtraEditIndex 17][!SetOption InputExtraNameMeasure DefaultValue "#Egame17#"][!SetOption InputExtraNameMeasure Y "[ExtraRow17Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow17Path]
Meter=String
Text=#Egame17Path#
MeterStyle=FormField
Group=TabExtra | ExtraRow17
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas17]
Y=r
X=114
W=(#ContentW#-234)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 17][!SetOption InputExtraPathMeasure DefaultValue "#Egame17Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow17Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow17Browse]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b7]"
DynamicVariables=1
Group=TabExtra | ExtraRow17
Container=ExtraListContainer
Hidden=[ExtraHas17]
Y=r
X=(#ContentW#-112)
LeftMouseUpAction=[!SetVariable _PickTarget Egame17Path][!SetVariable ExtraEditIndex 17][!CommandMeasure FileChooseFile "ChooseFile 1"]

[ExtraRow17Vis]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe7b3]"
DynamicVariables=1
Group=TabExtra | ExtraRow17
Container=ExtraListContainer
Hidden=[ExtraHas17]
Y=r
X=(#ContentW#-84)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 17][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]

[ExtraRow17Icon]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b9]"
DynamicVariables=1
Group=TabExtra | ExtraRow17
Container=ExtraListContainer
Hidden=[ExtraHas17]
Y=r
X=(#ContentW#-56)
LeftMouseUpAction=[!SetVariable _PickSlot 17][!SetVariable _PickSlotPadded "017"][!SetVariable ExtraEditIndex 17][!CommandMeasure FileChooseImage "ChooseImage 1"]

[ExtraRow17Clear]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe74d]"
DynamicVariables=1
Group=TabExtra | ExtraRow17
Container=ExtraListContainer
Hidden=[ExtraHas17]
Y=r
X=(#ContentW#-28)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 17][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]

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
MeterStyle=FormField
Group=TabExtra | ExtraRow18
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas18]
Y=(((18-1)*#ExtraRowH#+16)-#ExtraOffset#)
X=0
W=110
LeftMouseUpAction=[!SetVariable ExtraEditIndex 18][!SetOption InputExtraNameMeasure DefaultValue "#Egame18#"][!SetOption InputExtraNameMeasure Y "[ExtraRow18Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow18Path]
Meter=String
Text=#Egame18Path#
MeterStyle=FormField
Group=TabExtra | ExtraRow18
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas18]
Y=r
X=114
W=(#ContentW#-234)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 18][!SetOption InputExtraPathMeasure DefaultValue "#Egame18Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow18Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow18Browse]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b7]"
DynamicVariables=1
Group=TabExtra | ExtraRow18
Container=ExtraListContainer
Hidden=[ExtraHas18]
Y=r
X=(#ContentW#-112)
LeftMouseUpAction=[!SetVariable _PickTarget Egame18Path][!SetVariable ExtraEditIndex 18][!CommandMeasure FileChooseFile "ChooseFile 1"]

[ExtraRow18Vis]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe7b3]"
DynamicVariables=1
Group=TabExtra | ExtraRow18
Container=ExtraListContainer
Hidden=[ExtraHas18]
Y=r
X=(#ContentW#-84)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 18][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]

[ExtraRow18Icon]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b9]"
DynamicVariables=1
Group=TabExtra | ExtraRow18
Container=ExtraListContainer
Hidden=[ExtraHas18]
Y=r
X=(#ContentW#-56)
LeftMouseUpAction=[!SetVariable _PickSlot 18][!SetVariable _PickSlotPadded "018"][!SetVariable ExtraEditIndex 18][!CommandMeasure FileChooseImage "ChooseImage 1"]

[ExtraRow18Clear]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe74d]"
DynamicVariables=1
Group=TabExtra | ExtraRow18
Container=ExtraListContainer
Hidden=[ExtraHas18]
Y=r
X=(#ContentW#-28)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 18][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]

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
MeterStyle=FormField
Group=TabExtra | ExtraRow19
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas19]
Y=(((19-1)*#ExtraRowH#+16)-#ExtraOffset#)
X=0
W=110
LeftMouseUpAction=[!SetVariable ExtraEditIndex 19][!SetOption InputExtraNameMeasure DefaultValue "#Egame19#"][!SetOption InputExtraNameMeasure Y "[ExtraRow19Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow19Path]
Meter=String
Text=#Egame19Path#
MeterStyle=FormField
Group=TabExtra | ExtraRow19
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas19]
Y=r
X=114
W=(#ContentW#-234)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 19][!SetOption InputExtraPathMeasure DefaultValue "#Egame19Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow19Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow19Browse]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b7]"
DynamicVariables=1
Group=TabExtra | ExtraRow19
Container=ExtraListContainer
Hidden=[ExtraHas19]
Y=r
X=(#ContentW#-112)
LeftMouseUpAction=[!SetVariable _PickTarget Egame19Path][!SetVariable ExtraEditIndex 19][!CommandMeasure FileChooseFile "ChooseFile 1"]

[ExtraRow19Vis]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe7b3]"
DynamicVariables=1
Group=TabExtra | ExtraRow19
Container=ExtraListContainer
Hidden=[ExtraHas19]
Y=r
X=(#ContentW#-84)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 19][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]

[ExtraRow19Icon]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b9]"
DynamicVariables=1
Group=TabExtra | ExtraRow19
Container=ExtraListContainer
Hidden=[ExtraHas19]
Y=r
X=(#ContentW#-56)
LeftMouseUpAction=[!SetVariable _PickSlot 19][!SetVariable _PickSlotPadded "019"][!SetVariable ExtraEditIndex 19][!CommandMeasure FileChooseImage "ChooseImage 1"]

[ExtraRow19Clear]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe74d]"
DynamicVariables=1
Group=TabExtra | ExtraRow19
Container=ExtraListContainer
Hidden=[ExtraHas19]
Y=r
X=(#ContentW#-28)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 19][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]

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
MeterStyle=FormField
Group=TabExtra | ExtraRow20
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas20]
Y=(((20-1)*#ExtraRowH#+16)-#ExtraOffset#)
X=0
W=110
LeftMouseUpAction=[!SetVariable ExtraEditIndex 20][!SetOption InputExtraNameMeasure DefaultValue "#Egame20#"][!SetOption InputExtraNameMeasure Y "[ExtraRow20Name:Y]"][!CommandMeasure InputExtraNameMeasure "ExecuteBatch 1"]

[ExtraRow20Path]
Meter=String
Text=#Egame20Path#
MeterStyle=FormField
Group=TabExtra | ExtraRow20
Container=ExtraListContainer
DynamicVariables=1
Hidden=[ExtraHas20]
Y=r
X=114
W=(#ContentW#-234)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 20][!SetOption InputExtraPathMeasure DefaultValue "#Egame20Path#"][!SetOption InputExtraPathMeasure Y "[ExtraRow20Path:Y]"][!CommandMeasure InputExtraPathMeasure "ExecuteBatch 1"]

[ExtraRow20Browse]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b7]"
DynamicVariables=1
Group=TabExtra | ExtraRow20
Container=ExtraListContainer
Hidden=[ExtraHas20]
Y=r
X=(#ContentW#-112)
LeftMouseUpAction=[!SetVariable _PickTarget Egame20Path][!SetVariable ExtraEditIndex 20][!CommandMeasure FileChooseFile "ChooseFile 1"]

[ExtraRow20Vis]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe7b3]"
DynamicVariables=1
Group=TabExtra | ExtraRow20
Container=ExtraListContainer
Hidden=[ExtraHas20]
Y=r
X=(#ContentW#-84)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 20][!EnableMeasure ExtraToggleVis][!UpdateMeasure ExtraToggleVis]

[ExtraRow20Icon]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe8b9]"
DynamicVariables=1
Group=TabExtra | ExtraRow20
Container=ExtraListContainer
Hidden=[ExtraHas20]
Y=r
X=(#ContentW#-56)
LeftMouseUpAction=[!SetVariable _PickSlot 20][!SetVariable _PickSlotPadded "020"][!SetVariable ExtraEditIndex 20][!CommandMeasure FileChooseImage "ChooseImage 1"]

[ExtraRow20Clear]
Meter=String
MeterStyle=FormIconBtn
Text="[\xe74d]"
DynamicVariables=1
Group=TabExtra | ExtraRow20
Container=ExtraListContainer
Hidden=[ExtraHas20]
Y=r
X=(#ContentW#-28)
LeftMouseUpAction=[!SetVariable ExtraEditIndex 20][!EnableMeasure ExtraClearSlot][!UpdateMeasure ExtraClearSlot]
