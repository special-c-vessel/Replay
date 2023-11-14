#ifndef MANAGE_MEMORY_UNIT_H
#define MANAGE_MEMORY_UNIT_H

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
#include "console_table_row.h"
#include "console_table_utils.h"

const int GROUP_SIZE = 10;

struct ShadowUnit {
    std::string name;
    std::string ptr;
    std::string value;
};

// manage memory unit class
class MMU {
public: // class constructor, destructor
    MMU();
    ~MMU();

private: // class initialization method
    void Init();

public: // Shadow memory method
    void InitShadowMemories(const std::vector<RecordData*>&);
    void CalcShadowMemory(int, const std::vector<RecordData*>&);
    void UpdateShadowMemory(ShadowUnit);
public: // Print method
    void PrintShadowMemoryListUnit();
    void PrintShadowMemoryListUnit(int);
    void PrintShadowMemoryListSize();
    void PrintShadowMemoryTable();

public: // General method
    bool IsShadowUnitInShadowMemory(std::string);

private:
    std::vector<ShadowUnit> shadowMemory; // 현재 메모리 상황을 보여주는 그림자 메모리
    std::vector<std::vector<ShadowUnit> > shadowMemoryList;

    int groupSize; // 그룹화 크기 변수
};

#endif //MANAGE_MEMORY_UNIT_H