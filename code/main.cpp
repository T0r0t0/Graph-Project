//./graph.exe --start 86771 --end 110636 --file .\map_folder\graph_dc_area.2022-03-11.txt --algorithm bfs

#include "graphClass.h"
#include <CLI/CLI.hpp>
#include <iostream>
#include <chrono>

int main(int argc, char* argv[]) {
    CLI::App app{"A command-line tool to process graph data and perform pathfinding algorithms."};

    // Flags / options
    int start = 0;
    int end = 0;
    std::string algorithm = "astar";
    std::string file;

    app.add_option("--start", start, "Starting vertice for path search. Example: --start 0")
        ->default_val("-1");
    app.add_option("--end", end, "Ending vertice for path search. Example: --end 100")
        ->default_val("-1");
    app.add_option("--algorithm", algorithm, "Algorithm to use for searching. Supported: astar, bfs, dijkstra")
        ->check(CLI::IsMember({"astar", "bfs", "dijkstra"}))
        ->default_val("astar");
    app.add_option("--file", file, "Path to the graph map stored as a CSV file.")
        ->required();

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        return app.exit(e);
    }

    GraphClass myGraph(file);

    std::cout << "start_v: " << start << std::endl;
    std::cout << "end_v: " << end << std::endl;
    std::cout << "algorithm: " << algorithm << std::endl;
    std::cout << "file: " << file << std::endl;

    // Démarrage du chrono
    auto start_chrono = std::chrono::high_resolution_clock::now();

    if (algorithm == "bfs"){
        myGraph.BFS(start, end);
    } else if (algorithm == "astar"){
        myGraph.AS(start, end);
    } else if (algorithm == "dijkstra"){
        myGraph.DIJKSTRA(start, end);
    }

    // Fin du chrono
    auto end_chrono = std::chrono::high_resolution_clock::now();

    // Calcul de la durée
    std::chrono::duration<double> duration = end_chrono - start_chrono;

    std::cout << "Temps d'execution : " << duration.count() << " secondes\n";
    return 0;

    if (start == -1 || end == -1){ // No start and end value are given wen only show the graph
        std::cout << myGraph.to_string() << std::endl;
    }

    return 0;
}
