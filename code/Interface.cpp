// Interface.cpp
// SFML-based rendering and event handling for visualizing the graph and paths.

#include "Interface.h"
#include "GraphClass.h"
#include "Vertice.h"
#include "Edge.h"
#include <iostream>
#include <thread>
#include <chrono>
#define _USE_MATH_DEFINES
#include <cmath>
#include <chrono>
#include "Commify.h"



// Constructor: build initial drawable lists from the graph data
Interface::Interface(GraphClass& myGraph) : _myGraph(myGraph), window(sf::VideoMode::getDesktopMode(), "Graph Visualization"){
    // Initialize view to default window view
    view = window.getDefaultView();

    // Build drawable vertex and edge lists from the loaded graph
    for (const auto& p_iv : myGraph.getVerticeMap()) {
        // Add a circle marker for each vertex
        addVertice(p_iv.second.getX(), p_iv.second.getY());
        const Vertice& vSource = p_iv.second;
        // Add line primitives for each outgoing edge
        for (const auto& e : vSource.getEdges()) {
            const Vertice& vDest = _myGraph.getVerticeMap().at(e.getDestID());
            addEdge(vSource.getX(), vSource.getY(), vDest.getX(), vDest.getY());
        }
    }
}

// Create a small circle shape at the given projected coordinates and store it
void Interface::addVertice(double x, double y) {
    float rayon = 4.f;
    sf::CircleShape circle(rayon); // circle radius
    circle.setFillColor(sf::Color::Yellow);
    // Convert to window coordinates (y axis inverted) and center the circle
    circle.setPosition(sf::Vector2f(x, window.getSize().y - y) - sf::Vector2f(rayon, rayon));
    VerticeList.push_back(circle);
}

// Add a line (two sf::Vertex points) between two projected points
void Interface::addEdge(double x1, double y1, double x2, double y2){
    std::vector<sf::Vertex> line;
    line.push_back(sf::Vertex(sf::Vector2f(x1, window.getSize().y - y1), sf::Color::Yellow));
    line.push_back(sf::Vertex(sf::Vector2f(x2, window.getSize().y - y2), sf::Color::Yellow));
    EdgeList.push_back(line);
}


// Draw the base graph and any highlighted path elements
void Interface::drawGraph() {
    window.clear(sf::Color::Black);
    // Apply the current view (pan/zoom)
    window.setView(view);
    // Draw vertices
    for (const auto& v : VerticeList){
        window.draw(v);
    }
    // Draw raw edges (stored as vectors of sf::Vertex)
    for (const auto& e : EdgeList){
        window.draw(e.data(), e.size(), sf::Lines);
    }
    // Draw highlighted final path edges and vertices
    for (const auto& e : FinalPathEdgeList){
        window.draw(e);
    }
    for (const auto& v : FinalPathVerticeList){
        window.draw(v);
    }
    window.display();
}

void Interface::update(){
    updatePath();
    updateLabels();
}

void Interface::updateLabels(){
    id_label.setFont(font);
    id_label.setCharacterSize(24);
    id_label.setFillColor(sf::Color::White);
    id_label.setString("Algorithm: " + algoName + " | Start ID: " + std::to_string(vstart_id) + " | End ID: " + std::to_string(vend_id) + " | time: " + duration + " us");

    sf::Vector2u windowSize = window.getSize();
    id_label.setPosition(0.f, windowSize.y - 30.f );

}

