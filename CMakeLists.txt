cmake_minimum_required(VERSION 3.16)
project(graph LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Place executables in the parent folder of the build
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")

include_directories(${CMAKE_SOURCE_DIR}/include)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)


add_executable(${PROJECT_NAME} 
    code/main.cpp
    code/Edge.cpp
    code/Vertice.cpp
    code/GraphClass.cpp
    code/Interface.cpp
    code/Commify.cpp
    )

target_include_directories(${PROJECT_NAME}
    PRIVATE
        &{PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-system)

file(COPY ${CMAKE_SOURCE_DIR}/fonts DESTINATION ${CMAKE_BINARY_DIR})