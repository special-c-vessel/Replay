#include "manage_memory_unit.h"

MMU::MMU() {
    this->N = 10;
}

MMU::~MMU() {

}

void MMU::InitShadowMemories(const std::vector<RecordData*>& _records) {
    std::cout << std::endl << "===========Call InitShadowMemories func(MMU)===========" << std::endl << std::endl;

    std::cout << "records data size : " << _records.size() << std::endl;
    for(int _recordIdx = 0; _recordIdx < _records.size(); _recordIdx++) {
        ShadowMemory _shadowMemoryObj;
        _shadowMemoryObj.name = _records[_recordIdx]->name;
        _shadowMemoryObj.ptr = _records[_recordIdx]->ptr;
        _shadowMemoryObj.value = _records[_recordIdx]->value;
        shadowMemory.push_back(_shadowMemoryObj);
        std::cout << "record index : " << _recordIdx << std::endl;
        if(_recordIdx == 10 || _recordIdx == 20) {
            std::cout << "tsdsdsdsd" << std::endl;
            std::vector<ShadowMemory> _copiedShadowMemory;
            std::cout << "tsdsdsdsd" << std::endl;
            _copiedShadowMemory.assign(shadowMemory.begin(), shadowMemory.end());
            std::cout << "tsdsdsdsd" << std::endl;
            //shadowMemoryList.push_back(_copiedShadowMemory);
        }
    }

    std::cout << "Complete shadow memory initialization!!!!!, shadowMemory size is " << shadowMemory.size() << " , shadowMemoryList size is " << shadowMemoryList.size() << std::endl;

    std::cout << std::endl << "============================================================" << std::endl << std::endl;
}