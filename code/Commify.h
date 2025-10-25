// Commify.h
// Declaration of a tiny helper type that wraps an integer and provides
// an overloaded stream operator to print the integer with thousands separators.

#ifndef COMMIFY_H
#define COMMIFY_H

#include <iostream>
#include <string>

class Commify {
private:
    // Value to format when printing
    int value;

public:
    // Construct the wrapper with the integer to format
    Commify();
    explicit Commify(int val);
    std::string to_string() const;

    // Allow streaming the formatted representation
    friend std::ostream& operator<<(std::ostream& os, const Commify& c);
    friend std::string operator+(const std::string& str, const Commify& c);
};

#endif // COMMIFY_H
