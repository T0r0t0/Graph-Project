#include "graphClass.h"
#include "Edge.h"
#include "verticeClass.h"
#include <regex>
#include <set>
#include <map>
#include <queue>
#include <algorithm>
#include <cmath>

GraphClass::GraphClass(std::string filePath){
    std::ifstream file(filePath);
    std::string line;
    while (std::getline(file, line)) {

        std::regex mysep(R"(\s*,\s*)");
        std::sregex_token_iterator it{line.cbegin(), line.cend(), mysep, -1};
        std::sregex_token_iterator end;


        if (*it == "V"){
            int _ID = std::stoi(*++it);
            double _x = std::stod(*++it);
            double _y = std::stod(*++it);
            VerticeClass newVertice(_ID, _x, _y);
            verticeMap[_ID] = newVertice;
        } else if (*it == "E"){
            int source_v_Id= std::stoi(*++it);
            int dest_v_Id= std::stoi(*++it);
            double weight = std::stod(*++it);
            std::string name = *++it;
            Edge newEdge = Edge(source_v_Id, dest_v_Id, weight, weight, name);
            verticeMap[source_v_Id].addEdge(newEdge);
        }
    }
};


std::string GraphClass::to_string() const{
    std::string str = "";
    for (const auto& pair : verticeMap) {
        str += pair.second.to_string() + "\n";
    }
    return str;
}

void GraphClass::BFS(int vstartID, int vendID){
    deque<int> active_queue; 
    std::set<int> closed_set; 
    std::map<int, Edge> previous;

    // ID of the start vertex 
    active_queue.push_back(vstartID);
    previous.insert_or_assign(vstartID, Edge{-1, -1, 0, 0, ""});
    do { 
        // from the current vertex in the front of the queue 
        // compute all vertices reachable in 1 step
        
            int vcurrent = active_queue.front(); 
            active_queue.pop_front(); 

        if (vcurrent == vendID) {
            std::cout << "Breadth First Search : "<< std::endl;
            std::cout << "Destination atteinte : " << vendID << std::endl;

            std::vector<int> path;
            int currentID = vendID;
            double length = 0;
            while (previous.find(currentID) != previous.end()) {
                path.push_back(currentID);
                currentID = previous[currentID].getSourceID();
            }
            std::reverse(path.begin(), path.end());

            int counterVertex = 0;
            std::cout << "Total visited vertex = " << closed_set.size() + 1 << std::endl; 
            std::cout << "Total vertex on path from start to end = " << path.size() << std::endl; 
            for (int id : path) {
                length += previous[id].getWeight();
                counterVertex++;
                std::cout << "Vertex[" << counterVertex << "] = " << id
                    << ", length = " << length << std::endl;
            }
            break;
        }

        closed_set.insert(vcurrent);
        for(Edge &enext : verticeMap[vcurrent].getReachableVertices()) {
            if (closed_set.find(enext.getDestID()) != closed_set.end()) { 
                continue; 
            } 
            
            if (std::find(active_queue.begin(), active_queue.end(), enext.getDestID()) == active_queue.end()) {
                active_queue.push_back(enext.getDestID());
                
                if (previous.find(enext.getDestID()) == previous.end()) {
                    previous.insert_or_assign(enext.getDestID(), enext);
                }
            } 
        }
    } while (active_queue.size() != 0) ;
    
}

