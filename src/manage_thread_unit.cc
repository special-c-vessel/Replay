#include "manage_thread_unit.h"

MTU::MTU() {    
    Init();
}

MTU::~MTU() {

}

/* class Initiailization method====================================================================================================== */

void MTU::Init() {
    this->threadCnt = 0;
    this->curThreadId = "";
}

/* thread method===================================================================================================================== */

void MTU::InitThreads(const std::vector<RecordData*>& _records) {
    std::cout << std::endl << "==================Call InitThreads func(MTU)================" << std::endl << std::endl;

    curThreadId = _records[0]->threadId;

    for(int _recordIdx = 0; _recordIdx < _records.size(); _recordIdx++) {
        if(!IsThreadIdInThreads(_records[_recordIdx]->threadId)) {
            ThreadUnit _threadUnit;
            _threadUnit.id = _records[_recordIdx]->threadId;
            _threadUnit.color = "\033[0;3" + std::to_string(threads.size() + 5) + "m";
            _threadUnit.line = -1;
            threads.push_back(_threadUnit);
        }
    }

    threadCnt = threads.size();

    std::cout << "Complete threadnma initialization!!!!!, thread is count is " << threads.size() << std::endl;
    
    std::cout << std::endl << "============================================================" << std::endl << std::endl;
}

void MTU::UpdateThreads(const RecordData& _recordData) {
    std::cout << std::endl << "=================Call UpdateThreads func(MTU)===============" << std::endl << std::endl;

    if(IsThreadIdInThreads(_recordData.threadId)) {
        threads[FindIdxFromThreads(_recordData.threadId)].line = std::stoi(_recordData.line);
    }

    PrintThreads();

    std::cout << std::endl << "============================================================" << std::endl << std::endl;
}

/* find method======================================================================================================================= */

bool MTU::IsThreadIdInThreads(std::string _threadId) {
    for(int _threadsIdx = 0; _threadsIdx < threads.size(); _threadsIdx++) {
        if(threads[_threadsIdx].id == _threadId) return true;
    }
    return false;
}

int MTU::FindIdxFromThreads(std::string _threadId) {
    for(int _threadsIdx = 0; _threadsIdx < threads.size(); _threadsIdx++) {
        if(threads[_threadsIdx].id == _threadId) return _threadsIdx;
    }
    return -1;
}

/* get set method==================================================================================================================== */

void MTU::SetCurThreadId(std::string _threadId) {
    this->curThreadId = _threadId;
}

std::string MTU::GetThreadId(int _line) {
    for (int _threadsIdx = 0; _threadsIdx < threads.size(); _threadsIdx++) {
        if(threads[_threadsIdx].line == _line) {
            return threads[_threadsIdx].color + "Thread " + threads[_threadsIdx].id + "---->\033[0m";
        }
    }
    return "";
}

/* print method====================================================================================================================== */

void MTU::PrintThreads() {
    std::cout << std::endl << "=================Call PrintThreads func(MTU)================" << std::endl << std::endl;

    for (int _threadsIdx = 0; _threadsIdx < threads.size(); _threadsIdx++) {
        std::cout << "Thread ID : " << threads[_threadsIdx].id << ", Line number : " << threads[_threadsIdx].line << std::endl;
    }

    std::cout << std::endl << "============================================================" << std::endl << std::endl;
}

/* general method====================================================================================================================== */

void MTU::SetColor(std::string _threadId, std::string _color) {
    std::cout << "\033[0;3" + _color + "m";
}

std::string MTU::GetColor(std::string _threadId) {
    return "";
}
