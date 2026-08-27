# SteamyRain Settings UI — 공통 디자인 시스템

날짜: 2026-07-27
상태: 설계 승인됨 (구현 계획 대기)
선행: `2026-07-26-settings-ui-overhaul-design.md`, `2026-07-27-settings-large-panel-relayout-design.md`

## 배경 / 문제

520×640 대형 패널 리레이아웃(`Settings/styles/SettingsForm.inc`)이 Layout 탭 위주로 들어갔지만, 다른 탭은 여전히 구식 스타일 체계에 머물러 있어 탭마다 컴포넌트의 생김새·간격 규칙이 다르다.

실제 증상 (스크린샷으로 확인):
- Layout 탭 체크박스의 on/off 채움색이 뒤바뀌어 있었음(수정 완료, 본 스펙에서 규칙으로 고정)
- Paths 탭: Locale 현재값 박스가 프리셋 칩과 다른 스타일이라 라벨 없이 붕 떠 보임, 콘텐츠가 상단에만 차고 하단이 크게 빔
- Extra 탭(`TabExtra.inc`, 20슬롯 × 6미터): 아이콘 버튼(Browse/Vis/Icon/Clear) 4종의 `FontFace/FontSize/FontColor/SolidColor` 등이 슬롯마다 그대로 복붙되어 총 80회 반복. `ButtonStyle`이 이미 있는데도 미사용
- Hidden 탭(`@Resources/extraMeters/HiddenList.inc`)은 `HiddenNameStyle`/`VisStyle`/`HiddenButtonStyle`이라는 별도 스타일 세트를 갖고 있어 Form* 계열과 시각적으로는 비슷하지만 코드상 분리되어 있음
- 죽은 변수: `Settings.ini`의 `RowH=28`, `SectionGap=12`는 어디서도 참조되지 않음(실사용은 `SettingsForm.inc`의 `FormRowH=32`). `FormSectionGap=8`도 정의만 있고 미적용 — 섹션 헤더(Display/Colors) 위아래 간격이 일반 행과 동일해 시각적 구분이 약함

## 목표

`Settings/styles/SettingsForm.inc`를 4개 탭(Layout/Paths/Extra/Hidden) 전부가 따르는 **단일 컴포넌트 체계**로 확장하고, 지금까지 발견된 탭별 불일치를 이 체계 안에서 해소한다.

## 비목표

- Header/TabStrip/Close/Logo(셸 크롬) 변경 — 이미 한 곳에서 공유돼 일관적이므로 범위 밖
- 색 토큰(`Dark1`/`Dark2`/`TextColor`/`TextBG`/`InputBG`/`SettingsHover`/`Opacity`) 자체 변경 — 재사용만 함
- 탭별 창 크기 가변화 (520×640 고정 유지)
- 테마/애니메이션 전면 개편
- Extra 슬롯 리넘버링, Python GUI 재도입

## 제약

- 순수 Rainmeter 문법(Meter/MeterStyle/Group/Container). 반복 슬롯(Extra 20개)은 Rainmeter에 루프가 없으므로 슬롯별 미터 자체는 유지하되, **공용 속성만 MeterStyle로 뺀다**.
- 기존 bang 로직(`!WriteKeyValue`, FileChoose, ExtraHas 규칙, HiddenList 공유)은 동작 변경 없음 — 스타일/배치만 손댐.
- `HiddenList.inc`는 Settings와 독립된 `Hidden/Hidden.ini`에서도 재사용되므로, 그쪽 호출부가 요구하는 변수(`HiddenListW`, `HiddenListH`, `EntrySize` 등)는 그대로 유지.

## 컴포넌트 인벤토리 (SettingsForm.inc 확장)

기존 유지:
- `FormLabel` — 행 라벨 (변경 없음)
- `FormSection` — 섹션 제목. **`FormSectionGap`을 Y 계산에 실제로 적용**해 헤더 위에 여백을 만듦
- `FormValue` — 인라인 짧은 텍스트/글리프
- `FormToggle` — 체크박스. **규칙 고정**: checked → `#TextColor#`(밝음, 눈에 띔), unchecked → `#Dark1#`(배경에 녹아듦). 이 방향을 반대로 바꾸지 않는다(Header 토글이 안 켜진 것처럼 보이던 버그의 재발 방지).

