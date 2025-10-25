// GraphClass.h
// Declares GraphClass which builds a graph from a file and provides
// pathfinding routines (BFS, A* and Dijkstra). The graph stores
// Vertice objects in a map keyed by their integer IDs.

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
        // Access the internal map of vertices by ID (read-only reference)
        const std::map<int, Vertice>& getVerticeMap() const;

        // Construct the graph by reading the provided file path
        GraphClass(std::string filePath);

        // Produce a debug string describing the whole graph
        std::string to_string() const;

        // Pathfinding APIs: return pair{path_vector, visited_set}
        std::pair<std::vector<int>, std::set<int>> BFS(int vstartID, int vendID);
        std::pair<std::vector<int>, std::set<int>> AS(int vstartID, int vendID);
        std::pair<std::vector<int>, std::set<int>> DIJKSTRA(int vstartID, int vendID);

    private:

        // Heuristic / distance function used by A* (Euclidean in projected coords)
        double h(double x1, double y1, double x2, double y2);

        // Internal storage: map vertex ID -> Vertice
        std::map<int, Vertice> verticeMap;

};


#endif