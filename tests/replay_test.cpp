#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unistd.h> // for sleep function
#include <ncurses.h>

using namespace std;

class DeclareData {
public:// Generator
	DeclareData() {
        name = "";
        type = "";
        value = "";
        ptr = "";
        line = "";

        parentData = nullptr;
        childData = nullptr;
    }   
public:// Nomal function
	void LinkDecData(DeclareData* _decData) {
        if(childData == nullptr) {
            cout << "child is empty" << endl;
            childData = _decData;
        }
        else {
            cout << "child is not empty" << endl;
            childData->LinkDecData(_decData);
        }
    }
public:// Getter Setter function
	DeclareData GetParentData();
	void SetParentData(DeclareData _decData);
private:
	DeclareData* parentData;
	DeclareData* childData;
public:
	string name;
	string ctype;
	string type;
	string value;
	string ptr;
	string line;
	string col;
	int repeat = 1;
    std::vector<int> repeatIndex;
};


// 오퍼레이션 기록 고조체
struct OpData {
    string op; // 오퍼레이션 타입
    string result_name; // 결과 변수 이름
    string result_val; // 결과 변수 값
    string result_ptr; // 결과 변수 주소
    string val1; 
    string val2;
    string val_line; // 기록이 발생한 줄 번호
};

// 전역 함수 선언
void FindRecData(int _line);
void PrintRecData(DeclareData _data, int _codeIndex);
void FindOpData(int _line);
void PrintOpData(OpData _data);
void PrintCode(int _codesIndex);
string FindValueName(int _lineNum, string _valName);
string FindValuePtr(int _lineNum, string _valName);
string TrimStr(const string& str);
bool isNumeric(std::string const &str);

// 전역 변수 선언
std::vector<DeclareData> recData;
std::vector<OpData> opData;
std::string line;
std::string codes[1000];
int lineRepeat[155] = {1};
int lineNum = 1;
int codesIndex = 1;
int repeatCount = 1;
int maxCodesIndex = 1;
int startMainFuncIndex = 0;
int prevRepeat;

int main(int argc, char* argv[]) {

    // 인자가 전달되었는지 검사
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " [source file] [record file]" << std::endl;
        return 1;
    }

    // 소스 파일이 제대로 열렸는지 검사
    std::ifstream srcFile(argv[1]);
    if (!srcFile) {
        std::cerr << "Unable to open source file: " << argv[1] << std::endl;
        return 1;
    }

    // 기록 파일이 제대로 열렸는지 검사
    std::ifstream recFile(argv[2]);
    if (!recFile) {
        std::cerr << "Unable to open record file: " << argv[2] << std::endl;
        return 1;
    }

    // 기록 파일을 반복문으로 한줄씩 확인
    while (getline(recFile, line)) {
        if(line == "declare") { // declare일 경우 변수 기록 객체를 생성 후 벡터에 푸시
            DeclareData data = DeclareData();
            getline(recFile, data.name);
            getline(recFile, data.ctype);
            getline(recFile, data.value);
            getline(recFile, data.ptr);
            getline(recFile, data.line);
            getline(recFile, data.col);

            lineRepeat[stoi(data.line)]++;
            data.repeat = lineRepeat[stoi(data.line)];

            cout << "Repeat Count : " + to_string(lineRepeat[stoi(data.line)]) << endl;

            recData.push_back(data);
        }
        else if(line == "operation") { // operation일 경우 op 기록 객체를 생성 후 벡터에 푸시
            OpData data;
            getline(recFile, data.op);
            getline(recFile, data.val1);
            getline(recFile, data.val2);
            getline(recFile, data.result_name);
            getline(recFile, data.result_val);
            getline(recFile, data.result_ptr);
            getline(recFile, data.val_line);
            opData.push_back(data);
        }
    }

    while (std::getline(srcFile, line)) {

        std::cout << "\033[2J\033[2;2H";
        std::cout << line << std::flush << std::endl; // 소스 코드를 한 줄 읽어온다

        if (line.find("main") != string::npos) {
		    startMainFuncIndex = maxCodesIndex;
            cout << "Main start index : " << startMainFuncIndex << std::endl;
        }
        codes[maxCodesIndex++] = line;
    }
    
    string input;

    codesIndex = 0;

    int maxRepeatIndex = 1;

    for(int i = 0; i < recData.size(); i++) {
        if(maxRepeatIndex < recData[i].repeat) maxRepeatIndex = recData[i].repeat;
    }
    while (true) {

        std::cout << "\033[2J\033[1;1H";
        std::cout << "Culry : replay module\n\n\n";
        std::cout << "origin src\n\n";
        
        PrintCode(codesIndex);
        std::cout <<"Max repeat Index : " << maxRepeatIndex << endl;
        std::cout <<"Repeat Index : " << repeatCount << endl;
        std::cout << "\n\nw : line up\ns : line down\na : repeat before\nd : repeat after\ndigit : move line number\n\ninput keyboard current code index " << codesIndex <<  ": ";
        std::cin >> input;

        prevRepeat = repeatCount;
        
        if(!isNumeric(input)) {
            if(input == "s") {
                if(codesIndex < recData.size() - 1) codesIndex++; // 줄번호를 1을 추가
                repeatCount = recData[codesIndex].repeat;
                for(int i = 0; i < recData.size(); i++) {
                    if(maxRepeatIndex < recData[i].repeat) maxRepeatIndex = recData[i].repeat;
                }
            }
            else if(input == "w") {
                if(codesIndex > 0)codesIndex--;
                repeatCount = recData[codesIndex].repeat;
                for(int i = 0; i < recData.size(); i++) {
                    if(maxRepeatIndex < recData[i].repeat) maxRepeatIndex = recData[i].repeat;
                }
            }
            else if(input == "a") {
                if(repeatCount > 1) repeatCount--;
            }
            else if(input == "d") {
                if(repeatCount < maxRepeatIndex) repeatCount++;
            }
            else {
                std::cout << "undefined command" << endl;
            }
        }
        else {
            for(int i = 0; i < recData.size(); i++) {
                if(stoi(recData[i].line) == stoi(input) && recData[i].repeat == repeatCount){
                    codesIndex = i;
                    break;
                }
            }
        }
    }

    return 0;
}

