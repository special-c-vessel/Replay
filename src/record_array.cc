#include "record_array.h"

RecordArray::RecordArray() {
    dimension = 1;
    std::cout << "Call Normal Constructor(RecordArray)" << std::endl;
    InitArray();
}

RecordArray::RecordArray(int _dimension) {
    std::cout << "Call Dimension Constructor(RecordArray), dimension = " << _dimension << std::endl;
    dimension = _dimension;
    InitArray();
}

void RecordArray::InitArray() {
    std::cout << "Call InitArray func(RecordArray), dimension = " << dimension << std::endl;
    switch(dimension) {
    case 1: {
        maxPageIndex = ((max_array1 % 100) / 10) +  1;
        array1.resize(max_array1);
        for(int i = 0; i < max_array1; i++) {
            array1.push_back("0");
        }
        break;
    }
    case 2: {
        maxPageIndex = ((max_array1 * max_array2) / 10) + 1;
        array2.resize(max_array1);
        for(int i = 0; i < max_array1; i++) {
            array2[i].resize(max_array2);
        }
        break;
    }
    case 3: {
        maxPageIndex = ((max_array1 * max_array2 * max_array3) / 10) + 1;
        array3.resize(max_array1);
        for(int i = 0; i < max_array1; i++) {
            array3[i].resize(max_array2);
            for(int j = 0; j < max_array2; j++) {
                array3[i][j].resize(max_array3);
                for(int z = 0; z < max_array3; z++) {
                    array3[i][j][z] = "0";
                }
            }
        }
        break;
    }
    }
}

RecordArray::~RecordArray() {
    switch(dimension) {
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    }
}

void RecordArray::InitRecordData(std::vector<std::string> _words) {
    std::cout << "Call InitRecordData func(RecordArray), dimension = " << dimension << std::endl;
    this->name = _words[1];
    this->type = _words[2];
    this->col = _words[_words.size() - 1];
    this->line = _words[_words.size() - 2];
    this->ptr = _words[_words.size() - 3];

    std::cout << ptr << std::endl;

    UpdateRecordData(_words);
}

void RecordArray::UpdateRecordData(std::vector<std::string> _words) {
    std::cout << "Call UpdateRecordData func(RecordArray), dimension = " << dimension << std::endl;
    switch(dimension) {
    case 1: {
        std::cout << "one dimension" << std::endl;
        int _arrayIndex = stoi(_words[4]);
        array1[_arrayIndex] = _words[5];
        break;
    }
    case 2: {
        std::cout << "two dimension" << std::endl;
        int _arrayIndex1 = stoi(_words[4]);
        int _arrayIndex2 = stoi(_words[5]);
        array2[_arrayIndex1][_arrayIndex2] = _words[6];
        break;
    }
    case 3: {
        std::cout << "three dimension" << std::endl;
        int _arrayIndex1 = stoi(_words[4]);
        int _arrayIndex2 = stoi(_words[5]);
        int _arrayIndex3 = stoi(_words[6]);
        std::cout << "one index : " << _arrayIndex1 << std::endl;
        std::cout << "two index : " << _arrayIndex2 << std::endl;
        std::cout << "three index : " << _arrayIndex3 << std::endl;

        array3[_arrayIndex1][_arrayIndex2][_arrayIndex3] = _words[7];
        break;
    }
    default:
        std::cout << "default dimension" << std::endl;
        break;
    }
}

void RecordArray::PrintRecordData() {
    std::cout << "\033[1m" << "\t\t\tval name : " << this->name << "\033[0m" << std::endl;
    std::cout << "\033[1m" << "\t\t\tval type : " << this->type << "\033[0m" << std::endl;
    std::cout << "\033[1m" << "\t\t\tval value : " << this->ptr << "\033[0m" << std::endl;
    std::cout << "\033[1m" << "\t\t\tval line : " << this->line << "\033[0m" << std::endl;
    switch(dimension) {
    case 1: {
        for(int i = 0 ; i < max_array1; i++) {
            std::cout << "value[" << i << "] : " << array1[i] << std::endl;
        }
        break;
    }
    case 2: {
        for(int i = 0 ; i < max_array1; i++) {
            for(int j = 0; j < max_array2; j++) {
                std::cout << "value[" << i << "][" << j << "] : " << array2[i][j] << std::endl;
            }
        }
        break;
    }
    }
    std::cout << std::endl;
}

