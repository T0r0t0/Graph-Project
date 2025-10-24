#include "Edge.h"
#include <string>


Edge::Edge(): _sourceID(0), _destID(0), _weight(0.0), _length(0.0), _name("") {}

Edge::Edge(int sourceID, int destID, double weight, double length, std::string name):
    _weight(weight), _name(name), _sourceID(sourceID), _destID(destID), _length(length) {}

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