#include "Edge.h"
#include <string>

Edge::Edge(int sourceID, int destID, double weight, std::string name):
    _weight(weight), _name(name), _sourceID(sourceID), _destID(destID) {}

bool Edge::operator<(const Edge& other) const {
    return _weight < other._weight;
}

std::string Edge::to_string() const{
    return "Weight: " + std::to_string(_weight) +
    "; Name: " + _name +
    "; sourceId: " + std::to_string(_sourceID) +
    "; DestId: " + std::to_string(_destID)
    + "\n";

}