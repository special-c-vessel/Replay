#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <unistd.h>
#include <chrono>


#include "record_data.h"
#include "record_array.h"
#include "record_vector.h"
#include "record_prim.h"
#include "record_struct.h"

#include "console_table.h"

#include "manage_memory_unit.h"
#include "manage_thread_unit.h"

#define CODE_SHOW_RANGE 10
#define JUDGMENT_INDEX 3

using namespace std;

enum InputState {
    Stop, Up, DoubleUp, Down, DoubleDown, Right, Left, Command, WARN
};

class App {
public: // class constructor, destructor
    App();
    App(char*, char*);
    ~App();
public: // main framework function
    void Run();
    void Init(); // 초기화 함수
    void Input(); // 사용자로부터 입력값을 받는 함수
    void Update(); // 사용자로부터 받은 입력값에 따라 내부 데이터를 업데이트하는 함수
    void Render(); // 업데이트된 내부 데이터를 토대로 콘솔에 그리는 함수
private: // app class method
    void AddRecordTable(int);

    // Find method
    int FindRecordDataPtr(std::string);
    int FindRecordDataStr(std::string);
    int FindRecordData(int);
    int FindIndexRecordData(std::string, std::string);
    int FindStructStructData(std::string);
    int FindStructRecordData(std::string, std::string, int);
    bool FindRecord(int);
    bool FindPrevRecordData(std::vector<std::string>, int);
    bool FindAfterRecordData(std::vector<std::string>, int);
    bool FindStringInString(std::string, std::string);

    // Check method
    bool IsNumber(std::string const&);
    bool IsEqualData(std::string, std::string, std::string);

    // Command method
    void InitCommand();
    bool FindCommand(std::string);

    // Error handling method
    void ErrorHandling(std::string);

    // Check & Split cut method
    std::string GetType(std::string&);
    std::string RemoveChar(const std::string&, char);
    int CountChar(std::string&, char);
    std::string RemoveLeadingWhitespace(const std::string&);
    std::vector<std::string> SplitString(const std::string&, char);

    // Time method
    void StartTime();
    void EndTime();
    
    // Color method
    void Red();
    void Blue();
    void Reset();

public: // main framework variable
    char* srcFile;
    char* recordFile;
    bool programOver;  
    int prevLine;
    int currentLine;
    int currentIndex;
    bool isDone;
    int prevCurPage;
    int afterCurPage;
    int afterTableIndex;
    int prevTableIndex;

    std::vector<RecordData*> records;
    std::vector<RecordStruct*> structs;
    std::vector<RecordData*> currentRecords;
    std::vector<std::string> codes;
    std::vector<std::string> recordLines;
    std::vector<std::string> commands;

    std::map<string, string> valShadowMemory;
    std::map<string, string> ptrShadowMemroy;

    InputState inputState;
    RecordType recordType;

    // Command valriable
    std::string commandMessage;

    // Error handling variable
    std::string systemMessage; // 시스템 메시지

    // Time check variable
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point endTime;

    // Manage memory unit
    MMU* mmu;

    // Manage thread unit
    MTU* mtu;
};