#include "console_table.h"


ConsoleTable::ConsoleTable(TableStyle _style) {
    SetTableStyle(_style);
    utils = new ConsoleTableUtils();
}

void ConsoleTable::AddColumn(std::string _name) {
    columns.push_back(_name);
}

void ConsoleTable::PrintTable() {

    // Calculate column maxima
    std::vector<int> maxWidths(this->columns.size());
    for (int row = 0; row < this->entries.size(); row++) {
        for (int col = 0; col < this->columns.size(); col++) {
            std::string cellText = this->entries[row]->GetEntry()[col];
            if (this->columns[col].length() > maxWidths[col])
                maxWidths[col] = this->columns[col].length();
            if (maxWidths[col] < cellText.length()) {
                maxWidths[col] = cellText.length();
            }
        }
    }

    PrintHorizontalSeperator(maxWidths, SEPERATOR_TOP);

    // Print column values
    for (int col = 0; col < this->columns.size(); col++) {
        std::string cellText = this->columns[col];
        int len = cellText.length();
        std::string paddedText = cellText + std::string(maxWidths[col] - len, ' ');
        std::cout << this->style_line_vertical << std::string(this->padding, ' ') << paddedText
                  << std::string(this->padding, ' ');
        std::cout << (col == this->columns.size() - 1 ? this->style_line_vertical + "\n" : "");
    }

    PrintHorizontalSeperator(maxWidths, SEPERATOR_MIDDLE);

    // Print cell values
    for (int row = 0; row < this->entries.size(); row++) {
        for (int col = 0; col < this->columns.size(); col++) {
            std::string cellText = this->entries[row]->GetEntry()[col];
            std::string paddedText = cellText + std::string(maxWidths[col] - cellText.length(), ' ');
            std::cout << this->style_line_vertical << std::string(this->padding, ' ') << paddedText
                      << std::string(this->padding, ' ');
        }
        std::cout << this->style_line_vertical << std::endl;
        if (row == this->entries.size() - 1)
            PrintHorizontalSeperator(maxWidths, SEPERATOR_BOTTOM);
        else
            PrintHorizontalSeperator(maxWidths, SEPERATOR_MIDDLE);
    }
}

void ConsoleTable::PrintHorizontalSeperator(const std::vector<int>& _maxWidths, HorizontalSeperator _seperator) const {
    for (int col = 0; col < columns.size(); ++col) {

        switch (_seperator) {
            case SEPERATOR_TOP: {
                std::cout << (col == 0 ? this->style_edge_topleft : "");
                std::cout << utils->RepeatString(this->style_line_horizontal, this->padding);
                std::cout << utils->RepeatString(this->style_line_horizontal, _maxWidths[col]);
                std::cout << utils->RepeatString(this->style_line_horizontal, this->padding);
                std::cout << (col != columns.size() - 1 ? this->style_t_intersect_top : this->style_edge_topright);
                std::cout << (col == columns.size() - 1 ? "\n" : "");
                break;
            }
            case SEPERATOR_MIDDLE: {
                std::cout << (col == 0 ? this->style_t_intersect_left : "");
                std::cout << utils->RepeatString(this->style_line_horizontal, this->padding);
                std::cout << utils->RepeatString(this->style_line_horizontal, _maxWidths[col]);
                std::cout << utils->RepeatString(this->style_line_horizontal, this->padding);
                std::cout << (col != columns.size() - 1 ? this->style_line_cross : this->style_t_intersect_right);
                std::cout << (col == columns.size() - 1 ? "\n" : "");
                break;
            }
            case SEPERATOR_BOTTOM: {
                std::cout << (col == 0 ? this->style_edge_buttomleft : "");
                std::cout << utils->RepeatString(this->style_line_horizontal, this->padding);
                std::cout << utils->RepeatString(this->style_line_horizontal, _maxWidths[col]);
                std::cout << utils->RepeatString(this->style_line_horizontal, this->padding);
                std::cout
                        << (col != columns.size() - 1 ? this->style_t_intersect_bottom : this->style_edge_buttomright);
                std::cout << (col == columns.size() - 1 ? "\n" : "");
                break;
            }
        }
    }
}

void ConsoleTable::AddRow(ConsoleTableRow* _item) {
    entries.push_back(_item);
}

bool ConsoleTable::RemoveRow(int _index) {
    if (_index > entries.size())
        return false;
    this->entries.erase(this->entries.begin() + _index);
    return true;
}

bool ConsoleTable::EditRow(std::string _data, int _row, int _col) {
    if(_row > entries.size())
        return false;

    if(_col > columns.size())
        return false;

    auto entry = this->entries[_row];
    entry->EditEntry(_data, _col);
    return true;
}

void ConsoleTable::SetPadding(unsigned int _width) {
    padding = _width;
}

void ConsoleTable::SetTableStyle(TableStyle _style) {
    switch (_style) {
        case BASIC: {
            this->style_line_horizontal = "-";
            this->style_line_vertical = "|";
            this->style_line_cross = "+";

            this->style_t_intersect_right = "+";
            this->style_t_intersect_left = "+";
            this->style_t_intersect_top = "+";
            this->style_t_intersect_bottom = "+";

            this->style_edge_topleft = "+";
            this->style_edge_topright = "+";
            this->style_edge_buttomleft = "+";
            this->style_edge_buttomright = "+";
            break;
        }
        case LINED: {
            this->style_line_horizontal = "━";
            this->style_line_vertical = "┃";
            this->style_line_cross = "╋";

            this->style_t_intersect_right = "┫";
            this->style_t_intersect_left = "┣";
            this->style_t_intersect_top = "┳";
            this->style_t_intersect_bottom = "┻";

            this->style_edge_topleft = "┏";
            this->style_edge_topright = "┓";
            this->style_edge_buttomleft = "┗";
            this->style_edge_buttomright = "┛";
            break;
        }
        case DOUBLE_LINE: {
            this->style_line_horizontal = "═";
            this->style_line_vertical = "║";
            this->style_line_cross = "╬";

            this->style_t_intersect_right = "╣";
            this->style_t_intersect_left = "╠";
            this->style_t_intersect_top = "╦";
            this->style_t_intersect_bottom = "╩";

            this->style_edge_topleft = "╔";
            this->style_edge_topright = "╗";
            this->style_edge_buttomleft = "╚";
            this->style_edge_buttomright = "╝";
            break;
        }
    }
}