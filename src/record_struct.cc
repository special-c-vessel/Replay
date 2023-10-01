#include "record_struct.h"

RecordStruct::RecordStruct() {
    std::cout << "Call Normal Constructor(RecordStruct)" << std::endl;
    shadowMaxIdx = 0;
    recordType = RecordType::Struct;
}

RecordStruct::RecordStruct(std::vector<std::string> _lines) {
    std::cout << "Call Normal Constructor(RecordStruct)" << std::endl;
    shadowMaxIdx = 0;
    recordType = RecordType::Struct;

    InitRecordData(_lines);
}


RecordStruct::~RecordStruct() {
    
}

void RecordStruct::InitRecordData(std::vector<std::string> _lines) {
    std::cout << "===========Call InitRecordData func(RecordStruct)===========" << std::endl;
    int _curLineIdx = 0;
    std::vector<std::string> _words = SplitString(_lines[_curLineIdx], ' ');
    std::vector<std::string> _names = SplitString(_words[1], '-');
    
    accessType = "struct";
    dataFunc = _names[0];
    name = _names[1];
    type = GetType(_words[2]);

    int _endIndex = _words.size() - 1;
    int _index = VALUE_START_IDX;

    while(_index < _endIndex) {
        std::string _typeStr = GetType(_words[_index]);
        std::string _valStr = "";

        if(_typeStr == "string") {
            _index += 3;
            if(FindStringInString(_lines[0], STRING_END) && _lines.size() == 1) { // 문자열 데이터 값에 개행이 없을 경우
                if(_words[_index] != STRING_END) {
                    _valStr = _words[_index];
                    _index++;
                    while(_words[_index] != STRING_END) {
                        _valStr = _valStr + " " + _words[_index];
                        _index++;
                    }
                }
            } // 문자열 값에 개행이 있을 경우
            else {
                for(int _i = _index; _i <= _endIndex; _i++) {
                    if(_i == _endIndex) {
                        _valStr = _valStr + _words[_i] + "\\n";
                    }
                    else {
                        _valStr = _valStr + _words[_i] + " ";
                    }
                }
                _curLineIdx++; // 현재 라인 벡터의 인덱스를 1 증가
                while(!FindStringInString(_lines[_curLineIdx], STRING_END)) { // 라인 벡터의 String end 가 나올떄까지 값에 추가
                    _valStr = _valStr + _lines[_curLineIdx] + "\\n";
                    _curLineIdx++;
                }
                std::vector<std::string> _strEndVector = SplitString(_lines[_curLineIdx], ' ');
                int _i = 0;
                while(_strEndVector[_i] != "StringEnd") {
                    _valStr += _strEndVector[_i];
                    _valStr += " ";
                    _i++;
                }
            }
        }
        else {
            _valStr = _words[_index + 1];
        }

        std::cout << "val str : " << _valStr << std::endl;
        std::cout << "type str : " << _typeStr << std::endl;

        GetType(_words[_index]);
        std::string _valueStr = _words[_index + 1];
        DataStruct _dataStruct;
        _dataStruct.type = _typeStr;
        _dataStruct.value = _valueStr;
        dataStructs.push_back(_dataStruct);
        _index += 2;
    }


    std::cout << "============================================================" << std::endl;
}

void RecordStruct::UpdateRecordData(std::vector<std::string> _words) {
    std::cout << "===========Call UpdateRecordData func(RecordStruct)===========" << std::endl;
    
    std::cout << "============================================================" << std::endl;
}

void RecordStruct::PrintRecordData() {
    std::cout << "\033[1m" << "\t\t\tval name : " << this->name << "\033[0m" << std::endl;
    std::cout << "\033[1m" << "\t\t\tval type : " << this->type << "\033[0m" << std::endl;
    std::cout << "\033[1m" << "\t\t\tval value : " << this->ptr << "\033[0m" << std::endl;
    std::cout << "\033[1m" << "\t\t\tval line : " << this->line << "\033[0m" << std::endl;
    std::cout << std::endl;
}

std::string RecordStruct::PrintRecordTable(std::string _message) {
   std::string _returnMessage;


    return _returnMessage;
}

void RecordStruct::SetShadowMemory(std::map<std::string, std::string> _shadowMemory) {
    this->shadowMemory = _shadowMemory;
    this->shadowMaxIdx = this->shadowMemory.size();
}

std::map<std::string, std::string> RecordStruct::GetShadowMemory() {
    return this->shadowMemory;
}

void RecordStruct::SetShadowMemorySize(int _size) {
    this->shadowMaxIdx = _size;
}

int RecordStruct::GetShadowMemorySize() {
    return this->shadowMaxIdx;
}

void RecordStruct::SetArrrays(std::vector<ArrayStruct> _arrays) {
    
}

std::vector<ArrayStruct> RecordStruct::GetArrays() {
    
}