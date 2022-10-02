#include <SFML/Graphics.hpp>
#include "../level/level.h"
#include "../graph/graph.h"
#include "environment.h"

Environment::Environment() : level(generateCharacterLevel()), graph(levelToGraph(level, true)) {
    Environment::rows = level.rows;
    Environment::cols = level.cols;
}

Graph Environment::getGraph() const {
    return graph;
}
Level Environment::getLevel() const {
    return level;
}
int Environment::getRows() const { return rows; }
int Environment::getCols() const { return cols; }