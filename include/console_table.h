#ifndef CONSOLETABLE_CONSOLETABLE_H
#define CONSOLETABLE_CONSOLETABLE_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "console_table_row.h"
#include "console_table_utils.h"

enum TableStyle {
    BASIC,
    LINED,
    DOUBLE_LINE,
};

enum HorizontalSeperator{
    SEPERATOR_TOP,
    SEPERATOR_MIDDLE,
    SEPERATOR_BOTTOM
};

class ConsoleTable {
public: // 클래스 생성자
    ConsoleTable(TableStyle);
public: // 클래스 함수
    void SetPadding(unsigned int);
    void AddColumn(std::string);
    void AddRow(ConsoleTableRow*);
    bool RemoveRow(int index);
    bool EditRow(std::string, int, int);
    void PrintTable();
    void PrintHorizontalSeperator(const std::vector<int>&, HorizontalSeperator) const;
    void SetTableStyle(TableStyle);
private: // 클래스 변수
    unsigned int padding = 1;

    std::vector<std::string> columns;
    std::vector<ConsoleTableRow*> entries;
    ConsoleTableUtils* utils;

    // 테이블 스타일 변수
    std::string style_line_horizontal;
    std::string style_line_vertical;
    std::string style_line_cross;
    std::string style_t_intersect_right;
    std::string style_t_intersect_left;
    std::string style_t_intersect_top;
    std::string style_t_intersect_bottom;
    std::string style_edge_topleft;
    std::string style_edge_topright;
    std::string style_edge_buttomleft;
    std::string style_edge_buttomright;
};


#endif //CONSOLETABLE_CONSOLETABLE_H