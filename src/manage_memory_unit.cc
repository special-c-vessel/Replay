#include "manage_memory_unit.h"

MMU::MMU() {    
    std::cout << "Shadow memory list size is " << shadowMemoryList.size() << std::endl;
    Init();
}

MMU::~MMU() {

}

/* Class Init method================================================================================================================= */

void MMU::Init() {
    groupSize = GROUP_SIZE; // 그룹 사이즈 초기화
}

/* Shadow memory method============================================================================================================== */

void MMU::InitShadowMemories(const std::vector<RecordData*>& _records) {
    std::cout << std::endl << "===========Call InitShadowMemories func(MMU)===========" << std::endl << std::endl;
    std::cout << "Shadow memory list size is " << shadowMemoryList.size() << std::endl;
    std::cout << "records data size : " << _records.size() << std::endl;
    std::vector<ShadowUnit> _acc;
    std::cout << "Current shadow memory size is " << _acc.size() << std::endl;

    for(int _recordIdx = 0; _recordIdx < _records.size(); _recordIdx++) { // 기록 데이터 리스트의 크기만큼 반복 수행
        std::cout << "(init)Current shadow memory size is " << _acc.size() << std::endl;
        
        if(_records[_recordIdx]->recordType == RecordType::Prim
        || _records[_recordIdx]->recordType == RecordType::String) {

            ShadowUnit _shadowUnit; // 기록 데이터의 정보를 담을 그림자 유닛
            _shadowUnit.name = _records[_recordIdx]->name;
            _shadowUnit.ptr = _records[_recordIdx]->ptr;
            _shadowUnit.value = _records[_recordIdx]->value;
            _shadowUnit.recordIndex = _recordIdx;
            PushValue(_acc, _shadowUnit);// 그림자 메모리의 적용될 그림자 유닛 리스트

            std::cout << "(end)Current shadow memory size is " << _acc.size() << std::endl;

        }
        else if(_records[_recordIdx]->recordType == RecordType::Array) {
            std::cout << "Record Data is Array data" << std::endl;
            for(int _arrayIdx = 0; _arrayIdx < _records[_recordIdx]->GetArrays().size(); _arrayIdx++) {
                ShadowUnit _shadowUnit; // 기록 데이터의 정보를 담을 그림자 유닛
                _shadowUnit.name = _records[_recordIdx]->GetArrays()[_arrayIdx].arrayName;
                _shadowUnit.ptr = _records[_recordIdx]->GetArrays()[_arrayIdx].arrayPtr;
                _shadowUnit.value = _records[_recordIdx]->GetArrays()[_arrayIdx].arrayValue;
                _shadowUnit.recordIndex = _recordIdx;
                PushValue(_acc, _shadowUnit); // 그림자 메모리의 적용될 그림자 유닛 리스트
            }
        }
        else if(_records[_recordIdx]->recordType == RecordType::Vector) {
            std::cout << "Record Data is Vector data" << std::endl;
            for(int _vectorIdx = 0; _vectorIdx < _records[_recordIdx]->GetVectors().size(); _vectorIdx++) {
                ShadowUnit _shadowUnit; // 기록 데이터의 정보를 담을 그림자 유닛
                _shadowUnit.name = _records[_recordIdx]->GetVectors()[_vectorIdx].vectorName;
                _shadowUnit.ptr = _records[_recordIdx]->GetVectors()[_vectorIdx].vectorPtr;
                _shadowUnit.value = _records[_recordIdx]->GetVectors()[_vectorIdx].vectorValue;
                _shadowUnit.recordIndex = _recordIdx;
                PushValue(_acc, _shadowUnit); // 그림자 메모리의 적용될 그림자 유닛 리스트
            }
        }

        if(_recordIdx != 0 && _recordIdx % groupSize == 0) { // 그림자 메모리 복사본 사이의 기록 데이터 개수 N 마다 그림자 메모리 복사본 생성
            std::vector<ShadowUnit> _copiedAcc(_acc.begin(), _acc.end()); // acc의 내용을 복사 후 저장
            shadowMemoryList.push_back(_copiedAcc); // 그림자 메모리 복사본을 생성
            _acc.clear(); // acc 초기화
        }
    }

    shadowMemory.clear();

    std::cout << "Complete shadow memory initialization!!!!!, shadowMemory size is " << shadowMemory.size();
    std::cout << " , shadowMemoryList size is " << shadowMemoryList.size() << std::endl;

    PrintShadowMemoryListUnit();

    std::cout << std::endl << "============================================================" << std::endl << std::endl;
}

