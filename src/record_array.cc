#include "record_array.h"

RecordArray::RecordArray() {
    std::cout << "Call Normal Constructor(RecordArray)" << std::endl;
    currentPage = 1;
    prevPage = 1;
}

RecordArray::RecordArray(std::vector<std::string> _words, int _dimension) {
    std::cout << "Call Dimension Constructor(RecordArray), dimension = " << std::endl;
    currentPage = 1;
    prevPage = 1;
    shadowMaxIdx = 0;
    dimension = _dimension;

    InitRecordData(_words);
}

RecordArray::~RecordArray() {
    
}

void RecordArray::InitRecordData(std::vector<std::string> _words) {
    std::cout << "Call InitRecordData func(RecordArray), dimension = " <<  dimension << std::endl;

    std::stringstream _ss(_words[1]);
    // 공백 분리 결과를 저장할 배열
    std::vector<std::string> _names;
    std::string _word;
    // 스트림을 한 줄씩 읽어, 공백 단위로 분리한 뒤, 결과 배열에 저장
    while (getline(_ss, _word, '_')){
        _names.push_back(_word);
    }

    ArrayStruct _arrayStruct;
    _arrayStruct.arrayFunc = _names[0];
    _arrayStruct.arrayName = _names[1];
    _arrayStruct.arrayType = _words[3];
    _arrayStruct.arrayValue = _words[_words.size() - 4];
    _arrayStruct.arrayPtr = _words[_words.size() - 3];
    _arrayStruct.arrayLine = _words[_words.size() - 2];
    _arrayStruct.arrayCol = _words[_words.size() - 1];

    for(int i = 0; i < dimension; i++) {
        _arrayStruct.arrayIndex.push_back(_words[4 + i]);
    }

    this->dataFunc = _arrayStruct.arrayFunc;
    this->name = _arrayStruct.arrayName;
    this->ptr = _arrayStruct.arrayPtr;
    this->line = _arrayStruct.arrayLine;
    this->col = _arrayStruct.arrayCol;

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

    arrays.push_back(_arrayStruct);
    
    this->shadowMemory[_arrayStruct.arrayPtr] = _arrayStruct.arrayValue;

    maxPageIndex = (arrays.size() / 10) + 1;
    std::cout << "maxPageIndex : " << maxPageIndex << std::endl;
}

void RecordArray::UpdateRecordData(std::vector<std::string> _words) {
    std::cout << "Call UpdateRecordData func(RecordArray), dimension = " << dimension << std::endl;

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
    std::cout << "print record table message : " << _message << ", currentpage : " << currentPage << std::endl;

    ConsoleTable ct(BASIC);
    ct.SetPadding(1);
    ct.AddColumn("Name");
    ct.AddColumn("Type");
    ct.AddColumn("Value");
    ct.AddColumn("Ptr");

    for(int i = 0; i < dimension; i++) {
        ct.AddColumn("Index");
    }

    int _startIndex = (currentPage * 10) - 10;
    int _endIndex = (currentPage * 10);
    if(_startIndex > arrays.size() - 1) _startIndex = arrays.size() - 1;
    if(_endIndex > arrays.size()) _endIndex = arrays.size();
    for(int i = _startIndex ; i < _endIndex; i++) {
        ConsoleTableRow* entry = new ConsoleTableRow(6);
        entry->AddEntry(arrays[i].arrayName, 0);
        entry->AddEntry(arrays[i].arrayType, 1);
        entry->AddEntry(arrays[i].arrayValue, 2);
        entry->AddEntry(arrays[i].arrayPtr, 3);
        for(int j = 0; j < arrays[i].arrayIndex.size(); j++)
            entry->AddEntry(arrays[i].arrayIndex[j], 4 + j);
            
        ct.AddRow(entry);
    }
    ct.PrintTable();
    
    prevPage = currentPage;
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