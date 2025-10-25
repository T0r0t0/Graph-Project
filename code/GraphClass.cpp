#include "GraphClass.h"
#include "Edge.h"
#include "Vertice.h"
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
            double _long = std::stod(*++it);
            double _lat = std::stod(*++it);
            Vertice newVertice(_ID, _long, _lat);
            verticeMap[_ID] = newVertice;
        } else if (*it == "E"){
            int source_v_Id= std::stoi(*++it);
            int dest_v_Id= std::stoi(*++it);
            double weight = std::stod(*++it);
            std::string name = *++it;
            double length = h(verticeMap[source_v_Id].getX(), verticeMap[source_v_Id].getY(), verticeMap[dest_v_Id].getX(), verticeMap[dest_v_Id].getY());
            Edge newEdge = Edge(source_v_Id, dest_v_Id, weight, length, name);
            verticeMap[source_v_Id].addEdge(newEdge);
        }
    }
};

const std::map<int, Vertice>& GraphClass::getVerticeMap() const {
    return verticeMap;
}


std::string GraphClass::to_string() const{
    std::string str = "";
    for (const auto& pair : verticeMap) {
        str += pair.second.to_string() + "\n";
    }
    return str;
}

std::pair<std::vector<int>, std::set<int>> GraphClass::BFS(int vstartID, int vendID){
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
            while (previous.find(currentID) != previous.end()) {
                path.push_back(currentID);
                currentID = previous[currentID].getSourceID();
            }
            std::reverse(path.begin(), path.end());

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

        closed_set.insert(vcurrent);
        for(const Edge &enext : verticeMap[vcurrent].getEdges()) {
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
    } while (active_queue.size() != 0);
    return pair{std::vector<int>{-1},std::set<int>{}};
    
}


std::pair<std::vector<int>, std::set<int>> GraphClass::DIJKSTRA(int vstartID, int vendID){

    auto CompareEdge = [](const std::pair<Edge, double>& a, const std::pair<Edge, double>& b) {
    return a.second > b.second; // => plus petit coût = plus prioritaire
};

    std::priority_queue<std::pair<Edge, double>, std::vector<std::pair<Edge, double>>, decltype(CompareEdge)> active_queue(CompareEdge); //toujours trié avec le Edge le moins couteux au top
    std::set<int> closed_set; //ID des Vertice déja visité
    std::map<int, std::pair<Edge, double>> previous; //pour reconstruire le chemin final
    
    
    active_queue.emplace(std::make_pair(Edge{-1, vstartID, 0, 0, ""}, 0)); //initialise le premier Vertice à visiter

    do { 
        Edge ecurrent = active_queue.top().first; //récupération du premier Edge
        double ecurrent_values = active_queue.top().second; //et de son poids affilié
        active_queue.pop(); //suppression  de ecurrent de la queue

        if (ecurrent.getDestID() == vendID) { //compte rendu si la destination est atteinte
            std::cout << "Dijkstra : "<< std::endl;
            std::cout << "Destination atteinte : " << vendID << std::endl;

            std::vector<int> path;
            int currentID = vendID;
            
            while (previous.find(currentID) != previous.end()) { //remonte la map previous pour remplir un vecteur path
                path.push_back(currentID);
                currentID = previous[currentID].first.getSourceID();
            }
            path.push_back(vstartID);
            std::reverse(path.begin(), path.end());

            int counterVertex = 0; //affiche les resultats
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
            return result; //renvoie le chemin final (par ID) et le closed_set pour l'interface graphique
        }

        closed_set.insert(ecurrent.getDestID()); 

        for (const Edge &enext : verticeMap[ecurrent.getDestID()].getEdges()) {
            if (closed_set.find(enext.getDestID()) != closed_set.end()) continue; // vérifie si le Vertice a déja été parcouru
                
                double g = ecurrent_values + enext.getWeight(); //Calcule le nouveau poids total du chemin pour acceder au Vertice
                
                //ajoute le Vertice quoi qu'il arrive (contrainte du priority_queue)
                //(si le poids est plus gros ce chemin sera simplement jamais emprunté car plus long que les autres grâce au tri)
                active_queue.emplace(std::make_pair(enext, g));

                if (previous.find(enext.getDestID()) != previous.end()) { //change le chemin pour accéder à ce Vertice uniquement si il n'existe pas de chemin ou alors que le nouveau est plus avantageux
                    if (g < previous[enext.getDestID()].second) previous.insert_or_assign(enext.getDestID(), std::make_pair(enext, g));
                } else previous.insert_or_assign(enext.getDestID(), std::make_pair(enext, g));

                
            }
    } while (!active_queue.empty()); //continue tant que des Vertex sont en lice
    return pair{std::vector<int>{-1},std::set<int>{}};
}

std::pair<std::vector<int>, std::set<int>> GraphClass::AS(int vstartID, int vendID){

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

        closed_set.insert(ecurrent.getDestID()); 

        for (const Edge &enext : verticeMap[ecurrent.getDestID()].getEdges()) {
            if (closed_set.find(enext.getDestID()) != closed_set.end()) continue; // vérifie si le Vertex a déja été parcouru
                
                double g = ecurrent_values.first + enext.getWeight(); //Calcul le nouveau poids
                double f = g + h(   verticeMap[vendID].getX(),verticeMap[vendID].getY(),
                                    verticeMap[enext.getDestID()].getX(),verticeMap[enext.getDestID()].getY());
                
                //ajoute le Vertex quoi qu'il arrive
                //(si le poids est plus gros ce chemin sera simplement jamais emprunté car plus long que les autres grâce au tri)
                active_queue.emplace(std::make_pair(enext, std::make_pair(g, f)));

                if (previous.find(enext.getDestID()) != previous.end()) { //change le chemin pour accéder à ce Vertex uniquement si il n'existe pas de chemin ou alors que le nouveau est plus avantageux
                    if (f < previous[enext.getDestID()].second.second) previous.insert_or_assign(enext.getDestID(), std::make_pair(enext, std::make_pair(g, f)));
                } else previous.insert_or_assign(enext.getDestID(), std::make_pair(enext, std::make_pair(g, f)));

                
            }
    } while (!active_queue.empty()); //continue tant que des Vertex sont en lice
    return pair{std::vector<int>{-1},std::set<int>{}};
}


double GraphClass::h(double x1, double y1, double x2, double y2){
    return std::sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}