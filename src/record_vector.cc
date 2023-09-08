#include "record_vector.h"

RecordVector::RecordVector() {
    std::cout << "Call Normal Constructor(RecordVector)" << std::endl;
    currentPage = 1;
    prevPage = 1;
    shadowMaxIdx = 0;
}

RecordVector::RecordVector(std::vector<std::string> _words) {
    std::cout << "Call Normal Constructor(RecordVector)" << std::endl;
    currentPage = 1;
    prevPage = 1;
    shadowMaxIdx = 0;

    InitRecordData(_words);
}


RecordVector::~RecordVector() {
    
}

void RecordVector::InitRecordData(std::vector<std::string> _words) {
    std::cout << "Call InitRecordData func(RecordVector)" << std::endl;

    std::stringstream _ss(_words[2]);
    // 공백 분리 결과를 저장할 배열
    std::vector<std::string> _names;
    std::string _word;
    // 스트림을 한 줄씩 읽어, 공백 단위로 분리한 뒤, 결과 배열에 저장
    while (getline(_ss, _word, '_')){
        _names.push_back(_word);
    }

    dataFunc = _names[0];
    name = _names[1];
    type = _words[3];
    col = _words[_words.size() - 1];
    line = _words[_words.size() - 2];
    ptr = _words[_words.size() - 3];
    value = _words[_words.size() - 4];

    arrayTypeSize = 4;

    shadowMemory[(AddHexaInt(ptr, arrayTypeSize * shadowMaxIdx))] = _words[_words.size() - 4];

}

void RecordVector::UpdateRecordData(std::vector<std::string> _words) {
    std::cout << "Call UpdateRecordData func(RecordVector)" << std::endl;
    int _idx = std::stoi(_words[4]);
    int _subIdx = shadowMaxIdx - _idx;
    
    if(_subIdx < 0) { 
        _subIdx *= -1;
        for(int i = 0; i < _subIdx; i++) {
            if(i == _subIdx - 1) {
                shadowMaxIdx++;
                shadowMemory[AddHexaInt(this->ptr, shadowMaxIdx * arrayTypeSize)] = _words[_words.size() - 4];
            }
            else {
                shadowMaxIdx++;
                shadowMemory[AddHexaInt(this->ptr, shadowMaxIdx * arrayTypeSize)] = " ";
            }
        }
    }
    else {
        shadowMemory[AddHexaInt(this->ptr, shadowMaxIdx * _idx)] = _words[5]; 
    }
}

void RecordVector::PrintRecordData() {
    std::cout << "\033[1m" << "\t\t\tval name : " << this->name << "\033[0m" << std::endl;
    std::cout << "\033[1m" << "\t\t\tval type : " << this->type << "\033[0m" << std::endl;
    std::cout << "\033[1m" << "\t\t\tval value : " << this->ptr << "\033[0m" << std::endl;
    std::cout << "\033[1m" << "\t\t\tval line : " << this->line << "\033[0m" << std::endl;
    std::cout << std::endl;
}

std::string RecordVector::PrintRecordTable(std::string _message) {
    std::string _returnMessage;
    std::cout << "Call PrintRecordTable func(RecordVector)" << std::endl;

    ConsoleTable ct(BASIC);
    ct.SetPadding(1);
    ct.AddColumn("Function");
    ct.AddColumn("Name");
    ct.AddColumn("Type");
    ct.AddColumn("Value");
    ct.AddColumn("Ptr");
    ct.AddColumn("Index");

    int _tableIdx = 0;

    for (auto iter = shadowMemory.begin(); iter != shadowMemory.end(); ++iter){
        ConsoleTableRow* _entry = new ConsoleTableRow(6);
        _entry->AddEntry(this->dataFunc, 0);
        _entry->AddEntry(this->name, 1);
        _entry->AddEntry(this->type, 2);
        _entry->AddEntry(iter->second, 3);
        _entry->AddEntry(iter->first, 4);
        _entry->AddEntry(std::to_string(_tableIdx), 5);
        _tableIdx++;
        ct.AddRow(_entry);
    }

    ct.PrintTable();
    
    prevPage = currentPage;
    return _returnMessage;
}

bool RecordVector::IsNumber(std::string const &_str) {
    auto it = _str.begin();
    while(it != _str.end() && std::isdigit(*it)) {
        it++;
    }
    return !_str.empty() && it == _str.end();
}

std::string RecordVector::AddHexaInt(std::string _hexaStr, int _addVal) {
    std::string pureHex = _hexaStr.substr(2);

    // "0x" 포함 16진수 문자열을 int 형으로 변환
    unsigned long long intVal = std::stoull(pureHex, nullptr, 16);

    intVal += _addVal;

    // int 값을 다시 16진수 문자열로 변환
    std::stringstream ss;
    ss << "0x" << std::hex << intVal;
    std::string convertedHexStr = ss.str();

    return convertedHexStr;
}