#ifndef EDGE_H
#define EDGE_H

#include <string>

class Edge{
    public:
        Edge(int sourceID, int destID, double weight, double length, std::string name);
        Edge();
        
        bool operator<(const Edge& other) const;
        std::string to_string() const;

        double getWeight() const;
        double getLength() const;
        std::string getName() const;
        int getSourceID() const;
        int getDestID() const;

    private:
        double _weight;
        double _length;
        std::string _name;
        int _sourceID;
        int _destID;
};

#endif