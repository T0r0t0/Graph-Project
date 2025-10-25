// Interface.h
// Header for the Interface class which renders the graph using SFML
// and exposes methods to build draw lists, handle user events and
// display computed paths.

#ifndef INTERFACE_H
#define INTERFACE_H

#pragma once

#include <SFML/Graphics.hpp>
#include "GraphClass.h"
#include <vector>
#include <string>
#include "Vertice.h"
#include "Edge.h"

#include <chrono>

class Interface {
public:
    // Construct the visual interface given a loaded GraphClass
    Interface(GraphClass& myGraph);

    // Helpers to create drawable primitives from graph coordinates
    void addVertice(double x, double y);
    void addEdge(double x1, double y1, double x2, double y2);

    // Optional containers (not used by rendering directly) kept for reference
    std::vector<Vertice> vertices;
    std::vector<Edge> edges;

    // SFML window/view and optional file path
    sf::RenderWindow window;
    sf::View view;
    std::string file;

    // Primary operations
    void drawGraph();      // draw base graph + path overlays
    void updatePath();     // compute and prepare path overlays
    void updateLabels();    //compute and update labels
    void update();        // main update loop call every function to update
    bool event();          // poll and handle events
    int searchVertice(sf::Vector2f MousePos); // find nearest vertex to a point

    // Reference to the underlying graph data
    GraphClass& _myGraph;

    // Drawable primitives built from graph data
    std::vector<sf::CircleShape> VerticeList;
    std::vector<std::vector<sf::Vertex>> EdgeList;
    std::vector<sf::RectangleShape> FinalPathEdgeList; // thick edges for highlighted path
    std::vector<sf::CircleShape> FinalPathVerticeList; // highlighted vertices along path
    sf::Vector2i lastMousePixel; // last processed mouse position (projected coords)
    sf::Text id_label; //Label to show vertex ID and more
    sf::Font font; //Font for the label

    bool dragging = false;     // whether middle-drag panning is active
    
    // Current algorithm selection and chosen start/end IDs
    std::string algoName = "astar";
    int vstart_id = -1;
    int vend_id = -1;

    // Timing info for performance measurement
    std::chrono::time_point<std::chrono::high_resolution_clock> start_chrono;
    std::chrono::time_point<std::chrono::high_resolution_clock> end_chrono;
    Commify duration;
    
    private:
        // Small helper: Euclidean distance in projected coordinates
        double dist(double x1, double y1, double x2, double y2);
};
#endif