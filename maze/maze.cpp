#include <SFML/Graphics.hpp>
#include "../level/level.h"
#include "../graph/graph.h"
#include "maze.h"

Maze::Maze(int rows, int cols) : maze(generateMaze(rows, cols)), graph(levelToGraph(maze)) {
    Maze::rows = rows;
    Maze::cols = cols;
}

Graph Maze::getGraph() {
    return graph;
}
Level Maze::getLevel() {
    return maze;
}
int Maze::getRows() { return rows; }
int Maze::getCols() { return cols; }