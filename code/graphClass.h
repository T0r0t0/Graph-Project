#ifndef graphClass
#define graphClass


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
        void BFS(int vstartID, int vendID);
        void AS(int vstartID, int vendID);
        void DIJKSTRA(int vstartID, int vendID);
        double h(double long1, double lat1, double long2, double lat2);
    private:
        std::map<int, VerticeClass> verticeMap;

};


#endif