// -1 - error
// 0  - last index
// 1  - complete
std::string RecordArray::PrintRecordTable(int _index) {
        ConsoleTable ct(BASIC);
        ct.SetPadding(1);
        ct.AddColumn("Name");
        ct.AddColumn("Type");
        ct.AddColumn("Value");
        ct.AddColumn("Ptr");
        
        switch(this->dimension) {
        case 1: {
            if(_index * 10 <= max_array1) {
                std::vector<ArrayStruct> _arrays;
                for(int i = 0; i < max_array1; i++) {
                    ArrayStruct _arrayStruct;
                    _arrayStruct.arrayName = this->name;
                    _arrayStruct.arrayType = this->type;
                    _arrayStruct.arrayValue = array1[i];
                    _arrayStruct.arrayPtr = this->ptr;
                    _arrayStruct.arrayIndex.push_back(std::to_string(i));
                    _arrays.push_back(_arrayStruct);
                }

                ct.AddColumn("Index");
                int _startIndex = (_index * 10) - 10;
                int _endIndex = (_index * 10);
                if(_startIndex > _arrays.size() - 1) _startIndex = _arrays.size() - 1;
                if(_endIndex > _arrays.size()) _endIndex = _arrays.size();
                for(int i = _startIndex ; i < _endIndex; i++) {
                    ConsoleTableRow* entry = new ConsoleTableRow(5);
                    entry->AddEntry(_arrays[i].arrayName, 0);
                    entry->AddEntry(_arrays[i].arrayType, 1);
                    entry->AddEntry(_arrays[i].arrayValue, 2);
                    entry->AddEntry(_arrays[i].arrayPtr, 3);
                    entry->AddEntry(_arrays[i].arrayIndex[0], 4);
                    ct.AddRow(entry);
                }
                ct.PrintTable();
            }
            else {
                return "WARNING this array last index is a " + max_array1;
            }
            break;
        }
        case 2: {
            std::vector<ArrayStruct> _arrays;
            for(int i = 0; i < max_array1; i++) {
                for(int j = 0; j < max_array2; j++) {
                    ArrayStruct _arrayStruct;
                    _arrayStruct.arrayName = this->name;
                    _arrayStruct.arrayType = this->type;
                    _arrayStruct.arrayValue = array2[i][j];
                    _arrayStruct.arrayPtr = this->ptr;
                    _arrayStruct.arrayIndex.push_back(std::to_string(i));
                    _arrayStruct.arrayIndex.push_back(std::to_string(j));
                    _arrays.push_back(_arrayStruct);
                }
            }

            ct.AddColumn("Index 1");
            ct.AddColumn("Index 2");

            int _startIndex = (_index * 10) - 10;
            int _endIndex = (_index * 10);
            if(_startIndex > _arrays.size() - 1) _startIndex = _arrays.size() - 1;
            if(_endIndex > _arrays.size()) _endIndex = _arrays.size();

            for(int i = _startIndex ; i < _endIndex; i++) {
                ConsoleTableRow* _entry = new ConsoleTableRow(6);
                _entry->AddEntry(_arrays[i].arrayName, 0);
                _entry->AddEntry(_arrays[i].arrayType, 1);
                _entry->AddEntry(_arrays[i].arrayValue, 2);
                _entry->AddEntry(_arrays[i].arrayPtr, 3);
                _entry->AddEntry(_arrays[i].arrayIndex[0], 4);
                _entry->AddEntry(_arrays[i].arrayIndex[1], 5);
                ct.AddRow(_entry);
            }
            ct.PrintTable();
            break;
        }
        case 3: {
            std::vector<ArrayStruct> _arrays;
            for(int i = 0; i < max_array1; i++) {
                for(int j = 0; j < max_array2; j++) {
                    for(int z = 0; z < max_array3; z++) {
                        ArrayStruct _arrayStruct;
                        _arrayStruct.arrayName = this->name;
                        _arrayStruct.arrayType = this->type;
                        _arrayStruct.arrayValue = array3[i][j][z];
                        _arrayStruct.arrayPtr = this->ptr;
                        _arrayStruct.arrayIndex.push_back(std::to_string(i));
                        _arrayStruct.arrayIndex.push_back(std::to_string(j));
                        _arrayStruct.arrayIndex.push_back(std::to_string(z));
                        _arrays.push_back(_arrayStruct);
                    }
                }
            }

            ct.AddColumn("Index 1");
            ct.AddColumn("Index 2");
            ct.AddColumn("Index 3");

            int _startIndex = (_index * 10) - 10;
            int _endIndex = (_index * 10);
            if(_startIndex > _arrays.size() - 1) _startIndex = _arrays.size() - 1;
            if(_endIndex > _arrays.size()) _endIndex = _arrays.size();

            for(int i = _startIndex ; i < _endIndex; i++) {
                ConsoleTableRow* _entry = new ConsoleTableRow(7);
                _entry->AddEntry(_arrays[i].arrayName, 0);
                _entry->AddEntry(_arrays[i].arrayType, 1);
                _entry->AddEntry(_arrays[i].arrayValue, 2);
                _entry->AddEntry(_arrays[i].arrayPtr, 3);
                _entry->AddEntry(_arrays[i].arrayIndex[0], 4);
                _entry->AddEntry(_arrays[i].arrayIndex[1], 5);
                _entry->AddEntry(_arrays[i].arrayIndex[2], 6);
                ct.AddRow(_entry);
            }
            ct.PrintTable();
            break;
        }
        }

        return "Print complete";
}