#ifndef RECORD_STRUCT_H
#define RECORD_STRUCT_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "record_data.h"
#include "console_table.h"
#include "console_table_row.h"

class RecordStruct : public RecordData {
public: // 클래스 생성자, 소멸자
    RecordStruct(); // 일반 생성자
    RecordStruct(std::vector<std::string>);
    ~RecordStruct();
public: // 오버라이드 함수
    void InitRecordData(std::vector<std::string>) override;
    void UpdateRecordData(std::vector<std::string>) override;
    void PrintRecordData() override;
    std::string PrintRecordTable(std::string) override;

    void SetShadowMemory(std::map<std::string, std::string>) override;
    std::map<std::string, std::string> GetShadowMemory() override;

    void SetShadowMemorySize(int) override;
    int GetShadowMemorySize() override;

    void SetArrrays(std::vector<ArrayStruct>) override;
    std::vector<ArrayStruct> GetArrays() override;

    void SetStruct(RecordStruct&) override;
    void SetDataStruct(std::vector<DataStruct>) override;
    std::vector<DataStruct> GetDataStruct() override;

private:
    void PrintStructData();

public:
    std::map<std::string, std::string> shadowMemory;
    std::vector<DataStruct> dataStructs;
    int shadowMaxIdx = 0;

    std::string structName;
};

#endif //RECORD_STRUCT_H