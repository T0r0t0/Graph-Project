#include "graphClass.h"

GraphClass::GraphClass(std::string filePath){
    std::ifstream fichier(filePath);
    std::string ligne;
    while (std::getline(fichier, ligne)) {

        std::vector<std::string> result;
        std::stringstream ss(ligne);
        std::string item;

        while (std::getline(ss, item, ',')) { // ',' est le séparateur
            result.push_back(item);
        }

        if (result[0] == "V"){
            int _ID = std::stoi(result[1]);
            double _x = std::stod(result[2]);
            double _y = std::stod(result[3]);
            VerticeClass newVertice(_ID, _x, _y);
            verticeMap[_ID] = newVertice;
        } else if (result[0] == "E"){
            Edge newEdge = {std::stod(result[3]), result[4], std::stoi(result[1]), std::stoi(result[2])};
            verticeMap[std::stoi(result[1])].addEdge(newEdge);
            if (result[3] == ""){std::cout << result[4];}
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