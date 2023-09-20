#include "record_vector.h"

RecordVector::RecordVector() {
    std::cout << "Call Normal Constructor(RecordVector)" << std::endl;
    currentPage = 1;
    prevPage = 1;
    shadowMaxIdx = 0;
    recordType = RecordType::Array;
}

RecordVector::RecordVector(std::vector<std::string> _words) {
    std::cout << "Call Normal Constructor(RecordVector)" << std::endl;
    currentPage = 1;
    prevPage = 1;
    shadowMaxIdx = 0;
    recordType = RecordType::Array;

    InitRecordData(_words);
}


RecordVector::~RecordVector() {
    
}

void RecordVector::InitRecordData(std::vector<std::string> _words) {
    std::cout << "===========Call InitRecordData func(RecordVector)===========" << std::endl;

    std::stringstream _ss(_words[2]);
    // 공백 분리 결과를 저장할 배열
    std::vector<std::string> _names;
    std::string _word;
    // 스트림을 한 줄씩 읽어, 공백 단위로 분리한 뒤, 결과 배열에 저장
    while (getline(_ss, _word, '-')){
        _names.push_back(_word);
    }
    std::cout << "RecordVector Initialization Info" << std::endl;
    std::cout << "dataFunc : " << _names[0] << std::endl;
    std::cout << "name : " << _names[1] << std::endl;
    std::cout << "type : " << _words[3] << std::endl;
    std::cout << "ptr : " << _words[_words.size() - 3] << std::endl;
    std::cout << "value : " << _words[_words.size() - 4] << std::endl;
    std::cout << "line : " << _words[_words.size() - 2] << std::endl;
    std::cout << "access type : " << _words[0] << std::endl;

    accessType = _words[0];
    dataFunc = _names[0];
    name = _names[1];
    type = GetType(_words[3]);
    col = _words[_words.size() - 1];
    line = _words[_words.size() - 2];
    ptr = _words[_words.size() - 3];
    value = _words[_words.size() - 4];
    arrayTypeSize = 4;

    std::cout << "============================================================" << std::endl;
}

