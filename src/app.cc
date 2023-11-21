#include "app.h"

App::App() {
    std::cout << "Constructor app class" << std::endl;
}

App::App(char* _srcFile, char* _recordFile) 
: srcFile(_srcFile), recordFile(_recordFile) {
    std::cout << "Initialize app class" << std::endl;
}

App::~App() {
    std::cout << "Destory app class" << std::endl;
    for(std::vector<RecordData*>::iterator iter = records.begin(); iter != records.end(); ++iter) {
        delete *iter;
    }
    records.clear();
    std::vector<RecordData*>().swap(records);

    for(std::vector<RecordData*>::iterator iter = currentRecords.begin(); iter != currentRecords.end(); ++iter) {
        delete *iter;
    }
    currentRecords.clear();
    std::vector<RecordData*>().swap(currentRecords);

    std::map<std::string, std::string>().swap(valShadowMemory);
    std::map<std::string, std::string>().swap(ptrShadowMemroy);
}

void App::Run() {
    Init();

    while(!programOver) {
        Input();
        Update();
        Render();
    }
}

void App::Init() {
    std::cout << "Initialize app class" << std::endl;

    // 시간 측정 시작
    StartTime();

    // 설정 초기화
    isDone = false;
    programOver = false;
    currentLine = 1;
    currentIndex = -1;
    prevLine = -1;
    prevCurPage = 1;
    afterCurPage = 1;
    afterTableIndex = -1;
    prevTableIndex = -1;
    inputState = InputState::Stop;
    systemMessage = "";
    InitCommand();
    

    // 가바지 밸류 삭제
    recordLines.clear();
    std::vector<std::string>().swap(recordLines);

    // 기록 파일이 제대로 열렸는지 검사
    std::ifstream _recordStream(recordFile, std::ios::in);
    if (!_recordStream.is_open()) {
        std::cerr << "Unable to open record file: " << recordFile << std::endl;
        exit(0);
    }

    std::string _line;
    int _prevRecordSize = recordLines.size();
    while (std::getline(_recordStream, _line)) {
        //std::cout << "file open records line size : " << recordLines.size() << std::endl;
        std::cout << "first size : " << recordLines.size() << " line size : " << _line.size() << std::endl;
        recordLines.push_back(_line);
        if(recordLines.size() - 1 != _prevRecordSize) {
            std::cout << "Error occur!!!!! Line Size : " << recordLines.size() << " Line is : " << _line << std::endl;
            break;
        }
        else {
            _prevRecordSize = recordLines.size();
        }
    }

    _recordStream.close();

    std::cout << "file open records line size : " << recordLines.size() << std::endl;
    
    int _addIndex = 0;
    std::string _lineStr("");

    for(int i = 0 ; i < recordLines.size(); i++) {
        _line = recordLines[i];
        // 문자열을 스트림화
        std::stringstream _ss(_line);
        // 공백 분리 결과를 저장할 배열
        std::vector<std::string> _words;
        std::string _word;
        // 스트림을 한 줄씩 읽어, 공백 단위로 분리한 뒤, 결과 배열에 저장
        while (getline(_ss, _word, ' ')){
            _words.push_back(_word);
        }
    
        if(_words[1] != "retval") {
            if(_words[0] == "isStruct") { // 구조체
                RecordStruct* _data; // 기록 구조체 객체 생성
                std::vector<std::string> _lines; // 기록 구조체 객체에게 전달할 라인 목록
                _lines.push_back(_line);
                if(FindStringInString(_line, "string") 
                 && !FindStringInString(_line, "StringEnd")) { // 구조체 할당 값 중에서 문자열이고 줄 내림이 있을 경우
                    i++;
                    while(!FindStringInString(recordLines[i], "StringEnd")) {
                        _lines.push_back(recordLines[i]);
                        i++;
                    }
                    _lines.push_back(recordLines[i]);
                }
                _data = new RecordStruct(_lines);
                structs.push_back(_data);    
            }
            else if(_words[JUDGMENT_INDEX] == "isArr" || _words[JUDGMENT_INDEX] == "isPointerArr") { // 배열일 경우
                int _dimension = _words.size() - 9;
                int _findIdx = -1;
                for(int j = records.size() - 1; j >= 0; j--) {
                    std::string _nameFunc = records[j]->dataFunc + SPLIT_FUNC_NAME_CHAR + records[j]->name;
                    if(_nameFunc == _words[CURFUNC_NAME_IDX]) {
                        _findIdx = j;
                        break;
                    }
                }

                if(_findIdx != -1) {
                    //기존 기록 파일에 벡터 기록 데이터가 있을 경우 해당 기록 데이터의 shadow memory의 값들을 복사
                    RecordData* _data = new RecordArray(_words, _dimension);
                    _data->SetArrrays(records[_findIdx]->GetArrays());
                    records.push_back(_data);
                    
                    std::string _checkLine = recordLines[i + 1];
                    std::vector<std::string> _splitWords = SplitString(_checkLine, ' ');
                    
                    if(_splitWords[2] == _words[2]
                    && _splitWords[_splitWords.size() - 2] == _words[_words.size() - 2]) {
                        std::cout << "next array data" << std::endl;
                        _data->UpdateRecordData(_splitWords);
                        i += 2;
                        _checkLine = recordLines[i];
                        _splitWords = SplitString(_checkLine, ' ');
                        while(_splitWords[2] == _words[2]
                            && i < recordLines.size()
                            && _splitWords[_splitWords.size() - 2] == _words[_words.size() - 2]) {
                            std::cout << "next array data in while" << std::endl;
                            _data->UpdateRecordData(_splitWords);
                            i++;
                            _checkLine = recordLines[i];
                            _splitWords = SplitString(_checkLine, ' ');
                        }
                        i--;
                    }
                }
                else {
                    std::cout << "create array data" << std::endl;
                    RecordData* _data = new RecordArray(_words, _dimension);
                    records.push_back(_data);
                    std::cout << "push array data" << std::endl;

                    std::string _checkLine = recordLines[i + 1];
                    std::vector<std::string> _splitWords = SplitString(_checkLine, ' ');
                    
                    if(_splitWords[2] == _words[2]
                    && _splitWords[_splitWords.size() - 2] == _words[_words.size() - 2]) {
                        std::cout << "next array data" << std::endl;
                        _data->UpdateRecordData(_splitWords);
                        i += 2;
                        _checkLine = recordLines[i];
                        _splitWords = SplitString(_checkLine, ' ');
                        while(_splitWords[2] == _words[2]
                            && i < recordLines.size()
                            && _splitWords[_splitWords.size() - 2] == _words[_words.size() - 2]) {
                            std::cout << "next array data in while" << std::endl;
                            _data->UpdateRecordData(_splitWords);
                            i++;
                            _checkLine = recordLines[i];
                            _splitWords = SplitString(_checkLine, ' ');
                        }
                        i--;
                    }
                    std::cout << "end array data" << std::endl;
                }
            }
            else if(_words[VECTOR_CHECK_IDX] == "push_back" ) { // 벡터일 경우
                std::cout << "vector push back data" << std::endl;
                int _findIdx = -1; // 이전 기록 데이터 파일이 있는지 검사
                for(int i = records.size() - 1; i >= 0; i--) { // 기록 데이터 리스트를 돌면서 검사
                    std::string _comparisonStr = records[i]->dataFunc + "-" + records[i]->name;
                    if(_comparisonStr == _words[CURFUNC_NAME_IDX + 1]) { // 함수와 이름이 같다면 찾았다고 판단
                        _findIdx = i;
                        std::cout << "_findIdx : " << _findIdx << std::endl;
                        break;
                    }
                }
                if(_findIdx != -1) { 
                    std::cout << "detect vector record data" << std::endl;
                    //기존 기록 파일에 벡터 기록 데이터가 있을 경우 해당 기록 데이터의 shadow memory의 값들을 복사
                    RecordData* _data = new RecordVector(_words);
                    _data->SetVectors(records[_findIdx]->GetVectors());
                    records.push_back(_data);

                } else {
                    std::cout << "vector recordata create" << std::endl;
                    RecordData* _data = new RecordVector(_words);
                    records.push_back(_data);
                }
            }
            else if(_line.find("-ref.tmp") != std::string::npos) {
                std::cout << "inside ref tmp" << std::endl;
                ///////////////////////////string 일 경우/////////////////////////////
                std::string _refValue;
                std::string _refType = _words[3];
                if(_words[TYPE_IDX] == "string") { // 참조 값의 타입이 문자열일 경우
                    if(FindStringInString(_line, STRING_END)) {
                        int _i = START_STRING_IDX;
                        while(_words[_i] != STRING_END) {
                            _refValue += _words[_i];
                            _refValue += " ";
                            _i++;
                        }
                    }
                    else {
                        int _strValIndex = START_STRING_IDX;
                        for(int _i = _strValIndex; _i < _words.size(); _i++) {
                            if(_i == _words.size() - 1) {
                                _refValue = _refValue + _words[_i] + "\\n";
                            }
                            else {
                                _refValue = _refValue + _words[_i] + " ";
                            }
                        }
                        i++;
                        while(recordLines[i].find("StringEnd") == std::string::npos) {
                            _refValue = _refValue + recordLines[i] + "\\n";
                            i++;
                        }
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
                            _refValue += _words2[_i];
                            _refValue += " ";
                            _i++;
                        }
                    }
                }
                else { // 참조 값의 타입이 문자열이 아닐 경우
                    _refValue = _words[4];
                }
                ////////////////////////////////////////////////////////////////////
                std::cout << "_refValue : " << _refValue << std::endl;
                i++;
                std::cout << "recordLines : " << recordLines[i] << std::endl;
                std::stringstream _ss2(recordLines[i]);
                // 공백 분리 결과를 저장할 배열
                std::vector<std::string> _words2;
                std::string _word2;
                // 스트림을 한 줄씩 읽어, 공백 단위로 분리한 뒤, 결과 배열에 저장
                while (getline(_ss2, _word2, ' ')){
                    _words2.push_back(_word2);
                }
                if(_words2[VECTOR_CHECK_IDX] == "push_back" ) { // 벡터일 경우
                    _words2[5] = _refValue;
                    _words2[4] = _refType;
                    std::cout << "vector push back data" << std::endl;
                    int _findIdx = -1;
                    for(int i = records.size() - 1; i >= 0; i--) {
                        std::string _comparisonStr = records[i]->dataFunc + "-" + records[i]->name;
                        if(_comparisonStr == _words2[3]) {
                            _findIdx = i;
                            std::cout << "_findIdx : " << _findIdx << std::endl;
                            break;
                        }
                    }
                    if(_findIdx != -1) { 
                        std::cout << "detect vector record data" << std::endl;
                        //기존 기록 파일에 벡터 기록 데이터가 있을 경우 해당 기록 데이터의 shadow memory의 값들을 복사
                        RecordData* _data = new RecordVector(_words2);
                        _data->SetVectors(records[_findIdx]->GetVectors());
                        records.push_back(_data);

                    } else {
                        std::cout << "vector recordata create" << std::endl;
                        RecordData* _data = new RecordVector(_words2);
                        records.push_back(_data);
                    }
                }
            }
            else { // 일반 기록파일
                if(_words[TYPE_IDX] == "string") { // String 타입의 데이터일 경우
                    std::cout << "String data" << std::endl;
                    RecordData* _data = new RecordString();
                    std::vector<std::string> _lines; // 기록 구조체 객체에게 전달할 라인 목록
                    _lines.push_back(_line);
                    std::cout << _line;
                    if(!FindStringInString(_line, STRING_END)) {
                        i++;
                        _line = recordLines[i];
                        while(!FindStringInString(_line, STRING_END)) {
                            std::cout << _line;
                            _lines.push_back(_line);
                            i++;
                            _line = recordLines[i];
                        }
                        std::cout << _line;
                        _lines.push_back(_line);
                    }
                    _data->InitRecordData(_lines);
                    records.push_back(_data);
                }
                else if(FindRecordDataPtr(_words[_words.size() - 3]) != -1) {
                    /*
                    int _findIdx = FindRecordDataPtr(_words[_words.size() - 3]);
                    std::cout << "find record data, _findIdx :  " << _findIdx << std::endl;
                    int _changeIndex = std::stoi(_words[3]);
                    std::cout << "change index :  " << _changeIndex << std::endl;
                    _nameVal = records[_findIdx]->dataFunc + "-" + records[_findIdx]->name;
                    std::cout << "name :  " << _nameVal << std::endl;
                    _typeVal = records[_findIdx]->ptr;
                    std::cout << "type :  " << _typeVal << std::endl;
                    _strVal = records[_findIdx]->value;
                    _strVal[_changeIndex] = _words[4].c_str()[0];
                    std::cout << "value :  " << _strVal << std::endl;
                    _colVal = _words[_words.size() - 1];
                    _lineVal = _words[_words.size() - 2];
                    _ptrVal = _words[_words.size() - 3];
                    _lenVal = std::to_string(records[_findIdx]->value.size());
                    std::cout << "len :  " << _lenVal << std::endl;
                    if(std::stoi(_lenVal) < _changeIndex) {
                        _infoMessage = ERROR_STRING_RANGE;
                    }
                    else {
                        _infoMessage = " ";
                    }
                    std::cout << "set default information" << std::endl;
                    std::vector<std::string> _resultWord;
                    _resultWord.push_back(_words[0]);
                    _resultWord.push_back(_nameVal);
                    _resultWord.push_back(_typeVal);
                    _resultWord.push_back(_strVal);
                    _resultWord.push_back(_ptrVal);
                    _resultWord.push_back(_lineVal);
                    _resultWord.push_back(_colVal);
                    _resultWord.push_back(_infoMessage);
                    _resultWord.push_back(_lenVal);

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
                else { // 문자열을 제외한 일반 타입
                    int _findStructIdx = FindStructStructData(_words[_words.size() - 3]);
                    std::cout << "find struct index : " << _findStructIdx << std::endl;
                    int _findRecordIdx = FindStructRecordData(_words[_words.size() - 4], _words[_words.size() - 3], records.size() - 1);
                    std::cout << "find record index : " << _findRecordIdx << std::endl;
                    if(_findRecordIdx != -1) {
                        for(int dataIdx = 0 ; dataIdx < records[_findRecordIdx]->GetDataStruct().size(); dataIdx++) {
                            std::cout << "record data struct type : " << records[_findRecordIdx]->GetDataStruct()[dataIdx].type;
                            std::cout << " record data struct value : " << records[_findRecordIdx]->GetDataStruct()[dataIdx].value << std::endl;
                        } 
                        RecordData* _data = new RecordStruct(records[_findRecordIdx]->originStr);
                        _data->SetDataStruct(records[_findRecordIdx]->GetDataStruct());
                        _data->UpdateRecordData(_words);
                        records.push_back(_data);
                    }
                    else if(_findStructIdx != -1) {
                        RecordData* _data = new RecordStruct(structs[_findStructIdx]->originStr);
                        _data->UpdateRecordData(_words);
                        records.push_back(_data);
                    }
                    else {
                        RecordData* _data = new RecordPrim();
                        std::vector<std::string> _lines; // 기록 구조체 객체에게 전달할 라인 목록
                        _lines.push_back(_line);
                        _data->InitRecordData(_lines);
                        records.push_back(_data);
                    }
                }
            }
        }
    }

    if(records[records.size() - 1]->name == "retval") {
        isDone = true;
    }
    else {
        isDone = false;
        records[records.size() - 1]->infoMessage = ERROR_SEGMENTATION_FAULTS;
    }
    
    records[records.size() - 1]->infoMessage = records[records.size() - 1]->infoMessage + "last record data";

    // 소스 파일이 제대로 열렸는지 검사
    std::ifstream _srcStream(srcFile);
    if (!_srcStream.is_open()) {
        std::cerr << "Unable to open source file: " << srcFile << std::endl;
        exit(0);
    }

    std::cout << "record count : " << records.size() << std::endl;
    codes.push_back(" ");
    while (std::getline(_srcStream, _line)) {
        codes.push_back(_line);
        if(_line.find("main") != std::string::npos) {
            currentLine = codes.size() - 1;
        }
    }
    _srcStream.close();

    mmu = new MMU();
    mmu->InitShadowMemories(records);

    mtu = new MTU();
    mtu->InitThreads(records);
    
    for(int _recordIdx = 0; _recordIdx < records.size(); _recordIdx++) {
        records[_recordIdx]->threadId = mtu->GetThreadId(records[_recordIdx]->threadId);
    }

    Render();
}

// 사용자로부터 입력을 받는 함수
void App::Input() {
    std::string _input;
    EndTime();
    std::cout << std::endl << "User input : ";
    getline(std::cin, _input);
    StartTime();
    if(!IsNumber(_input)) { // 사용자 입력 값이 숫자가 아닐 경우
        if(_input == "s") {
            std::vector<std::string> _cmdWords = SplitString(_input, ' ');
            if(_cmdWords.size() == 2) {
                
            }
            else {
                std::cout << "Down" << std::endl;
                this->inputState = InputState::Down;
                this->commandMessage = "down";
            }
        }
        else if(_input == "ss") {
            std::cout << "Double Down" << std::endl;
            this->inputState = InputState::DoubleDown;
            this->commandMessage = "doubledown";
        }
        else if(_input == "w") {
            std::cout << "Up" << std::endl;
            this->inputState = InputState::Up;
            this->commandMessage = "up";
        }
        else if(_input == "ww") {
            std::cout << "Double Up" << std::endl;
            this->inputState = InputState::DoubleUp;
            this->commandMessage = "doubleup";
        }
        else if(_input == "a") {
            this->inputState = InputState::Left;
            this->commandMessage = "left";
        }
        else if(_input == "d") {
            this->inputState = InputState::Right;
            this->commandMessage = "right";
        }
        else if(FindCommand(_input)) {
            this->inputState = InputState::Command;
            this->commandMessage = _input;
            std::cout << "\ncommandMessage : " << commandMessage << std::endl;
        }
        else {
            std::cout << "undefined command" << std::endl;
        }
    }
    else { // 사용자 입력 값이 숫자일 경우
            
    }
}

void App::Update() {
    if(inputState == InputState::Up) {
        this->commandMessage = "up";
        std::cout << "current Index : " << currentIndex << std::endl;
        if(currentIndex > 0) {
            if(std::stoi(records[currentIndex - 1]->line) < std::stoi(records[currentIndex]->line)
            && std::stoi(records[currentIndex]->line) < currentLine) {
                
            }
            else {
                currentIndex--;
            }
            currentLine = std::stoi(records[currentIndex]->line);
            mtu->UpdateThreads(*records[currentIndex]);
            std::cout << "prev line : " << prevLine << std::endl;
            std::cout << "current line : " << currentLine << std::endl;

            afterCurPage = 1;
            prevCurPage = 1;
        }
        inputState = InputState::Stop;
    }
    else if(inputState == InputState::DoubleUp) {
        this->commandMessage = "doubleup";
        if(currentLine > 1) {
            // 이전 줄의 코드의 길이를 측정
            std::string _eraseStr = codes[currentLine - 1];
            _eraseStr.erase(remove(_eraseStr.begin(), _eraseStr.end(), ' '), _eraseStr.end());

            // 이전 줄의 코드가 짧을 경우
            if(_eraseStr.size() < 2) {
                while(_eraseStr.size() < 2 && (currentLine) <= (codes.size() - 1)) {
                    currentLine--;
                    _eraseStr = codes[currentLine];
                    _eraseStr.erase(remove(_eraseStr.begin(), _eraseStr.end(), ' '), _eraseStr.end());
                }
            }
            else {
                currentLine--;
            }
            
            for(int i = currentLine; i >= 0; i--) {
                int _findIndex = FindRecordData(i);
                if(_findIndex != -1) {
                    currentIndex = _findIndex - 1 ;
                    break;
                }
            }
            afterCurPage = 1;
            prevCurPage = 1;
        }
        inputState = InputState::Stop;
    }
    else if(inputState == InputState::Down) {
        this->commandMessage = "down";
        if(IsNumber(mtu->GetCurThreadId())) { // 현재 지정된 쓰레드 ID 가 존재할 경우
            std::cout << "current index : " << currentIndex << std::endl;
            std::cout << "records.size : " << records.size() - 1 << std::endl;
            if(currentIndex < records.size() - 1 || currentIndex == -1) {
                currentIndex++;
                while(records[currentIndex]->threadId != mtu->GetCurThreadId()) {
                    std::cout << "cur thread id : " << mtu->GetCurThreadId() << std::endl;
                    std::cout << "record thread id : " << records[currentIndex]->threadId << std::endl;
                    currentIndex++;
                }
                currentLine = std::stoi(records[currentIndex]->line);
                mtu->UpdateThreads(*records[currentIndex]);
                std::cout << "current Index : " << currentIndex<< std::endl;
                std::cout << "current line : " << currentLine << std::endl;

                afterCurPage = 1;   
                prevCurPage = 1; 
            }
            inputState = InputState::Stop;
        } else { // 현재 지정된 쓰레드 ID 가 존재하지 않을 경우
            std::cout << "current index : " << currentIndex << std::endl;
            std::cout << "records.size : " << records.size() - 1 << std::endl;
            if(currentIndex < records.size() - 1 || currentIndex == -1) {
                currentIndex++;
                currentLine = std::stoi(records[currentIndex]->line);
                mtu->UpdateThreads(*records[currentIndex]);
                std::cout << "current Index : " << currentIndex<< std::endl;
                std::cout << "current line : " << currentLine << std::endl;

                afterCurPage = 1;   
                prevCurPage = 1; 
            }
            inputState = InputState::Stop;
        }
    } 
    else if(inputState == InputState::DoubleDown) {
        this->commandMessage = "double down";
        if(currentLine < codes.size() - 1) {
            // 다음 줄의 코드의 길이를 측정
            std::string _eraseStr = codes[currentLine + 1];
            _eraseStr.erase(remove(_eraseStr.begin(), _eraseStr.end(), ' '), _eraseStr.end());

            // 다음 줄의 코드가 짧을 경우
            if(_eraseStr.size() < 2) {
                // 다음 줄의 길 떄까지 코드를 이동
                while(_eraseStr.size() < 2 && (currentLine) < (codes.size() - 1)) {
                    currentLine++;
                    _eraseStr = codes[currentLine];
                    _eraseStr.erase(remove(_eraseStr.begin(), _eraseStr.end(), ' '), _eraseStr.end());
                }
            }
            // 다음 줄의 코드가 길 경우
            else {
                currentLine++;
            }

            for(int i = currentLine; i >= 0; i--) {
                int _findIndex = FindRecordData(i);
                if(_findIndex != -1) {
                    std::cout << "find index : " << _findIndex << std::endl;
                    currentIndex = _findIndex;
                    break;
                }
            }

            prevCurPage = 1;
            afterCurPage = 1;
        }
        inputState = InputState::Stop;
    }
    else if(inputState == InputState::Right) {
        this->commandMessage = "right";
        afterCurPage++;
    }
    else if(inputState == InputState::Left) {
        this->commandMessage = "left";
        if(afterCurPage > 1) {
            afterCurPage--;
        }
    }
    else if(inputState == InputState::Command) {
        if(commandMessage.find("mvline") != std::string::npos) {
            std::cout << "mvline inside" << std::endl;
            std::stringstream _cmdss(commandMessage);
            // 공백 분리 결과를 저장할 배열
            std::vector<std::string> _cmdWords;
            std::string _cmdWord;
            // 스트림을 한 줄씩 읽어, 공백 단위로 분리한 뒤, 결과 배열에 저장
            while (getline(_cmdss, _cmdWord, ' ')){
                _cmdWords.push_back(_cmdWord);
            }
            if(IsNumber(_cmdWords[1])) {
                currentLine = std::stoi(_cmdWords[1]);
                for(int i = currentLine; i >= 0; i--) {
                    int _findIndex = FindRecordData(i);
                    if(_findIndex != -1) {
                        currentIndex = _findIndex;
                        break;
                    }
                }
            }
            else {
                currentLine = std::stoi(records[records.size() - 1]->line);
                currentIndex = records.size() - 1;
            }
        }
        else if(commandMessage.find("prevright") != std::string::npos) {
            prevCurPage++;
        }
        else if(commandMessage.find("prevleft") != std::string::npos) {
            if(prevCurPage > 1) prevCurPage--;
        }
        else if(commandMessage.find("followright") != std::string::npos) {
            afterCurPage++;
        }
        else if(commandMessage.find("followleft") != std::string::npos) {
            if(afterCurPage > 1) afterCurPage--;
        }
        else if(commandMessage.find("prevmove") != std::string::npos) {
            std::vector<std::string> _cmdWords = SplitString(commandMessage, ' ');
            if(_cmdWords.size() > 1) {
                prevTableIndex = std::stoi(_cmdWords[1]) - 1;
            }
        }
        else if(commandMessage.find("followmove") != std::string::npos) {
            std::vector<std::string> _cmdWords = SplitString(commandMessage, ' ');
            if(_cmdWords.size() > 1) {
                afterTableIndex = std::stoi(_cmdWords[1]) - 1;
            }
        }
        else if(FindStringInString(commandMessage, "threadset")) {
            std::vector<std::string> _cmdWords = SplitString(commandMessage, ' ');
            if(IsNumber(_cmdWords[1])) {
                mtu->SetCurThreadId(_cmdWords[1]);
                std::cout << "current thread id : " << mtu->GetCurThreadId() << std::endl;
                //std::cout << "record thread id : " << records[currentIndex]->threadId << std::endl;

                if(currentIndex >= 0 && records[currentIndex]->threadId != mtu->GetCurThreadId()) {
                    std::cout << "1----current Index : " << currentIndex << std::endl;
                    for(int _recordIdx = currentIndex; _recordIdx < records.size(); _recordIdx++) {
                        if(records[_recordIdx]->threadId == mtu->GetCurThreadId()) {
                            currentIndex = _recordIdx;
                            currentLine = std::stoi(records[currentIndex]->line);
                            mtu->UpdateThreads(*records[currentIndex]);
                            std::cout << "2-----current Index : " << currentIndex<< std::endl;
                            std::cout << "current line : " << currentLine << std::endl;

                            afterCurPage = 1;   
                            prevCurPage = 1; 
                            break;
                        }
                    }
                }
            }
            else {
                if(_cmdWords[1] == "none") {
                    mtu->SetCurThreadId("None");
                }
            }
        }
    }
}

void App::Render() {
    std::cout << "\033[2J\033[1;1H"; // 화면에 출력된 정보들을 지운다.

    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "+------------------------------------------------------------------------------------------------------------------------+\n";
    std::cout << "|                                                            " << "\033[1m" << "Code" << "\033[0m" << "                                                        |\n";
    std::cout << "+------------------------------------------------------------------------------------------------------------------------+\n";

    int _startLine = (currentLine < CODE_SHOW_RANGE) 
    ? 1 : currentLine - CODE_SHOW_RANGE; //코드 시작 줄 번호
    int _endLine = ((currentLine + CODE_SHOW_RANGE) > codes.size()) 
    ? _endLine = codes.size() - 1 : _endLine = currentLine + CODE_SHOW_RANGE; // 코드 마지막 줄 번호

    for(int i = _startLine; i <= _endLine; i++) {
        std::cout << i << "      " << mtu->GetThreadId(i);
        if(i == currentLine) std::cout << "\033[1m" << codes[i] << "\033[0;31m" <<"   <<<<<< Current line" <<"\033[0m" <<std::endl;
        else std::cout << codes[i] << std::endl;
    }

    std::cout << "+------------------------------------------------------------------------------------------------------------------------+\n";
    std::cout << "|                                                        "<< "\033[1m" << "Info meseeage" << "\033[0m" <<"                                                   |\n";
    std::cout << "+------------------------------------------------------------------------------------------------------------------------+\n";

    //std::cout << std::endl << "current line : " << currentLine << std::endl;

    if(FindRecord(currentLine)) { // 해당 줄의 레코드 데이터가 있는지 검사// 일차원 배열
        mmu->CalcShadowMemory(currentIndex, records);
        
        if(commandMessage.find("findptr") != std::string::npos) {
            std::cout << "findptr inside" << std::endl;
            std::stringstream _cmdss(commandMessage);
            // 공백 분리 결과를 저장할 배열
            std::vector<std::string> _cmdWords;
            std::string _cmdWord;
            // 스트림을 한 줄씩 읽어, 공백 단위로 분리한 뒤, 결과 배열에 저장
            while (getline(_cmdss, _cmdWord, ' ')){
                _cmdWords.push_back(_cmdWord);
            }

            ConsoleTable _ct(BASIC);
            _ct.SetPadding(1);
            _ct.AddColumn(" ");
            _ct.AddColumn("Current Function");
            _ct.AddColumn("Operation");
            _ct.AddColumn("Name");
            _ct.AddColumn("Type");
            _ct.AddColumn("Value");
            _ct.AddColumn("Pointer Address");
            _ct.AddColumn("Line");
            _ct.AddColumn("Column");

            int _tableIndex = 0;
            for(int i = 0; i <= currentIndex; i++) {
                if(records[i]->ptr == _cmdWords[1] && records[i]->dataFunc == records[currentIndex]->dataFunc) {
                    ConsoleTableRow* _entry = new ConsoleTableRow(9);
                    _tableIndex++;
                    _entry->AddEntry(std::to_string(_tableIndex), 0);
                    _entry->AddEntry(records[i]->dataFunc, 1);
                    _entry->AddEntry(records[i]->accessType, 2);
                    _entry->AddEntry(records[i]->name, 3);
                    _entry->AddEntry(records[i]->type, 4);
                    _entry->AddEntry(records[i]->value, 5);
                    _entry->AddEntry(records[i]->ptr, 6);
                    _entry->AddEntry(records[i]->line, 7);
                    _entry->AddEntry(records[i]->col, 8);
                    _ct.AddRow(_entry);
                }
            }

            _ct.PrintTable();
        }
        else if(commandMessage.find("findname") != std::string::npos) {
            std::cout << "findname inside" << std::endl;
            std::stringstream _cmdss(commandMessage);
            // 공백 분리 결과를 저장할 배열
            std::vector<std::string> _cmdWords;
            std::string _cmdWord;
            // 스트림을 한 줄씩 읽어, 공백 단위로 분리한 뒤, 결과 배열에 저장
            while (getline(_cmdss, _cmdWord, ' ')){
                _cmdWords.push_back(_cmdWord);
            }

            ConsoleTable _ct(BASIC);
            _ct.SetPadding(1);
            _ct.AddColumn(" ");
            _ct.AddColumn("Current Function");
            _ct.AddColumn("Operation");
            _ct.AddColumn("Name");
            _ct.AddColumn("Type");
            _ct.AddColumn("Value");
            _ct.AddColumn("Pointer Address");
            _ct.AddColumn("Line");
            _ct.AddColumn("Column");

            int _tableIndex = 0;
            for(int i = 0; i <= currentIndex; i++) {
                if(records[i]->name == _cmdWords[1] && records[i]->dataFunc == records[currentIndex]->dataFunc) {
                    ConsoleTableRow* _entry = new ConsoleTableRow(9);
                    _tableIndex++;
                    _entry->AddEntry(std::to_string(_tableIndex), 0);
                    _entry->AddEntry(records[i]->dataFunc, 1);
                    _entry->AddEntry(records[i]->accessType, 2);
                    _entry->AddEntry(records[i]->name, 3);
                    _entry->AddEntry(records[i]->type, 4);
                    _entry->AddEntry(records[i]->value, 5);
                    _entry->AddEntry(records[i]->ptr, 6);
                    _entry->AddEntry(records[i]->line, 7);
                    _entry->AddEntry(records[i]->col, 8);
                    _ct.AddRow(_entry);
                }
            }

            _ct.PrintTable();
        }
        else if(commandMessage.find("findtype") != std::string::npos) {
            std::cout << "findtype inside" << std::endl;
            std::stringstream _cmdss(commandMessage);
            // 공백 분리 결과를 저장할 배열
            std::vector<std::string> _cmdWords;
            std::string _cmdWord;
            // 스트림을 한 줄씩 읽어, 공백 단위로 분리한 뒤, 결과 배열에 저장
            while (getline(_cmdss, _cmdWord, ' ')){
                _cmdWords.push_back(_cmdWord);
            }

            ConsoleTable _ct(BASIC);
            _ct.SetPadding(1);
            _ct.AddColumn("Name");
            _ct.AddColumn("Type");
            _ct.AddColumn("Ptr");
            _ct.AddColumn("Value");
            _ct.AddColumn("Line");

            for(int i = 0; i <= currentIndex; i++) {
                if(records[i]->type == _cmdWords[1] && records[i]->dataFunc == records[currentIndex]->dataFunc) {
                    ConsoleTableRow* _entry = new ConsoleTableRow(5);
                    _entry->AddEntry(records[i]->name, 0);
                    _entry->AddEntry(records[i]->type, 1);
                    _entry->AddEntry(records[i]->ptr, 2);
                    _entry->AddEntry(records[i]->value, 3);
                    _entry->AddEntry(records[i]->line, 4);
                    _ct.AddRow(_entry);
                }
            }

            _ct.PrintTable();
        }
        else {
            std::vector<std::string> _info;

            _info.push_back(records[currentIndex]->ptr);
            _info.push_back(records[currentIndex]->dataFunc);
            _info.push_back(records[currentIndex]->name);
            _info.push_back(records[currentIndex]->value);
            _info.push_back(records[currentIndex]->type);

            if(FindPrevRecordData(_info, currentIndex)) {
                /*
                std::cout << "\033[1m" << "Previous Data List ,  next page - prevright,  prev page - prevleft, ";
                ConsoleTable _ct(BASIC);
                _ct.SetPadding(1);
                _ct.AddColumn(" ");
                _ct.AddColumn("Current Function");
                _ct.AddColumn("Operation");
                _ct.AddColumn("Name");
                _ct.AddColumn("Type");
                _ct.AddColumn("Value");
                _ct.AddColumn("Pointer Address");
                _ct.AddColumn("Line");
                _ct.AddColumn("Column");
                
                int _tableIndex = 0;
                std::vector<ConsoleTableRow*> _entrys;
                for(int i = 0; i < currentIndex; i++) {
                    if(_info[0] == records[i]->ptr 
                    && _info[1] == records[i]->dataFunc 
                    && _info[2] == records[i]->name 
                    && _info[4] == records[i]->type
                    && records[i]->recordType != RecordType::Array
                    && records[i]->recordType != RecordType::Struct) {
                        ConsoleTableRow* _entry = new ConsoleTableRow(9);
                        _tableIndex++;
                        _entry->AddEntry(std::to_string(_tableIndex), 0);
                        _entry->AddEntry(records[i]->dataFunc, 1);
                        _entry->AddEntry(records[i]->accessType, 2);
                        _entry->AddEntry(records[i]->name, 3);
                        _entry->AddEntry(records[i]->type, 4);
                        _entry->AddEntry(records[i]->value, 5);
                        _entry->AddEntry(records[i]->ptr, 6);
                        _entry->AddEntry(records[i]->line, 7);
                        _entry->AddEntry(records[i]->col, 8);

                        _entrys.push_back(_entry);
                    }
                }

                int _maxPage = (_entrys.size() / 5) + 1;
                
                if(prevTableIndex != -1) {
                    prevCurPage = (prevTableIndex / 5) + 1;
                    prevTableIndex = -1;
                }
                int _startIndex = (_entrys.size() > (prevCurPage - 1) * 5) ? (prevCurPage - 1) * 5 : 0;
                int _endIndex = (_entrys.size() > (prevCurPage * 5) - 1) ? (prevCurPage * 5) - 1 : _entrys.size() - 1;

                //std::cout << "StartIndex : " << _startIndex << std::endl;
                //std::cout << "EndIndex : " << _endIndex << std::endl;

                std::cout << "prev data count : " << _entrys.size() << " current page - " << prevCurPage << "/" << _maxPage << "\033[0m"<< std::endl;

                if(_maxPage < prevCurPage) prevCurPage = _maxPage;

                for(int i = _startIndex; i <= _endIndex; i++) {
                    //std::cout << "PrevData Index : " << i << std::endl;
                    _ct.AddRow(_entrys[i]);
                }
                _ct.PrintTable();
                */
            }
            if(FindAfterRecordData(_info, currentIndex)) {
                /*
                std::cout << std::endl << "\033[1m" << "Following Data List,  next page - followright,  prev page - followleft, ";
                ConsoleTable _ct(BASIC);
                _ct.SetPadding(1);
                _ct.AddColumn(" ");
                _ct.AddColumn("Current Function");
                _ct.AddColumn("Operation");
                _ct.AddColumn("Name");
                _ct.AddColumn("Type");
                _ct.AddColumn("Value");
                _ct.AddColumn("Pointer Address");
                _ct.AddColumn("Line");
                _ct.AddColumn("Column");
                
                int _tableIndex = 0;
                std::vector<ConsoleTableRow*> _entrys;
                for(int i = currentIndex + 1; i < records.size(); i++) {
                    if(_info[0] == records[i]->ptr 
                    && _info[1] == records[i]->dataFunc 
                    && _info[2] == records[i]->name 
                    && _info[4] == records[i]->type
                    && records[i]->recordType != RecordType::Array
                    && records[i]->recordType != RecordType::Struct) {
                        ConsoleTableRow* _entry = new ConsoleTableRow(9);
                        _tableIndex++;
                        _entry->AddEntry(std::to_string(_tableIndex), 0);
                        _entry->AddEntry(records[i]->dataFunc, 1);
                        _entry->AddEntry(records[i]->accessType, 2);
                        _entry->AddEntry(records[i]->name, 3);
                        _entry->AddEntry(records[i]->type, 4);
                        _entry->AddEntry(records[i]->value, 5);
                        _entry->AddEntry(records[i]->ptr, 6);
                        _entry->AddEntry(records[i]->line, 7);
                        _entry->AddEntry(records[i]->col, 8);

                        _entrys.push_back(_entry);
                    }
                }

                int _maxPage = (_entrys.size() / 5) + 1;

                if(afterTableIndex != -1) {
                    afterCurPage = (afterTableIndex / 5) + 1;
                    afterTableIndex = -1;
                }

                std::cout << "following data count : " << _entrys.size() << " current page : " << afterCurPage << "/" << _maxPage << "\033[0m"<< std::endl;
                
                int _startIndex = (_entrys.size() > (afterCurPage - 1) * 5) ? (afterCurPage - 1) * 5 : 0;
                int _endIndex = (_entrys.size() > (afterCurPage * 5) - 1) ? (afterCurPage * 5) - 1 : _entrys.size() - 1;
                //std::cout << "StartIndex : " << _startIndex << std::endl;
                //std::cout << "EndIndex : " << _endIndex << std::endl;
                //std::cout << "After data size : " << _entrys.size() << std::endl;

                if(_maxPage < afterCurPage) afterCurPage = _maxPage;

                for(int i = _startIndex; i <= _endIndex; i++) {
                    //std::cout << "AfterData Index : " << i << std::endl;
                    _ct.AddRow(_entrys[i]);
                }
                _ct.PrintTable();
                */
            }
            std::cout << "Current thread : " << mtu->GetCurThreadId() << ", ";
            std::cout << "\033[1m" << "Current Data Information, code - " << RemoveLeadingWhitespace(codes[currentLine]) << "\033[0m" << std::endl;
            records[currentIndex]->PrintRecordTable(commandMessage);
        }
    }
    else {
        std::cout << "                             기록정보 없음                               \n";
    }

    if(systemMessage.size() > 2) {

        std::cout << "+------------------------------------------------------------------------------------------------------------------------+\n";
        std::cout << "|                                                       " << "\033[1m" <<"System meseeage" << "\033[0m" << "                                                  |\n";
        std::cout << "+------------------------------------------------------------------------------------------------------------------------+\n";
        std::cout << "  " <<systemMessage << std::endl;
        std::cout << "+------------------------------------------------------------------------------------------------------------------------+\n";
    }
}

