#define graphClass
#ifdef graphClass

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include "verticeClass.h"

class GraphClass{
    public:
        GraphClass(std::string filePath);
        std::string to_string() const;
    private:
        std::map<int, VerticeClass> verticeMap;
};


#endif