#ifndef graphClass
#define graphClass


#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include "Vertice.h"

class GraphClass{
    public:
        GraphClass(std::string filePath);
        std::string to_string() const;
        std::vector<int> BFS(int vstartID, int vendID);
        std::vector<int> AS(int vstartID, int vendID);
        std::vector<int> DIJKSTRA(int vstartID, int vendID);

        std::map<int, Vertice> getVertices() const;
    private:

        double h(double long1, double lat1, double long2, double lat2);
        std::map<int, Vertice> verticeMap;

};


#endif