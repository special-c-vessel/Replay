#ifndef RECORD_ARRAY_H
#define RECORD_ARRAY_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "record_data.h"
#include "console_table.h"
#include "console_table_row.h"

using namespace std;

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
    RecordArray(std::vector<std::string>, std::vector<std::string>); // 차원을 전달받는 생성자
    ~RecordArray();
public: // 오버라이드 함수
    void InitRecordData(std::vector<std::string>) override;
    void UpdateRecordData(std::vector<std::string>) override;
    void PrintRecordData() override;
    std::string PrintRecordTable(std::string) override;
private: // 편의성 함수
    bool IsNumber(std::string const&);
    std::string AddHexaInt(std::string, int);

private:
    int currentPage = 0;
    int prevPage = 0;
    int shadowMaxIdx = 0;
    int arrayTypeSize = 0;

    std::map<std::string, std::string> shadowMemory;
    std::vector<int> dimension;
};

#endif //RECORD_ARRAY_H