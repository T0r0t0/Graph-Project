#define verticeClass
#ifdef verticeClass

#include <string>
#include <vector>
#include "Edge.h"
using namespace std;

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