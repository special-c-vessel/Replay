#include "record_array.h"

RecordArray::RecordArray() {
    std::cout << "Call Normal Constructor(RecordArray)" << std::endl;
    currentPage = 1;
    prevPage = 1;
    shadowMaxIdx = 0;
    threadId = "0";
}

RecordArray::RecordArray(std::vector<std::string> _words, int _dimension) {
    std::cout << "Call Dimension Constructor(RecordArray), dimension = " << std::endl;
    currentPage = 1;
    prevPage = 1;
    shadowMaxIdx = 0;
    dimension = _dimension;
    threadId = "0";
    InitRecordData(_words);
}

RecordArray::~RecordArray() {
    
}

void RecordArray::InitRecordData(std::vector<std::string> _words) {
    std::cout << "===========Call InitRecordData func(RecordArray)===========" << std::endl << std::endl;
    
    recordType = RecordType::Array;

    std::stringstream _ss(_words[CURFUNC_NAME_IDX]);
    // 공백 분리 결과를 저장할 배열
    std::vector<std::string> _names;
    std::string _word;
    // 스트림을 한 줄씩 읽어, 공백 단위로 분리한 뒤, 결과 배열에 저장
    while (getline(_ss, _word, '-')){
        _names.push_back(_word);
    }

    ArrayStruct _arrayStruct;
    _arrayStruct.arrayAccesType = _words[1];
    _arrayStruct.arrayFunc = _names[0];
    _arrayStruct.arrayName = _names[1];
    _arrayStruct.arrayType = GetType(_words[TYPE_IDX]);
    _arrayStruct.arrayValue = _words[_words.size() - 4];
    _arrayStruct.arrayPtr = _words[_words.size() - 3];
    _arrayStruct.arrayLine = _words[_words.size() - 2];
    _arrayStruct.arrayCol = _words[_words.size() - 1];

    for(int i = 0; i < dimension; i++) {
        _arrayStruct.arrayIndex.push_back(_words[5 + i]);
            std::cout << "_words[4 + i]" << _words[5 + i] << std::endl;
    }

    this->threadId = _words[THREAD_ID_IDX];
    this->accessType = _arrayStruct.arrayAccesType;
    this->dataFunc = _arrayStruct.arrayFunc;
    this->name = _arrayStruct.arrayName;
    this->ptr = _arrayStruct.arrayPtr;
    this->line = _arrayStruct.arrayLine;
    this->col = _arrayStruct.arrayCol;

    arrays.push_back(_arrayStruct);
    
    maxPageIndex = (arrays.size() / 10) + 1;

    std::cout << "Access Type : " << _arrayStruct.arrayAccesType << std::endl;
    std::cout << "Func : " << _arrayStruct.arrayFunc << std::endl;
    std::cout << "Name : " << _arrayStruct.arrayName << std::endl;
    std::cout << "Type : " << _arrayStruct.arrayType << std::endl;
    std::cout << "Value : " << _arrayStruct.arrayValue << std::endl;
    std::cout << "arrayPtr : " << _arrayStruct.arrayPtr << std::endl;
    std::cout << "arrayLine : " << _arrayStruct.arrayLine << std::endl;
    std::cout << "arrayCol : " << _arrayStruct.arrayCol << std::endl;

    for(int i = 0; i < _arrayStruct.arrayIndex.size(); i++) {
        std::cout << "Index" << i << " : " << _arrayStruct.arrayIndex[i] << std::endl;
    }
    
    std::cout << "===========================================================" << std::endl << std::endl;
}

