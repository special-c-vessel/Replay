#include "record_prim.h"

RecordPrim::RecordPrim() {
    std::cout << "Call Normal Constructor(RecordPrim)" << std::endl;
    shadowMaxIdx = 0;
    recordType = RecordType::Prim;
}

RecordPrim::~RecordPrim() {

}

void RecordPrim::InitRecordData(std::vector<std::string> _words) {
    std::cout << "Call InitRecordData func(RecordPrim), words size = " << _words.size() << std::endl;
    
    std::stringstream _ss(_words[1]);
    // 공백 분리 결과를 저장할 배열
    std::vector<std::string> _names;
    std::string _word;
    // 스트림을 한 줄씩 읽어, 공백 단위로 분리한 뒤, 결과 배열에 저장
    while (getline(_ss, _word, '_')){
        _names.push_back(_word);
    }

    if(IsNumeric(_names[_names.size() - 1])) {
        this->name = "literal";
    }
    else {
        this->name = _names[_names.size() - 1];
    }

    if(_words[3] == "0x0") {
        this->value = "nullptr";
    }
    else {
        this->value = _words[3];
    }
    this->accessType = _words[0];
    this->originName = _words[1];
    this->dataFunc = _names[_names.size() - 2];
    this->type = _words[2];
    this->ptr = _words[4];
    this->line = _words[5];
    this->col = _words[6];
    this->infoMessage = _words[7];

    this->shadowMemory[this->ptr] = this->value;
    shadowMaxIdx++;
}

void RecordPrim::UpdateRecordData(std::vector<std::string> _words) {

}

void RecordPrim::PrintRecordData() {

}

std::string RecordPrim::PrintRecordTable(std::string _message) {
    std::string _returnMessage = "";
    
        ConsoleTable _ct(BASIC);
        _ct.SetPadding(1);
        _ct.AddColumn("Current Function");
        _ct.AddColumn("Access Type");
        _ct.AddColumn("Name");
        _ct.AddColumn("Type");
        _ct.AddColumn("Value");
        _ct.AddColumn("Pointer Address");
        _ct.AddColumn("Line");
        _ct.AddColumn("Column");

        ConsoleTableRow* _entry = new ConsoleTableRow(8);

        _entry->AddEntry(this->dataFunc, 0);
        _entry->AddEntry(this->accessType, 1);
        _entry->AddEntry(this->name, 2);
        _entry->AddEntry(this->type, 3);
        _entry->AddEntry(this->value, 4);
        _entry->AddEntry(this->ptr, 5);
        _entry->AddEntry(this->line, 6);
        _entry->AddEntry(this->col, 7);

        _ct.AddRow(_entry);

        _ct.PrintTable();

    if(this->infoMessage != "") {
        _returnMessage = this->infoMessage;
    }
    return _returnMessage;
}

void RecordPrim::SetShadowMemory(std::map<std::string, std::string> _shadowMemory) {
    this->shadowMemory = _shadowMemory;
    this->shadowMaxIdx = this->shadowMemory.size();
    PrintShadowMemory();
}

std::map<std::string, std::string> RecordPrim::GetShadowMemory() {
    return this->shadowMemory;
}

void RecordPrim::PrintShadowMemory() {
    for (auto iter = shadowMemory.begin() ; iter !=  shadowMemory.end(); iter++) {
        std::cout << "Shadow Memory : " << iter->first << "," << iter->second << std::endl;
    }
}

void RecordPrim::SetShadowMemorySize(int _size) {
    this->shadowMaxIdx = _size;
}

int RecordPrim::GetShadowMemorySize() {
    return this->shadowMaxIdx;
}

bool RecordPrim::IsNumeric(const std::string& _str) {
    // 문자열이 비어있는 경우 숫자가 아님
    if (_str.empty()) {
        return false;
    }

    // 문자열의 각 문자를 확인하며 숫자인지 검사
    for (char _c : _str) {
        if (!isdigit(_c)) {
            return false;
        }
    }

    return true;
}

void RecordPrim::SetArrrays(std::vector<ArrayStruct> _arrays) {
    
}

std::vector<ArrayStruct> RecordPrim::GetArrays() {
    
}