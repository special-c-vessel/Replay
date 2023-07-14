#include "record_array.h"

RecordArray::RecordArray() {
    dimension = 1;
    currentPage = 1;
    prevPage = 1;
    std::cout << "Call Normal Constructor(RecordArray)" << std::endl;
    InitArray();
}

RecordArray::RecordArray(int _dimension) {
    std::cout << "Call Dimension Constructor(RecordArray), dimension = " << _dimension << std::endl;
    dimension = _dimension;
    currentPage = 1;
    prevPage = 1;
    InitArray();
}

void RecordArray::InitArray() {
    std::cout << "Call InitArray func(RecordArray), dimension = " << dimension << std::endl;
    switch(dimension) {
    case 1: {
        maxPageIndex = ((max_array1 % 100) / 10) +  1;
        array1.resize(max_array1);
        for(int i = 0; i < max_array1; i++) {
            array1.push_back("0");
        }
        break;
    }
    case 2: {
        maxPageIndex = ((max_array1 * max_array2) / 10) + 1;
        array2.resize(max_array1);
        for(int i = 0; i < max_array1; i++) {
            array2[i].resize(max_array2);
        }
        break;
    }
    case 3: {
        maxPageIndex = ((max_array1 * max_array2 * max_array3) / 10) + 1;
        array3.resize(max_array1);
        for(int i = 0; i < max_array1; i++) {
            array3[i].resize(max_array2);
            for(int j = 0; j < max_array2; j++) {
                array3[i][j].resize(max_array3);
                for(int z = 0; z < max_array3; z++) {
                    array3[i][j][z] = "0";
                }
            }
        }
        break;
    }
    }
}

RecordArray::~RecordArray() {
    switch(dimension) {
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    }
}

void RecordArray::InitRecordData(std::vector<std::string> _words) {
    std::cout << "Call InitRecordData func(RecordArray), dimension = " << dimension << std::endl;
    this->name = _words[1];
    this->type = _words[2];
    this->col = _words[_words.size() - 1];
    this->line = _words[_words.size() - 2];
    this->ptr = _words[_words.size() - 3];

    std::cout << ptr << std::endl;

    UpdateRecordData(_words);
}

void RecordArray::UpdateRecordData(std::vector<std::string> _words) {
    std::cout << "Call UpdateRecordData func(RecordArray), dimension = " << dimension << std::endl;
    switch(dimension) {
    case 1: {
        std::cout << "one dimension" << std::endl;
        int _arrayIndex = stoi(_words[4]);
        array1[_arrayIndex] = _words[5];
        break;
    }
    case 2: {
        std::cout << "two dimension" << std::endl;
        int _arrayIndex1 = stoi(_words[4]);
        int _arrayIndex2 = stoi(_words[5]);
        array2[_arrayIndex1][_arrayIndex2] = _words[6];
        break;
    }
    case 3: {
        std::cout << "three dimension" << std::endl;
        int _arrayIndex1 = stoi(_words[4]);
        int _arrayIndex2 = stoi(_words[5]);
        int _arrayIndex3 = stoi(_words[6]);
        std::cout << "one index : " << _arrayIndex1 << std::endl;
        std::cout << "two index : " << _arrayIndex2 << std::endl;
        std::cout << "three index : " << _arrayIndex3 << std::endl;

        array3[_arrayIndex1][_arrayIndex2][_arrayIndex3] = _words[7];
        break;
    }
    default:
        std::cout << "default dimension" << std::endl;
        break;
    }
}

