#ifndef COMMIFY_H
#define COMMIFY_H

#include <iostream>
#include <string>

class Commify {
private:
    int value;

public:
    explicit Commify(int val);

    friend std::ostream& operator<<(std::ostream& os, const Commify& c);
};

#endif // COMMIFY_H
