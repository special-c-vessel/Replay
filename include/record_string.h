#ifndef RECORD_STRING_H
#define RECORD_STRING_H

#include <iostream>
#include <string>
#include <vector>

#include "record_data.h"
#include "console_table.h"

class RecordString : public RecordData {
public: // class constructor, destroyer
    RecordString();
    ~RecordString();

public: // override method from RecordData
    void InitRecordData(std::vector<std::string>) override;
    void UpdateRecordData(std::vector<std::string>) override;

public: // print method
    void PrintRecordData() override;
    std::string PrintRecordTable(std::string) override;

private:
     int stringLenth;
};

#endif //RECORD_STRING_H