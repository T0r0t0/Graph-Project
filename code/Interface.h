#ifndef INTERFACE_H
#define INTERFACE_H

#pragma once

#include <SFML/Graphics.hpp>
#include "GraphClass.h"
#include <vector>
#include <string>
#include "Vertice.h"
#include "Edge.h"

class Interface {
public:
    Interface(GraphClass& myGraph);

    void addVertice(double x, double y);
    void addEdge(double x1, double y1, double x2, double y2);

    std::vector<Vertice> vertices;
    std::vector<Edge> edges;

    sf::RenderWindow window;
    sf::View view;
    std::string file;

    void drawGraph();
    void updatePath();
    bool event();
    int searchVertice(sf::Vector2f MousePos);

    GraphClass& _myGraph;
    std::vector<sf::CircleShape> VerticeList;
    std::vector<std::vector<sf::Vertex>> EdgeList;
    std::vector<sf::RectangleShape> FinalPathEdgeList;
    std::vector<sf::CircleShape> FinalPathVerticeList;
    sf::Vector2f lastMousePos;
    bool dragging = false;

    std::string algoName = "astar";

    int vstart_id = -1;
    int vend_id = -1;

    private:
        double dist(double x1, double y1, double x2, double y2);
};
#endif