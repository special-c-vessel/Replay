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

    originStr = _lines;

    int _curLineIdx = 0;
    std::vector<std::string> _words = SplitString(_lines[_curLineIdx], ' ');
    std::vector<std::string> _names = SplitString(_words[1], '-');
    
    accessType = "struct";
    dataFunc = _names[0];
    name = _names[1];
    type = _words[2];

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
                    _index++;
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
        else if(_typeStr == "isStruct") {
            std::cout << "check strcut" << std::endl;
            _typeStr = _words[_index + 1];
            _valStr = "struct object";
            _index += 2;
        }
        else {
            _valStr = _words[_index + 1];
            _index += 2;
        }

        GetType(_words[_index]);
        DataStruct _dataStruct;
        _dataStruct.type = _typeStr;
        _dataStruct.value = _valStr;
        dataStructs.push_back(_dataStruct);

        if(_index == _endIndex) {
            ptr = _words[_index];
        }
    }
    std::cout << "struct record data ptr : " << ptr << std::endl;

    dataStructs[0].ptr = ptr;

    for(int i = 1 ; i < dataStructs.size(); i++) {
        dataStructs[i].ptr = AddHexaInt(dataStructs[i - 1].ptr, GetSizeByType(dataStructs[i - 1].type));
        shadowMemory[dataStructs[i].ptr] = dataStructs[i].value;
    }

    PrintStructData();

    std::cout << "============================================================" << std::endl;
}

void RecordStruct::UpdateRecordData(std::vector<std::string> _words) {
    std::cout << "===========Call UpdateRecordData func(RecordStruct)===========" << std::endl;
    line = _words[_words.size() - 2];
    col = _words[_words.size() - 1];
    std::string _changePtr = _words[_words.size() - 3];
    std::string _chageValue = _words[_words.size() - 4];


    int _dataStructIdx = -1;

    for(int i = 0; i < _words.size(); i++) {
        std::cout << "data struct ptr : " << dataStructs[i].ptr << std::endl;
        std::cout << "chage ptr : " << _changePtr << std::endl;
        if(dataStructs[i].ptr == _changePtr) {
            std::cout << "data struct value : " << dataStructs[i].value << std::endl;
            std::cout << "chage value : " << _chageValue << std::endl;
            dataStructs[i].value = _chageValue;
        }
    }

    PrintStructData();
    
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

   //std::cout << "\033[1m" << "\n\nstruct name : " << this->type << "\033[0m" << std::endl;
   //std::cout << "\033[1m" << "struct variable name : " << this->name << "\033[0m" << std::endl;

    ConsoleTable _ct2(BASIC);
    _ct2.SetPadding(1);
    _ct2.AddColumn("Struct name");
    _ct2.AddColumn(type);

    ConsoleTableRow* _entry2 = new ConsoleTableRow(2);

    _entry2->AddEntry("Struct variable name", 0);
    _entry2->AddEntry(name, 1);

    _ct2.AddRow(_entry2);

    _ct2.PrintTable();

    ConsoleTable _ct(BASIC);
    _ct.SetPadding(1);
    _ct.AddColumn(" ");
    _ct.AddColumn("Struct data type");
    _ct.AddColumn("Struct data value");
    _ct.AddColumn("Struct data ptr");

    for(int i = 0 ; i < dataStructs.size(); i++) {
        ConsoleTableRow* _entry = new ConsoleTableRow(4);

        _entry->AddEntry("data " + std::to_string(i + 1), 0);
        _entry->AddEntry(dataStructs[i].type, 1);
        _entry->AddEntry(dataStructs[i].value, 2);
        _entry->AddEntry(dataStructs[i].ptr, 3);

        _ct.AddRow(_entry);

    }

    _ct.PrintTable();


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

void RecordStruct::PrintStructData() {
    for(int i = 0; i < dataStructs.size(); i++) {
        std::cout << "data type : " << dataStructs[i].type << ", data value : " << dataStructs[i].value << ", data ptr : " << dataStructs[i].ptr << std::endl;
    }
}

void RecordStruct::SetStruct(RecordStruct& _struct) {
    std::cout << "name : " << _struct.name;
    name = _struct.name;
}

void RecordStruct::SetDataStruct(std::vector<DataStruct> _dataStructs) {
    this->dataStructs = _dataStructs;
}

std::vector<DataStruct> RecordStruct::GetDataStruct() {
    return this->dataStructs;
}