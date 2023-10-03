#ifndef RECORD_DATA_H
#define RECORD_DATA_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>

class RecordStruct;

const std::string SPLIT_STR = "sddsdsdsd";
const char SPLIT_FUNC_NAME_CHAR = '-';
const std::string STRING_START = "StringStart";
const std::string STRING_END = "StringEnd";
const std::string CHECK_STRUCT = "isStruct";

#define ARRAY_TYPE_IDX 2

enum RecordType {
    None,
    Array,
    Prim,
    Struct
};

struct ArrayStruct
{
    std::string arrayFunc;
    std::string arrayName;
    std::string arrayType;
    std::string arrayValue;
    std::string arrayPtr;
    std::string arrayLine;
    std::string arrayCol;
    std::string arrayAccesType;
    std::vector<std::string> arrayIndex;
};

struct DataStruct
{
    std::string type;
    std::string value;
    std::string ptr;
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
    virtual void SetShadowMemory(std::map<std::string, std::string>);
    virtual std::map<std::string, std::string> GetShadowMemory();

    virtual void SetShadowMemorySize(int);
    virtual int GetShadowMemorySize();

    virtual void SetArrrays(std::vector<ArrayStruct>);
    virtual std::vector<ArrayStruct> GetArrays();

    virtual void SetStruct(RecordStruct&);
    virtual void SetDataStruct(std::vector<DataStruct>);
    virtual std::vector<DataStruct> GetDataStruct();

public:
    bool IsNumber(std::string const&);
    std::string AddHexaInt(std::string, int);
    std::string GetType(std::string&);
    int CountChar(std::string&, char);
    std::string RemoveChar(const std::string&, char);
    std::vector<std::string> SplitString(const std::string&, char);
    bool FindStringInString(std::string, std::string);
    int GetSizeByType(std::string);

public:
    std::string accessType;
	std::string name;
	std::string type;
	std::string value;
	std::string ptr;
	std::string line;
	std::string col;
    std::string length;
    std::string dataFunc;
    std::string originName;
    std::vector<std::string> originStr;

    RecordType recordType;

    int maxPageIndex;
    
    // 해당 기록파일의 특이 정보를 가지고 있는 문자열
    std::string infoMessage;
};

#endif //RECORD_DATA_H