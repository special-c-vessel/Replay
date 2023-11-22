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
    threadCnt = 0;
    curThreadId = "none";

    for(int _recordIdx = 0; _recordIdx < _records.size(); _recordIdx++) {
        //std::cout << "record thread ID : " << _records[_recordIdx]->threadId << std::endl;
        //std::cout << "record line : " << _records[_recordIdx]->line << std::endl;
        if(!IsThreadIdInThreads(_records[_recordIdx]->threadId)) {
            std::cout << "thread ID : " << _records[_recordIdx]->threadId << std::endl;
            ThreadUnit _threadUnit;
            _threadUnit.id = std::to_string(threadCnt);
            _threadUnit.color = "\033[0;3" + std::to_string(threads.size() + 5) + "m";
            _threadUnit.line = -1;
            threadIds.push_back(_records[_recordIdx]->threadId);
            threads.push_back(_threadUnit);
            threadCnt++;
        }
    }

    std::cout << "Complete threadnma initialization!!!!!, thread is count is " << threads.size() << std::endl;
    
    std::cout << std::endl << "============================================================" << std::endl << std::endl;
}

void MTU::UpdateThreads(const RecordData& _recordData) {
    std::cout << std::endl << "=================Call UpdateThreads func(MTU)===============" << std::endl << std::endl;

    std::cout << "record data thread id : " << _recordData.threadId << ", index : " << FindIdxFromThreads(_recordData.threadId) << std::endl;

    if(FindIdxFromThreads(_recordData.threadId) != -1) {
        //std::cout << "record data thread id : " << _recordData.threadId << ", index : " << FindIdxFromThreads(_recordData.threadId) << std::endl;
        threads[FindIdxFromThreads(_recordData.threadId)].line = std::stoi(_recordData.line);
        //curThreadId = threads[FindIdxFromThreads(_recordData.threadId)].id;
    }

    PrintThreads();

    std::cout << std::endl << "============================================================" << std::endl << std::endl;
}

/* find method======================================================================================================================= */

bool MTU::IsThreadIdInThreads(std::string _threadId) {
    for(int _threadsIdx = 0; _threadsIdx < threadIds.size(); _threadsIdx++) {
        if(threadIds[_threadsIdx] == _threadId) return true;
    }
    return false;
}

int MTU::FindIdxFromThreads(std::string _threadId) {
    for(int _threadsIdx = 0; _threadsIdx < threadIds.size(); _threadsIdx++) {
        if(threads[_threadsIdx].id == _threadId) return _threadsIdx;
    }
    return -1;
}

/* get set method==================================================================================================================== */

void MTU::SetCurThreadId(std::string _threadId) {
    this->curThreadId = _threadId;
}

std::string MTU::GetCurThreadId() {
    return this->curThreadId;
}

std::string MTU::GetThreadId(int _line) {
    std::string _returnStr = "";
    bool _findFile = false;
    for (int _threadsIdx = 0; _threadsIdx < threads.size(); _threadsIdx++) {
        //std::cout << "thread line : " << threads[_threadsIdx].line << ", line : " << std::to_string(_line) << std::endl;
        if(threads[_threadsIdx].line == _line) {
            _returnStr += " " + threads[_threadsIdx].color + "Thread " + threads[_threadsIdx].id; + " ";
            _findFile = true;
            //return "\033[0;0H" + threads[_threadsIdx].color + "Thread " + threads[_threadsIdx].id + "---->\033[0m";
        }
        if(_findFile && _threadsIdx == threads.size() - 1) {
            _returnStr += "---->\033[0m";
        }
    }
    return _returnStr;
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

std::string MTU::GetThreadId(std::string _normalThreadId) {
    for(int i = 0; i < threadIds.size(); i++) {
        if(threadIds[i]  == _normalThreadId) return std::to_string(i);
    }
}
