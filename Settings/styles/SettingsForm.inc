;=================================================
; SettingsForm.inc — Settings form grid system
;=================================================
; Rainmeter has no Flex/Grid. This file is the contract:
;   full-width row bar | label text left | control right-aligned
;
; Requires shell vars already set: Pad, ContentW, ContentTop
;
; Let RowEnd = (#FormControlX#+#FormControlW#) — the row's right edge.
; Narrow controls hang off RowEnd so they sit at the row's right side;
; only wide controls (sliders) start at #FormControlX# and fill the column.
;
; USAGE (row index n = 0,1,2,...):
;   Y=(#FormRowY0#+n*#FormRowH#)
;   Label:  MeterStyle=FormLabel   (full-width bar, X/W from style)
;   Slider: X=#FormControlX#; W=#FormControlW#
;   Stepper: Less @ RowEnd-82, Text @ RowEnd-46 (W=28), More @ RowEnd-14
;   Color:   Reset @ RowEnd-122, Value @ RowEnd-100 (W=100)
;   Browse:  X=(RowEnd-24)
;   Toggle:  MeterStyle=FormToggle; X=(RowEnd-12); Y=(row)+8
;   Field:   MeterStyle=FormField (click-to-edit value box)
;   IconBtn: MeterStyle=FormIconBtn (glyph button, hover built-in)
;   Pill:    MeterStyle=FormPill (preset chip; pair with a String-measure
;            selection check that flips SolidColor/FontColor on match)
; Do NOT use OptionStyle Y=2R or HalfW for form rows.
;=================================================

[Variables]
FormGutter=12
FormLabelW=168
FormControlX=(#Pad#+#FormLabelW#+#FormGutter#)
FormControlW=(#ContentW#-#FormLabelW#-#FormGutter#)
FormRowH=32
FormRowY0=#ContentTop#

;_________________________________________________
;_________________FORM STYLES_____________________
; Row bar spans the full content width so the label (left) and its control
; (right-aligned at FormControlX+FormControlW) read as one row, not as a
; chip and a floating widget separated by dead space.
[FormLabel]
DynamicVariables=1
AntiAlias=1
FontSize=10
FontColor=#TextColor#
SolidColor=#InputBG#
Padding=8,6,8,6
W=#ContentW#
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

[FormField]
DynamicVariables=1
AntiAlias=1
FontSize=10
FontColor=#TextColor#,180
SolidColor=#TextBG#
Padding=4,1,4,1
H=14
ClipString=1
X=#FormControlX#

[FormIconBtn]
DynamicVariables=1
AntiAlias=1
FontFace=Segoe Fluent Icons
FontSize=11
FontColor=#TextColor#
SolidColor=#Dark2#,1
MouseActionCursor=0
MouseOverAction=[!SetOption #CURRENTSECTION# FontColor "#SettingsHover#"][!UpdateMeter #CURRENTSECTION#][!Redraw]
MouseLeaveAction=[!SetOption #CURRENTSECTION# FontColor "#TextColor#"][!UpdateMeter #CURRENTSECTION#][!Redraw]
X=#FormControlX#

[FormPill]
DynamicVariables=1
AntiAlias=1
FontSize=9
FontColor=#TextColor#
SolidColor=#TextBG#
Padding=4,2,4,2
X=#FormControlX#