void GraphClass::DIJKSTRA(int vstartID, int vendID){

    auto CompareEdge = [](const std::pair<Edge, double>& a, const std::pair<Edge, double>& b) {
    return a.second > b.second; // => plus petit coût = plus prioritaire
};

    std::priority_queue<std::pair<Edge, double>, std::vector<std::pair<Edge, double>>, decltype(CompareEdge)> active_queue(CompareEdge); //toujours trié avec le Vertex le moins couteux au top
    std::set<int> closed_set; //ID des Vertex déja visité
    std::map<int, std::pair<Edge, double>> previous; //pour reconstruire le chemin
    
    
    active_queue.emplace(std::make_pair(Edge{-1, vstartID, 0, 0, ""}, 0)); //initialise le premier Vertex à visiter

    do { 
        Edge ecurrent = active_queue.top().first; //extraction du premier Edge du deque<Vertex>
        double ecurrent_values = active_queue.top().second;
        active_queue.pop();

        if (closed_set.find(ecurrent.getDestID()) != closed_set.end()) continue; // vérifie si le Vertex a déja été parcouru PAS OBLIGATOIRE

        if (ecurrent.getDestID() == vendID) { //compte rendu si la destination est atteinte
            std::cout << "Dijkstra : "<< std::endl;
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
            std::cout << "Total visited vertex = " << closed_set.size() + 1 << std::endl; 
            std::cout << "Total vertex on path from start to end = " << path.size() << std::endl; 
            for (int id : path) {
                counterVertex++;
                std::cout << "Vertex[" << counterVertex << "] = " << id
                    << ", length = " << previous[id].second << std::endl;
            }
            break;
        }

        closed_set.insert(ecurrent.getDestID()); 

        for (Edge &vnext : verticeMap[ecurrent.getDestID()].getReachableVertices()) {
            if (closed_set.find(vnext.getDestID()) != closed_set.end()) continue; // vérifie si le Vertex a déja été parcouru
                
                double g = ecurrent_values + vnext.getWeight(); //Calcul le nouveau poids
                
                //ajoute le Vertex quoi qu'il arrive
                //(si le poids est plus gros ce chemin sera simplement jamais emprunté car plus long que les autres grâce au tri)
                active_queue.emplace(std::make_pair(vnext, g));

                if (previous.find(vnext.getDestID()) != previous.end()) { //change le chemin pour accéder à ce Vertex uniquement si il n'existe pas de chemin ou alors que le nouveau est plus avantageux
                    if (g < previous[vnext.getDestID()].second) previous.insert_or_assign(vnext.getDestID(), std::make_pair(vnext, g));
                } else previous.insert_or_assign(vnext.getDestID(), std::make_pair(vnext, g));

                
            }
    } while (!active_queue.empty()); //continue tant que des Vertex sont en lice
}

void GraphClass::AS(int vstartID, int vendID){

    auto CompareEdge = [](const std::pair<Edge, std::pair<double, double>>& a, const std::pair<Edge, std::pair<double, double>>& b) {
    return a.second.second > b.second.second; // => plus petit coût = plus prioritaire
};

    std::priority_queue<std::pair<Edge, std::pair<double, double>>, std::vector<std::pair<Edge, std::pair<double, double>>>, decltype(CompareEdge)> active_queue(CompareEdge); //toujours trié avec le Vertex le moins couteux au top
    std::set<int> closed_set; //ID des Vertex déja visité
    std::map<int, std::pair<Edge, std::pair<double, double>>> previous; //pour reconstruire le chemin
    
    
    active_queue.emplace(std::make_pair(Edge{-1, vstartID, 0, 0, ""}, std::make_pair(0, 0))); //initialise le premier Vertex à visiter

    do { 
        Edge ecurrent = active_queue.top().first; //extraction du premier Edge du deque<Vertex>
        std::pair<double, double> ecurrent_values = active_queue.top().second;
        active_queue.pop();

        if (closed_set.find(ecurrent.getDestID()) != closed_set.end()) continue; // vérifie si le Vertex a déja été parcouru PAS OBLIGATOIRE

        if (ecurrent.getDestID() == vendID) { //compte rendu si la destination est atteinte
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
            std::cout << "Total visited vertex = " << closed_set.size() + 1 << std::endl; 
            std::cout << "Total vertex on path from start to end = " << path.size() << std::endl; 
            for (int id : path) {
                counterVertex++;
                std::cout << "Vertex[" << counterVertex << "] = " << id
                    << ", length = " << previous[id].second.first << std::endl;
            }
            break;
        }

        closed_set.insert(ecurrent.getDestID()); 

        for (Edge &vnext : verticeMap[ecurrent.getDestID()].getReachableVertices()) {
            if (closed_set.find(vnext.getDestID()) != closed_set.end()) continue; // vérifie si le Vertex a déja été parcouru
                
                double g = ecurrent_values.first + vnext.getWeight(); //Calcul le nouveau poids
                double f = g + h(   verticeMap[vendID].getLong(),verticeMap[vendID].getLat(),
                                    verticeMap[vnext.getDestID()].getLong(),verticeMap[vnext.getDestID()].getLat());
                
                //ajoute le Vertex quoi qu'il arrive
                //(si le poids est plus gros ce chemin sera simplement jamais emprunté car plus long que les autres grâce au tri)
                active_queue.emplace(std::make_pair(vnext, std::make_pair(g, f)));

                if (previous.find(vnext.getDestID()) != previous.end()) { //change le chemin pour accéder à ce Vertex uniquement si il n'existe pas de chemin ou alors que le nouveau est plus avantageux
                    if (f < previous[vnext.getDestID()].second.second) previous.insert_or_assign(vnext.getDestID(), std::make_pair(vnext, std::make_pair(g, f)));
                } else previous.insert_or_assign(vnext.getDestID(), std::make_pair(vnext, std::make_pair(g, f)));

                
            }
    } while (!active_queue.empty()); //continue tant que des Vertex sont en lice
}

