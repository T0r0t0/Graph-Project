# Graph-Project

**Graph-Project** is a C++ application designed to manage and visualize graph data structures, providing functionalities for graph creation, manipulation, and traversal.

## Table of Contents

* [Features](#features)
* [Installation](#installation)
* [Usage](#usage)

## Features

* **Traversal Algorithms**: Implementations of BFS, A*, and Dijkstra for graph traversal.
* **Visualization**: GUI and CLI interfaces to display and interact with graph structures.
* **CLI Interface**: Command-line interface for user interaction.

## Installation

### Prerequisites

* C++17 compatible compiler
* CMake 3.10 or higher
* sfml
#### Windows /VSCode
 ...
####Linux:
   ```bash
   sudo apt install libsfml-dev  	
   ```

### Steps with cmake

1. Clone the repository:

   ```bash
   git clone https://github.com/T0r0t0/Graph-Project.git
   cd Graph-Project
   ```

2. Create a build directory and navigate into it:

   ```bash
   mkdir build
   cd build
   ```

3. Run CMake to configure the project:

   ```bash
   cmake ..
   ```

4. Build the project:

   ```bash
   cmake --build .
   ```

5. Run the application:

   ```bash
   cd ..
   ./graph --start 86771 --end 110636 --file ./map_folder/graph_dc_area.2022-03-11.txt --algorithm bfs
   ```
6. For more option you can use --help

## Usage

Graph-Project allows you to interact with graphs through both a **GUI** and **CLI**. The main functionalities include:

* **Create a graph from a CSV file**: Load vertices and edges from a CSV dataset.
* **Graph algorithms**: Apply **A***, **BFS**, or **Dijkstra** for pathfinding and traversal.
* **Visualization**: View and interact with the graph structure through a GUI or CLI.

Follow the in-app instructions for choosing the interface and executing algorithms.

