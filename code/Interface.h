#ifndef INTERFACE_H
#define INTERFACE_H

#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Vertice.h"
#include "Edge.h"

class Interface {
public:
    Interface();  // Affiche la carte une seule fois

    void addVertice(double x, double y);
    void addEdge(double x1, double y1, double x2, double y2);
    std::vector<Vertice> vertices;
    std::vector<Edge> edges;

    sf::RenderWindow window;
    void drawGraph();  // Dessine les points et arête
};
#endif