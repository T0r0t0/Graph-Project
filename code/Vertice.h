// Vertice.h
// Declaration of Vertice: represents a graph node with geographic coordinates
// and a list of outgoing edges. Provides Mercator projection helpers used
// by rendering and heuristic calculations.

#ifndef VERTICE_H
#define VERTICE_H


#include <string>
#include <vector>
#include "Edge.h"
using namespace std;

class Vertice{
    public:
        // Default and parameterized constructors
        Vertice();
        Vertice(int ID, double Long, double Lat);

        // Debug output and adjacency management
        std::string to_string() const;
        void addEdge(const Edge newEdge);
        void sortEdge();

        // Accessors for ID, geographic coords and projected coords
        int getID() const;
        double getLong() const;
        double getLat() const;
        double getX() const;
        double getY() const;
        
        // Read-only access to incident edges
        const std::vector<Edge>& getEdges() const;


    private:
        // Projection helpers (convert lon/lat to X/Y)
        double MercatorProjectionLong2X(double longi);
        double MercatorProjectionLat2Y(double lat);
        double deg2rad(double deg);

        // Stored data
        int _ID;
        double _long; // geographic longitude
        double _lat;  // geographic latitude
        double _x;    // projected X coordinate for drawing/heuristics
        double _y;    // projected Y coordinate

        std::vector<Edge> _edges; // adjacency list
        
        // Constants used by the projection functions
        static constexpr int R0 = 6378137;
        static constexpr double phiC = 0.678;
        static constexpr double lambdaC = -1.344;
};


#endif