double deg2rad(double deg) {
    return deg * M_PI / 180.0;
}

double GraphClass::h(double long1, double lat1, double long2, double lat2){
    int R0 = 6378137;
    double phiC = 0.678;
    double lambdaC = -1.344;
    double x1 = R0 * cos(phiC) * (deg2rad(long1)   - lambdaC);
    double y1 = R0 * log( tan(M_PI/4.0 + (deg2rad(lat1) - phiC)/2.0) );
    double x2 = R0 * cos(phiC) * (deg2rad(long2) - lambdaC);
    double y2 = R0 * log( tan(M_PI/4.0 + (deg2rad(lat2) - phiC)/2.0) );
    return std::sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

// void GraphClass::DIJKSTRA(int vstartID, int vendID){
//     std::priority_queue<Vertex, std::vector<Vertex>, CompareVertex> active_queue; //toujours trié avec le Vertex le moins couteux au top
//     std::set<int> closed_set; //ID des Vertex déja visité
//     std::map<int, Vertex> previous; //pour reconstruire le chemin
    
    
//     active_queue.emplace(vstartID, 0); //initialise le premier Veretx à visiter

//     do { 

//         Vertex vcurrent = active_queue.top(); //extraction du premier Vertex du deque<Vertex>
//         active_queue.pop(); 

//         if (closed_set.find(vcurrent.ID) != closed_set.end()) continue; // vérifie si le Vertex a déja été parcouru PAS OBLIGATOIRE

//         if (vcurrent.ID == vendID) { //compte rendu si la destination est atteinte
//             std::cout << "Dijkstra : "<< std::endl;
//             std::cout << "Destination atteinte : " << vendID << std::endl;

//             std::vector<int> path;
//             int currentID = vendID;
            
//             while (previous.find(currentID) != previous.end()) {
//                 path.push_back(currentID);
//                 currentID = previous[currentID].ID;
//             }
//             path.push_back(vstartID);
//             std::reverse(path.begin(), path.end());

//             int counterVertex = 0;
//             std::cout << "Total visited vertex = " << closed_set.size() + 1 << std::endl; 
//             std::cout << "Total vertex on path from start to end = " << path.size() << std::endl; 
//             for (int id : path) {
//                 counterVertex++;
//                 std::cout << "Vertex[" << counterVertex << "] = " << id
//                     << ", length = " << previous[id].weight << std::endl;
//             }
//             break;
//         }

//         closed_set.insert(vcurrent.ID); 

//         for (Edge &vnext : myGraph.verticeMap[vcurrent.ID].getReachableVertices()) {
//             if (closed_set.find(vnext.destID) != closed_set.end()) continue; // vérifie si le Vertex a déja été parcouru

//                 double w = vcurrent.weight + vnext.weight; //Calcul le nouveau poids
                
//                 //ajoute le Vertex quoi qu'il arrive
//                 //(si le poids est plus gros ce chemin sera simplement jamais emprunté car plus long que les autres grâce au tri)
//                 active_queue.emplace(vnext.destID, w);

//                 if (previous.find(vnext.destID) != previous.end()) { //change le chemin pour accéder à ce Vertex uniquement si il n'existe pas de chemin ou alors que le nouveau est plus avantageux
//                     if (w < previous[vnext.destID].weight) previous.insert_or_assign(vnext.destID, Vertex{vcurrent.ID, w});
//                 } else previous.insert_or_assign(vnext.destID, Vertex{vcurrent.ID, w});

                
//             }
//     } while (!active_queue.empty()); //continue tant que des Vertex sont en lice
// }
