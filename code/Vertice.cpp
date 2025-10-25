// Vertice.cpp
// Implementation for Vertice: stores ID and geographic coordinates, maintains
// a list of incident edges and provides Mercator projection helpers.

#include "Vertice.h"
#include "Edge.h"
#include <algorithm>
#define _USE_MATH_DEFINES
#include <cmath>

// Default constructor: zero-initialized vertex
Vertice::Vertice(): _ID(0), _long(0.0), _lat(0.0), _x(0.0), _y(0.0) {}

// Construct a vertex with geographic coords; compute projected X/Y
Vertice::Vertice(int ID, double Long, double Lat){
    _ID = ID;
    _long = Long;
    _lat = Lat;
    // Convert geographic lon/lat into projected coordinates used for drawing and distance
    _x = MercatorProjectionLong2X(Long);
    _y = MercatorProjectionLat2Y(Lat);
};

// Return a debug string that includes the vertex data and its edges
std::string Vertice::to_string() const{
    std::string str = "v_ID" + std::to_string(getID()) + " " +  "v_Long" + std::to_string(getLong()) + " " +  "v_Lat" + std::to_string(getLat()) + " " + "\n" + "Edges: " + "\n";
    for (int i = 0; i < _edges.size(); ++i){
        str += "\t";
        str += _edges[i].to_string();
    }
    return str;

}

// Add an incident edge to this vertex's adjacency list
void Vertice::addEdge(const Edge newEdge){
    _edges.push_back(newEdge);
}

// Sort the incident edges using Edge::operator< (by weight)
void Vertice::sortEdge(){
    std::sort(_edges.begin(), _edges.end());
}

// Simple accessors
int Vertice::getID() const { return _ID; }
double Vertice::getLong() const { return _long; }
double Vertice::getLat() const { return _lat; }
double Vertice::getX() const { return _x; }
double Vertice::getY() const { return _y; }
const std::vector<Edge>& Vertice::getEdges() const {
    return _edges;
}


// Convert degrees to radians
double Vertice::deg2rad(double deg) {
    return deg * M_PI / 180.0;
}

// Mercator projection helpers: convert lon/lat to projected coordinates
double Vertice::MercatorProjectionLong2X(double longi){
    double x = R0 * cos(phiC) * (deg2rad(longi)   - lambdaC);
    return x;
}

double Vertice::MercatorProjectionLat2Y(double lat){
    double y = R0 * log( tan(M_PI/4.0 + (deg2rad(lat) - phiC)/2.0) );
    return y;
}