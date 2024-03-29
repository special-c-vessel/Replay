#include "record_vector.h"

RecordVector::RecordVector() {
    std::cout << "Call Normal Constructor(RecordVector)" << std::endl;
    currentPage = 1;
    prevPage = 1;
    shadowMaxIdx = 0;
    threadId = "0";
}

RecordVector::RecordVector(std::vector<std::string> _words) {
    std::cout << "Call Normal Constructor(RecordVector)" << std::endl;
    currentPage = 1;
    prevPage = 1;
    shadowMaxIdx = 0;
    threadId = "0";

    InitRecordData(_words);
}


RecordVector::~RecordVector() {
    
}

void RecordVector::InitRecordData(std::vector<std::string> _words) {
    std::cout << "===========Call InitRecordData func(RecordVector)===========" << std::endl;

    recordType = RecordType::Vector;

    std::stringstream _ss(_words[CURFUNC_NAME_IDX + 1]);
    // 공백 분리 결과를 저장할 배열
    std::vector<std::string> _names;
    std::string _word;
    // 스트림을 한 줄씩 읽어, 공백 단위로 분리한 뒤, 결과 배열에 저장
    while (getline(_ss, _word, '-')){
        _names.push_back(_word);
    }

    threadId = _words[THREAD_ID_IDX];
    accessType = _words[OP_TYPE_IDX];
    dataFunc = _names[0];
    name = _names[1];
    type = GetType(_words[TYPE_IDX + 1]);
    col = _words[_words.size() - 1];
    line = _words[_words.size() - 2];
    ptr = _words[_words.size() - 3];
    value = _words[_words.size() - 4];
    arrayTypeSize = 4;

    VectorStruct _vectorStruct;
    _vectorStruct.vectorThreadId = _words[THREAD_ID_IDX];
    _vectorStruct.vectorAccessType = _words[OP_TYPE_IDX];
    _vectorStruct.vectorFunc = _names[0];
    _vectorStruct.vectorName = _names[1];
    _vectorStruct.vectorType = GetType(_words[TYPE_IDX + 1]);
    _vectorStruct.vectorValue = _words[_words.size() - 4];
    _vectorStruct.vectorPtr = _words[_words.size() - 3];

    vectors.push_back(_vectorStruct);

    maxPageIndex = (vectors.size() / 10) + 1;

    std::cout << "============================================================" << std::endl;
}

void RecordVector::UpdateRecordData(std::vector<std::string> _words) {
    std::cout << "===========Call UpdateRecordData func(RecordVector)===========" << std::endl;
    
    std::stringstream _ss(_words[CURFUNC_NAME_IDX + 1]);
    // 공백 분리 결과를 저장할 배열
    std::vector<std::string> _names;
    std::string _word;
    // 스트림을 한 줄씩 읽어, 공백 단위로 분리한 뒤, 결과 배열에 저장
    while (getline(_ss, _word, '-')){
        _names.push_back(_word);
    }

    VectorStruct _vectorStruct;
    _vectorStruct.vectorThreadId = _words[THREAD_ID_IDX];
    _vectorStruct.vectorAccessType = _words[OP_TYPE_IDX];
    _vectorStruct.vectorFunc = _names[0];
    _vectorStruct.vectorName = _names[1];
    _vectorStruct.vectorType = GetType(_words[TYPE_IDX + 1]);
    _vectorStruct.vectorValue = _words[_words.size() - 4];
    _vectorStruct.vectorPtr = AddHexaInt(vectors[vectors.size() - 1].vectorPtr, GetSizeByType(type));

    vectors.push_back(_vectorStruct);

    maxPageIndex = (vectors.size() / 10) + 1;

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

    for(int i = 0 ; i < vectors.size(); i++) {
        vectors[i].vectorIndex = std::to_string(i);
    }

    int _startIndex = (currentPage * 10) - 10;
    int _endIndex = (currentPage * 10);
    if(_startIndex > vectors.size() - 1) _startIndex = vectors.size() - 1;
    if(_endIndex > vectors.size()) _endIndex = vectors.size();
    int _tableIndex = 0;
    for(int i = _startIndex ; i < _endIndex; i++) {
        ConsoleTableRow* entry = new ConsoleTableRow(9);
        _tableIndex++;
        entry->AddEntry(std::to_string(_tableIndex), 0);
        entry->AddEntry(vectors[i].vectorFunc, 1);
        entry->AddEntry(vectors[i].vectorAccessType, 2);
        entry->AddEntry(vectors[i].vectorName, 3);
        entry->AddEntry(vectors[i].vectorType, 4);
        entry->AddEntry(vectors[i].vectorValue, 5);
        entry->AddEntry(vectors[i].vectorPtr, 6);
        entry->AddEntry(vectors[i].vectorIndex, 7);
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

void RecordVector::SetVectors(std::vector<VectorStruct> _vectors) {
    for(int i = 0; i < _vectors.size(); i++) {
        this->vectors.push_back(_vectors[i]);
    }
}

std::vector<VectorStruct> RecordVector::GetVectors() {
    return this->vectors;

}

void RecordVector::SetStruct(RecordStruct& _struct) {

}