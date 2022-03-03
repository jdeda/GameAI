#include <SFML/Graphics.hpp>
#include "../level/level.h"
#include "../graph/graph.h"
#include "maze.h"
    
Maze::Maze(int rows, int cols) {
    Maze::rows = rows;
    Maze::cols = cols;
    maze = Level(rows, cols);
    graph = levelToGraph(maze);
}
