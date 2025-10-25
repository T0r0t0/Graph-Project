#include "Interface.h"
#include "GraphClass.h"
#include "Vertice.h"
#include "Edge.h"
#include <iostream>
#include <thread>
#include <chrono>
#define _USE_MATH_DEFINES
#include <cmath>


Interface::Interface(GraphClass& myGraph) : _myGraph(myGraph), window(sf::VideoMode::getDesktopMode(), "Graph Visualization"){
    view = window.getDefaultView();

    // Dessin des vertices
    for (const auto& p_iv : myGraph.getVerticeMap()) {
        addVertice(p_iv.second.getX(), p_iv.second.getY());
        const Vertice& vSource = p_iv.second;
        for (const auto& e : vSource.getEdges()) {
            const Vertice& vDest = _myGraph.getVerticeMap().at(e.getDestID());
            addEdge(vSource.getX(), vSource.getY(), vDest.getX(), vDest.getY());
        }
    }
}


void Interface::addVertice(double x, double y) {
    float rayon = 4.f;
    sf::CircleShape circle(rayon); // taille légèrement augmentée
    circle.setFillColor(sf::Color::Yellow);
    circle.setPosition(sf::Vector2f(x, window.getSize().y - y) - sf::Vector2f(rayon, rayon));
    VerticeList.push_back(circle);
}
void Interface::addEdge(double x1, double y1, double x2, double y2){
    std::vector<sf::Vertex> line;
    
    line.push_back(sf::Vertex(sf::Vector2f(x1, window.getSize().y - y1), sf::Color::Yellow));
    line.push_back(sf::Vertex(sf::Vector2f(x2, window.getSize().y - y2), sf::Color::Yellow));
    
    EdgeList.push_back(line);    
}

void Interface::drawGraph() {
    window.clear(sf::Color::Black);
    window.setView(view);  // applique la vue avant de dessiner
    for (const auto& v : VerticeList){
        window.draw(v);
    }
    for (const auto& e : EdgeList){
        window.draw(e.data(), e.size(), sf::Lines);
    }
    for (const auto& e : FinalPathEdgeList){
        window.draw(e);
    }
    for (const auto& v : FinalPathVerticeList){
        window.draw(v);
    }
    window.display();
}

void Interface::updatePath(){
    std::map<int, Vertice> map = _myGraph.getVerticeMap();
    FinalPathEdgeList.clear();
    FinalPathVerticeList.clear();

    std::pair<std::vector<int>, std::set<int>> result;

    if (algoName == "bfs"){
        result = _myGraph.BFS(vstart_id, vend_id);
    } else if (algoName == "dijkstra"){
        result = _myGraph.DIJKSTRA(vstart_id, vend_id);
    } else if (algoName == "astar"){
        result = _myGraph.AS(vstart_id, vend_id);
    }

    std::vector<int> path = result.first;
    std::set<int> closed_set = result.second;
    auto it = path.begin();
    int prev_id = *it;
    path.erase(it);

    
    for (const auto& current_id : closed_set){
            float rayon = 10.f;
            sf::CircleShape circle(rayon); // taille légèrement augmentée
            circle.setFillColor(sf::Color::Red);
            circle.setPosition(sf::Vector2f(map[current_id].getX(), window.getSize().y - map[current_id].getY()) - sf::Vector2f(rayon, rayon));
            FinalPathVerticeList.push_back(circle);
    }

    float rayon = 12.f;
    sf::CircleShape circle(rayon); // taille légèrement augmentée
    circle.setFillColor(sf::Color::Cyan);
    circle.setOutlineThickness(3.f); // épaisseur du contour
    circle.setOutlineColor(sf::Color::Magenta);
    circle.setPosition(sf::Vector2f(map[prev_id].getX(), window.getSize().y - map[prev_id].getY()) - sf::Vector2f(rayon, rayon));
    FinalPathVerticeList.push_back(circle);

    
    for (const auto& current_id : path){

        float rayon = 10.f;
        sf::CircleShape circle(rayon); // taille légèrement augmentée
        circle.setFillColor(sf::Color::Magenta);
        circle.setPosition(sf::Vector2f(map[current_id].getX(), window.getSize().y - map[current_id].getY()) - sf::Vector2f(rayon, rayon));
        FinalPathVerticeList.push_back(circle);

        



        sf::Vector2f p1(map[current_id].getX(), window.getSize().y - map[current_id].getY());
        sf::Vector2f p2(map[prev_id].getX(), window.getSize().y - map[prev_id].getY());

        sf::Vector2f direction = p2 - p1;
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        float thickness = 8.f;

        sf::RectangleShape line(sf::Vector2f(length, thickness));
        line.setOrigin(0.f, thickness / 2.f);
        line.setPosition(p1);

        // Orienter vers p2
        float angle = std::atan2(direction.y, direction.x) * 180.f / M_PI;
        line.setRotation(angle);

        line.setFillColor(sf::Color::Cyan);

        FinalPathEdgeList.push_back(line);




        prev_id = current_id;
    }
    FinalPathVerticeList.back().setRadius(12.f); 
    FinalPathVerticeList.back().setFillColor(sf::Color::Cyan);
    FinalPathVerticeList.back().setOutlineThickness(3.f); // épaisseur du contour
    FinalPathVerticeList.back().setOutlineColor(sf::Color::Magenta);
}

