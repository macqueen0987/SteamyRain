; Hidden tab — shared list UI (no second Close; Settings chrome owns window close).
; HiddenList meters use Group TabHidden + #HiddenListHidden#; Container clips dynamic rows.

@include30=#@#extraMeters\HiddenList.inc

[HiddenTabTitle]
Meter=String
Text="Hidden Games"
FontWeight=600
StringCase=Upper
FontSize=8
AntiAlias=1
FontColor=#TextColor#
SolidColor=#Dark2#,1
Group=TabHidden
Hidden=1
Y=36
X=8