void RecordArray::PrintRecordData() {
    std::cout << "\033[1m" << "\t\t\tval name : " << this->name << "\033[0m" << std::endl;
    std::cout << "\033[1m" << "\t\t\tval type : " << this->type << "\033[0m" << std::endl;
    std::cout << "\033[1m" << "\t\t\tval value : " << this->ptr << "\033[0m" << std::endl;
    std::cout << "\033[1m" << "\t\t\tval line : " << this->line << "\033[0m" << std::endl;
    switch(dimension) {
    case 1: {
        for(int i = 0 ; i < max_array1; i++) {
            std::cout << "value[" << i << "] : " << array1[i] << std::endl;
        }
        break;
    }
    case 2: {
        for(int i = 0 ; i < max_array1; i++) {
            for(int j = 0; j < max_array2; j++) {
                std::cout << "value[" << i << "][" << j << "] : " << array2[i][j] << std::endl;
            }
        }
        break;
    }
    }
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
            if (_words.size() > dimension + 1) { // 명령어의 수가 차원보다 한개 높은지 검사
                _returnMessage =  "WARNING 배열의 차원과 명령어가 맞지 않습니다."; // 잘못된 인덱스의 수
            }
            else {
                ////////////////////////// 차원에 따른 처리 //////////////////////////
                if(_words.size() == 2) { // 일차원 배열
                    int _assignIndex = std::stoi(_words[1]);
                    if(_assignIndex < max_array1) {
                        currentPage = (_assignIndex / 10) + 1;
                    }
                    else {
                        currentPage = prevPage;
                        _returnMessage = "WARNING 배열의 범위를 벗어난 값입니다.";
                    }
                }
                else if(_words.size() == 3) { // 이차원 배열
                    int _assignIndex1 = std::stoi(_words[1]);
                    int _assignIndex2 = std::stoi(_words[2]);
                    if(_assignIndex1 < max_array1 && _assignIndex2 < max_array2) {
                        int _index = 0;
                        for(int i = 0; i < max_array1; i++) {
                            for(int j = 0; j < max_array2; j++) {
                                if(i == _assignIndex1 && j == _assignIndex2) goto endLoop2;
                                else _index++;
                            }   
                        }
                        endLoop2:
                        currentPage = (_index / 10) + 1;
                    }
                    else {
                        currentPage = prevPage;
                        _returnMessage = "WARNING 배열의 범위를 벗어난 값입니다.";
                    }
                }
                else if(_words.size() == 4) { // 삼차원 배열
                    int _assignIndex1 = std::stoi(_words[1]);
                    int _assignIndex2 = std::stoi(_words[2]);
                    int _assignIndex3 = std::stoi(_words[3]);
                    if(_assignIndex1 < max_array1 && _assignIndex2 < max_array2 && _assignIndex3 < max_array3) {
                        int _index = 0;
                        for(int i = 0; i < max_array1; i++) {
                            for(int j = 0; j < max_array2; j++) {
                                for(int z=  0; z < max_array3; z++) {
                                    if(i == _assignIndex1 && j == _assignIndex2 && z == _assignIndex3) goto endLoop3;
                                    else _index++;
                                }
                            }   
                        }
                        endLoop3:
                        currentPage = (_index / 10) + 1;
                    }
                    else {
                        currentPage = prevPage;
                        _returnMessage = "WARNING 배열의 범위를 벗어난 값입니다.";
                    }
                }
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
    
    switch(this->dimension) {
    case 1: {
        std::vector<ArrayStruct> _arrays;
        for(int i = 0; i < max_array1; i++) {
            ArrayStruct _arrayStruct;
            _arrayStruct.arrayName = this->name;
            _arrayStruct.arrayType = this->type;
            _arrayStruct.arrayValue = array1[i];
            _arrayStruct.arrayPtr = this->ptr;
            _arrayStruct.arrayIndex.push_back(std::to_string(i));
            _arrays.push_back(_arrayStruct);
        }

        ct.AddColumn("Index");
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
            entry->AddEntry(_arrays[i].arrayIndex[0], 4);
            ct.AddRow(entry);
        }
        ct.PrintTable();
        break;
    }
    case 2: {
        std::vector<ArrayStruct> _arrays;
        for(int i = 0; i < max_array1; i++) {
            for(int j = 0; j < max_array2; j++) {
                ArrayStruct _arrayStruct;
                _arrayStruct.arrayName = this->name;
                _arrayStruct.arrayType = this->type;
                _arrayStruct.arrayValue = array2[i][j];
                _arrayStruct.arrayPtr = this->ptr;
                _arrayStruct.arrayIndex.push_back(std::to_string(i));
                _arrayStruct.arrayIndex.push_back(std::to_string(j));
                _arrays.push_back(_arrayStruct);
            }
        }

        ct.AddColumn("Index 1");
        ct.AddColumn("Index 2");

        int _startIndex = (currentPage * 10) - 10;
        int _endIndex = (currentPage * 10);
        if(_startIndex > _arrays.size() - 1) _startIndex = _arrays.size() - 1;
        if(_endIndex > _arrays.size()) _endIndex = _arrays.size();

        for(int i = _startIndex ; i < _endIndex; i++) {
            ConsoleTableRow* _entry = new ConsoleTableRow(6);
            _entry->AddEntry(_arrays[i].arrayName, 0);
            _entry->AddEntry(_arrays[i].arrayType, 1);
            _entry->AddEntry(_arrays[i].arrayValue, 2);
            _entry->AddEntry(_arrays[i].arrayPtr, 3);
            _entry->AddEntry(_arrays[i].arrayIndex[0], 4);
            _entry->AddEntry(_arrays[i].arrayIndex[1], 5);
            ct.AddRow(_entry);
        }
        ct.PrintTable();
        break;
    }
    case 3: {
        std::vector<ArrayStruct> _arrays;
        for(int i = 0; i < max_array1; i++) {
            for(int j = 0; j < max_array2; j++) {
                for(int z = 0; z < max_array3; z++) {
                    ArrayStruct _arrayStruct;
                    _arrayStruct.arrayName = this->name;
                    _arrayStruct.arrayType = this->type;
                    _arrayStruct.arrayValue = array3[i][j][z];
                    _arrayStruct.arrayPtr = this->ptr;
                    _arrayStruct.arrayIndex.push_back(std::to_string(i));
                    _arrayStruct.arrayIndex.push_back(std::to_string(j));
                    _arrayStruct.arrayIndex.push_back(std::to_string(z));
                    _arrays.push_back(_arrayStruct);
                }
            }
        }

        ct.AddColumn("Index 1");
        ct.AddColumn("Index 2");
        ct.AddColumn("Index 3");

        int _startIndex = (currentPage * 10) - 10;
        int _endIndex = (currentPage * 10);
        if(_startIndex > _arrays.size() - 1) _startIndex = _arrays.size() - 1;
        if(_endIndex > _arrays.size()) _endIndex = _arrays.size();

        for(int i = _startIndex ; i < _endIndex; i++) {
            ConsoleTableRow* _entry = new ConsoleTableRow(7);
            _entry->AddEntry(_arrays[i].arrayName, 0);
            _entry->AddEntry(_arrays[i].arrayType, 1);
            _entry->AddEntry(_arrays[i].arrayValue, 2);
            _entry->AddEntry(_arrays[i].arrayPtr, 3);
            _entry->AddEntry(_arrays[i].arrayIndex[0], 4);
            _entry->AddEntry(_arrays[i].arrayIndex[1], 5);
            _entry->AddEntry(_arrays[i].arrayIndex[2], 6);
            ct.AddRow(_entry);
        }
        ct.PrintTable();
        break;
    }
    }

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