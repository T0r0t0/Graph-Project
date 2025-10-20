#ifndef EDGE_H
#define EDGE_H

#include <string>

class Edge{
    public:
        Edge(int sourceID, int destID, double weight, std::string name);
        
        bool operator<(const Edge& other) const;
        std::string to_string() const;

        double _weight;
        std::string _name;
        int _sourceID;
        int _destID;
};

#endif