// 줄 번호로 RecData 객체를 찾는 함수
void FindRecData(int _line) {
    // 벡터 안에서 전달받은 줄 번호에 해당하는 기록이 있는지 검사
    for(int i = 0; i < recData.size(); i++) {
        // 줄 번호가 string으로 저장되있으므로 조건문 검사를 위해 int로 변환
        int recDataLine = std::stoi(recData[i].line);
        int _repeatCount = recData[i].repeat;
        // vector 안에 줄 번호에 해당하는 객체가 있다면 출력
        if (recDataLine == _line && _repeatCount == repeatCount) {
            std::cout << "\n\t\t------------------------------------------------------------------\n";
            //std::cout << "\n\n\t\t\t" << "\033[1m" << "\tRepeat information" << "\033[0m" << "\n\n";
            PrintRecData(recData[i], _line);
            std::cout << "\n\t\t------------------------------------------------------------------\n";
        }
    }
}

// 인자로 받은 RecData의 값을 출력해주는 함수
void PrintRecData(DeclareData _data, int _codeIndex) {
    for(int i = 0; i < recData.size(); i++) {
        // 줄 번호가 string으로 저장되있으므로 조건문 검사를 위해 int로 변환
        int _recDataLine = std::stoi(recData[i].line);
        int _repeatCount = recData[i].repeat;
        // vector 안에 줄 번호에 해당하는 객체가 있다면 출력
        if (_recDataLine == stoi(_data.line) && _repeatCount == repeatCount) {
            std::cout << "\033[1m" << "\t\t\tval repeat index : " << recData[i].repeat << "\033[0m" << std::endl;
            std::cout << "\033[1m" << "\t\t\tval name : " << recData[i].name << "\033[0m" << std::endl;
            std::cout << "\033[1m" << "\t\t\tval type : " << recData[i].ctype << "\033[0m" << std::endl;
            std::cout << "\033[1m" << "\t\t\tval ptr : " << recData[i].ptr << "\033[0m" << std::endl;
            std::cout << "\033[1m" << "\t\t\tval value : " << recData[i].value << "\033[0m" << std::endl;
            std::cout << "\033[1m" << "\t\t\tval line : " << recData[i].line << "\033[0m" << std::endl;
            std::cout << std::endl;

            //for(int j = _codeIndex; j >= 0; j--){
                //if(recData[j].repeat == repeatCount && recData[i].line != recData[j].line && (prevRepeat - repeatCount) <= 1) {
                    //std::cout << std::endl;
                    //std::cout << "\t\t\tval name : " << recData[j].name << std::endl;
                    //std::cout << "\t\t\tval value : " << recData[j].value << std::endl;
                    //std::cout << "\t\t\tval line : " << recData[j].line << std::endl;
                    //std::cout << std::endl;
                //}
            //}
        }
    }
    
}