bool Interface::event() {
    sf::Event event;
    bool flag = false;
    bool rightClick = false;
    bool leftClick = false;
    
    while (window.pollEvent(event)) {
        flag = true;
        if (event.type == sf::Event::Closed)
            window.close();

        //  Clic Gauche
        else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            lastMousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
            lastMousePos.y = window.getSize().y - lastMousePos.y;
            leftClick = true;
        }

        //  Clic Droit
        else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
            lastMousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
            lastMousePos.y = window.getSize().y - lastMousePos.y;
            rightClick = true;
        }

        // Début du drag
        else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Middle) {
            dragging = true;
            lastMousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
        }

        // Fin du drag
        else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Middle) {
            dragging = false;
        }

        // Mouvement pendant le drag
        else if (event.type == sf::Event::MouseMoved && dragging) {
            sf::Vector2f newPos = window.mapPixelToCoords({event.mouseMove.x, event.mouseMove.y});
            sf::Vector2f delta = lastMousePos - newPos;
            view.move(delta); // déplace la vue
            lastMousePos = window.mapPixelToCoords({event.mouseMove.x, event.mouseMove.y});
        }

        // Zoom avec molette
        else if (event.type == sf::Event::MouseWheelScrolled) {
            if (event.mouseWheelScroll.delta > 0)
                view.zoom(0.9f); // zoom avant
            else
                view.zoom(1.1f); // zoom arrière
        }

        //Touche Num1
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num1) {
            std::cout << "Algorithm change en BFS" << std::endl;
            algoName = "bfs";
            updatePath();
        }
        //Touche Num2
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num2) {
            std::cout << "Algorithm change en DIJKSTRA" << std::endl;
            algoName = "dijkstra";
            updatePath();
        }
        //Touche Num3
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num3) {
            std::cout << "Algorithm change en ASTAR" << std::endl;
            algoName = "astar";
            updatePath();
        }
    }
    
    if (leftClick){
            vstart_id = searchVertice(lastMousePos);
    }
    if (rightClick){
            vend_id = searchVertice(lastMousePos);
        }
    if (rightClick || leftClick){
        if (vstart_id != -1 && vend_id != -1){
            updatePath();
        }
    }

    return flag;
}

double Interface::dist(double x1, double y1, double x2, double y2){
    return std::sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

int Interface::searchVertice(sf::Vector2f lastMousePos){
    pair<int, double> closest; //ID, Dist
    closest.second = INFINITY;
    for (const auto& p_iv : _myGraph.getVerticeMap()){
        Vertice vcurrent = p_iv.second;
        double vcurrent_dist = dist(vcurrent.getX(), vcurrent.getY(), lastMousePos.x, lastMousePos.y);
        if (vcurrent_dist < closest.second){
            closest.second = vcurrent_dist;
            closest.first = p_iv.first;
        }
    }
    return closest.first;
}