#ifndef RECORD_DATA_H
#define RECORD_DATA_H
#include <string>
#include <vector>
#include <iostream>

class RecordData {
public: // class constructor, destructor
    RecordData();
    virtual ~RecordData();
public:
    enum RecordType {
        None,
        Array
    };
public: // variable getter setter
    void SetName(std::string);
    std::string GetName();
    void SetType(std::string);
    std::string GetType();
    void SetValue(std::string);
    std::string GetValue();
    void SetPtr(std::string);
    std::string GetPtr();
    void SetLine(std::string);
    std::string GetLine();
    void SetCol(std::string);
    std::string GetCol();
public: // 가상 함수
    virtual void InitRecordData(std::vector<std::string>);
    virtual void UpdateRecordData(std::vector<std::string>);
    virtual void PrintRecordData();
    virtual void PrintRecordTable(int);
public:
	std::string name;
	std::string type;
	std::string value;
	std::string ptr;
	std::string line;
	std::string col;

    RecordType recordType;
};

#endif //RECORD_DATA_H