#ifndef GRAPHCLASS_H
#define GRAPHCLASS_H


#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "Vertice.h"
#include <set>

class GraphClass{
    public:
        const std::map<int, Vertice>& getVerticeMap() const;

        GraphClass(std::string filePath);
        std::string to_string() const;
        std::pair<std::vector<int>, std::set<int>> BFS(int vstartID, int vendID);
        std::pair<std::vector<int>, std::set<int>> AS(int vstartID, int vendID);
        std::pair<std::vector<int>, std::set<int>> DIJKSTRA(int vstartID, int vendID);
    private:

        double h(double x1, double y1, double x2, double y2);

        std::map<int, Vertice> verticeMap;

};


#endif