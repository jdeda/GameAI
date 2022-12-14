#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "../graph/graph.h"
#include "location.h"

using namespace std;
using namespace sf;
using namespace graph;


enum ConnectionCost
{
    normal,
    pricey,
    expensive,
    wall
};

float mapConnectionCost(ConnectionCost cost);

/** Represents an edge in the level. */
class Connections
{
    public:

    /** Cost of a given connection. */
    ConnectionCost cost = normal;

    /** True if connection is within the level. */
    bool inLevel = false;

    /** Possible directions at a given time (right, up, down, left). */
    bool directions[4] = { false, false, false, false };

    void setFalse();
    void setTrue();
};

/** Represents cell in a Level. */
class LevelCell : public RectangleShape
{

    public:

    /** Dimensions of the cell. */
    static Vector2f dims;

    /** Constructs a level cell via a given connections and location.*/
    LevelCell(const Location& location, bool inLevel);

    /** Constructs a level cell via a given connections and status. */
    LevelCell(const Location& location, int status);

    /** Constructs a level cell via a given connections and ConnectionCost. */
    LevelCell(const Location& location, ConnectionCost connectionCost, bool flag);

    /** Draws cell onto the window. */
    inline void draw(RenderWindow* window) {
        window->draw(*this);
    }
};

/** Basic tile grid level. Represents a maze when using the mazeGenerator algorithm. */
class Level
{
    public:

    /**
     * Each list represents (dx, dy, direction index) for cells (in order right, up, down, left) of the cell.
     * TRANSPOSE!: this is really: down, right, left, up
     * */
    vector<vector<int>> NEIGHBORS = { {1, 0, 0}, {0, 1, 1}, {0, -1, 2}, {-1, 0, 3} };

    /** Each list pair represents possible corner neighbors dx and dy. (in order of right, up, down, left). */
    vector<vector<vector<int>>> BLOCK_NEIGHBORS = { {{1, 1}, {1, -1}}, {{1, 1}, {-1, 1}}, {{1, -1}, {-1, -1}}, {{-1, 1}, {-1, -1}} };

    /** Number of rows of level. */
    int rows;

    /** Number of columns of level. */
    int cols;

    /** 2D grid representing level. */
    vector<vector<Connections>> cells;

    /** Initializes level with all its fields. */
    Level(int w, int h);

    /** Copy constructor. */
    Level(const Level& level);

    /** Start level generation at given location. */
    void startAt(Location location);

    /** Determines if the given coordinates are within bounds of the level. */
    bool inBounds(int x, int y) const;

    /** Return true if coordinates and direction allow a corridor. */
    bool canPlaceCorridor(int x, int y) const;

    /** Return true if coordinates and direction allow a corridor. */
    bool canPlaceCorridorDeep(Location o, int x, int y, int fromDirIdx) const;

    /** Makes connections around a location. */
    Location makeConnections(Location location);

    /** Returns index repressenting direction of expansion from o to (x,y). */
    int getDirIdx(Location o, int x, int y) const;

    /** Returns Level cells as LevelCells. */
    vector<vector<LevelCell>> toSFML() const;

    /** Level draws itself on the window. */
    inline void draw(RenderWindow* window) {
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                LevelCell cell(Location(row, col), cells[row][col].inLevel);
                cell.draw(window);
            }
        }
    }

    /** Level draws itself on the window. */
    inline void drawSpecial(RenderWindow* window) {
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                LevelCell cell(Location(row, col), cells[row][col].cost, true);
                cell.draw(window);
            }
        }
    }

    /** Prints the level. */
    void print();

    /** Getters. */
    int getRows();
    int getCols();
};

/** Generates a maze of given cols and rows. */
Level generateMaze(int r, int c);

/** Converts the given level into a graph and returns it.*/
Graph levelToGraph(const Level& level);

/** Converts the given level into a graph and returns it.*/
Graph levelToGraph(const Level& level, bool flag);

/** Generates fixed size level. */
Level generateCharacterLevel();

#endif