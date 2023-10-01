#include "record_data.h"

RecordData::RecordData() 
: name(""), type(""), value(""), ptr(""), line(""), col(""), recordType(RecordType::None), infoMessage("") {
    std::cout << "Call Normal Constructor(RecordData)" << std::endl;
}

RecordData::~RecordData() {
    std::cout << "Call Normal Destory(RecordData)" << std::endl;
}

void RecordData::InitRecordData(std::vector<std::string> _words) {

}

void RecordData::UpdateRecordData(std::vector<std::string> _words) {

}

void RecordData::PrintRecordData() {

}

std::string RecordData::PrintRecordTable(std::string _message) {

}

void RecordData::SetShadowMemory(std::map<std::string, std::string> _shadowMemory) {
    
}

std::map<std::string, std::string> RecordData::GetShadowMemory() {
    
}

void RecordData::SetShadowMemorySize(int _size) {

}

int RecordData::GetShadowMemorySize() {
    
}

void RecordData::SetArrrays(std::vector<ArrayStruct> _arrays) {
    
}

std::vector<ArrayStruct> RecordData::GetArrays() {
    
}

// 편의성 함수

// 주어진 문자열에 숫자가 포함되어 있는지 판단하는 함수
bool RecordData::IsNumber(std::string const &_str) {
    auto it = _str.begin();
    while(it != _str.end() && std::isdigit(*it)) {
        it++;
    }
    return !_str.empty() && it == _str.end();
}

// 첫번째 인자로 받은 string 형식으로 된 16진수 문자열의 두번째 인자로 전달받은 정수형의 크기만큼 더한 후 문자열을 반환하는 함수
std::string RecordData::AddHexaInt(std::string _hexaStr, int _addVal) {
    std::string convertedHexStr = _hexaStr;
    if(_addVal != 0) {
        std::string pureHex = _hexaStr.substr(2);

        // "0x" 포함 16진수 문자열을 int 형으로 변환
        unsigned long long intVal = std::stoull(pureHex, nullptr, 16);

        intVal += _addVal;

        // int 값을 다시 16진수 문자열로 변환
        std::stringstream _ss;
        _ss << "0x" << std::hex << intVal;
        convertedHexStr = _ss.str();
    }

    return convertedHexStr;
}

std::string RecordData::GetType(std::string& _word) {
    if(_word.find("p") != std::string::npos) { // pointer data type
        std::string _removeStr = RemoveChar(_word, 'p');

        if(_removeStr == "i32") {
            _removeStr = "int";
        }
        else if(_removeStr == "i64") {
            _removeStr = "long long int";
        }
        else if(_removeStr == "i8") {
            _removeStr = "char";
        }
        else if(_removeStr == "float") {
            _removeStr = "float";
        }
        else if(_removeStr == "i16") {
            _removeStr  ="short";
        }
        else if(_removeStr == "double") {
            _removeStr = "double";
        }
        else if(_removeStr == "string") {
            _removeStr = "string";
        }
        else {
            _removeStr = "undefined type";
        }

        std::string _pointerStr;
        int _pCount = CountChar(_word, 'p');

        if(_pCount == 1) {
            _pointerStr = " pointer";
        }
        else {
            _pointerStr = " multiple pointer-" + std::to_string(_pCount);
        }

        return _removeStr + _pointerStr;
    }
    else { // normal data type
        if(_word == "i32") {
            return "int";
        }
        else if(_word == "i64") {
            return "long long int";
        }
        else if(_word == "i8") {
            return "char";
        }
        else if(_word == "float") {
            return "float";
        }
        else if(_word == "i16") {
            return "short";
        }
        else if(_word == "double") {
            return "double";
        }
        else if(_word == "string") {
            return "string";
        }
        else {
            return "undefined type";
        }
    }
}

int RecordData::CountChar(std::string &_str, char _findWord) {
    int _count = 0;
    for (char _ch : _str) {
        if (_ch == _findWord) {
            _count++;
        }
    }
    return _count;
}

std::string RecordData::RemoveChar(const std::string &_str, char _removeWord) {
    std::string _result = _str;
    size_t _pos = 0;
    
    while ((_pos = _result.find(_removeWord, _pos)) != std::string::npos) {
        _result.erase(_pos, 1);
    }
    
    return _result;
}

std::vector<std::string> RecordData::SplitString(const std::string &_str, char _delimiter) {
    std::vector<std::string> _result;
    std::string _token;
    std::istringstream _tokenStream(_str);

    while (std::getline(_tokenStream, _token, _delimiter)) {
        _result.push_back(_token);
    }

    return _result;
}

bool RecordData::FindStringInString(std::string _str, std::string _findStr) {
    return _str.find(_findStr) != std::string::npos;
}