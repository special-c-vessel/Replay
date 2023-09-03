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

    /*
    std::string _nameVal = "";
    std::string _typeVal = "";
    std::string _strVal = "";
    std::string _ptrVal = "";
    std::string _lenVal = "";
    std::string _lineVal = "";
    std::string _colVal = "";
    std::string _infoMessage = "";
    
    std::cout<< "word index 1 : " << FindRecordDataStr(_words[1]) << std::endl;

    if(_line.find("StringEnd") != std::string::npos) {
        int _i = JUDGMENT_INDEX + 2;
        while(_words[_i] != "StringEnd") {
            _strVal += _words[_i];
            _strVal += " ";
            _i++;
        }
        _colVal = _words[_words.size() - 1];
        _lineVal = _words[_words.size() - 2];
        _ptrVal = _words[_words.size() - 3];
        _lenVal = _words[JUDGMENT_INDEX + 1];
        std::cout << "strVal : " << _strVal << std::endl;
    }
    else if(FindRecordDataStr(_words[1]) != -1) {
        std::cout << "find record data " << std::endl;
        int _findIndex = FindRecordDataStr(_words[1]);
        int _changeIndex = std::stoi(_words[3]);
        _nameVal = _words[1];
        _typeVal = _words[2];
        _strVal = records[_findIndex]->value;
        _strVal[_changeIndex] = _words[4].c_str()[0];
        _colVal = _words[_words.size() - 1];
        _lineVal = _words[_words.size() - 2];
        _ptrVal = _words[_words.size() - 3];
        _lenVal = records[_findIndex]->length;
        if(stoi(records[_findIndex]->length) < _changeIndex) {
            _infoMessage = "문자열의 범위 밖 요청입니다";
        }
    }
    else {
        int _strValIndex = JUDGMENT_INDEX + 2;
        for(int _i = _strValIndex; _i < _words.size(); _i++) {
            if(_i == _words.size() - 1) {
                _strVal = _strVal + _words[_i] + "\\n";
            }
            else {
                _strVal = _strVal + _words[_i] + " ";
            }
        }
        i++;
        while(recordLines[i].find("StringEnd") == std::string::npos) {
            _strVal = _strVal + recordLines[i] + "\\n";
            i++;
        }
        std::cout << "reocrdLines1 : " << recordLines[i] << std::endl;
        std::stringstream _ss2(recordLines[i]);
        // 공백 분리 결과를 저장할 배열
        std::vector<std::string> _words2;
        std::string _word2;
        // 스트림을 한 줄씩 읽어, 공백 단위로 분리한 뒤, 결과 배열에 저장
        while (getline(_ss2, _word2, ' ')){
            _words2.push_back(_word2);
        }
        int _i = 0;
        while(_words2[_i] != "StringEnd") {
            _strVal += _words2[_i];
            _strVal += " ";
            _i++;
        }
        _colVal = _words2[_words2.size() - 1];
        _lineVal = _words2[_words2.size() - 2];
        _ptrVal = _words2[_words2.size() - 3];
        _lenVal = _words[JUDGMENT_INDEX + 1];
        std::cout << "strVal : " << _strVal << std::endl;
    }
    std::vector<std::string> _resultWord;
    _resultWord.push_back(_words[1]);
    _resultWord.push_back(_words[2]);
    _resultWord.push_back(_strVal);
    _resultWord.push_back(_ptrVal);
    _resultWord.push_back(_lineVal);
    _resultWord.push_back(_colVal);
    _resultWord.push_back(_lenVal);
    _resultWord.push_back(_infoMessage);

    for(int i = 0; i < _resultWord.size(); i++) {
        std::cout << "result word : " << _resultWord[i] << std::endl;
    }

    RecordData* _data = new RecordPrim();

    _data->InitRecordData(_resultWord);
    _data->recordType = RecordType::Prim;
    records.push_back(_data);
    _addIndex = records.size() - 1;
    */
}

void RecordPrim::UpdateRecordData(std::vector<std::string> _words) {

}

void RecordPrim::PrintRecordData() {

}

std::string RecordPrim::PrintRecordTable(std::string _message) {
    std::string _returnMessage = "";
    
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

    if(this->infoMessage != "") {
        _returnMessage = this->infoMessage;
    }
    return _returnMessage;
}