void RecordArray::UpdateRecordData(std::vector<std::string> _words) {
    std::stringstream _ss(_words[1]);
    // 공백 분리 결과를 저장할 배열
    std::vector<std::string> _names;
    std::string _word;
    // 스트림을 한 줄씩 읽어, 공백 단위로 분리한 뒤, 결과 배열에 저장
    while (getline(_ss, _word, '-')){
        _names.push_back(_word);
    }

    ArrayStruct _arrayStruct;
    _arrayStruct.arrayAccesType = _words[0];
    _arrayStruct.arrayFunc = _names[0];
    _arrayStruct.arrayName = _names[1];
    _arrayStruct.arrayType = GetType(_words[2]);
    _arrayStruct.arrayValue = _words[_words.size() - 4];
    _arrayStruct.arrayPtr = _words[_words.size() - 3];
    _arrayStruct.arrayLine = _words[_words.size() - 2];
    _arrayStruct.arrayCol = _words[_words.size() - 1];

    for(int i = 0; i < dimension; i++) {
        _arrayStruct.arrayIndex.push_back(_words[4 + i]);
    }

    arrays.push_back(_arrayStruct);

    std::cout << "array size : " << arrays.size() << std::endl;
    
    this->shadowMemory[_arrayStruct.arrayPtr] = _arrayStruct.arrayValue;

    maxPageIndex = (arrays.size() / 10) + 1;
    std::cout << "Access Type : " << _arrayStruct.arrayAccesType << std::endl;
    std::cout << "Func : " << _arrayStruct.arrayFunc << std::endl;
    std::cout << "Name : " << _arrayStruct.arrayName << std::endl;
    std::cout << "Type : " << _arrayStruct.arrayType << std::endl;
    std::cout << "Value : " << _arrayStruct.arrayValue << std::endl;
    std::cout << "arrayPtr : " << _arrayStruct.arrayPtr << std::endl;
    std::cout << "arrayPtr : " << _arrayStruct.arrayLine << std::endl;
    std::cout << "arrayCol : " << _arrayStruct.arrayCol << std::endl;
    for(int i = 0; i < _arrayStruct.arrayIndex.size(); i++) {
        std::cout << "Index" << i << " : " << _arrayStruct.arrayIndex[i] << std::endl;
    }
}

void RecordArray::PrintRecordData() {
    std::cout << "\033[1m" << "\t\t\tval name : " << this->name << "\033[0m" << std::endl;
    std::cout << "\033[1m" << "\t\t\tval type : " << this->type << "\033[0m" << std::endl;
    std::cout << "\033[1m" << "\t\t\tval value : " << this->ptr << "\033[0m" << std::endl;
    std::cout << "\033[1m" << "\t\t\tval line : " << this->line << "\033[0m" << std::endl;
    std::cout << std::endl;
}

