cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# Place executables in the parent folder of the build
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(graph.exe 
    code/main.cpp
    code/Edge.cpp
    code/Vertice.cpp
    code/graphClass.cpp
    code/Interface.cpp
    )
