#ifndef VERTICE_H
#define VERTICE_H


#include <string>
#include <vector>
#include "Edge.h"
using namespace std;

class Vertice{
    public:
        Vertice();
        Vertice(int ID, double Long, double Lat);

        std::string to_string() const;
        void addEdge(const Edge newEdge);
        void sortEdge();

        int getID() const;
        double getLong() const;
        double getLat() const;
        double getX() const;
        double getY() const;
        
        const std::vector<Edge>& getEdges() const;


    private:
        double MercatorProjectionLong2X(double longi);
        double MercatorProjectionLat2Y(double lat);
        double deg2rad(double deg);

        int _ID;
        double _long;
        double _lat;
        double _x;
        double _y;

        std::vector<Edge> _edges;
        
        static constexpr int R0 = 6378137;
        static constexpr double phiC = 0.678;
        static constexpr double lambdaC = -1.344;
};


#endif