bool App::IsNumber(std::string const &_str) {
    auto it = _str.begin();
    while(it != _str.end() && std::isdigit(*it)) {
        it++;
    }
    return !_str.empty() && it == _str.end();
}

bool App::FindRecord(int _line) {
    for(int i = 0 ; i < records.size(); i++) {
        if(std::stoi(records[i]->line) == _line) return true;
    }
    return false;
}

int App::FindRecordDataPtr(std::string _ptr) {
    for(int i = records.size() - 1; i >= 0; i--) {
        if(records[i]->ptr == _ptr && records[i]->type == "string") return i; 
    }
    return -1;
}

int App::FindRecordDataStr(std::string _name) {
    //std::cout << "record size : " << records.size() << std::endl;
    for(int i = records.size() - 1 ; i >= 0; i--) {
        //std::cout << "records name : " << records[i]->originName << std::endl;
        //std::cout << "name : " << _name << std::endl;
        if(records[i]->originName == _name) return i;
    }
    return -1;
}

int App::FindRecordData(int _line) {
    for(int i = 0; i < records.size(); i++) {
        if(std::stoi(records[i]->line) == _line) {
            //std::cout << "FIndRecordData line : " << i << std::endl;
            return i;
        }
    }
    return -1;
}

bool App::IsEqualData(std::string _str, std::string _line, std::string _col) {
    // 문자열을 스트림화
    std::stringstream _ss(_str);
    // 공백 분리 결과를 저장할 배열
    std::vector<std::string> _words;
    std::string _word;
    // 스트림을 한 줄씩 읽어, 공백 단위로 분리한 뒤, 결과 배열에 저장
    while (getline(_ss, _word, ' ')){
        _words.push_back(_word);
    }

    return (_words[_words.size() - 1] == _col && _words[_words.size() - 2] == _line);
}

