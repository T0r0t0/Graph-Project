#include "GraphClass.h"
#include "Edge.h"
#include "Vertice.h"
#include <regex>
#include <set>
#include <map>
// GraphClass.cpp
// Implements the GraphClass which loads a graph from a CSV-like file and
// provides pathfinding algorithms: BFS, Dijkstra and A* (AS in code).

#include "GraphClass.h"
#include "Edge.h"
#include "Vertice.h"
#include <regex>
#include <set>
#include <map>
#include <queue>
#include <algorithm>
#include <cmath>

// Construct the graph from a text file. The file format uses lines starting
// with 'V' for vertices and 'E' for edges, with comma-separated fields.
GraphClass::GraphClass(std::string filePath){
    // Open the file for reading
    std::ifstream file(filePath);
    std::string line;

    // Read the file line by line
    while (std::getline(file, line)) {

        // Split by commas allowing spaces around them
        std::regex mysep(R"(\s*,\s*)");
        std::sregex_token_iterator it{line.cbegin(), line.cend(), mysep, -1};
        std::sregex_token_iterator end;

        // If the line describes a vertex (V), parse id and coordinates
        if (*it == "V"){
            int _ID = std::stoi(*++it);          // vertex ID
            double _long = std::stod(*++it);     // longitude
            double _lat = std::stod(*++it);      // latitude
            Vertice newVertice(_ID, _long, _lat);
            // Store the vertex in the map by ID
            verticeMap[_ID] = newVertice;

        // If the line describes an edge (E), parse source, dest, weight and name
        } else if (*it == "E"){
            int source_v_Id= std::stoi(*++it);
            int dest_v_Id= std::stoi(*++it);
            double weight = std::stod(*++it);
            std::string name = *++it;
            // Compute geometric length using projected coordinates
            double length = h(verticeMap[source_v_Id].getX(), verticeMap[source_v_Id].getY(), verticeMap[dest_v_Id].getX(), verticeMap[dest_v_Id].getY());
            Edge newEdge = Edge(source_v_Id, dest_v_Id, weight, length, name);
            // Add edge to the source vertex's adjacency list
            verticeMap[source_v_Id].addEdge(newEdge);
        }
    }
};

// Return const reference to internal vertex map (used by interface and tests)
const std::map<int, Vertice>& GraphClass::getVerticeMap() const {
    return verticeMap;
}


// Build a debug string for the whole graph by concatenating vertex string forms
std::string GraphClass::to_string() const{
    std::string str = "";
    for (const auto& pair : verticeMap) {
        str += pair.second.to_string() + "\n";
    }
    return str;
}

// Breadth-First Search: returns path (vector of IDs) and the set of visited nodes
std::pair<std::vector<int>, std::set<int>> GraphClass::BFS(int vstartID, int vendID){
    deque<int> active_queue; // FIFO queue for BFS
    std::set<int> closed_set; // visited set
    std::map<int, Edge> previous; // store the edge used to reach each vertex

    // Initialize BFS with the start vertex
    active_queue.push_back(vstartID);
    previous.insert_or_assign(vstartID, Edge{-1, -1, 0, 0, ""}); // marker for start

    do {
        // Pop the next vertex to explore
            int vcurrent = active_queue.front();
            active_queue.pop_front();

        // If we reached the target, reconstruct the path using `previous`
        if (vcurrent == vendID) {
            std::cout << "Breadth First Search : "<< std::endl;
            std::cout << "Destination atteinte : " << vendID << std::endl;

            std::vector<int> path;
            int currentID = vendID;
            // Walk back via previous edges to build path
            while (previous.find(currentID) != previous.end()) {
                path.push_back(currentID);
                currentID = previous[currentID].getSourceID();
            }
            std::reverse(path.begin(), path.end());

            // Optional: print path summary and details
            int counterVertex = 0;
            double length = 0;
            double weight = 0;
            std::cout << "Total visited vertex = " << closed_set.size() + 1 << std::endl;
            std::cout << "Total vertex on path from start to end = " << path.size() << std::endl;
            for (int id : path) {
                length += previous[id].getLength();
                weight += previous[id].getWeight();
                counterVertex++;
                std::cout << "Vertex[" << counterVertex << "] = " << id
                    << ", weight = " << weight
                    << ", length = " << length << std::endl;
            }
            std::pair<std::vector<int>, std::set<int>>  result = pair{path, closed_set};
            return result;
        }

        // Mark current as visited and enqueue its neighbors
        closed_set.insert(vcurrent);
        for(const Edge &enext : verticeMap[vcurrent].getEdges()) {
            // Skip already visited destinations
            if (closed_set.find(enext.getDestID()) != closed_set.end()) {
                continue;
            }

            // If not already queued, add to the queue and record the previous edge
            if (std::find(active_queue.begin(), active_queue.end(), enext.getDestID()) == active_queue.end()) {
                active_queue.push_back(enext.getDestID());

                if (previous.find(enext.getDestID()) == previous.end()) {
                    previous.insert_or_assign(enext.getDestID(), enext);
                }
            }
        }
    } while (active_queue.size() != 0);

    // If no path found, return indicator (-1)
    return pair{std::vector<int>{-1},std::set<int>{}};

}


