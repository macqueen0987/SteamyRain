; Hidden tab — Unhide All chrome at ContentTop; list below (HiddenListY).
; HiddenList meters use Group TabHidden + #HiddenListHidden# (TabSwitch sets 0 on tab 4, 1 off-tab); Container clips dynamic rows.
; Sizing vars (HiddenListW/H, HiddenEntrySize, HiddenChromeY) live in Settings.ini [Variables].

; HiddenList.inc is included by Settings.ini (@include9), not from here — an
; @include at the top of this file is skipped by Rainmeter's parser.

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
