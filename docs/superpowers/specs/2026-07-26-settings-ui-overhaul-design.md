# SteamyRain 설정 UI 대폭 강화 — 설계 문서

날짜: 2026-07-26
상태: 설계 승인됨 (구현 계획 대기)

## 배경 / 문제

현재 SteamyRain에서 파일을 직접 편집해야만 바꿀 수 있는 항목이 많다.

- 경로/환경: `SteamPath`, `GameDirs`, `RainMeterEXE`, `Locale` (`@Resources/SkinInfo.inc`)
- Non-Steam 게임: 이름/경로/표시여부/아이콘 (`@Resources/NonSteamGames.inc` + `img/EIcon`, `img/ELogo`)
- 숨김 게임: 별도 `Hidden` 창은 있으나 설정 흐름과 분리됨

기존 `Settings.ini`는 타일 레이아웃과 색상만 UI로 제공한다.

## 목표

파일을 직접 편집하지 않고 **Rainmeter 스킨 UI 안에서** 다음을 모두 관리한다.

1. 경로/환경 값
2. Non-Steam 게임 추가/편집/삭제 (아이콘 포함)
3. 숨김 게임 표시/복원

## 비목표 (v1 범위 밖)

- Python 기반 별도 설정 GUI (사용자가 파이썬 배제를 요청함)
- Non-Steam 슬롯 삭제 시 뒤 슬롯을 앞으로 당기는 리넘버링 (빈 슬롯 유지 방식으로 대체)
- 게임 타일 meter 재생성 자체 (기존 `Scan for Games` / `UpdateGames.pyw` 담당 유지)
- Locale 자동 감지

## 제약 / 전제

- 구현은 순수 Rainmeter (bang, `InputText`, `!WriteKeyValue`) 중심.
- Rainmeter에는 공식 내장 파일 다이얼로그가 없다. 커뮤니티 플러그인 **FileChoose** (`ChooseFile` / `ChooseFolder` / `ChooseImage`, `GetIcon`)를 스킨에 포함해 사용한다.
- FileChoose가 없으면 Browse 버튼은 비활성화되고 `InputText` 수동 입력으로 대체 가능해야 한다(그레이스풀 디그레이드).
- 설정 UI는 파이썬을 직접 호출하지 않는다. Extra 게임 변경 후 목록 반영은 기존 `Scan for Games`로 안내.

## 접근 방식 (선택안)

**탭형 Settings + FileChoose** 를 채택한다.

기존 `Settings.ini`를 4개 탭으로 확장한다: **Layout / Paths / Extra / Hidden**.
기존 색·타일 UI는 Layout 탭으로 그대로 유지하고, 파일 편집이 필요하던 항목만 새 탭으로 추가한다.

기각안:
- 단일 화면에 전부 쌓기: 225px 폭에 컨트롤 과밀, 숨김 창과 중복.
- 설정마다 별도 스킨 분리: 진입점 분산, QuickSettings 연동 불편.

## 화면 구조

상단 탭 4개. `ActiveTab` 변수(1..4)로 meter 그룹 Show/Hide.

| 탭 | 내용 |
|---|---|
| Layout | 기존 Settings (타일 크기, Visible Games, Nav/Fade/Header/Half 토글, 색상 5종 + Opacity) |
| Paths | `SteamPath`, `GameDirs`(다중, 세미콜론), `RainMeterEXE`, `Locale` |
| Extra | Non-Steam 목록: 이름, Path(Browse), Vis 토글, 아이콘(ChooseImage/GetIcon), 추가/삭제 |
| Hidden | Steam/Extra 숨김·복원 (기존 Hidden 로직 재사용) |

- 창 크기: `DynamicWindowSize=1`. Layout은 현행 유지(~225×245), Paths/Extra/Hidden은 세로로 확장.
- Extra/Hidden은 스크롤 가능한 리스트 컨테이너(기존 Hidden/메인 타일 스크롤 패턴 재사용).

## 데이터 모델

