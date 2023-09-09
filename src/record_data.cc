#include "record_data.h"

RecordData::RecordData() 
: name(""), type(""), value(""), ptr(""), line(""), col(""), recordType(RecordType::None), infoMessage("") {
    std::cout << "Call Normal Constructor(RecordData)" << std::endl;
}

RecordData::~RecordData() {
    std::cout << "Call Normal Destory(RecordData)" << std::endl;
}

void RecordData::InitRecordData(std::vector<std::string> _words) {

}

void RecordData::UpdateRecordData(std::vector<std::string> _words) {

}

void RecordData::PrintRecordData() {

}

std::string RecordData::PrintRecordTable(std::string _message) {

}

void RecordData::SetShadowMemory(std::map<std::string, std::string> _shadowMemory) {
    
}

std::map<std::string, std::string> RecordData::GetShadowMemory() {
    
}