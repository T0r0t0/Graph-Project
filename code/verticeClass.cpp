#include "verticeClass.h"
#include "Edge.h"
#include <algorithm>

VerticeClass::VerticeClass(): ID(0), Long(0.0), Lat(0.0) {}

VerticeClass::VerticeClass(int _ID, double _Long, double _Lat){
    ID = _ID;
    Long = _Long;
    Lat = _Lat;
};

std::string VerticeClass::to_string() const{
    std::string str = "v_ID" + std::to_string(getID()) + " " +  "v_Long" + std::to_string(getLong()) + " " +  "v_Lat" + std::to_string(getLat()) + " " + "\n" + "Edges: " + "\n";
    for (int i = 0; i < reachableVertices.size(); ++i){
        str += "\t";
        str += reachableVertices[i].to_string();
    }
    return str;

}

void VerticeClass::addEdge(const Edge newEdge){
    reachableVertices.push_back(newEdge);
}

void VerticeClass::sortEdge(){
    std::sort(reachableVertices.begin(), reachableVertices.end());
}

int VerticeClass::getID() const { return ID; }
double VerticeClass::getLong() const { return Long; }
double VerticeClass::getLat() const { return Lat; }
vector<Edge> VerticeClass::getReachableVertices() { return reachableVertices; }