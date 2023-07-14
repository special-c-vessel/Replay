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
    programOver = false;
    currentLine = 0;
    inputState = InputState::Stop;
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
    std::string _nameStr("");

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

        if(_words[JUDGMENT_INDEX] == "isArr" ) {
            if(_nameStr == _words[1]) {
                records[_addIndex]->UpdateRecordData(_words);
            } else {
                int _dimension = _words.size() - 8;

                RecordData* _data = new RecordArray(_dimension);

                if(_dimension == 1) _data->recordType = RecordType::Array1;
                else if(_dimension == 2) _data->recordType = RecordType::Array2;
                else if(_dimension == 3) _data->recordType = RecordType::Array3;

                _data->InitRecordData(_words);
                records.push_back(_data);
                _addIndex = records.size() - 1;
                _nameStr = _words[1];
            }
            //records[_addIndex]->PrintRecordData();
        }
    }
    
    while (std::getline(srcStream, _line)) {
        codes.push_back(_line);
        if(_line.find("main") != std::string::npos) {
            currentLine = codes.size() - 1;
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
        std::cout << "\ninput string : " << _input << std::endl;
        if(_input == "s") {
            std::cout << "Down" << std::endl;
            this->inputState = InputState::Down;
        }
        else if(_input == "w") {
            std::cout << "Up" << std::endl;
            this->inputState = InputState::Up;
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
    int _endLine = ((currentLine + CODE_SHOW_RANGE) > codes.size() - 1) 
    ? _endLine = codes.size() - 1 : _endLine = currentLine + CODE_SHOW_RANGE; // 코드 마지막 줄 번호

    for(int i = _startLine; i <= _endLine; i++) {
        std::cout << i + 1 << "      ";
        if(i == currentLine) std::cout << "\033[1m" << ">>>>   " << codes[i] << "\033[0m" <<std::endl;
        else std::cout << codes[i] << std::endl;
    }

    std::cout << std::endl;
    std::cout << "+--------------------------------------------------------------------------+\n";

    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "+--------------------------------------------------------------------------+\n";
    std::cout << "|                            Information meseeage                          |\n";
    std::cout << "+--------------------------------------------------------------------------+\n";
    std::cout << std::endl;

    if(FindRecord(currentLine + 1)) { // 해당 줄의 레코드 데이터가 있는지 검사
        if(records[FindRecordData(currentLine + 1)]->recordType == RecordType::Array1) { // 일차원 배열
            this->recordType = RecordType::Array1;
            systemMessage = records[FindRecordData(currentLine + 1)]->PrintRecordTable(commandMessage);
            commandMessage = "";
        }
        else if(records[FindRecordData(currentLine + 1)]->recordType == RecordType::Array2) { // 이차원 배열
            this->recordType = RecordType::Array2;
            systemMessage = records[FindRecordData(currentLine + 1)]->PrintRecordTable(commandMessage);
            commandMessage = "";    
        }
        else if(records[FindRecordData(currentLine + 1)]->recordType == RecordType::Array3) { // 삼차원 배열
            this->recordType = RecordType::Array3;
            systemMessage = records[FindRecordData(currentLine + 1)]->PrintRecordTable(commandMessage);
            commandMessage = "";
        }
    }
    std::cout << std::endl;
    std::cout << "+--------------------------------------------------------------------------+\n";

    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "+--------------------------------------------------------------------------+\n";
    std::cout << "|                            System meseeage                               |\n";
    std::cout << "+--------------------------------------------------------------------------+\n";
    std::cout << std::endl;
    std::cout << "               " << systemMessage << std::endl;
    std::cout << std::endl;
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