// 줄 번호로 OpData 객체를 찾는 함수
void FindOpData(int _line) {
    // 벡터 안에서 전달받은 줄 번호에 해당하는 기록이 있는지 검사
    for(int i = 0; i < opData.size(); i++) {
         // 줄 번호가 string으로 저장되있으므로 조건문 검사를 위해 int로 변환
        int opDataLine = std::stoi(opData[i].val_line);
        if (opDataLine == _line) {
            std::cout << "\n------------------------------------------------------------------\n";
            std::cout << "\n\n" << "\033[1m" << "\tAnalysis information" << "\033[0m" << "\n\n";
            PrintOpData(opData[i]);
            std::cout << "\n------------------------------------------------------------------\n";
        }
    }
}

// 인자로 받은 OpData 값을 출력해주는 함수
void PrintOpData(OpData _data) {
    std::cout << "\t\toperation : " << _data.op << std::endl;
    std::cout << "\t\tfirst val : " << _data.val1;
    std::cout << " , value : " << FindValueName(std::stoi(_data.val_line), _data.val1);
    std::cout << " , ptr : " << FindValuePtr(std::stoi(_data.val_line), _data.val1) << std::endl;
    std::cout << "\t\tsecond val : " << _data.val2;
    std::cout << " , value : " << FindValueName(std::stoi(_data.val_line), _data.val2);
    std::cout << " , ptr : " << FindValuePtr(std::stoi(_data.val_line), _data.val2) << std::endl;
    std::cout << "\t\tresult val : " << _data.result_name;
    std::cout << " , value : " << _data.result_val;
    std::cout << " , ptr : " << _data.result_ptr << std::endl;
    std::cout << std::endl;
}

string FindValueName(int _lineNum, string _valName) {
    for(int i = _lineNum; i >= 0; i--) {
        if (recData[i].name == _valName) {
            return recData[i].value;
        }
    }
    return "";
}

string FindValuePtr(int _lineNum, string _valName) {
    for(int i = _lineNum; i >= 0; i--) {
        if (recData[i].name == _valName) {
            return recData[i].ptr;
        }
    }
    return "";
}

string TrimStr(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    if (start != std::string::npos)
    {
        return str.substr(start);
    }
    return str;
}

void PrintCode(int _codesIndex) {
    std::cout << "line Number : " << stoi(recData[_codesIndex].line) << std::endl;
    int _index = stoi(recData[_codesIndex].line);
    if(_index > 10) {
        int _startIndex = _index - 10;
        int _endIndex = _index + 10;
        for(int i = _startIndex; i <= _endIndex; i++) {
            std::cout << i << "             ";
            if(i == _index) {
                printf("\033[1C");
                std::cout << "\033[1m"  << codes[i] << "\033[0m" << std::endl; // 소스 코드를 한 줄 읽어온다
                
                std::cout << "\n\t\t------------------------------------------------------------------\n";
                std::cout << "\n\n\t\t\t" << "\033[1m" << "\tAnalysis information" << "\033[0m" << "\n\n";
                PrintRecData(recData[_codesIndex], _codesIndex);
                //FindRecData(_codesIndex);
                std::cout << "\n\t\t------------------------------------------------------------------\n";
                //FindRecData(i); // 현재 줄번호에 해당하는 변수 기록이 있는지 확인
                //FindOpData(i); // 현재 줄번호에 해당하는 op 기록이 있는지 확인
                
            }
            else {
                std::cout << codes[i] << std::endl;
            }
        }
    }
    else {
        int _startIndex = _index;
        int _endIndex = _index + 10;
        for(int i = _startIndex; i <= _endIndex; i++) {
            std::cout << i << "             ";
            if(i == _index) {
                printf("\033[1C");
                std::cout << "\033[1m"  << codes[i] << "\033[0m" << std::endl; // 소스 코드를 한 줄 읽어온다
                
                std::cout << "\n\t\t------------------------------------------------------------------\n";
                std::cout << "\n\n\t\t\t" << "\033[1m" << "\tAnalysis information" << "\033[0m" << "\n\n";
                PrintRecData(recData[_codesIndex], _codesIndex);
                std::cout << "\n\t\t------------------------------------------------------------------\n";
                //FindRecData(i); // 현재 줄번호에 해당하는 변수 기록이 있는지 확인
                //FindOpData(i); // 현재 줄번호에 해당하는 op 기록이 있는지 확인
            }
            else {
                std::cout << codes[i] << std::endl;
            }
        }
    }
}

bool isNumeric(std::string const &str)
{
    auto it = str.begin();
    while (it != str.end() && std::isdigit(*it)) {
        it++;
    }
    return !str.empty() && it == str.end();
}