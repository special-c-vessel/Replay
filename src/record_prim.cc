#include "record_prim.h"

RecordPrim::RecordPrim() {
    std::cout << "Call Normal Constructor(RecordPrim)" << std::endl;
    shadowMaxIdx = 0;
}

RecordPrim::~RecordPrim() {

}

void RecordPrim::InitRecordData(std::vector<std::string> _words) {
    std::cout << "Call InitRecordData func(RecordPrim), words size = " << _words.size() << std::endl;
    
    std::stringstream _ss(_words[0]);
    // 공백 분리 결과를 저장할 배열
    std::vector<std::string> _names;
    std::string _word;
    // 스트림을 한 줄씩 읽어, 공백 단위로 분리한 뒤, 결과 배열에 저장
    while (getline(_ss, _word, '_')){
        _names.push_back(_word);
    }

    this->originName = _words[0];
    this->dataFunc = _names[0];
    this->name = _names[1];
    this->type = _words[1];
    this->value = _words[2];
    this->ptr = _words[3];
    this->line = _words[4];
    this->col = _words[5];
    this->infoMessage = _words[6];

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
        _ct.AddColumn("Function");
        _ct.AddColumn("Name");
        _ct.AddColumn("Type");
        _ct.AddColumn("Value");
        _ct.AddColumn("Ptr");
        _ct.AddColumn("Line");
        _ct.AddColumn("Col");

        ConsoleTableRow* _entry = new ConsoleTableRow(7);

        _entry->AddEntry(this->dataFunc, 0);
        _entry->AddEntry(this->name, 1);
        _entry->AddEntry(this->type, 2);
        _entry->AddEntry(this->value, 3);
        _entry->AddEntry(this->ptr, 4);
        _entry->AddEntry(this->line, 5);
        _entry->AddEntry(this->col, 6);

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