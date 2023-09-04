# 기록 파일 위치
RECORD_FILE_PATH=/Users/yongil/dev/c++/culry/Replay/tests/record.txt
# 소스 파일 위치
TARGET_FILE_PATH=/Users/yongil/dev/c++/culry/Replay/tests/test.cpp

make
./replay $TARGET_FILE_PATH $RECORD_FILE_PATH