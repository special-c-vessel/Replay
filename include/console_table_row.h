#ifndef CONSOLETABLE_CONSOLETABLEENTRY_H
#define CONSOLETABLE_CONSOLETABLEENTRY_H

#include <string>
#include <vector>

class ConsoleTableRow {
public: // 클래스 생성자
    ConsoleTableRow(int width);
public: // 클래스 함수
    void AddEntry(std::string data, int column);
    void EditEntry(std::string data, int column);
    std::vector <std::string> GetEntry();
private: // 클래스 변수
    std::vector <std::string> row;
};

#endif //CONSOLETABLE_CONSOLETABLEENTRY_H