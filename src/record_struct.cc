#include "record_struct.h"

RecordStruct::RecordStruct() {
    std::cout << "Call Normal Constructor(RecordStruct)" << std::endl;
    shadowMaxIdx = 0;
    recordType = RecordType::Struct;
}

RecordStruct::RecordStruct(std::vector<std::string> _words) {
    std::cout << "Call Normal Constructor(RecordStruct)" << std::endl;
    shadowMaxIdx = 0;
    recordType = RecordType::Struct;

    InitRecordData(_words);
}


RecordStruct::~RecordStruct() {
    
}

void RecordStruct::InitRecordData(std::vector<std::string> _words) {
    std::cout << "===========Call InitRecordData func(RecordStruct)===========" << std::endl;

    std::vector<std::string> _names = SplitString(_words[1], '-');
    
    accessType = "struct";
    dataFunc = _names[0];
    name = _names[1];
    type = GetType(_words[2]);

    int _endIndex = _words.size() - 1;
    int _index = VALUE_START_IDX;

    while(_index < _endIndex) {
        std::string _typeStr = GetType(_words[_index]);
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