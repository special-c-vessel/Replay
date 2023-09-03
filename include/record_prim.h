#ifndef RECORD_PRIM_H
#define RECORD_PRIM_H

#include <iostream>
#include <string>
#include <vector>

#include "record_data.h"
#include "console_table.h"
#include "console_table_row.h"

#define STRINGEND_IDX 3

class RecordPrim : public RecordData {
public: // 클래스 생성자, 소멸자
    RecordPrim(); // 일반 생성자
    ~RecordPrim();
public: // 오버라이드 함수
    void InitRecordData(std::vector<std::string>) override;
    void UpdateRecordData(std::vector<std::string>) override;
    void PrintRecordData() override;
    std::string PrintRecordTable(std::string) override;
};

#endif //RECORD_PRIM_H