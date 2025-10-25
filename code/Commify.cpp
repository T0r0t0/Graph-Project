// Commify.cpp
// Small utility that formats integer values with commas (thousands separators)
// so that durations or large numbers are easier to read when printed.

#include "Commify.h" // class declaration

// Construct a Commify wrapper holding an integer value to format.
Commify::Commify() : value(0) {}
Commify::Commify(int val) : value(val) {}

// Overloaded stream operator to write the formatted number to an output stream.
std::string Commify::to_string() const {
    // Convert integer to string for manipulation
    std::string numStr = std::to_string(value);

    // Start inserting commas every three digits from the right
    int insertPosition = static_cast<int>(numStr.length()) - 3;
    while (insertPosition > 0) {
        // Insert a comma at the computed position
        numStr.insert(insertPosition, ",");
        // Move three digits left for the next comma
        insertPosition -= 3;
    }
    return numStr;
}

// Overloaded stream operator to write the formatted number to an output stream.
std::ostream& operator<<(std::ostream& os, const Commify& c) {
    os << c.to_string();
    // Output the formatted string to the provided stream
    return os;
}

// Concatenation: std::string + Commify
std::string operator+(const std::string& str, const Commify& c) {
    return str + c.to_string();
}