// Dijkstra's algorithm: finds smallest-weight path from start to end
std::pair<std::vector<int>, std::set<int>> GraphClass::DIJKSTRA(int vstartID, int vendID){

    // Comparator for priority queue: smaller total cost has higher priority
    auto CompareEdge = [](const std::pair<Edge, double>& a, const std::pair<Edge, double>& b) {
    return a.second > b.second; // smaller cost comes first
};

    // Priority queue storing pairs (edge used to reach vertex, accumulated cost)
    std::priority_queue<std::pair<Edge, double>, std::vector<std::pair<Edge, double>>, decltype(CompareEdge)> active_queue(CompareEdge);
    std::set<int> closed_set; // visited vertices
    std::map<int, std::pair<Edge, double>> previous; // edge+cost to reconstruct path

    // Seed queue with start vertex (edge with dest = start)
    active_queue.emplace(std::make_pair(Edge{-1, vstartID, 0, 0, ""}, 0));

    do {
        // Extract current best edge/cost
        Edge ecurrent = active_queue.top().first;
        double ecurrent_values = active_queue.top().second;
        active_queue.pop();

        // If this edge leads to the destination, reconstruct path
        if (ecurrent.getDestID() == vendID) {
            std::cout << "Dijkstra : "<< std::endl;
            std::cout << "Destination atteinte : " << vendID << std::endl;

            std::vector<int> path;
            int currentID = vendID;

            // Walk back using `previous` map
            while (previous.find(currentID) != previous.end()) {
                path.push_back(currentID);
                currentID = previous[currentID].first.getSourceID();
            }
            path.push_back(vstartID);
            std::reverse(path.begin(), path.end());

            // Print summary and return
            int counterVertex = 0;
            double length = 0;
            std::cout << "Total visited vertex = " << closed_set.size() + 1 << std::endl;
            std::cout << "Total vertex on path from start to end = " << path.size() << std::endl;
            for (int id : path) {
                counterVertex++;
                std::cout << "Vertex[" << counterVertex << "] = " << id
                    << ", weight = " << previous[id].second
                    << ", length = " << length << std::endl;
                length += previous[id].first.getLength();
            }
            std::pair<std::vector<int>, std::set<int>>  result = pair{path, closed_set};
            return result; // return path and visited set
        }

        // Mark the destination of the current edge as visited
        closed_set.insert(ecurrent.getDestID());

        // Explore neighbours of current vertex
        for (const Edge &enext : verticeMap[ecurrent.getDestID()].getEdges()) {
            if (closed_set.find(enext.getDestID()) != closed_set.end()) continue; // skip visited

                // g is the new accumulated cost to reach enext.dest
                double g = ecurrent_values + enext.getWeight();

                // Add candidate path to the priority queue with its accumulated cost
                active_queue.emplace(std::make_pair(enext, g));

                // Record or update the best edge/cost to reach enext.dest
                if (previous.find(enext.getDestID()) != previous.end()) {
                    if (g < previous[enext.getDestID()].second) previous.insert_or_assign(enext.getDestID(), std::make_pair(enext, g));
                } else previous.insert_or_assign(enext.getDestID(), std::make_pair(enext, g));

            }
    } while (!active_queue.empty());
    // No path found
    return pair{std::vector<int>{-1},std::set<int>{}};
}

