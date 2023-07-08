#include "console_table_row.h"

ConsoleTableRow::ConsoleTableRow(int _width) {
    row.resize(_width);
}

void ConsoleTableRow::AddEntry(std::string _data, int _column) {
    row[_column] = _data;
}

std::vector<std::string> ConsoleTableRow::GetEntry() {
    return row;
}

void ConsoleTableRow::EditEntry(std::string _data, int _column) {
    row[_column] = _data;
}