void MMU::CalcShadowMemory(int _currentIdx, const std::vector<RecordData*>& _records) {
    shadowMemory.clear(); // 그림자 메모리를 초기화
    int _shadowMemoryCopyIdx = (_currentIdx / this->groupSize) - 1; // 그림자 메모리 복사본의 인덱스를 계산
    //std::cout << "shadow memory index : " << _shadowMemoryCopyIdx << std::endl;
    int _calcCnt = _currentIdx % this->groupSize;
    //std::cout << "calc count : " << _calcCnt << std::endl;

    if(_shadowMemoryCopyIdx >= 0) { // 그림자 메모리 복사본을 사용할 수 있을 때
        // 그림자 메모리 복사본을 메인 그림자 메모리에 복사
        //std::cout << "shadow memory copy index over the zero" << std::endl;
        for(int i = 0; i <= _shadowMemoryCopyIdx; i++) {
            for(int j = 0; j < shadowMemoryList[i].size(); j++) {
                //std::cout << "shadow memory variable name : " << shadowMemoryList[i][j].name << std::endl;
                PushValue(shadowMemory, shadowMemoryList[i][j]);
            }
        }
    }
    else {
        //std::cout << "shadow memory copy index under the zero" << std::endl;
        ShadowUnit _shadowUnit;
        _shadowUnit.name = _records[0]->name;
        _shadowUnit.ptr = _records[0]->ptr;
        _shadowUnit.value = _records[0]->value;
        shadowMemory.push_back(_shadowUnit);
    }
    //std::cout << "shadow memory size : " << shadowMemory.size() << std::endl;

    for(int _recordIdx = 1; _recordIdx <= _calcCnt; _recordIdx++) {
        int _changeIdx =  ((_currentIdx / this->groupSize) * this->groupSize) + _recordIdx;
        //std::cout << "change index " << _changeIdx << std::endl;
        if(_records[_changeIdx]->recordType == RecordType::Prim
        || _records[_changeIdx]->recordType == RecordType::String) {

            ShadowUnit _shadowUnit; // 기록 데이터의 정보를 담을 그림자 유닛
            _shadowUnit.name = _records[_changeIdx]->name;
            _shadowUnit.ptr = _records[_changeIdx]->ptr;
            _shadowUnit.value = _records[_changeIdx]->value;
            _shadowUnit.recordIndex = _changeIdx;
            PushValue(shadowMemory, _shadowUnit); // 그림자 메모리의 적용될 그림자 유닛 리스트
        }
        else if(_records[_changeIdx]->recordType == RecordType::Array) {
            //std::cout << "Record Data is Array data" << std::endl;
            for(int _arrayIdx = 0; _arrayIdx < _records[_changeIdx]->GetArrays().size(); _arrayIdx++) {
                ShadowUnit _shadowUnit; // 기록 데이터의 정보를 담을 그림자 유닛
                _shadowUnit.name = _records[_changeIdx]->GetArrays()[_arrayIdx].arrayName;
                _shadowUnit.ptr = _records[_changeIdx]->GetArrays()[_arrayIdx].arrayPtr;
                _shadowUnit.value = _records[_changeIdx]->GetArrays()[_arrayIdx].arrayValue;
                _shadowUnit.recordIndex = _changeIdx;
                PushValue(shadowMemory, _shadowUnit); // 그림자 메모리의 적용될 그림자 유닛 리스트
            }
        }
        else if(_records[_recordIdx]->recordType == RecordType::Vector) {
            //std::cout << "Record Data is Vector data" << std::endl;
            for(int _vectorIdx = 0; _vectorIdx < _records[_recordIdx]->GetVectors().size(); _vectorIdx++) {
                ShadowUnit _shadowUnit; // 기록 데이터의 정보를 담을 그림자 유닛
                _shadowUnit.name = _records[_recordIdx]->GetVectors()[_vectorIdx].vectorName;
                _shadowUnit.ptr = _records[_recordIdx]->GetVectors()[_vectorIdx].vectorPtr;
                _shadowUnit.value = _records[_recordIdx]->GetVectors()[_vectorIdx].vectorValue;
                _shadowUnit.recordIndex = _recordIdx;
                PushValue(shadowMemory, _shadowUnit); // 그림자 메모리의 적용될 그림자 유닛 리스트
            }
        }
    }
    //std::cout << "shadow memory size : " << shadowMemory.size() << std::endl;
    PrintShadowMemoryTable();
}

void MMU::UpdateShadowMemory(int _recordIdx, const std::vector<RecordData*>& _records) {

}

int MMU::GetShadowMemoryIndex(int _currentIdx) {
    for(int _sdwCopyIdx = 0; _sdwCopyIdx < shadowMemoryList.size(); _sdwCopyIdx++) {
        for(int _sdwIdx = 0; _sdwIdx < shadowMemoryList[_sdwCopyIdx].size(); _sdwIdx++) {
            if(shadowMemoryList[_sdwCopyIdx][_sdwIdx].recordIndex == _currentIdx
            && _sdwCopyIdx - 1 >= 0) {
                return _sdwCopyIdx - 1;
            }
        }
    }
    return -1;
}

/* Print Method====================================================================================================================== */