// A* search (AS): similar to Dijkstra but uses heuristic h to guide search
std::pair<std::vector<int>, std::set<int>> GraphClass::AS(int vstartID, int vendID){

    // Comparator on f = g + h (stored as pair.second.second)
    auto CompareEdge = [](const std::pair<Edge, std::pair<double, double>>& a, const std::pair<Edge, std::pair<double, double>>& b) {
    return a.second.second > b.second.second; // smaller f comes first
};

    std::priority_queue<std::pair<Edge, std::pair<double, double>>, std::vector<std::pair<Edge, std::pair<double, double>>>, decltype(CompareEdge)> active_queue(CompareEdge);
    std::set<int> closed_set; // visited
    std::map<int, std::pair<Edge, std::pair<double, double>>> previous; // store edge with (g,f)

    // Seed with start vertex (g=0,f=0)
    active_queue.emplace(std::make_pair(Edge{-1, vstartID, 0, 0, ""}, std::make_pair(0, 0)));

    do {
        // Get candidate with lowest f
        Edge ecurrent = active_queue.top().first;
        std::pair<double, double> ecurrent_values = active_queue.top().second;
        active_queue.pop();

        // Skip if this destination was already closed
        if (closed_set.find(ecurrent.getDestID()) != closed_set.end()) continue;

        // If reached goal, reconstruct path
        if (ecurrent.getDestID() == vendID) {
            std::cout << "A-Star : "<< std::endl;
            std::cout << "Destination atteinte : " << vendID << std::endl;

            std::vector<int> path;
            int currentID = vendID;

            while (previous.find(currentID) != previous.end()) {
                path.push_back(currentID);
                currentID = previous[currentID].first.getSourceID();
            }
            path.push_back(vstartID);
            std::reverse(path.begin(), path.end());

            int counterVertex = 0;
            double length = 0;
            std::cout << "Total visited vertex = " << closed_set.size() + 1 << std::endl;
            std::cout << "Total vertex on path from start to end = " << path.size() << std::endl;
            for (int id : path) {
                counterVertex++;
                std::cout << "Vertex[" << counterVertex << "] = " << id
                    << ", weight = " << previous[id].second.first
                    << ", length = " << length << std::endl;
                length += previous[id].first.getLength();
            }
            std::pair<std::vector<int>, std::set<int>>  result = pair{path, closed_set};
            return result;
        }

        // Mark current as visited
        closed_set.insert(ecurrent.getDestID());

        // For each neighbor, compute g and f, and push candidate into priority queue
        for (const Edge &enext : verticeMap[ecurrent.getDestID()].getEdges()) {
            if (closed_set.find(enext.getDestID()) != closed_set.end()) continue;

                double g = ecurrent_values.first + enext.getWeight();
                // Heuristic: Euclidean distance in projected coordinates
                double f = g + h(   verticeMap[vendID].getX(),verticeMap[vendID].getY(),
                                    verticeMap[enext.getDestID()].getX(),verticeMap[enext.getDestID()].getY());

                // Add candidate; previous map updated if this gives a better f
                active_queue.emplace(std::make_pair(enext, std::make_pair(g, f)));

                if (previous.find(enext.getDestID()) != previous.end()) {
                    if (f < previous[enext.getDestID()].second.second) previous.insert_or_assign(enext.getDestID(), std::make_pair(enext, std::make_pair(g, f)));
                } else previous.insert_or_assign(enext.getDestID(), std::make_pair(enext, std::make_pair(g, f)));

            }
    } while (!active_queue.empty());
    return pair{std::vector<int>{-1},std::set<int>{}};
}


// Heuristic function: Euclidean distance between two projected points
double GraphClass::h(double x1, double y1, double x2, double y2){
    return std::sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}