#include "record_data.h"

RecordData::RecordData() 
: name(""), type(""), value(""), ptr(""), line(""), col(""), recordType(RecordType::None) {
    std::cout << "Initialize record data class" << std::endl;
}

RecordData::~RecordData() {
    std::cout << "Destory record data class" << std::endl;
}

void RecordData::InitRecordData(std::vector<std::string> _words) {

}

void RecordData::UpdateRecordData(std::vector<std::string> _words) {

}

void RecordData::PrintRecordData() {

}

int RecordData::PrintRecordTable(int _index) {

}