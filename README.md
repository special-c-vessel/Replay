# Culry
소스코드 수준 기록-재현 도구

## Replay
재현 파트

### 실행 순서

1. git clone https://github.com/special-c-vessel/Replay.git
2. cd Replay
3. vi config.sh
4. 타겟 소스코드 파일의 위치와 기록파일의 로컬 위치를 기입 후 저장
5. ./config.sh

### 조작 방식

w - 상단 기록 파일로 이동
ww - 상단 코드로 이동
s - 하단 기록 파일로 이동
ss - 하단 코드로 이동
a - 이전 페이지
d - 다음 페이지

#### 명령어 목록

1. findptr [pointer address] : 지정한 포인터 주소값의 저장되어 있는 값을 출력 ex) findptr 0x25faddb2
2. mvarray [index] : 배열의 지정한 인덱스의 값을 출력 ex) mvarray 10


