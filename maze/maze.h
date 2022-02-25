#ifndef MAZE_H
#define MAZE_H

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

/** Represents a maze when using the mazeGenerator algorithm. */
class Level {

    public:

    /** Each list represents (dx, dy, direction delta). **/
    vector<vector<int>> NEIGHBORS = {{1, 0, 0}, {0, 1, 1}, {0, -1, 2}, {-1, 0, 3}};

    /** Height or rows of maze. */
    int height;

    /** Width or columns of maze. */
    int width;

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
};

/** Generates a maze by filling the level at a given start. */
void maze(Level level, Location start);

/** Generates a maze of given width and height. */
void generateMaze(int w, int h);

#endif