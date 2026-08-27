# SteamyRain Settings UI — Large Panel Relayout

날짜: 2026-07-27  
상태: 설계 승인됨 (구현 계획 대기)  
선행: `2026-07-26-settings-ui-overhaul-design.md` (탭·Paths/Extra/Hidden·FileChoose·파이썬 미사용)

## 배경 / 문제

2026-07-26 오버홀로 탭·Paths/Extra/Hidden·FileChoose는 들어갔지만, UI는 여전히 **구형 225×245 미니 패널**과 **절대좌표**에 묶여 있다.

실제 증상:
- 탭 바와 Layout 컨트롤 겹침 / z-order 혼란
- Visible Games·토글·Gap 스테퍼 정렬 붕괴
- `ShowMeterGroup TabLayout`이 DEBUG 미터를 강제 표시 (수정됨)
- Extra/Paths를 담기에 창이 너무 작음 → “본격 설정 UI 역부족”

## 목표

Rainmeter만으로 **520×640 고정 설정 창**을 만들고, Content 영역은 **상대 레이아웃**만 사용한다. Extra/Hidden은 Content 안 **내부 스크롤**.

기능(bang, `!WriteKeyValue`, FileChoose, Extra 슬롯 규칙, HiddenList 공유, Scan 안내)은 유지하고 **뼈대·배치·크기**만 재구축한다.

## 비목표

- Python/외부 GUI 재도입
- Extra 슬롯 리넘버링
- 테마/애니메이션 전면 개편
- 메인 타일 스킨 레이아웃 변경

## 제약

- 순수 Rainmeter + 기존 FileChoose
- `GameDirs` 구분자는 **콤마** (`UpdateGames.pyw`와 일치)
- 개인 파일(`SkinInfo` / `GamesInfo` / `NonSteamGames` / `dynamicMeters` / EIcon·ELogo) 동기화 시 미덮어씀
- 기존 시각 언어 유지 (Dark1, TextBG, SettingsHover, Segoe Fluent Icons)

## 접근

**고정 셸 + Content 재레이아웃** 채택.

기각: 절대좌표만 밀기(재발), 탭별 별도 스킨(진입점 분산).

## 셸 구조·치수

| 영역 | 값 |
|------|-----|
| 창 | W=**520**, H=**640** 고정 |
| 헤더 | H=28 — 로고 + 닫기만 |
| 탭 바 | Y=28, H=32 — Layout / Paths / Extra / Hidden |
| Content | Y=**60** (`ContentTop`), H=**580** (`ContentH`), 좌우 Pad=**16** → 폭 **488** |
| 변수 | `WinW`, `WinH`, `ContentTop`, `ContentH`, `Pad`, `RowH`≈28, `SectionGap`≈12 |

- 배경 Shape: `WinW × WinH` 단일(또는 헤더+바디 2 shape), 더 이상 225 고정 아님
- 탭 전환: 기존 `ActiveTab` + Show/Hide meter groups
- Content 안 **절대 Y 금지**. 첫 컨트롤 `Y=#ContentTop#`(또는 탭 전용 앵커), 이후 `Y=(#RowH#)R` / `Y=2R`
- 슬라이더·토글·InputText·Browse는 같은 행 앵커에 맞춤

## Layout 탭

위에서 아래:

1. Tile Size — 라벨 | 상대 배치 슬라이더(행 폭 활용)
2. Visible Games — 라벨 | `− N +` 스테퍼
3. Display 소제목 — 2열: Nav Arrows | Fading Tiles / Header | Half Tiles (라벨+토글 동일 기준선)
4. Gap between tiles — Visible과 동일 스테퍼
5. Colors 소제목 — 행: 라벨 | reset | 값(InputText). Main BG / Input BG / Text / Text BG / Opacity

bang·SkinInfo 쓰기는 현행 유지.

## Paths 탭

스크롤 없음. 행: 라벨 | 값(잘림, 클릭 InputText) | Browse  
SteamPath, GameDirs(Append+Replace), RainMeterEXE, Locale(프리셋 english/koreana + 입력).  
`HasFileChooseFlag=0`이면 Browse만 숨김.

## Extra 탭

- 상단 고정: Add, ScanNeeded 배너+버튼
- 아래: Content 높이 잔여분 스크롤 리스트
- 행: 이름 | Path Browse | Vis | Icon | Clear
- 슬롯 1–20, 빈 슬롯 숨김, 삭제 시 리넘버 없음, `iter_extra_game_indices` 스킵 유지

## Hidden 탭

- 상단: Unhide All
- 아래: 공유 `HiddenList.inc`를 새 Content 폭·엔트리 높이에 맞게 조정한 스크롤 리스트
- QuickSettings → ActiveTab=4, `hiddenWindow` 클리어 / 단독 Hidden 비활성 — 유지

## 파일 영향

| 파일 | 작업 |
|------|------|
| `Settings/Settings.ini` | WinW/H, 헤더/탭/배경 Shape, Content 변수, TabSwitch (치수 반영) |
| `Settings/tabs/TabLayout.inc` | 상대 레이아웃으로 재작성 |
| `Settings/tabs/TabPaths.inc` | 상대 레이아웃으로 재작성 |
| `Settings/tabs/TabExtra.inc` | 고정 헤더 + 스크롤 리스트로 재배치 |
| `Settings/tabs/TabHidden.inc` | Content 호스트 치수 |
| `@Resources/extraMeters/HiddenList.inc` | 폭/EntrySize/컨테이너를 Content에 맞춤 |
| README | 창 크기·레이아웃 한 줄 갱신(선택) |

UpdateGames / FileChoose / ExtraHas 로직은 **동작 변경 없이** UI만 연결.

## 테스트

- 수동 Rainmeter: 4탭 전환, Layout 겹침 없음, Paths Browse/입력, Extra 스크롤·Add/Clear, Hidden unhide
- `pytest tests -v` 회귀 (Python 변경 없으면 현행 유지)
- 배포 동기화 시 개인 inc 미덮어씀

## 성공 기준

1. 탭 라벨이 콘텐츠에 가리지 않음
2. Layout 컨트롤이 서로 겹치지 않음
3. 창이 약 520×640으로 읽히며 Extra 목록이 Content 안에서 스크롤됨
4. 기존 설정 저장 경로·Scan 흐름이 그대로 동작
