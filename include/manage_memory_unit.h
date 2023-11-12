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

struct ShadowMemory {
    std::string name;
    std::string ptr;
    std::string value;
};

// manage memory unit class
class MMU {
public: // class constructor, destructor
    MMU();
    ~MMU();

public:
    void InitShadowMemories(const std::vector<RecordData*>&);

private:
    std::vector<ShadowMemory> shadowMemory; // 현재 메모리 상황을 보여주는 그림자 메모리
    std::vector<std::vector<ShadowMemory> > shadowMemoryList; // 그림자 메모리를 

    int N;
};

#endif //MANAGE_MEMORY_UNIT_H