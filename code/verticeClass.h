#define verticeClass
#ifdef verticeClass

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

struct Edge{
    double weight;
    string name;
    int sourceID;
    int destID;

    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }

    std::string to_string() const{
        return "Weight: " + std::to_string(weight) +
       "; Name: " + name +
       "; sourceId: " + std::to_string(sourceID) +
       "; DestId: " + std::to_string(destID)
       + "\n";

    }
};


class VerticeClass{
    public:
        VerticeClass();
        VerticeClass(int _ID, double _x, double _y);

        std::string to_string() const;
        void addEdge(const Edge newEdge);
        void sortEdge();

        int getID() const;
        double getX() const;
        double getY() const;
        vector<Edge> getReachableVertices();

    private:
        int ID;
        double x;
        double y;
        vector<Edge> reachableVertices;
};


#endif