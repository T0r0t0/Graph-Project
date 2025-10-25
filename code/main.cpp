// main.cpp
// Program entrypoint: parse CLI arguments, load the graph and either
// run the interactive visualization or execute a selected pathfinding
// algorithm and print timing information.

#include "Interface.h"
#include "GraphClass.h"
#include "Commify.h"
#include <CLI/CLI.hpp>
#include <iostream>
#include <chrono>

void launchGUI(GraphClass& myGraph, std::string algorithm, int start, int end){
    std::cout << "\n\nLaunching graphical interface to display the graph..." << std::endl;
    
    Interface interface(myGraph);
    interface.algoName = algorithm; //Set l'algorithme à utiliser
    interface.vstart_id = start; //Set le point de départ 
    interface.vend_id = end; //Set le point d'arrivée

    interface.update(); //Met à jour le path et les labels initiaux
    interface.drawGraph(); // dessine le premier visuel du graphe car on update seulement après un event

    // Boucle principale de la fenêtre
    while (interface.window.isOpen()) {
        if (interface.event()){ //Check les events et retourne true si un un event à eu lieu
            interface.drawGraph(); // dessine le graphe
        }
    }
}

int main(int argc, char* argv[]) {
    // Define a small CLI app using CLI11. The app explains its purpose.
    CLI::App app{"A command-line tool to process graph data and perform pathfinding algorithms."};

    // Flags / options (start/end default to -1 to indicate "show GUI")
    int start = 0;
    int end = 0;
    std::string algorithm = "astar";
    std::string file;
    bool isLaunchGUI = false;

    app.add_option("-s, --start", start, "Starting vertice for path search. Example: --start 0")
        ->default_val("-1");
    app.add_option("-e, --end", end, "Ending vertice for path search. Example: --end 100")
        ->default_val("-1");
    app.add_option("-a, --algorithm", algorithm, "Algorithm to use for searching. Supported: astar, bfs, dijkstra")
        ->check(CLI::IsMember({"astar", "bfs", "dijkstra"}))
        ->default_val("astar");
    app.add_option("-f, --file", file, "Path to the graph map stored as a CSV file.")
        ->required();

    app.add_flag("-g, --gui", isLaunchGUI,"Launch graphical interface to display the graph.");

    // Parse CLI arguments and exit on parse error
    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        return app.exit(e);
    }
   
    // Echo provided arguments for clarity
    std::cout << "Your flags:\n";
    std::cout << "\tstart_v: " << start << std::endl;
    std::cout << "\tend_v: " << end << std::endl;
    std::cout << "\talgorithm: " << algorithm << std::endl;
    std::cout << "\tfile: " << file << std::endl;
    
    // Load graph from file (constructor parses the CSV-like file)
    std::cout << "\n\n**Loading graph from file: " << file  << "\n..."<< std::endl;
    GraphClass myGraph(file);

    if ((start == -1 || end == -1) ){ // If either start or end is -1, run the graphical interface instead of CLI search
        launchGUI(myGraph, algorithm, start, end);
        return 0;
    }
    if (isLaunchGUI) //Demande explicite de lancer l'interface graphique
    {
        launchGUI(myGraph, algorithm, start, end);
        return 0;
    }

    // Otherwise run the selected algorithm and time the execution
    std::cout << "\n\nPerforming " << algorithm << " from " << start << " to " << end << std::endl;
    auto start_chrono = std::chrono::high_resolution_clock::now();

    if (algorithm == "bfs"){
        myGraph.BFS(start, end);
    } else if (algorithm == "astar"){
        myGraph.AS(start, end);
    } else if (algorithm == "dijkstra"){
        myGraph.DIJKSTRA(start, end);
    }

    // Stop timer and print elapsed time in microseconds using Commify for readability
    auto end_chrono = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::microseconds>(end_chrono - start_chrono);
    std::cout << "TExecution Time : " << Commify(time.count()) << " us\n";
    return 0;
}
