#include "graphClass.h"
#include <iostream>


int main(int argc, char *argv[]){
    std::cout << argv[1] << std::endl;
    GraphClass myGraph(argv[1]);

    // Display the graph in the console
    std::cout << myGraph.to_string() << std::endl;
    return 0;
};