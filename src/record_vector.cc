#include "record_vector.h"

RecordVector::RecordVector() {
    std::cout << "Call Normal Constructor(RecordVector)" << std::endl;
    currentPage = 1;
    prevPage = 1;
    shadowMaxIdx = 0;
}

RecordVector::~RecordVector() {
    
}

void RecordVector::InitRecordData(std::vector<std::string> _words) {
    std::cout << "Call InitRecordData func(RecordVector)" << std::endl;
    name = _words[1];
    type = _words[2];
    col = _words[_words.size() - 1];
    line = _words[_words.size() - 2];
    ptr = _words[_words.size() - 3];

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

    if(_message == "left") { // 페이지 왼쪽으로 이동
        if(currentPage > 1) { // 현재 페이지가 1일 경우 첫번째 페이지이므로 이동 불가능
            currentPage--;
        }
    }
    else if(_message == "right") { // 페이지를 오른쪽으로 이동
        if(currentPage < maxPageIndex) { // 현재 페이지가 1일 경우 첫번째 페이지이므로 이동 불가능
            currentPage++;
        }
    }
    else { // 명령어 또는 정의되지 않은 명령어
        if(_message.find("mvarray") != std::string::npos) { // 정의된 명령어일 경우
            //////////////////////////// 명령어 분리 ////////////////////////////
            std::stringstream _ss(_message);
            // 공백 분리 결과를 저장할 배열
            std::vector<std::string> _words;
            std::string _word;
            // 스트림을 한 줄씩 읽어, 공백 단위로 분리한 뒤, 결과 배열에 저장
            while (getline(_ss, _word, ' ')){
                _words.push_back(_word);
            }
            //////////////////////////////////////////////////////////////////
        }
    }

    ConsoleTable ct(BASIC);
    ct.SetPadding(1);
    ct.AddColumn("Name");
    ct.AddColumn("Type");
    ct.AddColumn("Value");
    ct.AddColumn("Ptr");
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