void MMU::PrintShadowMemoryListUnit() {
    std::cout << std::endl << "===========Call PrintShadowMemoryList func(MMU)===========" << std::endl << std::endl;
    for(int _shadowMemoryListIdx = 0; _shadowMemoryListIdx < shadowMemoryList.size(); _shadowMemoryListIdx++) {
        for(int _shadowMemoryIdx = 0; _shadowMemoryIdx < shadowMemoryList[_shadowMemoryListIdx].size(); _shadowMemoryIdx++) {
            std::cout <<_shadowMemoryListIdx << "'s shadowMemory index " << _shadowMemoryIdx << std::endl;
            std::cout << "name : " << shadowMemoryList[_shadowMemoryListIdx][_shadowMemoryIdx].name << std::endl;
            std::cout << "ptr : " << shadowMemoryList[_shadowMemoryListIdx][_shadowMemoryIdx].ptr << std::endl;
            std::cout << "value : " << shadowMemoryList[_shadowMemoryListIdx][_shadowMemoryIdx].value << std::endl;
            std::cout << std::endl;
        }
    }
    std::cout << std::endl << "============================================================" << std::endl << std::endl;
}

void MMU::PrintShadowMemoryListUnit(int _index){
    std::cout << std::endl << "===========Call PrintShadowMemoryList func(MMU)===========" << std::endl << std::endl;
    for(int _shadowMemoryIdx = 0; _shadowMemoryIdx < shadowMemoryList[_index].size(); _shadowMemoryIdx++) {
        std::cout << _index << "'s shadowMemory index " << _shadowMemoryIdx << std::endl;
        std::cout << "name : " << shadowMemoryList[_index][_shadowMemoryIdx].name << std::endl;
        std::cout << "ptr : " << shadowMemoryList[_index][_shadowMemoryIdx].ptr << std::endl;
        std::cout << "value : " << shadowMemoryList[_index][_shadowMemoryIdx].value << std::endl;
        std::cout << std::endl;
    }
    std::cout << std::endl << "============================================================" << std::endl << std::endl;
}

void MMU::PrintShadowMemoryListSize() {
    std::cout << std::endl << "===========Call PrintShadowMemoryListSize func(MMU)===========" << std::endl << std::endl;
    for(int _shadowMemoryListIdx = 0; _shadowMemoryListIdx < shadowMemoryList.size(); _shadowMemoryListIdx++) {
        std::cout << _shadowMemoryListIdx << "'s shadow memory size is " << shadowMemoryList[_shadowMemoryListIdx].size() << std::endl;
    }
    std::cout << std::endl << "============================================================" << std::endl << std::endl;
}

void MMU::PrintShadowMemoryTable() {
    //std::cout << "\033[100;100H";
    ConsoleTable _ct(BASIC);
    _ct.SetPadding(1);
    _ct.AddColumn(" ");
    _ct.AddColumn("Variable ptr");
    _ct.AddColumn("Variable name");
    _ct.AddColumn("Variable value");

    for(int _shadowUnitIdx = 0; _shadowUnitIdx < shadowMemory.size(); _shadowUnitIdx++) {
        ConsoleTableRow* _entry = new ConsoleTableRow(4);
        _entry->AddEntry(" ", 0);
        _entry->AddEntry(shadowMemory[_shadowUnitIdx].ptr, 1);
        _entry->AddEntry(shadowMemory[_shadowUnitIdx].name, 2);
        _entry->AddEntry(shadowMemory[_shadowUnitIdx].value, 3);

        _ct.AddRow(_entry);   
    }

    _ct.PrintTable();
    //std::cout << "\033[0;0H";
}

/* General Method===================================================================================================================== */

bool MMU::IsShadowUnitInShadowMemory(std::string _ptr) {
    for(int _sdwMemIdx = 0; _sdwMemIdx < shadowMemory.size(); _sdwMemIdx++) {
        if(shadowMemory[_sdwMemIdx].ptr == _ptr) return true;
    }
    return false;
}

// 검사 함수 정의
bool MMU::IsValidValue(const ShadowUnit& _shadowUnit) {
    // 여기에 원하는 검사 조건을 추가
    // 예시로 id가 양수인 경우를 유효한 값으로 가정
    return _shadowUnit.ptr != "";
}

// 값 푸시 함수 정의
void MMU::PushValue(std::vector<ShadowUnit>& _shadowMemory, const ShadowUnit& _shadowUnit) {
    for(int _shadowMemoryIdx = 0; _shadowMemoryIdx < _shadowMemory.size(); _shadowMemoryIdx++) {
        if(_shadowMemory[_shadowMemoryIdx].ptr == _shadowUnit.ptr) {
            _shadowMemory[_shadowMemoryIdx].name = _shadowUnit.name;
            _shadowMemory[_shadowMemoryIdx].value = _shadowUnit.value;
            return;
        }
    }
     _shadowMemory.push_back(_shadowUnit);
}