///////////////////////////////Command method///////////////////////////////
void App::InitCommand() {
    commands.push_back("mvarray");
    commands.push_back("findptr");
    commands.push_back("mvindex");
    commands.push_back("mvline");
    commands.push_back("findtype");
    commands.push_back("findname");
    commands.push_back("followright");
    commands.push_back("followleft");
    commands.push_back("prevright");
    commands.push_back("prevleft");
    commands.push_back("prevmove");
    commands.push_back("followmove");
    commands.push_back("threadset");
}

bool App::FindCommand(std::string _command) {
    std::cout << "command : " << _command << std::endl;
    for(int i = 0; i < commands.size(); i++) {
        if(_command.find(commands[i]) != std::string::npos) {
            return true;
        }
    }
    return false;
}

void App::ErrorHandling(std::string _message) {
    if(_message.find("WARNING") != std::string::npos) {
        systemMessage = "";
        inputState = InputState::WARN;
    }
}

// 
std::string App::GetType(std::string& _word) {
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

int App::CountChar(std::string &_str, char _findWord) {
    int _count = 0;
    for (char _ch : _str) {
        if (_ch == _findWord) {
            _count++;
        }
    }
    return _count;
}

std::string App::RemoveChar(const std::string &_str, char _removeWord) {
    std::string _result = _str;
    size_t _pos = 0;
    
    while ((_pos = _result.find(_removeWord, _pos)) != std::string::npos) {
        _result.erase(_pos, 1);
    }
    
    return _result;
}

std::vector<std::string> App::SplitString(const std::string &_str, char _delimiter) {
    std::vector<std::string> _result;
    std::string _token;
    std::istringstream _tokenStream(_str);

    while (std::getline(_tokenStream, _token, _delimiter)) {
        _result.push_back(_token);
    }

    return _result;
}

bool App::FindPrevRecordData(std::vector<std::string> _str, int _currentIndex) {
    for(int i = _currentIndex - 1; i >= 0; i--) {
        if(_str[0] == records[i]->ptr 
        && _str[1] == records[i]->dataFunc 
        && _str[2] == records[i]->name 
        && _str[4] == records[i]->type
        && records[i]->recordType != RecordType::Array
        && records[i]->recordType != RecordType::Struct) {
            return true;
        }
    }
    return false;
}

bool App::FindAfterRecordData(std::vector<std::string> _str, int _currentIndex) {
    for(int i = _currentIndex + 1; i < records.size(); i++) {
        if(_str[0] == records[i]->ptr 
        && _str[1] == records[i]->dataFunc 
        && _str[2] == records[i]->name 
        && _str[4] == records[i]->type
        && records[i]->recordType != RecordType::Array
        && records[i]->recordType != RecordType::Struct) {
            //std::cout << "return true in find after data index : " << i << std::endl;
            return true;
        }
    }
    //std::cout << "return false in find after data" << std::endl;
    return false;
}

int App::FindIndexRecordData(std::string _line, std::string _name) {
    int _count = 0;
    for(int i = 0; i < records.size(); i++) {
        if(records[i]->line == _line) {
            _count++;
            if(records[i]->name == _name) {
                return _count;
            }
        }
    }
    return _count;
}

std::string App::RemoveLeadingWhitespace(const std::string& _input) {
    std::string _result;
    bool _leadingWhitespace = true; // 첫 번째 문자를 찾기 위한 플래그

    for (char _c : _input) {
        if (_leadingWhitespace) {
            if (_c != ' ' && _c != '\t') {
                _result += _c;
                _leadingWhitespace = false; // 첫 번째 문자를 찾았으므로 플래그를 끕니다.
            }
        } else {
            _result += _c;
        }
    }

    return _result;
}

bool App::FindStringInString(std::string _str, std::string _findStr) {
    return _str.find(_findStr) != std::string::npos;
}

int App::FindStructStructData(std::string _ptr) {
    for(int i = 0; i < structs.size(); i++) {
        std::cout << "struct datastruct size : " << structs[i]->dataStructs.size() << std::endl;
        for(int j = 0; j < structs[i]->dataStructs.size(); j++) {
            std::cout << "struct data : " << structs[i]->dataStructs[j].ptr << std::endl;
            if(structs[i]->dataStructs[j].ptr == _ptr) {
                return i;
            }
        }
    }
    return -1;
}

int App::FindStructRecordData(std::string _value, std::string _ptr, int _curIdx) {
    for(int i = _curIdx; i >= 0; i--) {
        if(records[i]->recordType == RecordType::Struct) {
            std::cout << "data strcut size : " << records[i]->GetDataStruct().size() << std::endl;
            for(int j = 0; j < records[i]->GetDataStruct().size(); j++) {
                if(_ptr == records[i]->GetDataStruct()[j].ptr
                || _value == records[i]->GetDataStruct()[j].ptr) return i;
            }
        }
    }
    return -1;
}

void App::Red() {
    std::cout << "\033[0;31m";
}

void App::Blue() {
    std::cout << "\033[0;34m";
}

void App::Reset() {
    std::cout << "\033[0m";
}

void App::StartTime() {
    startTime = std::chrono::high_resolution_clock::now();
}

void App::EndTime() {
    endTime = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> _timeSpan = endTime - startTime;

    std::cout << "Time : " << _timeSpan.count() << "s" << std::endl;
}
