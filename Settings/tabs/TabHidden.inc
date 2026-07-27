; Hidden tab — Unhide All chrome at ContentTop; list below (HiddenListY).
; HiddenList meters use Group TabHidden + #HiddenListHidden# (TabSwitch sets 0 on tab 4, 1 off-tab); Container clips dynamic rows.
; Sizing vars (HiddenListW/H, HiddenEntrySize, HiddenChromeY) live in Settings.ini [Variables].

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
Y=#ContentTop#
X=#Pad#
