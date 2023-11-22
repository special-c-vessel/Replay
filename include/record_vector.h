#ifndef RECORD_VECTOR_H
#define RECORD_VECTOR_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

#include "record_data.h"
#include "console_table.h"
#include "console_table_row.h"

using namespace std;

struct VectorStruct
{
    std::string vectorThreadId;
    std::string vectorFunc;
    std::string vectorName;
    std::string vectorType;
    std::string vectorValue;
    std::string vectorPtr;
    std::string vectorIndex;
    std::string vectorAccessType;
};

class RecordVector : public RecordData {
public: // 클래스 생성자, 소멸자
    RecordVector(); // 일반 생성자
    RecordVector(std::vector<std::string>);
    ~RecordVector();
public: // 오버라이드 함수
    void InitRecordData(std::vector<std::string>) override;
    void UpdateRecordData(std::vector<std::string>) override;
    void PrintRecordData() override;
    std::string PrintRecordTable(std::string) override;

    void SetShadowMemory(std::map<std::string, std::string>) override;
    std::map<std::string, std::string> GetShadowMemory() override;

    void SetShadowMemorySize(int) override;
    int GetShadowMemorySize() override;

    void SetVectors(std::vector<VectorStruct>) override;
    std::vector<VectorStruct> GetVectors() override;

    void SetStruct(RecordStruct&) override;

private: // 출력 함수
    void PrintShadowMemory();

private:
    int currentPage = 0;
    int prevPage = 0;
    int arrayTypeSize = 0;

    std::map<std::string, std::string> shadowMemory;
    int shadowMaxIdx = 0;

    std::vector<VectorStruct> vectors;

};

#endif //RECORD_VECTOR_H