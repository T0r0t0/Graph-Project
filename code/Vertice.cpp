#include "Vertice.h"
#include "Edge.h"
#include <algorithm>
#define _USE_MATH_DEFINES
#include <cmath>

Vertice::Vertice(): _ID(0), _long(0.0), _lat(0.0), _x(0.0), _y(0.0) {}

Vertice::Vertice(int ID, double Long, double Lat){
    _ID = ID;
    _long = Long;
    _lat = Lat;
    _x = MercatorProjectionLong2X(Long);
    _y = MercatorProjectionLat2Y(Lat);
};

std::string Vertice::to_string() const{
    std::string str = "v_ID" + std::to_string(getID()) + " " +  "v_Long" + std::to_string(getLong()) + " " +  "v_Lat" + std::to_string(getLat()) + " " + "\n" + "Edges: " + "\n";
    for (int i = 0; i < _edges.size(); ++i){
        str += "\t";
        str += _edges[i].to_string();
    }
    return str;

}

void Vertice::addEdge(const Edge newEdge){
    _edges.push_back(newEdge);
}

void Vertice::sortEdge(){
    std::sort(_edges.begin(), _edges.end());
}

int Vertice::getID() const { return _ID; }
double Vertice::getLong() const { return _long; }
double Vertice::getLat() const { return _lat; }
double Vertice::getX() const { return _x; }
double Vertice::getY() const { return _y; }
const std::vector<Edge>& Vertice::getEdges() const {
    return _edges;
}



double Vertice::deg2rad(double deg) {
    return deg * M_PI / 180.0;
}

double Vertice::MercatorProjectionLong2X(double longi){
    double x = R0 * cos(phiC) * (deg2rad(longi)   - lambdaC);
    return x;
}

double Vertice::MercatorProjectionLat2Y(double lat){
    double y = R0 * log( tan(M_PI/4.0 + (deg2rad(lat) - phiC)/2.0) );
    return y;
}