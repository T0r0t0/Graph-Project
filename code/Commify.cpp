#include "Commify.h"

Commify::Commify(int val) : value(val) {}

std::ostream& operator<<(std::ostream& os, const Commify& c) {
    std::string numStr = std::to_string(c.value);
    int insertPosition = static_cast<int>(numStr.length()) - 3;
    while (insertPosition > 0) {
        numStr.insert(insertPosition, ",");
        insertPosition -= 3;
    }
    os << numStr;
    return os;
}
