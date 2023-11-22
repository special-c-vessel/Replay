# Culry
소스코드 수준 기록-재현 도구

## Replay
재현 파트

## 기존 디버깅 도구와 다른점

### 지원 목록
- 

### 실행 순서

1. git clone https://github.com/special-c-vessel/Replay.git
2. cd Replay
3. vi config.sh
4. 타겟 소스코드 파일의 위치와 기록파일의 로컬 위치를 기입 후 저장
5. ./config.sh

### 조작 방식

1. w : 상단 기록 데이터로 이동
2. s : 하단 기록 데이터로 이동

#### 일반 명령어 목록

1. findptr [address] : 지정한 포인터 주소값의 저장되어 있는 값을 출력 ex) findptr 0x25faddb2
2. findtype [type] : 찾고자 하는 타입의 기록 정보들을 출력 ex) findtype string
3. findname [name] : 찾고자 하는 이름의 기록 정보들을 출력 ex) findname num1
4. mvline [index] : 원하는 줄 번호로 이동 ex) mvline 150
5. threadset [threadId] : 원하는 쓰레드 아이디의 기록 정보들만을 출력 ex) threadset 1
6. opset [operation type] : 원하는 오퍼레이션 타입의 기록 정보들만을 출력 ex) opset load
7. nameset [name] : 원하는 이름의 기록 정보들만을 출력 ex) nameset num1

### 배열 명령어 목록
1. mvarray [index] : 배열의 지정한 인덱스의 값을 출력 ex) mvarray 10
2. d : 배열 인덱스 목록을 다음 페이지로 이동
3. a : 배열 인덱스 목록을 이전 페이ㅣㅈ로 이동

### 이전 데이터 테이블 명령어 목록
1. prevright : previous data table 다음 페이지
2. prevleft : previous data table 이전 페이지
3. prevmove [index] : 원하는 번호의 이전 데이터 페이지로 이동 ex) prevmove 26

### 이후 데이터 테이블 명령어 목록
1. followright : following data table 다음 페이지
2. followleft : previous data table 이전 페이지
3. followmove [index] : 원하는 번호의 이후 데이터 페이지로 이동 ex) followmove 26



