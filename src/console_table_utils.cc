#include "console_table_utils.h"

std::string ConsoleTableUtils::RepeatString(std::string _input, int _n) const {
    std::ostringstream _os;
    for (int i = 0; i < _n; i++)
        _os << _input;
    return _os.str();
}