void RecordVector::UpdateRecordData(std::vector<std::string> _words) {
    std::cout << "===========Call UpdateRecordData func(RecordVector)===========" << std::endl;
    
    std::string _shadowMemoryStr = (AddHexaInt(ptr, arrayTypeSize * shadowMaxIdx));
    shadowMaxIdx++;
    
    PrintShadowMemory();

    std::cout << "Shadow memory str : " << _shadowMemoryStr << std::endl;
    std::cout << "Value : " << _words[_words.size() - 4] << std::endl;

    shadowMemory[_shadowMemoryStr] = _words[_words.size() - 4];

    PrintShadowMemory();
    std::cout << "============================================================" << std::endl;
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
        if(_message.find("mvindex") != std::string::npos) { // 정의된 명령어일 경우
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
            
            /////////////// 명령어가 인덱스의 수를 제대로 전달했는지 검사 ///////////////
            if (_words.size() != 2) { // 명령어의 수가 차원보다 한개 높은지 검사
                _returnMessage =  "WARNING 명령어의 인덱싱이 올바르지 않습니다."; // 잘못된 인덱스의 수
            }
            else {
                ////////////////////////// 차원에 따른 처리 //////////////////////////
                int _assignIndex = std::stoi(_words[2]);
                currentPage = (_assignIndex / 10) + 1;
                //////////////////////////////////////////////////////////////////
            }
            //////////////////////////////////////////////////////////////////
        }
        else { // 정의되지 않은 명령어일 경우

        }
    }

    ConsoleTable ct(BASIC);
    ct.SetPadding(1);
    ct.AddColumn(" ");
    ct.AddColumn("Current Function");
    ct.AddColumn("Operation");
    ct.AddColumn("Name");
    ct.AddColumn("Type");
    ct.AddColumn("Value");
    ct.AddColumn("Pointer Address");
    ct.AddColumn("Index");
    ct.AddColumn("Container Type");

    std::vector<VectorStruct> _vectors;
    for (auto iter = shadowMemory.begin(); iter != shadowMemory.end(); ++iter){
        VectorStruct _vectorStruct;
        _vectorStruct.vectorPtr = iter->first;
        _vectorStruct.vectorValue = iter->second;
        _vectors.push_back(_vectorStruct);
    }

    for(int i = 0 ; i < _vectors.size(); i++) {
        _vectors[i].vectorFunc = this->dataFunc;
        _vectors[i].vectorName = this->name;
        _vectors[i].vectorType = this->type;
        _vectors[i].vectorAccessType = this->accessType;
        _vectors[i].vectorIndex = std::to_string(i);
    }

    int _startIndex = (currentPage * 10) - 10;
    int _endIndex = (currentPage * 10);
    if(_startIndex > _vectors.size() - 1) _startIndex = _vectors.size() - 1;
    if(_endIndex > _vectors.size()) _endIndex = _vectors.size();
    int _tableIndex = 0;
    for(int i = _startIndex ; i < _endIndex; i++) {
        ConsoleTableRow* entry = new ConsoleTableRow(9);
        _tableIndex++;
        entry->AddEntry(std::to_string(_tableIndex), 0);
        entry->AddEntry(_vectors[i].vectorFunc, 1);
        entry->AddEntry(_vectors[i].vectorAccessType, 2);
        entry->AddEntry(_vectors[i].vectorName, 3);
        entry->AddEntry(_vectors[i].vectorType, 4);
        entry->AddEntry(_vectors[i].vectorValue, 5);
        entry->AddEntry(_vectors[i].vectorPtr, 6);
        entry->AddEntry(_vectors[i].vectorIndex, 7);
        entry->AddEntry("Vector", 8);
            
        ct.AddRow(entry);
    }
    ct.PrintTable();
    
    prevPage = currentPage;
    if(this->infoMessage != "") {
        _returnMessage = infoMessage;
    }
    std::cout << "return message : " << _returnMessage << std::endl;
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
    std::string convertedHexStr = _hexaStr;
    if(_addVal != 0) {
        std::string pureHex = _hexaStr.substr(2);

        // "0x" 포함 16진수 문자열을 int 형으로 변환
        unsigned long long intVal = std::stoull(pureHex, nullptr, 16);

        intVal += _addVal;

        // int 값을 다시 16진수 문자열로 변환
        std::stringstream ss;
        ss << "0x" << std::hex << intVal;
        convertedHexStr = ss.str();
    }

    return convertedHexStr;
}

void RecordVector::SetShadowMemory(std::map<std::string, std::string> _shadowMemory) {
    this->shadowMemory = _shadowMemory;
    this->shadowMaxIdx = this->shadowMemory.size();
    PrintShadowMemory();
}

std::map<std::string, std::string> RecordVector::GetShadowMemory() {
    return this->shadowMemory;
}

void RecordVector::PrintShadowMemory() {
    for (auto iter = shadowMemory.begin() ; iter !=  shadowMemory.end(); iter++) {
        std::cout << "Shadow Memory : " << iter->first << "," << iter->second << std::endl;
    }
}

void RecordVector::SetShadowMemorySize(int _size) {
    this->shadowMaxIdx = _size;
}

int RecordVector::GetShadowMemorySize() {
    return this->shadowMaxIdx;
}

void RecordVector::SetArrrays(std::vector<ArrayStruct> _arrays) {
    
}

std::vector<ArrayStruct> RecordVector::GetArrays() {
    
}

std::string RecordVector::GetType(std::string& _word) {
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

int RecordVector::CountChar(std::string &_str, char _findWord) {
    int _count = 0;
    for (char _ch : _str) {
        if (_ch == _findWord) {
            _count++;
        }
    }
    return _count;
}

std::string RecordVector::RemoveChar(const std::string &_str, char _removeWord) {
    std::string _result = _str;
    size_t _pos = 0;
    
    while ((_pos = _result.find(_removeWord, _pos)) != std::string::npos) {
        _result.erase(_pos, 1);
    }
    
    return _result;
}