std::string RecordArray::PrintRecordTable(std::string _message) {
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
            
            /////////////// 명령어가 인덱스의 수를 제대로 전달했는지 검사 ///////////////
            if (_words.size() != dimension + 1) { // 명령어의 수가 차원보다 한개 높은지 검사
                _returnMessage =  "WARNING 배열의 차원과 명령어가 맞지 않습니다."; // 잘못된 인덱스의 수
            }
            else {
                ////////////////////////// 차원에 따른 처리 //////////////////////////
                int _assignIndex = -1;
                std::vector<std::string> _indexs;
                for(int i = 0; i < dimension; i++) {
                    _indexs.push_back(_words[i + 1]);
                }
                for(int i = 0 ; i < arrays.size(); i++) {
                    if(IsSameIndex(arrays[i].arrayIndex, _indexs)) {
                        _assignIndex = i;
                        break;
                    }
                }
                currentPage = (_assignIndex / 10) + 1;
                std::cout << "_assignIndex : " << _assignIndex << std::endl;
                std::cout << "current page : " << currentPage << std::endl;
                //////////////////////////////////////////////////////////////////
            }
            //////////////////////////////////////////////////////////////////
        }
        else { // 정의되지 않은 명령어일 경우

        }
    }
    //std::cout << "print record table message : " << _message << ", currentpage : " << currentPage << std::endl;

    ConsoleTable ct(BASIC);
    ct.SetPadding(1);
    ct.AddColumn(" ");
    ct.AddColumn("Thread ID");
    ct.AddColumn("Current Function");
    ct.AddColumn("Operation");
    ct.AddColumn("Name");
    ct.AddColumn("Type");
    ct.AddColumn("Value");
    ct.AddColumn("Pointer Address");

    for(int i = 0; i < dimension; i++) {
        ct.AddColumn("dimension-" + std::to_string(i + 1));
    }
    ct.AddColumn("Container Type");

    int _startIndex = (currentPage * 10) - 10;
    int _endIndex = (currentPage * 10);
    if(_startIndex > arrays.size() - 1) _startIndex = arrays.size() - 1;
    if(_endIndex > arrays.size()) _endIndex = arrays.size();
    int _tableIndex = 0;
    for(int i = _startIndex ; i < _endIndex; i++) {
        ConsoleTableRow* entry = new ConsoleTableRow(9 + dimension);
        _tableIndex++;
        entry->AddEntry(std::to_string(_tableIndex), 0);
        entry->AddEntry(threadId, 1);
        entry->AddEntry(arrays[i].arrayFunc, 2);
        entry->AddEntry(arrays[i].arrayAccesType, 3);
        entry->AddEntry(arrays[i].arrayName, 4);
        entry->AddEntry(arrays[i].arrayType, 5);
        entry->AddEntry(arrays[i].arrayValue, 6);
        entry->AddEntry(arrays[i].arrayPtr, 7);
        int _addIndex = 0;
        for(int j = 0; j < arrays[i].arrayIndex.size(); j++) {
            entry->AddEntry(arrays[i].arrayIndex[j], 8 + j);
            _addIndex = j;
        }
        entry->AddEntry("Array", 9 + _addIndex);   
        
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

bool RecordArray::IsNumber(std::string const &_str) {
    auto it = _str.begin();
    while(it != _str.end() && std::isdigit(*it)) {
        it++;
    }
    return !_str.empty() && it == _str.end();
}

std::string RecordArray::AddHexaInt(std::string _hexaStr, int _addVal) {
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

bool RecordArray::IsSameIndex(std::vector<std::string> _index1, std::vector<std::string> _index2) {
    if(_index1.size() != _index2.size()) return false;

    for(int i = 0; i < _index1.size(); i++) {
        std::cout << "index1 : " << _index1[i] << ", index2 : " << _index2[i] << std::endl;
        if(_index1[i] != _index2[i]) return false;
    }
    return true;
}

void RecordArray::SetShadowMemory(std::map<std::string, std::string> _shadowMemory) {
    this->shadowMemory = _shadowMemory;
    this->shadowMaxIdx = this->shadowMemory.size();
    PrintShadowMemory();
}

std::map<std::string, std::string> RecordArray::GetShadowMemory() {
    return this->shadowMemory;
}

void RecordArray::PrintShadowMemory() {
    for (auto iter = shadowMemory.begin() ; iter !=  shadowMemory.end(); iter++) {
        std::cout << "Shadow Memory : " << iter->first << "," << iter->second << std::endl;
    }
}

void RecordArray::SetShadowMemorySize(int _size) {
    this->shadowMaxIdx = _size;
}

int RecordArray::GetShadowMemorySize() {
    return this->shadowMaxIdx;
}

void RecordArray::SetArrrays(std::vector<ArrayStruct> _arrays) {
    for(int i = 0; i < _arrays.size(); i++) {
        this->arrays.push_back(_arrays[i]);
    }
}

std::vector<ArrayStruct> RecordArray::GetArrays() {
    return this->arrays;
}

std::string RecordArray::GetType(std::string& _word) {
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
        else {
            return "undefined type";
        }
    }
}

int RecordArray::CountChar(std::string &_str, char _findWord) {
    int _count = 0;
    for (char _ch : _str) {
        if (_ch == _findWord) {
            _count++;
        }
    }
    return _count;
}

std::string RecordArray::RemoveChar(const std::string &_str, char _removeWord) {
    std::string _result = _str;
    size_t _pos = 0;
    
    while ((_pos = _result.find(_removeWord, _pos)) != std::string::npos) {
        _result.erase(_pos, 1);
    }
    
    return _result;
}

void RecordArray::SetStruct(RecordStruct& _struct) {
    
}