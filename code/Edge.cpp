// Edge.cpp
// Implements the simple Edge class used to represent a directed, weighted
// connection between two vertices in the graph.

#include "Edge.h"
#include <string>

// Default constructor: create an empty/zeroed edge
Edge::Edge(): _sourceID(0), _destID(0), _weight(0.0), _length(0.0), _name("") {}

// Construct an edge with source ID, destination ID, weight, length and a name
Edge::Edge(int sourceID, int destID, double weight, double length, std::string name):
    _weight(weight), _name(name), _sourceID(sourceID), _destID(destID), _length(length) {}

// Comparison operator - used to sort edges by weight (ascending)
bool Edge::operator<(const Edge& other) const {
    return _weight < other._weight;
}

// Return a small textual representation of the edge for debugging/logging
std::string Edge::to_string() const{
    return "Weight: " + std::to_string(_weight) +
    "; Name: " + _name +
    "; sourceId: " + std::to_string(_sourceID) +
    "; DestId: " + std::to_string(_destID)
    + "\n";

}

// Simple accessors used by algorithms and the interface
double Edge::getWeight() const{
    return _weight;
}
double Edge::getLength() const{
    return _length;
}
std::string Edge::getName() const{
    return _name;
}
int Edge::getSourceID() const{
    return _sourceID;
}
int Edge::getDestID() const{
    return _destID;
}