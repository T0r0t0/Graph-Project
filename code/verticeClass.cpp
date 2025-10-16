#include "verticeClass.h"

VerticeClass::VerticeClass(): ID(0), x(0.0), y(0.0) {}

VerticeClass::VerticeClass(int _ID, double _x, double _y){
    ID = _ID;
    x = _x;
    y = _y;
};

std::string VerticeClass::to_string() const{
    std::string str = "v_ID" + std::to_string(getID()) + " " +  "v_X" + std::to_string(getX()) + " " +  "v_Y" + std::to_string(getY()) + " " + "\n" + "Edges: " + "\n";
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
double VerticeClass::getX() const { return x; }
double VerticeClass::getY() const { return y; }
vector<Edge> VerticeClass::getReachableVertices() { return reachableVertices; }