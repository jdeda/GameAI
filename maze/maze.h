#ifndef MAZE_H
#define MAZE_H

#include <SFML/Graphics.hpp>
#include "../level/level.h"
#include "../graph/graph.h"
#include "maze.h"

using namespace sf;

/** Represents a maze. */
class Maze {

    public:

    /** Number of rows in the maze. */
    int rows;

    /** Number of columns in the maze. */
    int cols;
    
    /** The maze. */
    Level maze;

    /** The graph representation of the maze.*/
    Graph graph;

    /** Default constructor for the maze. */
    Maze(int rows, int cols);

};

#endif