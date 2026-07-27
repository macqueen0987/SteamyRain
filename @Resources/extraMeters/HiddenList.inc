; Shared Hidden games list (measures, styles, scroll container, UnHide All).
; Used by Hidden/Hidden.ini (standalone host) and Settings/tabs/TabHidden.inc.
; NameStyle is HiddenNameStyle here so Settings' NameStyle is not overwritten.
; Host chrome (Header/Close/Logo/Title) and ModeCheck/MoveWindow/SettingCheck stay in Hidden.ini.
; Required host vars: HiddenListW, HiddenListH, EntrySize (or HiddenEntrySize), HiddenListX,
; HiddenListY, HiddenChromeY, HiddenListHidden, VisibleTilesS (~ HiddenListH/EntrySize).

;_________________________________________________
;____________________MEASURES_____________________
[HiddenGames]
Measure=Calc
Formula=((#GameCount#+#ExtraGamesCount#)-(#GameCountPLUS#+#ExtraGameCountPLUS#))
DynamicVariables=1
OnUpdateAction=[!UpdateMeterGroup Games][!UpdateMeasure Lenght][!UpdateMeasure Offset][!ReDraw]

[Lenght]
Measure=Calc
Formula=([HiddenGames]+0.25)*#EntrySize#
IfCondition=Lenght<(#EntrySize#*#VisibleTilesS#)
IfCondition2=[HiddenGames]=0
IfTrueAction=[!SetVariable Offset 0][!UpdateMeterGroup Games][!DisableMouseActionGroup "MouseScrollUpAction|MouseScrollDownAction" Container][!UpdatemeterGroup Container][!SetVariable Arrows 1][!SetOption ArrowMore Hidden 1][!ReDraw]
IfTrueAction2=[!ShowMeter NoGameText][!HideMeter UnHideAllButton][!ReDraw]
IfFalseAction=[!EnableMouseActionGroup "MouseScrollUpAction|MouseScrollDownAction" Container][!UpdatemeterGroup Container][!SetVariable Arrows 0][!ReDraw]
IfFalseAction2=[!HideMeter NoGameText][!ShowMeter UnHideAllButton][!ReDraw]
OnUpdateAction=[!UpdateMeasure FadeSize][!UpdateMeasure ContainerSize][!ReDraw]
IfConditionMode=1
DynamicVariables=1

[HideGame]
Measure=Calc
IfCondition=#Offset#>#EntrySize#
IfTrueAction=[!SetVariable Offset (#Offset#-#EntrySize#)][!UpdateMeterGroup Games]
IfConditionMode=1
DynamicVariables=1
Disabled=1

[ContainerSize]
Measure=Calc
Formula=#HiddenListH#
OnUpdateAction=[!UpdatemeterGroup Container][!UpdatemeterGroup HeaderContainer][!UpdatemeterGroup Container][!UpdateMeterGroup Arrows][!ReDraw]
DynamicVariables=1

[FadeSize]
Measure=Calc
Formula=(#FadeSizeS#/(([HiddenGames]+0.01)*#EntrySize#))
OnUpdateAction=[!UpdateMeter Container][!ReDraw]
DynamicVariables=1

[Offset]
Measure=Calc
IfCondition=#Offset#=0
IfCondition2=#Offset#=([Lenght]-#EntrySize#*#VisibleTilesS#)
IfCondition3=Lenght<(#EntrySize#*#VisibleTilesS#)
IfTrueAction=[!Setvariable UP 1][!Setvariable START 1][!SetOption ArrowLess Hidden 1]
IfTrueAction2=[!Setvariable DOWN 1][!SetOption ArrowMore Hidden 1]
IfTrueAction3=[!Setvariable fadeMode OFF]
IfFalseAction=[!Setvariable UP (0+[FadeSize])][!Setvariable START 0.0][!SetOption ArrowLess Hidden #Arrows#]
IfFalseAction2=[!Setvariable DOWN (1-[FadeSize])][!SetOption ArrowMore Hidden #Arrows#]
IfFalseAction3=[!Setvariable fadeMode ON]
OnUpdateAction=[!UpdateMeterGroup Arrows][!UpdateMeter Container][!ReDraw]
Group=MoveTimer
IfConditionMode=1
DynamicVariables=1

[MoveCalc]
Measure=Calc
Formula=((#EntrySize#*(Floor(#VisibleTilesS#)))/10)
Group=MoveTimer
DynamicVariables=1

[MoveTimer]
Measure=Plugin
Plugin=ActionTimer
ActionList1=Repeat MoveRight, 5, [MoveCalc]
ActionList2=Repeat MoveLeft, 5, [MoveCalc]
MoveRight=[!SetVariable Offset "(Clamp((#Offset#+10),0,([Lenght]-#EntrySize#*#VisibleTilesS#)))"][!UpdateMeterGroup Games][!UpdateMeasureGroup MoveTimer][!Redraw]
MoveLeft=[!SetVariable Offset "(Clamp((#Offset#-10),0,([Lenght]-#EntrySize#*#VisibleTilesS#)))"][!UpdateMeterGroup Games][!UpdateMeasureGroup MoveTimer][!Redraw]
Group=MoveTimer
DynamicVariables=1

[HiddenWindow]
Measure=Calc
; Standalone Hidden window OR Settings Hidden tab (ActiveTab=4; unset/0 in Hidden.ini)
IfCondition=(#hiddenWindow#=1) || (#ActiveTab#=4)
IfTrueAction=[!SetVariable #UpdateVar2# (([#[#UpdateVar2]]-1)+1) "SteamyRain" "#ActiveConfig#"][!SetVariable #UpdateVar# 0 "SteamyRain" "#ActiveConfig#"][!ShowMeterGroup #UpdateVar3# "SteamyRain" "#ActiveConfig#"][!Updatemeasure TilePixelsY "SteamyRain" "#ActiveConfig#"][!ReDraw "SteamyRain" "#ActiveConfig#"][!UpdateMeasure HiddenGames]
DynamicVariables=1
IfConditionMode=1
Disabled=1

[UnHideAllGames]
Measure=Loop
StartValue=0
EndValue=#GameCount#
Increment=1
LoopCount=1
OnUpdateAction=[!UpdateMeasure VisCheck]
Group=UnHideAll
DynamicVariables=1

[VisCheck]
Measure=Calc
IfCondition=[#Vis[&UnHideAllGames]]=1
IfTrueAction=[!WriteKeyValue Variables "Vis[UnHideAllGames]" 0 "#@#GamesInfo.inc"][!WriteKeyValue Variables GameCountPLUS "(Clamp((#GameCountPLUS#+1),0,#GameCount#))" "#@#GamesInfo.inc"][!SetVariable GameCountPLUS "(Clamp((#GameCountPLUS#+1),0,#GameCount#))"][!SetVariable Vis[UnHideAllGames] 0][!Updatemeasure Lenght][!HideMeterGroup G[UnHideAllGames]][!UpdateMeterGroup G[UnHideAllGames]][!ReDraw][!SetVariable UpdateVar "Vis[UnHideAllGames]"][!SetVariable UpdateVar2 "GameCountPLUS"][!SetVariable UpdateVar3 "G[UnHideAllGames]"][!Enablemeasure HiddenWindow][!Updatemeasure HiddenWindow][!UpdateMeasure HideCheck]
IfFalseAction=[!UpdateMeasure HideCheck]
IfConditionMode=1
DynamicVariables=1
Group=UnHideAll
Disabled=1

[HideCheck]
Measure=Calc
IfCondition=[UnHideAllGames]<>#GameCount#
IfTrueAction=[!UpdateMeasure UnHideAllGames]
IfFalseAction=[!CommandMeasure UnHideAllGames "Reset"]
IfCondition2=([UnHideAllExtraGames]=#ExtraGamesCount#) && ([UnHideAllGames]=#GameCount#)
IfTrueAction2=[!CommandMeasure UnHideAllGames "Reset"][!DisableMeasureGroup UnHideAll]
IfConditionMode=1
DynamicVariables=1
Group=UnHideAll
Disabled=1

[UnHideAllExtraGames]
Measure=Loop
StartValue=0
EndValue=#ExtraGamesCount#
Increment=1
LoopCount=1
OnUpdateAction=[!UpdateMeasure EVisCheck]
DynamicVariables=1
Group=UnHideAll

[EVisCheck]
Measure=Calc
IfCondition=[#Egame[&UnHideAllExtraGames]Vis]=1
IfTrueAction=[!WriteKeyValue Variables "Egame[UnHideAllExtraGames]Vis" 0 "#@#NonSteamGames.inc"][!WriteKeyValue Variables ExtraGameCountPLUS "(Clamp((#ExtraGameCountPLUS#+1),0,#ExtraGamesCount#))" "#@#NonSteamGames.inc"][!SetVariable ExtraGameCountPLUS "(Clamp((#ExtraGameCountPLUS#+1),0,#ExtraGamesCount#))"][!SetVariable Egame[UnHideAllExtraGames]Vis 0][!Updatemeasure Lenght][!HideMeterGroup EG[UnHideAllExtraGames]][!UpdateMeterGroup EG[UnHideAllExtraGames]][!ReDraw][!SetVariable UpdateVar "Egame[UnHideAllExtraGames]Vis"][!SetVariable UpdateVar2 "ExtraGameCountPLUS"][!SetVariable UpdateVar3 "EG[UnHideAllExtraGames]"][!Enablemeasure HiddenWindow][!Updatemeasure HiddenWindow][!UpdateMeasure ExtraHideCheck]
IfFalseAction=[!UpdateMeasure ExtraHideCheck]
DynamicVariables=1
IfConditionMode=1
Group=UnHideAll
Disabled=1

[ExtraHideCheck]
Measure=Calc
IfCondition=[UnHideAllExtraGames]<>#ExtraGamesCount#
IfTrueAction=[!UpdateMeasure UnHideAllExtraGames]
IfFalseAction=[!CommandMeasure UnHideAllExtraGames "Reset"]
IfCondition2=([UnHideAllExtraGames]=#ExtraGamesCount#) && ([UnHideAllGames]=#GameCount#)
IfTrueAction2=[!DisableMeasureGroup UnHideAll]
IfConditionMode=1
DynamicVariables=1
Group=UnHideAll
Disabled=1

;_________________________________________________
;__________________METER STYLES___________________
[GameStyle]
Container=Container
PreserveAspectRatio=1
DynamicVariables=1
Padding=0,0,0,2
AntiAlias=1
X=2
Y=-1r
W=18
H=18

; Hidden list row title style (not Settings NameStyle)
[HiddenNameStyle]
Container=Container
DynamicVariables=1
AntiAlias=1
FontSize=10
FontColor=#TextColor#
SolidColor=#TextBG#
Padding=25,0,40,0
ClipString=1
MouseOverAction=[!SetOption #CURRENTSECTION# SolidColor #Dark1#][!UpdateMeter #CURRENTSECTION#][!ReDraw]
MouseLeaveAction=[!SetOption #CURRENTSECTION# SolidColor #TextBG#][!UpdateMeter #CURRENTSECTION#][!ReDraw]
Y=([MoveNames:H]+2)r
W=(#HiddenListW#-65)

[VisStyle]
Text="[\xed1a]"
Container=Container
StringAlign=Right
Padding=0,2,0,0
FontColor=#TextColor#,100
MouseOverAction=[!SetOption #CURRENTSECTION# FontColor "#TextColor#"][!SetOption #CURRENTSECTION# Text "[\xe7b3]"][!UpdateMeter #CURRENTSECTION#][!ReDraw]
MouseLeaveAction=[!SetOption #CURRENTSECTION# FontColor "#TextColor#,100"][!SetOption #CURRENTSECTION# Text "[\xed1a]"][!UpdateMeter #CURRENTSECTION#][!ReDraw]
FontFace=Segoe Fluent Icons
MouseActionCursor=0
DynamicVariables=1
FontSize=12
AntiAlias=1
Y=-#EntrySize#R
X=(#HiddenListW#-5)

[HiddenButtonStyle]
SolidColor=#Dark2#,1
FontColor=#TextColor#
FontFace=Segoe Fluent Icons
MouseActionCursor=0
DynamicVariables=1
FontWeight=400
FontSize=12
AntiAlias=1

[ExtraStyle]
FontColor=#TextColor#
Padding=46,2,46,1
StringCase=Upper
StringAlign=Center
DynamicVariables=1
AntiAlias=1

;_________________________________________________
;_____________________METERS______________________
[Container]
Meter=Shape
Shape=Rectangle 0,0,#HiddenListW#,[ContainerSize] | Fill Color #TextColor# | StrokeWidth 0 | Fill LinearGradient #fadeMode# | Scale #Container#,#Container#,0,0
ON=-90 | #Dark2#,1 ; #START# | #Dark2# ; #UP# | #Dark2# ; #DOWN# | #Dark2#,1 ; 1.0
OFF=180 | #Dark2# ; #START# | #Dark2# ; #UP# | #Dark2# ; #DOWN# | #Dark2# ; 1.0
MouseScrollUpAction=[!SetVariable Offset (Clamp((#Offset#-#EntrySize#),0,([Lenght]-#EntrySize#*#VisibleTilesS#)))][!UpdateMeterGroup Games][!UpdateMeasure Offset][!HideMeterGroup QuickSettings][!ReDraw]
MouseScrollDownAction=[!SetVariable Offset (Clamp((#Offset#+#EntrySize#),0,([Lenght]-#EntrySize#*#VisibleTilesS#)))][!UpdateMeterGroup Games][!UpdateMeasure Offset][!HideMeterGroup QuickSettings][!ReDraw]
DynamicVariables=1
Group=Container | Scaling | Games | TabHidden
Hidden=#HiddenListHidden#
X=#HiddenListX#
Y=#HiddenListY#

;_________________________________________________
;_________________DYNAMIC METERS__________________
[MoveNames]
Meter=Image
Y=(#FirstGame#-#Offset#)
MeterStyle=GameStyle
Group=Games

@include20=#@#dynamicMeters\dynamicHiddenMeters.inc

;_________________________________________________
;__________________EXTRA METERS___________________
[ArrowLess]
Meter=String
Text="[\xe935]"
FontColor=#TextBG#
LeftMouseUpAction=[!CommandMeasure MoveTimer "Execute 2"][!UpdateMeter ArrowLess][!UpdateMeasureGroup MoveTimer][!ReDraw]
MouseOverAction=[!SetOption ArrowLess FontColor "#TextColor#"][!UpdateMeter ArrowLess][!ReDraw]
MouseLeaveAction=[!SetOption ArrowLess FontColor "#TextBG#"][!UpdateMeter ArrowLess][!ReDraw]
InlineSetting=Shadow | 0 | -1 | 2 | #Dark2#
MeterStyle=HiddenButtonStyle
Group=Incr | Mode | Arrows | TabHidden
Hidden=1
X=(#HiddenListX#+#HiddenListW#/2)
Y=(#HiddenListY#-5)

[ArrowMore]
Meter=String
Text="[\xe936]"
FontColor=#TextBG#
LeftMouseUpAction=[!CommandMeasure MoveTimer "Execute 1"][!UpdateMeter ArrowMore][!UpdateMeasureGroup MoveTimer][!ReDraw]
MouseOverAction=[!SetOption ArrowMore FontColor "#TextColor#"][!UpdateMeter ArrowMore][!ReDraw]
MouseLeaveAction=[!SetOption ArrowMore FontColor "#TextBG#"][!UpdateMeter ArrowMore][!ReDraw]
InlineSetting=Shadow | 0 | -1 | 2 | #Dark2#
MeterStyle=HiddenButtonStyle
Group=Incr | Mode | Arrows | TabHidden
Hidden=1
Y=([ContainerSize]+#HiddenListY#-10)
X=r

[UnHideAllButton]
Meter=String
Text="Unhide all"
SolidColor=#SettingsHover#
MeterStyle=ExtraStyle
Group=HeaderContainer | TabHidden
MouseActionCursor=0
LeftMouseUpAction=[!EnableMeasureGroup UnHideAll][!UpdateMeasure UnHideAllGames][!UpdateMeasure UnHideAllExtraGames]
MouseOverAction=[!Setoption UnHideAllButton SolidColor #TextBG#][!UpdateMeter UnHideAllButton][!ReDraw]
MouseLeaveAction=[!Setoption UnHideAllButton SolidColor #SettingsHover#][!UpdateMeter UnHideAllButton][!ReDraw]
Hidden=1
; Chrome row: Settings uses ContentTop; standalone Hidden.ini sets HiddenChromeY near list bottom
Y=#HiddenChromeY#
X=(#HiddenListX#+#HiddenListW#/2)

[NoGameText]
Meter=String
Text="No hidden games"
MeterStyle=ExtraStyle
SolidColor=#TextBG#
Group=TabHidden
Hidden=#HiddenListHidden#
Y=(#HiddenListY#+3)
X=(#HiddenListX#+#HiddenListW#/2)
