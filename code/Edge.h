// Edge.h
// Header describing the Edge type used in the graph. Each edge stores
// source/destination IDs, a numeric weight, a geometric length and a name.

#ifndef EDGE_H
#define EDGE_H

#include <string>

class Edge{
    public:
        // Construct an edge with explicit parameters
        Edge(int sourceID, int destID, double weight, double length, std::string name);
        // Default empty edge
        Edge();
        
        // Compare edges by weight (used by sorting / priority queues)
        bool operator<(const Edge& other) const;
        // Small human-readable representation for debugging
        std::string to_string() const;

        // Accessors
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