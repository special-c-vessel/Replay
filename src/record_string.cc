#include "record_string.h"

/* class constructor, destructor===================================================================================================== */

RecordString::RecordString() {
    std::cout << "==============Call Constructor(RecordString)==============" << std::endl << std::endl;
    stringLenth = 0;

    std::cout << "==========================================================" << std::endl << std::endl;
}

RecordString::~RecordString() {
    std::cout << "==============Call Destructor(RecordString)===============" << std::endl << std::endl;

    std::cout << "==========================================================" << std::endl << std::endl;
}

/* record data method================================================================================================================ */

void RecordString::InitRecordData(std::vector<std::string> _lines) {
    std::cout << "=========Call InitRecordData func(RecordString)===========" << std::endl << std::endl;

    int _currentLineIdx = 0;

    std::vector<std::string> _dataes = SplitString(_lines[_currentLineIdx], SPLIT_DATA_CHAR);
    std::vector<std::string> _names = SplitString(_dataes[CURFUNC_NAME_IDX], SPLIT_FUNC_NAME_CHAR);

    this->threadId = _dataes[THREAD_ID_IDX];
    this->accessType = _dataes[OP_TYPE_IDX];
    this->originName = _dataes[CURFUNC_NAME_IDX];
    if(IsNumber(_names[_names.size() - 1])) this->name = "literal";
    else this->name = _names[_names.size() - 1];
    this->dataFunc = _names[_names.size() - 2];
    this->type = "string";

    if(FindStringInString(_lines[_currentLineIdx], STRING_END)) { // string 의 값에 "\n"이 없을 경우
        int _strValIdx = START_STRING_IDX;
        value = "";
        while(_dataes[_strValIdx] != STRING_END) {
            value += _dataes[_strValIdx];
            value += " ";
            _strValIdx++;
        }
        this->stringLenth = std::stoi(_dataes[STRING_LENGTH_IDX]);
        this->ptr = _dataes[_dataes.size() - 3];
        this->line = _dataes[_dataes.size() - 2];
        this->col = _dataes[_dataes.size() - 1];

    } else { // string의 값에 "\n"이 있을 경우
        this->stringLenth = std::stoi(_dataes[STRING_LENGTH_IDX]);
        int _strValIdx = START_STRING_IDX;
        for(int _i = _strValIdx; _i < _dataes.size(); _i++) {
            if(_i == _dataes.size() - 1) value = value + _dataes[_i] + "\\n";
            else value = value + _dataes[_i] + " s";
        }
        _currentLineIdx++;
        while(FindStringInString(_lines[_currentLineIdx], STRING_END)) {
            value = value + _lines[_currentLineIdx] + "\\n";
            _currentLineIdx++;
        }
        
        _dataes = SplitString(_lines[_currentLineIdx], SPLIT_DATA_CHAR);
        int _stringEndLineIdx = 0;
        while(_dataes[_stringEndLineIdx] != STRING_END) {
            value += _dataes[_stringEndLineIdx];
            value += " ";
            _stringEndLineIdx++;
        }
        this->ptr = _dataes[_dataes.size() - 3];
        this->line = _dataes[_dataes.size() - 2];
        this->col = _dataes[_dataes.size() - 1];
    }

    std::cout << "==========================================================" << std::endl << std::endl;
}

void RecordString::UpdateRecordData(std::vector<std::string> _words) {
    std::cout << "=========Call UpdateRecordData func(RecordString)=========" << std::endl << std::endl;

    std::cout << "==========================================================" << std::endl << std::endl;
}

/* print method====================================================================================================================== */

void RecordString::PrintRecordData() {
    std::cout << "=========Call PrintRecordData func(RecordString)==========" << std::endl << std::endl;
    
    std::cout << "==========================================================" << std::endl << std::endl;
}
std::string RecordString::PrintRecordTable(std::string) {
    std::cout << "========Call PrintRecordTable func(RecordString)==========" << std::endl << std::endl;

    std::cout << "==========================================================" << std::endl << std::endl;
}