// Compute and prepare the path visualization depending on selected algorithm
void Interface::updatePath(){
    if (vstart_id != -1 && vend_id != -1){
        // Copy vertex map for lookup
        std::map<int, Vertice> map = _myGraph.getVerticeMap();
        FinalPathEdgeList.clear();
        FinalPathVerticeList.clear();

        std::pair<std::vector<int>, std::set<int>> result;

        start_chrono = std::chrono::high_resolution_clock::now();

        // Call selected algorithm on the graph
        if (algoName == "bfs"){
            result = _myGraph.BFS(vstart_id, vend_id);
        } else if (algoName == "dijkstra"){
            result = _myGraph.DIJKSTRA(vstart_id, vend_id);
        } else if (algoName == "astar"){
            result = _myGraph.AS(vstart_id, vend_id);
        }

        end_chrono = std::chrono::high_resolution_clock::now();
        // Compute duration
        auto time = std::chrono::duration_cast<std::chrono::microseconds>(end_chrono - start_chrono);
        duration = Commify(time.count());
        std::cout << "Execution time : " << duration << " us\n";

        // Unpack result: path vector and visited set
        std::vector<int> path = result.first;
        std::set<int> closed_set = result.second;

        // Prepare to build a highlighted path; first element is start
        auto it = path.begin();
        int prev_id = *it;
        path.erase(it);

        // Mark visited nodes (closed_set) in red
        for (const auto& current_id : closed_set){
                float rayon = 10.f;
                sf::CircleShape circle(rayon);
                circle.setFillColor(sf::Color::Red);
                circle.setPosition(sf::Vector2f(map[current_id].getX(), window.getSize().y - map[current_id].getY()) - sf::Vector2f(rayon, rayon));
                FinalPathVerticeList.push_back(circle);
        }

        // Add the starting vertex marker (distinct style)
        float rayon = 12.f;
        sf::CircleShape circle(rayon);
        circle.setFillColor(sf::Color::Cyan);
        circle.setOutlineThickness(3.f);
        circle.setOutlineColor(sf::Color::Magenta);
        circle.setPosition(sf::Vector2f(map[prev_id].getX(), window.getSize().y - map[prev_id].getY()) - sf::Vector2f(rayon, rayon));
        FinalPathVerticeList.push_back(circle);

        // Draw path vertices and connecting thick rectangles to visually connect them
        for (const auto& current_id : path){
            float rayon = 10.f;
            sf::CircleShape circle(rayon);
            circle.setFillColor(sf::Color::Magenta);
            circle.setPosition(sf::Vector2f(map[current_id].getX(), window.getSize().y - map[current_id].getY()) - sf::Vector2f(rayon, rayon));
            FinalPathVerticeList.push_back(circle);

            // Create a rotated rectangle between prev and current to represent a thick edge
            sf::Vector2f p1(map[current_id].getX(), window.getSize().y - map[current_id].getY());
            sf::Vector2f p2(map[prev_id].getX(), window.getSize().y - map[prev_id].getY());

            sf::Vector2f direction = p2 - p1;
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            float thickness = 8.f;

            sf::RectangleShape line(sf::Vector2f(length, thickness));
            line.setOrigin(0.f, thickness / 2.f);
            line.setPosition(p1);

            // Orient the rectangle so it points to p2
            float angle = std::atan2(direction.y, direction.x) * 180.f / M_PI;
            line.setRotation(angle);
            line.setFillColor(sf::Color::Cyan);
            FinalPathEdgeList.push_back(line);

            prev_id = current_id;
        }

        // Style the last vertex marker (destination) with larger radius and outline
        FinalPathVerticeList.back().setRadius(12.f);
        FinalPathVerticeList.back().setFillColor(sf::Color::Cyan);
        FinalPathVerticeList.back().setOutlineThickness(3.f);
        FinalPathVerticeList.back().setOutlineColor(sf::Color::Magenta);
    }
}


// Poll window events and translate them into interactions
bool Interface::event() {
    sf::Event event;
    bool flag = false;          // indicates whether any event was processed
    bool rightClick = false;    // remember right click occurred
    bool leftClick = false;     // remember left click occurred

    while (window.pollEvent(event)) {
        flag = true;
        if (event.type == sf::Event::Closed)
            window.close();

        // Left click: record position (used as start vertex selection)
        else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            lastMousePixel = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
            sf::Vector2f lastMousePos = window.mapPixelToCoords(lastMousePixel);
            lastMousePos.y = window.getSize().y - lastMousePos.y;
            leftClick = true;
        }

        // Right click: record position (used as end vertex selection)
        else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
            lastMousePixel = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
            sf::Vector2f lastMousePos = window.mapPixelToCoords(lastMousePixel);
            lastMousePos.y = window.getSize().y - lastMousePos.y;
            rightClick = true;
        }

        // Middle mouse pressed: begin dragging (panning)
        else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Middle) {
            dragging = true;
            lastMousePixel = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
        }

        // Middle mouse released: stop dragging
        else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Middle) {
            dragging = false;
        }

        // While dragging, move the view by mouse delta
        else if (event.type == sf::Event::MouseMoved && dragging) {
            sf::Vector2f lastMousePos = window.mapPixelToCoords(lastMousePixel);
            sf::Vector2i newPixel = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
            sf::Vector2f newPos = window.mapPixelToCoords(newPixel);
            sf::Vector2f delta = lastMousePos - newPos;
            std::cout << dist(delta.x, delta.y, 0, 0)  << ", " << delta.x << ", " << delta.y << std::endl;

            view.move(delta); // pan the view
            lastMousePixel = newPixel;
        }

        // Mouse wheel zoom: zoom in/out
        else if (event.type == sf::Event::MouseWheelScrolled) {
            if (event.mouseWheelScroll.delta > 0)
                view.zoom(0.9f); // zoom in
            else
                view.zoom(1.1f); // zoom out
        }

        // Keyboard shortcuts: change algorithm and recompute path
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num1) {
            std::cout << "Algorithm change to BFS" << std::endl;
            algoName = "bfs";
            update();
        }
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num2) {
            std::cout << "Algorithm change to DIJKSTRA" << std::endl;
            algoName = "dijkstra";
            update();
        }
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num3) {
            std::cout << "Algorithm change to ASTAR" << std::endl;
            algoName = "astar";
            update();
        }
    }

    // If mouse clicks occurred, map them to vertex selections
    if (leftClick){
            vstart_id = searchVertice(window.mapPixelToCoords(lastMousePixel));
    }
    if (rightClick){
            vend_id = searchVertice(window.mapPixelToCoords(lastMousePixel));
        }
    if (rightClick || leftClick){
         update();
    }

    return flag;
}


// Euclidean distance helper used for nearest-vertex search
double Interface::dist(double x1, double y1, double x2, double y2){
    return std::sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}


// Find the closest vertex to the given projected mouse position
int Interface::searchVertice(sf::Vector2f lastMousePos){
    pair<int, double> closest; // (vertex ID, distance)
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