### Paths (`@Resources/SkinInfo.inc`)
- 단일 변수 유지. `GameDirs`는 세미콜론 구분 다중 경로 형식 유지.
- UI: 라벨 + 현재 값(길면 잘림) + Browse(FileChoose) + 클릭 시 `InputText` 직접 수정.
- `GameDirs`는 Browse 시 "추가(append)" 및 "전체 교체" 옵션 제공.
- `Locale`은 프리셋 버튼(`english`, `koreana` 등) + 직접 입력.

### Extra (`@Resources/NonSteamGames.inc`)
- 슬롯 상한: 20 (`Egame1`…`Egame20`).
- 키: `EgameN`(이름), `EgameNPath`(실행 경로), `EgameNVis`(0=표시, 1=숨김).
- `ExtraGamesCount` = 사용 중 마지막 인덱스. `ExtraGameCountPLUS`는 기존 의미(표시 중 개수) 유지.
- 추가: 다음 빈 슬롯에 기록, count 갱신.
- 삭제: 해당 슬롯 값 비우고 count 재계산. 리넘버링 안 함(빈 슬롯 유지). UI는 빈 슬롯 숨김.
- 아이콘: FileChoose `GetIcon` → `@Resources/img/EIcon/00N.jpg`. 로고는 선택적 `ChooseImage` → `@Resources/img/ELogo/00N.jpg`.
- Extra 변경 후 목록 반영은 "Scan for Games 필요" 안내 + 버튼.

### Hidden (`@Resources/GamesInfo.inc`, `NonSteamGames.inc`)
- Steam 게임: `VisN` + `GameCountPLUS` 갱신 (기존 방식).
- Extra 게임: `EgameNVis` + `ExtraGameCountPLUS` 갱신.
- Settings Hidden 탭은 기존 `Hidden.ini`의 bang 로직을 `@include`로 공유하거나 재사용.

## 상호작용 / 데이터 흐름

1. 사용자가 탭 클릭 → `ActiveTab` 변경 → 해당 그룹만 표시.
2. 값 편집:
   - 텍스트/색상: `InputText` → `!WriteKeyValue` 대상 inc → `!SetVariable` → 필요 시 `!Refresh`/`!RefreshApp`.
   - 파일/폴더/이미지: FileChoose `Command N`에서 `$Path$`/`$Icon$` → `!WriteKeyValue` → 갱신.
   - 토글/증감: 기존 Settings 패턴(Calc measure + `!WriteKeyValue`) 유지.
3. Extra 추가/삭제 → inc 갱신 → "스캔 필요" 안내.

## 에러 처리 / 그레이스풀 디그레이드

- FileChoose 미탑재: Browse 비활성 표시, 수동 `InputText` 입력 유지.
- 잘못된 경로: 저장은 허용하되 존재 여부는 강제하지 않음(기존 동작과 일치). 필요 시 경고 텍스트.
- 빈 Extra 슬롯: UI에서 숨기고 스캔 시 스킵 여부를 `UpdateGames.pyw`와 정합 확인.
- 값 저장은 항상 즉시 `#Variable#` 반영 + 관련 스킨만 최소 Refresh.

## 배포

- `@Resources/Plugins/FileChoose.dll` 포함. `Plugin=FileChoose`(상대 경로)로 로드.
- README에 "FileChoose 포함/필요" 명시.
- 개인 파일 동기화 주의(DEVNOTES): `SkinInfo.inc`, `GamesInfo.inc`, `NonSteamGames.inc`, `img/*`, `dynamicMeters/*`는 배포 시 덮어쓰지 않음.

## 테스트 전략

- 수동: 각 탭 전환, 값 저장→inc 반영→UI 갱신 확인. FileChoose 유/무 두 경우.
- Extra 추가/삭제 후 `Scan for Games` 실행 → 타일 정상 생성.
- 기존 `tests/test_update_games.py`가 `NonSteamGames.inc` 파싱에 의존하면, 새 키/빈 슬롯 형식과 호환되는지 확인/보강.

## 열린 항목 (구현 계획에서 확정)

- FileChoose.dll 정확한 배치 경로 및 라이선스/재배포 조건 확인.
- Hidden 탭을 기존 `Hidden.ini` 흡수 vs 얇은 래퍼 유지 중 택1.
- 탭 전환을 단일 `Settings.ini` 내 그룹 토글 vs `@include` 분리 파일로 구성할지.
