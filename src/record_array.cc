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
        for(int i = 0; i < max_array1; i++) {
            array1.push_back("0");
        }
        break;
    }
    case 2:
        array2 = (std::string **) malloc ( sizeof(std::string *) * max_array1);
        array2[0] = (std::string *) malloc ( sizeof(std::string) * max_array2 * max_array1 );
        for( int i = 1; i < max_array1; i++){
            array2[i] = array2[ i - 1 ] + max_array2;
        }
        break;
    case 3:
        array3 = new std::string **[max_array1];
        for (int i = 0; i < max_array1; i++) {
            array3[i] = new std::string *[max_array2];
            for (int j = 0; j < max_array2; j++) {
                array3[i][j] = new std::string[max_array3];
            }
        }
        break;
    }
}

RecordArray::~RecordArray() {
    switch(dimension) {
    case 1:
        break;
    case 2:
        for(int i = 1; i < max_array2; i++){
            free(array2[i]);
        }
        free(array2);
        break;
    case 3:
        for (int i = 0; i < max_array1; i++) {
            for (int j = 0; j < max_array2; j++) {
                delete[] array3[i][j];
            }
            delete[] array3[i];
        }
        delete[] array3;
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

void RecordArray::PrintRecordTable(int _index) {
        ConsoleTable ct(BASIC);
        ct.SetPadding(1);
        ct.AddColumn("Name");
        ct.AddColumn("Type");
        ct.AddColumn("Index");
        ct.AddColumn("Value");
        ct.AddColumn("Ptr");
        
        switch(dimension) {
        case 1: {
            for(int i = 10 ; i < 20; i++) {
                ConsoleTableRow* entry = new ConsoleTableRow(5);
                entry->AddEntry(this->name, 0);
                entry->AddEntry(this->type, 1);
                entry->AddEntry(std::to_string(i), 2);
                entry->AddEntry(array1[i], 3);
                entry->AddEntry(this->ptr, 4);
                ct.AddRow(entry);
            }
            break;
        }
        }

        ct.PrintTable();
}