신규 추가:

| 스타일 | 용도 | 핵심 속성 | 대체 대상 |
|---|---|---|---|
| `FormField` | 클릭해서 편집하는 값 박스 | `SolidColor=#TextBG#`, `Padding=4,1,4,1`, `H=14`, `ClipString=1` | SteamPathValue/GameDirsValue/RainMeterEXEValue/LocaleValue, Extra Row Name/Path |
| `FormIconBtn` | Segoe Fluent Icons 글리프 버튼 | `SolidColor=#Dark2#,1`, `FontColor=#TextColor#`, hover→`#SettingsHover#`, leave→`#TextColor#` | Browse/Reset 아이콘들, Extra Row의 Browse/Vis/Icon/Clear, Hidden의 화살표·Unhide 버튼 |
| `FormPill` | 프리셋 선택 칩 | 선택됨: `SolidColor=#SettingsHover#` + `FontColor=#Dark1#` / 비선택: `SolidColor=#TextBG#` + `FontColor=#TextColor#,180` (현재 값과 비교하는 Calc measure로 상태 판정, `OptionsCheck` 패턴 재사용) | Locale english/koreana 칩 (현재는 선택 상태 표시 규칙이 코드에 없음 — 이번에 도입) |

리스트 행(Extra/Hidden)은 위 스타일을 그대로 쓰되 `Container=` 컨텍스트 안에서 밀도만 조절한다(`ExtraRowH`/`HiddenEntrySize`가 리스트 전용 행간).

## 마이그레이션 순서

1. `SettingsForm.inc`에 `FormField`/`FormIconBtn`/`FormPill` 추가, `FormSectionGap` 적용, `Settings.ini`의 죽은 `RowH`/`SectionGap` 변수 제거
2. **Layout 탭**: 색상 값 표시(`InputColorMainBG` 등)를 `FormField`로 교체, 섹션 간격 육안 확인
3. **Paths 탭**: 값 박스 4개 → `FormField`, Browse/Reset → `FormIconBtn`, Locale 프리셋 → `FormPill`
4. **Extra 탭**: 슬롯 1을 템플릿으로 `FormField`+`FormIconBtn` 적용 확정 → 슬롯 2~20에 동일 패턴 기계적 적용 (반복 속성 제거로 파일 크기 축소 기대)
5. **Hidden 탭** (`HiddenList.inc`): `HiddenNameStyle`→`FormField` 계열, `VisStyle`/`HiddenButtonStyle`→`FormIconBtn` 계열로 통합. standalone `Hidden.ini` 호출부가 깨지지 않는지 별도 확인

## Paths 탭 구체적 수정

- 빈 하단 공간: **의도된 여백으로 유지**. 상단 정렬 그대로, 필러 콘텐츠 추가하지 않음
- "koreana" 고아 박스(LocaleValue): `FormField`로 스타일 통일 → 다른 값 박스들과 톤이 같아져 "현재 로케일 값(직접 편집 가능)"으로 자연스럽게 읽힘. 별도 라벨 추가 없음(과설계 방지)
- "Repl" 버튼: 약어 텍스트 유지, `FormIconBtn`과 동일한 배경/패딩 규칙 적용해 다른 아이콘 버튼과 무게감 통일

## 테스트

- 수동: 4탭 전환 시 라벨/버튼 안 겹침, hover 상태 정상 동작, Layout 토글 on/off 색이 규칙대로 표시
- `pytest tests -v` 회귀 (Python 변경 없으므로 현행 유지 기대)
- 배포 동기화(`robocopy` → Documents\Rainmeter\Skins\SteamyRain) 후 실제 Rainmeter에서 확인

## 성공 기준

1. 4개 탭이 동일한 컴포넌트 스타일(FormField/FormIconBtn/FormPill/FormLabel/FormSection/FormToggle)만 사용, 탭 전용 임시 스타일 없음
2. Extra 탭 아이콘 버튼 속성 중복이 MeterStyle로 통합되어 슬롯당 코드량 감소
3. Paths 탭에서 라벨 없는 고아 박스처럼 보이는 요소 없음
4. 체크박스 on/off 색 규칙이 문서화되어 향후 회귀 방지
5. 기존 저장 경로·Scan 흐름·Extra/Hidden 로직 동작 변화 없음
