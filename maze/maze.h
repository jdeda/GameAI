#ifndef MAZE_H
#define MAZE_H

#include <SFML/Graphics.hpp>
#include <vector>
using namespace std;

/** X and Y coordinates in a maze. */
class Location {
    public:

    /** Constructs a location with all its fields. */
    Location(int a, int b);

    /** X coordinate in maze. */
    int x;

    /** Y coordinate in maze. */
    int y;
};

/** Represents an edge in the maze. */
class Connections {
    public:

    /** True if connection is within the maze. */
    bool inMaze = false;

    /** Possible directions at a given time (left, up, down, right). */
    char directions[4] = {false, false, false, false};
};

/** Represents cell in a Level. */
class LevelCell: sf::RectangleShape {

    public:
    /** Constructs a level cell via a given connections and location.*/
    LevelCell(const Location& location, const Connections& connections);

};

/** Represents a maze when using the mazeGenerator algorithm. */
class Level {

    public:

    /** Each list represents (dx, dy, direction delta). **/
    vector<vector<int>> NEIGHBORS = {{1, 0, 0}, {0, 1, 1}, {0, -1, 2}, {-1, 0, 3}};

    /** rowss of maze. */
    int rows;

    /** cols or columns of maze. */
    int cols;

    /** 2D grid representing maze. */
    vector<vector<Connections>> cells;

    /** Initializes level with all its fields. */
    Level(int w, int h);

    /** Start maze generation at given location. */
    void startAt(Location location);

    /** Return true if coordinates and direction allow a corridor. */
    bool canPlaceCorridor(int x, int y, int dirn);

    /** Makes connections around a location. */
    Location makeConnections(Location location);

    /** Returns Level cells as LevelCells. */
    vector<vector<LevelCell>> toSFML();

    /** Prints the level. */
    void print();
};

/** Generates a maze by filling the level at a given start. */
void generateMaze(Level level, Location start);

/** Generates a maze of given cols and rows. */
Level generateMaze(int r, int c);

#endif