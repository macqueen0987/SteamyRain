;=================================================
; SettingsForm.inc — Settings form grid system
;=================================================
; Rainmeter has no Flex/Grid. This file is the contract:
;   label column | gutter | control column
;
; Requires shell vars already set: Pad, ContentW, ContentTop
;
; USAGE (row index n = 0,1,2,...):
;   Y=(#FormRowY0#+n*#FormRowH#)
;   Label:  MeterStyle=FormLabel   (X/W from style)
;   Control spine: X=#FormControlX#
;   Stepper: Less @ FormControlX+0, Text @ +36, More @ +68
;   Color:   Reset @ FormControlX, Value @ FormControlX+28
;   Browse:  X=(#FormControlX#+#FormControlW#-24)
;   Toggle:  MeterStyle=FormToggle; X=#FormControlX#; Y=(row)+8
; Do NOT use OptionStyle Y=2R or HalfW for form rows.
;=================================================

[Variables]
FormGutter=12
FormLabelW=168
FormControlX=(#Pad#+#FormLabelW#+#FormGutter#)
FormControlW=(#ContentW#-#FormLabelW#-#FormGutter#)
FormRowH=32
FormSectionGap=8
FormRowY0=#ContentTop#

;_________________________________________________
;_________________FORM STYLES_____________________
[FormLabel]
DynamicVariables=1
AntiAlias=1
FontSize=10
FontColor=#TextColor#
SolidColor=#TextBG#
Padding=8,6,8,6
W=#FormLabelW#
X=#Pad#

[FormSection]
DynamicVariables=1
AntiAlias=1
FontSize=10
FontWeight=600
FontColor=#TextColor#,200
SolidColor=#TextBG#
Padding=8,6,8,6
W=#ContentW#
X=#Pad#

[FormValue]
DynamicVariables=1
AntiAlias=1
FontSize=10
FontColor=#TextColor#,180
X=#FormControlX#

[FormToggle]
MouseOverAction=[!SetOption #CURRENTSECTION# Button "StrokeWidth 2 | Stroke Color #SettingsHover#"][!UpdateMeter #CURRENTSECTION#][!ReDraw]
MouseLeaveAction=[!SetOption #CURRENTSECTION# Button "StrokeWidth 2 | Stroke Color #Dark2#"][!UpdateMeter #CURRENTSECTION#][!ReDraw]
DynamicVariables=1
