#include "Interface.h"
#include "graphClass.h"
#include "Vertice.h"
#include "Edge.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>


Interface::Interface() : window(sf::VideoMode::getDesktopMode(), "Graph Visualization"){
    drawGraph();
}

void Interface::addVertice(double x, double y) {
    int id = vertices.size();  // ID auto-incrémenté
    Vertice v(id, x, y);
    vertices.push_back(v);
}

void Interface::addEdge(double x1, double y1, double x2, double y2) {
    int id1 = -1, id2 = -1;

    for (const auto& v : vertices) {
        if (v.getX() == x1 && v.getY() == y1) id1 = v.getID();
        if (v.getX() == x2 && v.getY() == y2) id2 = v.getID();
    }

    if (id1 != -1 && id2 != -1) {
        double weight = 1.0;
        double length = std::hypot(x2 - x1, y2 - y1);
        std::string name = "Edge_" + std::to_string(id1) + "_" + std::to_string(id2);
        Edge e(id1, id2, weight, length, name);
        edges.push_back(e);
    } else {
        std::cerr << "Erreur : coordonnées non trouvées pour créer une arête.\n";
    }
}

void Interface::drawGraph() {
    
    window.clear(sf::Color::White);

    // Dessin des arêtes
    for (const auto& edge : edges) {
        const Vertice& v1 = vertices[edge.getSourceID()];
        const Vertice& v2 = vertices[edge.getDestID()];


        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(v1.getX(), v1.getY()), sf::Color::Black),
            sf::Vertex(sf::Vector2f(v2.getX(), v2.getY()), sf::Color::Black)
        };
        window.draw(line, 2, sf::Lines);
    }

    // Dessin des sommets
    for (const auto& v : vertices) {
        sf::CircleShape point(4);
        point.setFillColor(sf::Color::Red);
        point.setPosition(v.getX() - 4, v.getY() - 4);
        window.draw(point);
    }

    std::this_thread::sleep_for(std::chrono::seconds(10));
    window.close();
}
