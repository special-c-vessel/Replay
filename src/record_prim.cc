#include "record_prim.h"

RecordPrim::RecordPrim() {
    std::cout << "Call Normal Constructor(RecordPrim)" << std::endl;
}

RecordPrim::~RecordPrim() {

}

void RecordPrim::InitRecordData(std::vector<std::string> _words) {
    std::cout << "Call InitRecordData func(RecordPrim), words size = " << _words.size() << std::endl;
    this->name = _words[0];
    this->type = _words[1];
    this->value = _words[2];
    this->ptr = _words[3];
    this->line = _words[4];
    this->col = _words[5];
    this->length = _words[6];
    this->infoMessage = _words[7];
}

void RecordPrim::UpdateRecordData(std::vector<std::string> _words) {

}

void RecordPrim::PrintRecordData() {

}

std::string RecordPrim::PrintRecordTable(std::string _message) {
    std::string _returnMessage = "";

    if(this->type != "string") {
        ConsoleTable _ct(BASIC);
        _ct.SetPadding(1);
        _ct.AddColumn("Name");
        _ct.AddColumn("Type");
        _ct.AddColumn("Value");
        _ct.AddColumn("Ptr");
        _ct.AddColumn("Line");
        _ct.AddColumn("Col");

        ConsoleTableRow* _entry = new ConsoleTableRow(6);

        _entry->AddEntry(this->name, 0);
        _entry->AddEntry(this->type, 1);
        _entry->AddEntry(this->value, 2);
        _entry->AddEntry(this->ptr, 3);
        _entry->AddEntry(this->line, 4);
        _entry->AddEntry(this->col, 5);

        _ct.AddRow(_entry);

        _ct.PrintTable();
    }
    else {
        std::cout << "variable name : " << this->name << std::endl;
        std::cout << "ariable type : " << this->type << std::endl;
        std::cout << "variable ptr : " << this->ptr << std::endl;
        std::cout << "variable line : " << this->line << std::endl;
        std::cout << "variable col : " << this->col << std::endl;

        std::string str = this->value;


        std::string searchStr = "\\n";
        std::string replaceStr = "\n";

        size_t pos = str.find(searchStr);
        while (pos != std::string::npos) {
            str.replace(pos, searchStr.length(), replaceStr);
            pos = str.find(searchStr, pos + replaceStr.length());
        }
        std::cout << "variable value" << std::endl << str << std::endl;
    }

    if(this->infoMessage != "") {
        _returnMessage = this->infoMessage;
    }
    return _returnMessage;
}