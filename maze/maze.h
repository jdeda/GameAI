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

    /** The graph representation of the maze .*/
    Graph graph;

    /** Default constructor for the maze. */
    Maze(int rows, int cols);

    /** Maps vertex in graph to location in maze (level). */
    inline Location localize(const graph::Vertex& vertex) {
        return graph.localize(vertex);
    }

    /** Maps location in maze (level) to GraphNode in graph. */
    inline GraphNode quantize(const Location& location) {
        return graph.quantize(location);
    }
};

#endif