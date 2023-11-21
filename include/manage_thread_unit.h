#ifndef MANAGE_THREAD_UNIT_H
#define MANAGE_THREAD_UNIT_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>

#include "record_data.h"
#include "record_array.h"
#include "record_vector.h"
#include "record_struct.h"
#include "record_prim.h"

#include "console_table.h"

struct ThreadUnit {
    std::string id;
    int line;
    std::string color;
};

// manage thread unit class
class MTU {
public: // class constructor, destructor
    MTU();
    ~MTU();

private: // class initialize method
    void Init();

public: // thread method
    void InitThreads(const std::vector<RecordData*>&);
    void UpdateThreads(const RecordData&);

private: // check method
    bool IsThreadIdInThreads(std::string);
    int FindIdxFromThreads(std::string); // 입력받은 threadID의 인덱스를 반환

public: // get; set;
    void SetCurThreadId(std::string); // 현재 threadID 설정하는 함수
    std::string GetCurThreadId();
    std::string GetThreadId(int); // threadID 반환 함수
    std::string GetThreadId(std::string);

public: // print method
    void PrintThreads(); // thread 목록을 출력하는 함수

public: // general method
    // color method
    void SetColor(std::string, std::string); // threadID의 색을 설정하는 함수
    std::string GetColor(std::string); // 입력받은 threadID의 색을 반환하는 함수

private: // class variable
    // current thread information
    std::vector<ThreadUnit> threads;
    std::vector<std::string> threadIds;
    int threadCnt;
    std::string curThreadId;
};

#endif //MANAGE_THREAD_UNIT_H