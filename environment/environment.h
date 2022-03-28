#ifndef GAME_ENVIRONMENT_H
#define GAME_ENVIRONMENT_H

#include <SFML/Graphics.hpp>
#include "../level/level.h"
#include "../graph/graph.h"

using namespace sf;

class Environment
{


    public:

    int rows;

    int cols;

    Level level;

    Graph graph;

    Environment();

    inline Location localize(const graph::Vertex& vertex) {
        return graph.localize(vertex);
    }

    inline GraphNode quantize(const Location& location) {
        return graph.quantize(location);
    }

    inline void draw(RenderWindow* window) const {
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                Location location = Location(col, row);
                LevelCell cell(location, level.cells[row][col].inLevel);
                cell.draw(window);
            }
        }
    }

    Graph getGraph() const;
    Level getLevel() const;
    int getRows() const;
    int getCols() const;
};

#endif