#include "app.h"

App::App() {
    std::cout << "Initialize app class" << std::endl;
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
    isDone = false;
    programOver = false;
    currentLine = 0;
    currentIndex = -1;
    prevLine = -1;
    inputState = InputState::Stop;
    systemMessage = "None";
    InitCommand();

    // 소스 파일이 제대로 열렸는지 검사
    std::ifstream srcStream(srcFile);
    if (!srcStream) {
        std::cerr << "Unable to open source file: " << srcFile << std::endl;
        exit(0);
    }

    // 기록 파일이 제대로 열렸는지 검사
    std::ifstream recordStream(recordFile);
    if (!recordStream) {
        std::cerr << "Unable to open record file: " << recordFile << std::endl;
        exit(0);
    }

    std::string _line;
    while (getline(recordStream, _line)) {
        recordLines.push_back(_line);
    }
    
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
            if(_words[0] == "isStruct") {

            }
            else if(_words[JUDGMENT_INDEX] == "isArr" || _words[JUDGMENT_INDEX] == "isPointerArr") { // 배열일 경우
                int _dimension = _words.size() - 8;
                int _findIdx = -1;
                for(int j = records.size() - 1; j >= 0; j--) {
                    std::string _nameFunc = records[j]->dataFunc + "_" + records[j]->name;
                    if(_nameFunc == _words[1]) {
                        _findIdx = j;
                        break;
                    }
                }

                if(_findIdx != -1) {
                    std::cout << "detect vector record data" << std::endl;
                    //기존 기록 파일에 벡터 기록 데이터가 있을 경우 해당 기록 데이터의 shadow memory의 값들을 복사
                    RecordData* _data = new RecordArray(_words, _dimension);
                    std::cout << "Shadow memory size : " << records[_findIdx]->GetShadowMemory().size() << std::endl; 
                    _data->SetShadowMemory(records[_findIdx]->GetShadowMemory());
                    _data->SetArrrays(records[_findIdx]->GetArrays());
                    records.push_back(_data);
                }
                else {
                    RecordData* _data = new RecordArray(_words, _dimension);
                    records.push_back(_data);
                    std::string _checkLine = recordLines[i + 1];
                    std::vector<std::string> _splitWords = SplitString(_checkLine, ' ');
                    
                    if(_splitWords[1] == _words[1]
                    && _splitWords[_splitWords.size() - 2] == _words[_words.size() - 2]) {
                        _data->UpdateRecordData(_splitWords);
                        i += 2;
                        _checkLine = recordLines[i];
                        _splitWords = SplitString(_checkLine, ' ');
                        while(_splitWords[1] == _words[1]
                            && _splitWords[_splitWords.size() - 2] == _words[_words.size() - 2]) {
                            _data->UpdateRecordData(_splitWords);
                            i++;
                            _checkLine = recordLines[i];
                            _splitWords = SplitString(_checkLine, ' ');
                        }
                        i--;
                    }
                }
            }
            else if(_words[1] == "push_back" ) { // 벡터일 경우
                std::cout << "vector push back data" << std::endl;
                int _findIdx = -1;
                for(int i = records.size() - 1; i >= 0; i--) {
                    std::string _comparisonStr = records[i]->dataFunc + "_" + records[i]->name;
                    if(_comparisonStr == _words[2]) {
                        _findIdx = i;
                        std::cout << "_findIdx : " << _findIdx << std::endl;
                        break;
                    }
                }
                if(_findIdx != -1) { 
                    std::cout << "detect vector record data" << std::endl;
                    //기존 기록 파일에 벡터 기록 데이터가 있을 경우 해당 기록 데이터의 shadow memory의 값들을 복사
                    RecordData* _data = new RecordVector(_words);
                    std::cout << "Shadow memory size : " << records[_findIdx]->GetShadowMemory().size() << std::endl; 
                    _data->SetShadowMemory(records[_findIdx]->GetShadowMemory());
                    _data->UpdateRecordData(_words);
                    records.push_back(_data);

                } else {
                    std::cout << "vector recordata create" << std::endl;
                    RecordData* _data = new RecordVector(_words);
                    _data->UpdateRecordData(_words);
                    records.push_back(_data);
                }
            }
            else if(_line.find("_ref.tmp") != std::string::npos) {
                std::cout << "inside ref tmp" << std::endl;
                ///////////////////////////string 일 경우/////////////////////////////
                std::string _refValue;
                std::string _refType = _words[3];
                if(_words[TYPE_INDEX] == "string") {
                    if(_line.find("StringEnd") != std::string::npos) {
                        int _i = STRING_START_IDX;
                        while(_words[_i] != "StringEnd") {
                            _refValue += _words[_i];
                            _refValue += " ";
                            _i++;
                        }
                    }
                    else {
                        int _strValIndex = STRING_START_IDX;
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
                else {
                    _refValue = _words[3];
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
                if(_words2[1] == "push_back" ) { // 벡터일 경우
                    _words2[4] = _refValue;
                    _words2[3] = _refType;
                    std::cout << "vector push back data" << std::endl;
                    int _findIdx = -1;
                    for(int i = records.size() - 1; i >= 0; i--) {
                        std::string _comparisonStr = records[i]->dataFunc + "_" + records[i]->name;
                        if(_comparisonStr == _words2[2]) {
                            _findIdx = i;
                            std::cout << "_findIdx : " << _findIdx << std::endl;
                            break;
                        }
                    }
                    if(_findIdx != -1) { 
                        std::cout << "detect vector record data" << std::endl;
                        //기존 기록 파일에 벡터 기록 데이터가 있을 경우 해당 기록 데이터의 shadow memory의 값들을 복사
                        RecordData* _data = new RecordVector(_words2);
                        std::cout << "Shadow memory size : " << records[_findIdx]->GetShadowMemory().size() << std::endl; 
                        _data->SetShadowMemory(records[_findIdx]->GetShadowMemory());
                        _data->UpdateRecordData(_words2);
                        records.push_back(_data);

                    } else {
                        std::cout << "vector recordata create" << std::endl;
                        RecordData* _data = new RecordVector(_words2);
                        _data->UpdateRecordData(_words2);
                        records.push_back(_data);
                    }
                }
            }
            else if(_words[1] == "push_back" ) { // 벡터일 경우
                std::cout << "vector push back data" << std::endl;
                int _findIdx = -1;
                for(int i = records.size() - 1; i >= 0; i--) {
                    std::string _comparisonStr = records[i]->dataFunc + "_" + records[i]->name;
                    if(_comparisonStr == _words[2]) {
                        _findIdx = i;
                        std::cout << "_findIdx : " << _findIdx << std::endl;
                        break;
                    }
                }
                if(_findIdx != -1) { 
                    std::cout << "detect vector record data" << std::endl;
                    //기존 기록 파일에 벡터 기록 데이터가 있을 경우 해당 기록 데이터의 shadow memory의 값들을 복사
                    RecordData* _data = new RecordVector(_words);
                    std::cout << "Shadow memory size : " << records[_findIdx]->GetShadowMemory().size() << std::endl; 
                    _data->SetShadowMemory(records[_findIdx]->GetShadowMemory());
                    _data->UpdateRecordData(_words);
                    records.push_back(_data);

                } else {
                    std::cout << "vector recordata create" << std::endl;
                    RecordData* _data = new RecordVector(_words);
                    _data->UpdateRecordData(_words);
                    records.push_back(_data);
                }
            }
            else { // 일반 기록파일
                std::string _nameVal = "";
                std::string _typeVal = "";
                std::string _strVal = "";
                std::string _ptrVal = "";
                std::string _lenVal = "";
                std::string _lineVal = "";
                std::string _colVal = "";
                std::string _infoMessage = "";
                if(_words[TYPE_INDEX] == "string") {
                    std::cout<< "word index 1 : " << FindRecordDataStr(_words[1]) << std::endl;

                    if(_line.find("StringEnd") != std::string::npos) {
                        int _i = STRING_START_IDX;
                        while(_words[_i] != "StringEnd") {
                            _strVal += _words[_i];
                            _strVal += " ";
                            _i++;
                        }
                        _colVal = _words[_words.size() - 1];
                        _lineVal = _words[_words.size() - 2];
                        _ptrVal = _words[_words.size() - 3];
                        _lenVal = _words[STRING_LEN_IDX];
                        std::cout << "strVal : " << _strVal << std::endl;
                    }
                    else if(FindRecordDataStr(_words[2]) != -1) {
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
                        int _strValIndex = STRING_START_IDX;
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
                    _resultWord.push_back(_words[2]);
                    _resultWord.push_back(_words[3]);
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
                }
                else if(FindRecordDataPtr(_words[_words.size() - 3]) != -1) {
                    int _findIdx = FindRecordDataPtr(_words[_words.size() - 3]);
                    std::cout << "find record data, _findIdx :  " << _findIdx << std::endl;
                    int _changeIndex = std::stoi(_words[3]);
                    std::cout << "change index :  " << _changeIndex << std::endl;
                    _nameVal = records[_findIdx]->dataFunc + "_" + records[_findIdx]->name;
                    std::cout << "name :  " << _nameVal << std::endl;
                    _typeVal = records[_findIdx]->ptr;
                    std::cout << "type :  " << _typeVal << std::endl;
                    _strVal = records[_findIdx]->value;
                    _strVal[_changeIndex] = _words[4].c_str()[0];
                    std::cout << "value :  " << _strVal << std::endl;
                    _colVal = _words[_words.size() - 1];
                    _lineVal = _words[_words.size() - 2];
                    _ptrVal = _words[_words.size() - 3];
                    _lenVal = _words[3];
                    std::cout << "len :  " << _lenVal << std::endl;
                    if(std::stoi(_lenVal) < _changeIndex) {
                        _infoMessage = "문자열의 범위 밖 요청입니다";
                    }
                    else {
                        _infoMessage = " ";
                    }
                    std::cout << "set default information" << std::endl;
                    std::vector<std::string> _resultWord;
                    _resultWord.push_back(_nameVal);
                    _resultWord.push_back(_typeVal);
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

                }
                else { // 문자열을 제외한 일반 타입
                    std::vector<std::string> _resultWord;
                    _resultWord.push_back(_words[1]);
                    _resultWord.push_back(GetType(_words[2]));
                    _resultWord.push_back(_words[3]);
                    _resultWord.push_back(_words[4]);
                    _resultWord.push_back(_words[5]);
                    _resultWord.push_back(_words[6]);
                    _resultWord.push_back("");

                    for(int i = 0; i < _resultWord.size(); i++) {
                        std::cout << "result word : " << _resultWord[i] << std::endl;
                    }

                    RecordData* _data = new RecordPrim();

                    _data->InitRecordData(_resultWord);
                    _data->recordType = RecordType::Prim;
                    records.push_back(_data);
                    std::vector<std::string>().swap(_resultWord);
                }
            }
        }
    }

    if(records[records.size() - 1]->name == "retval") {
        isDone = true;
    }
    else {
        isDone = false;
        records[records.size() - 1]->infoMessage = "오류 발생 지점 예상";
    }

    std::cout << "record count : " << records.size() << std::endl;

    while (std::getline(srcStream, _line)) {
        codes.push_back(_line);
        if(_line.find("main") != std::string::npos) {
            currentLine = codes.size();
        }
    }
    Render();
}

// 사용자로부터 입력을 받는 함수
void App::Input() {
    std::string _input;
    std::cout << std::endl << "User input : ";
    getline(std::cin, _input);
    if(!IsNumber(_input)) { // 사용자 입력 값이 숫자가 아닐 경우
        if(_input == "s") {
            std::cout << "Down" << std::endl;
            this->inputState = InputState::Down;
            this->commandMessage = "down";
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
        for(int i = currentLine; i > 0; i--) {
            currentLine--;
            if(FindRecord(currentLine + 1)) break;
        }
        inputState = InputState::Stop;
    }
    else if(inputState == InputState::DoubleUp) {
        this->commandMessage = "doubleup";
        if(currentLine > 0) {
            std::string _eraseStr = codes[currentLine - 1];
            _eraseStr.erase(remove(_eraseStr.begin(), _eraseStr.end(), ' '), _eraseStr.end());
            if(_eraseStr.size() < 2) {
                while(_eraseStr.size() < 2 && (currentLine + 1) <= (codes.size() - 1)) {
                    currentLine--;
                    _eraseStr = codes[currentLine];
                    _eraseStr.erase(remove(_eraseStr.begin(), _eraseStr.end(), ' '), _eraseStr.end());
                }
            }
            else {
                currentLine--;
            }
            inputState = InputState::Stop;
        }
    }
    else if(inputState == InputState::Down) {
        this->commandMessage = "down";
        prevLine = currentLine;
        currentIndex++;
        currentLine = std::stoi(records[currentIndex]->line);
        std::cout << "prev line : " << prevLine << std::endl;
        std::cout << "current line : " << currentLine << std::endl;

        /*
        while(prevLine == currentLine) {
            currentLine = std::stoi(records[currentIndex]->line);
            currentIndex++;
        }
        */
        inputState = InputState::Stop;
    } 
    else if(inputState == InputState::DoubleDown) {
        this->commandMessage = "double down";
        if(currentLine < codes.size() - 1) {
            std::string _eraseStr = codes[currentLine + 1];
            _eraseStr.erase(remove(_eraseStr.begin(), _eraseStr.end(), ' '), _eraseStr.end());
            if(_eraseStr.size() < 2) {
                while(_eraseStr.size() < 2 && (currentLine + 1) <= (codes.size() - 1)) {
                    currentLine++;
                    _eraseStr = codes[currentLine];
                    _eraseStr.erase(remove(_eraseStr.begin(), _eraseStr.end(), ' '), _eraseStr.end());
                }
            }
            else {
                currentLine++;
            }
            inputState = InputState::Stop;
        }
    }
    else if(inputState == InputState::Right) {
        this->commandMessage = "right";
    }
    else if(inputState == InputState::Left) {
        this->commandMessage = "left";
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

            currentLine = std::stoi(_cmdWords[1]) - 1;
        }
    }
}

void App::Render() {
    std::cout << "\033[2J\033[1;1H"; // 화면에 출력된 정보들을 지운다.

    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "+--------------------------------------------------------------------------+\n";
    std::cout << "|                                    code                                  |\n";
    std::cout << "+--------------------------------------------------------------------------+\n";
    std::cout << std::endl;

    int _startLine = (currentLine < CODE_SHOW_RANGE) 
    ? 0 : currentLine - CODE_SHOW_RANGE; //코드 시작 줄 번호
    int _endLine = ((currentLine + CODE_SHOW_RANGE) > codes.size()) 
    ? _endLine = codes.size() - 1 : _endLine = currentLine + CODE_SHOW_RANGE; // 코드 마지막 줄 번호

    for(int i = _startLine; i <= _endLine; i++) {
        std::cout << i + 1 << "      ";
        if(i == currentLine - 1) std::cout << "\033[1m" << ">>>>   " << codes[i] << "\033[0m" <<std::endl;
        else std::cout << codes[i] << std::endl;
    }

    std::cout << std::endl;
    std::cout << "+--------------------------------------------------------------------------+\n";

    std::cout << std::endl;
    std::cout << "+--------------------------------------------------------------------------+\n";
    std::cout << "|                            Information meseeage                          |\n";
    std::cout << "+--------------------------------------------------------------------------+\n";
    std::cout << std::endl;

    std::cout << std::endl << "current line : " << currentLine << std::endl;

    if(FindRecord(currentLine)) { // 해당 줄의 레코드 데이터가 있는지 검사// 일차원 배열
        //std::cout << "find record data" << std::endl;
        //std::cout << "current index : " << currentLine + 1 << std::endl;
        int _findIndex = FindRecordData(currentLine);
        valShadowMemory.clear();
        ptrShadowMemroy.clear();
        map<string, string>::iterator p;

        for(int i = 0 ; i <= _findIndex; i++) {
            std::map<std::string, std::string> _shadowMem = records[i]->GetShadowMemory();
            for (p = _shadowMem.begin(); p != _shadowMem.end(); ++p) {
                valShadowMemory[p->first] = records[i]->name;
                ptrShadowMemroy[p->first] = p->second;
            }
        }
        /*
        for (p = valShadowMemory.begin(); p != valShadowMemory.end(); ++p) {
            cout << "(" << p->first << "," << p->second << ")\n";
        }

        for (p = ptrShadowMemroy.begin(); p != ptrShadowMemroy.end(); ++p) {
            cout << "(" << p->first << "," << p->second << ")\n";
        }
        */
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
            _ct.AddColumn("Name");
            _ct.AddColumn("Ptr");
            _ct.AddColumn("Value");

            ConsoleTableRow* _entry = new ConsoleTableRow(3);
            
            _entry->AddEntry(valShadowMemory[_cmdWords[1]], 0);
            _entry->AddEntry(_cmdWords[1], 1);
            _entry->AddEntry(ptrShadowMemroy[_cmdWords[1]], 2);

            _ct.AddRow(_entry);

            _ct.PrintTable();
        }
        else {
            std::cout << "line information" << std::endl << std::endl;
            std::vector<std::string> _names;
            std::string _dataFunc;
            std::string _line;
            std::string _ptr;

            records[currentIndex]->PrintRecordTable(commandMessage);
            _names.push_back(records[currentIndex]->name);
            _dataFunc = records[currentIndex]->dataFunc;
            _ptr = records[currentIndex]->ptr;
            
            for(int i = currentIndex - 1; i >= 0; i--) {
                for(int j = 0; j < _names.size(); j++) {
                    if(_ptr == records[i]->ptr 
                    && _dataFunc == records[i]->dataFunc 
                    && _names[j] == records[i]->name 
                    && records[i]->recordType != RecordType::Array) {
                        records[i]->PrintRecordTable(commandMessage);
                    }
                }
            }
        }
    }
    else {
        std::cout << "                             기록정보 없음                               \n";
    }

    std::cout << std::endl;
    std::cout << "+--------------------------------------------------------------------------+\n";

    std::cout << std::endl;
    std::cout << "+--------------------------------------------------------------------------+\n";
    std::cout << "|                            System meseeage                               |\n";
    std::cout << "+--------------------------------------------------------------------------+\n";
    std::cout << "                        " << systemMessage << std::endl;
    std::cout << "+--------------------------------------------------------------------------+\n";
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
    std::cout << "record size : " << records.size() << std::endl;
    for(int i = records.size() - 1 ; i >= 0; i--) {
        std::cout << "records name : " << records[i]->name << std::endl;
        std::cout << "name : " << _name << std::endl;
        if(records[i]->name == _name) return i;
    }
    return -1;
}

int App::FindRecordData(int _line) {
    for(int i = 0; i < records.size(); i++) {
        if(std::stoi(records[i]->line) == _line) return i;
    }
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
            _removeStr = "boolean";
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
            return "boolean";
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