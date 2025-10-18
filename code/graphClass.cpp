#include "graphClass.h"
#include <regex>

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
            Edge newEdge = Edge(source_v_Id, dest_v_Id, weight, name);
            verticeMap[dest_v_Id].addEdge(newEdge);
            // if (result[3] == ""){std::cout << result[4];}
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