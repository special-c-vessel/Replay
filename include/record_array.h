#ifndef RECORD_ARRAY_H
#define RECORD_ARRAY_H

#include <iostream>
#include <string>
#include <vector>

#include "record_data.h"
#include "console_table.h"
#include "console_table_row.h"

struct ArrayStruct
{
    std::string arrayName;
    std::string arrayType;
    std::string arrayValue;
    std::string arrayPtr;
    std::vector<std::string> arrayIndex;
};

class RecordArray : public RecordData {
public: // 클래스 생성자, 소멸자
    RecordArray(); // 일반 생성자
    RecordArray(int); // 차원을 전달받는 생성자
    ~RecordArray();
private: // 동적 할당 메소드
    void InitArray();
public: // 오버라이드 함수
    void InitRecordData(std::vector<std::string>) override;
    void UpdateRecordData(std::vector<std::string>) override;
    void PrintRecordData() override;
    std::string PrintRecordTable(int) override;
private:
    int dimension = 0;
    int max_array1 = 50;
    int max_array2 = 20;
    int max_array3 = 20;
    std::vector<std::string> array1;
    std::vector<std::vector<std::string> > array2;
    std::vector<std::vector<std::vector<std::string> > > array3;
};

#endif //RECORD_ARRAY_H