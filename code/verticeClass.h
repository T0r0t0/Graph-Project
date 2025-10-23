#ifndef verticeClass
#define verticeClass


#include <string>
#include <vector>
#include "Edge.h"
using namespace std;

class VerticeClass{
    public:
        VerticeClass();
        VerticeClass(int _ID, double _x, double _y);
        VerticeClass(int _ID, double _x, double _y, double weight);

        std::string to_string() const;
        void addEdge(const Edge newEdge);
        void sortEdge();

        int getID() const;
        double getLong() const;
        double getLat() const;
        double getWeight() const;

        double setWeight();
        
        vector<Edge> getReachableVertices();

    private:
        int ID;
        double Long;
        double Lat;
        double weight;
        vector<Edge> reachableVertices;
};


#endif