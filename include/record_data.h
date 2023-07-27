#ifndef RECORD_DATA_H
#define RECORD_DATA_H
#include <string>
#include <vector>
#include <iostream>

enum RecordType {
    None,
    Array1,
    Array2,
    Array3,
    Prim
};

class RecordData {
public: // class constructor, destructor
    RecordData();
    virtual ~RecordData();
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
    virtual std::string PrintRecordTable(std::string);
public:
	std::string name;
	std::string type;
	std::string value;
	std::string ptr;
	std::string line;
	std::string col;
    std::string length;

    RecordType recordType;

    int maxPageIndex;
    
    // 해당 기록파일의 특이 정보를 가지고 있는 문자열
    std::string infoMessage;
};

#endif //RECORD_DATA_H