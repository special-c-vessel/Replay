#include "record_array.h"

RecordArray::RecordArray() {
    std::cout << "Call Normal Constructor(RecordArray)" << std::endl;
    currentPage = 1;
    prevPage = 1;
}

RecordArray::RecordArray(std::vector<std::string> _words, std::vector<std::string> _dimension) {
    std::cout << "Call Dimension Constructor(RecordArray), dimension = " << std::endl;
    currentPage = 1;
    prevPage = 1;
    shadowMaxIdx = 0;

    for(int i = 0; i < _dimension.size(); i++) {
        dimension.push_back(std::stoi(_dimension[i]));
    }

    InitRecordData(_words);
}

RecordArray::~RecordArray() {
    
}

/*
words[0] - dimension
words[1] - name
words[2] - type
words[3] - isArr
words[4] - idx
words[5] - value
words[6] - ptr
words[7] - line
words[8] - col
*/

void RecordArray::InitRecordData(std::vector<std::string> _words) {
    std::cout << "Call InitRecordData func(RecordArray), dimension = " <<  dimension.size() << std::endl;
    name = _words[1];
    type = _words[2];
    col = _words[_words.size() - 1];
    line = _words[_words.size() - 2];
    ptr = _words[_words.size() - 3];

    /*
    if(type == "int") {
        arrayTypeSize = 4;
    }
    else {
        arrayTypeSize = 1;
    }

    */
    arrayTypeSize = 4;
    shadowMemory[(AddHexaInt(ptr, arrayTypeSize * shadowMaxIdx))] = _words[_words.size() - 4];
}

void RecordArray::UpdateRecordData(std::vector<std::string> _words) {
    std::cout << "Call UpdateRecordData func(RecordArray), dimension = " << dimension.size() << std::endl;
    int _idx = std::stoi(_words[4]);
    int _subIdx = shadowMaxIdx - _idx;
    
    if(_subIdx < 0) { // 기존 shadow memory에 추가를 하지 않을 경우
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
            if (_words.size() != dimension.size() + 1) { // 명령어의 수가 차원보다 한개 높은지 검사
                _returnMessage =  "WARNING 배열의 차원과 명령어가 맞지 않습니다."; // 잘못된 인덱스의 수
            }
            else {
                ////////////////////////// 차원에 따른 처리 //////////////////////////
                int _assignIndex = 0;
                for(int i = 0 ; i < dimension.size(); i++) {
                    if(i != dimension.size() - 1)  {
                        int _addIndex = std::stoi(_words[i + 1]);
                        for(int j = i + 1; j < dimension.size(); j++) {
                            _addIndex *= dimension[j];
                        }
                        _assignIndex += _addIndex;
                    }
                    else {
                        _assignIndex += std::stoi(_words[i + 1]);
                    }
                }
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
    ct.AddColumn("Name");
    ct.AddColumn("Type");
    ct.AddColumn("Value");
    ct.AddColumn("Ptr");

    std::vector<ArrayStruct> _arrays;
    for (auto iter = shadowMemory.begin(); iter != shadowMemory.end(); ++iter){
        ArrayStruct _arrayStruct;
        _arrayStruct.arrayName = this->name;
        _arrayStruct.arrayType = this->type;
        _arrayStruct.arrayValue = iter->second;
        _arrayStruct.arrayPtr = iter->first;
        _arrays.push_back(_arrayStruct);
    }

    for(int i = dimension.size() - 1; i >= 0; i--) {
        int _dimensionIdx = 0;
        for(int j = 0; j < _arrays.size(); j++) {
            _arrays[j].arrayIndex.push_back(std::to_string(_dimensionIdx));
            if(_dimensionIdx == dimension[i] - 1) {
                _dimensionIdx = 0;
            }
            else {
                _dimensionIdx++;
            }
        }
        ct.AddColumn("Index");
    }

    int _startIndex = (currentPage * 10) - 10;
    int _endIndex = (currentPage * 10);
    if(_startIndex > _arrays.size() - 1) _startIndex = _arrays.size() - 1;
    if(_endIndex > _arrays.size()) _endIndex = _arrays.size();
    for(int i = _startIndex ; i < _endIndex; i++) {
        ConsoleTableRow* entry = new ConsoleTableRow(5);
        entry->AddEntry(_arrays[i].arrayName, 0);
        entry->AddEntry(_arrays[i].arrayType, 1);
        entry->AddEntry(_arrays[i].arrayValue, 2);
        entry->AddEntry(_arrays[i].arrayPtr, 3);
        for(int j = 0; j < dimension.size(); j++)
            entry->AddEntry(_arrays[i].arrayIndex[j], 4 + j);
            
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