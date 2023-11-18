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
const char SPLIT_DATA_CHAR = ' ';
const std::string STRING_START = "StringStart";
const std::string STRING_END = "StringEnd";
const std::string CHECK_STRUCT = "isStruct";
const std::string ERROR_STRING_RANGE = "Out of range request for the string";
const std::string ERROR_SEGMENTATION_FAULTS = "Expected location for segmentation faults error";

const int THREAD_ID_IDX = 0;
const int OP_TYPE_IDX = 1;
const int CURFUNC_NAME_IDX = 2;
const int TYPE_IDX = 3;
const int VALUE_IDX = 4;
const int PTR_IDX = 5;
const int LINE_IDX = 6;
const int COL_IDX = 7;
const int START_STRUCTDATA_VALUE_IDX = 4;
const int START_STRING_IDX = 6;
const int STRING_LENGTH_IDX = 5;

#define ARRAY_TYPE_IDX 2

enum RecordType {
    None,
    Vector,
    Array,
    Prim,
    Struct,
    String
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
    std::string threadId;

    RecordType recordType;

    int maxPageIndex;
    
    // 해당 기록파일의 특이 정보를 가지고 있는 문자열
    std::string infoMessage;

    // Error handlin variable
    bool isErrorRecData = false;
};

#endif //RECORD_DATA_H