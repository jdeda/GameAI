#include <SFML/Graphics.hpp>
#include "../level/level.h"
#include "../graph/graph.h"
#include "maze.h"

// TODO: These constructors are garbage!
// TODO: Add copy constructors!
Maze::Maze(int rows, int cols) : maze(generateMaze(rows, cols)), graph(levelToGraph(maze)) {
    Level temp = generateMaze(rows, cols);
    Maze::maze = temp;
    Maze::graph = graph;
    Maze::rows = rows;
    Maze::cols = cols;
}

Graph Maze::getGraph() {
    return graph;
}
Level Maze::getLevel() {
    return maze;
}
int Maze::getRows() { return rows;}
int Maze::getCols() { return cols; }