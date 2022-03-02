#ifndef MAZE_H
#define MAZE_H

#include <SFML/Graphics.hpp>
#include <vector>

using namespace std;
using namespace sf;


/** X and Y coordinates in a maze. */
class Location
{
    public:

    /** Constructs a location with all its fields. */
    Location(int a, int b);

    /** X coordinate in maze. */
    int x;

    /** Y coordinate in maze. */
    int y;
};

/** Represents an edge in the maze. */
class Connections
{
    public:

    /** True if connection is within the maze. */
    bool inMaze = false;

    /** Possible directions at a given time (left, up, down, right). */
    char directions[4] = { false, false, false, false };
};

/** Represents cell in a Level. */
class LevelCell : RectangleShape
{

    public:

    static Vector2f dims;

    /** Constructs a level cell via a given connections and location.*/
    LevelCell(const Location& location, const Connections& connections);

    inline void draw(RenderWindow* window) {
        window->draw(*this);
    }

};

/** Represents a maze when using the mazeGenerator algorithm. */
class Level
{

    public:


    /** Each list represents (dx, dy, direction delta). */
    vector<vector<int>> NEIGHBORS = { {1, 0, 0}, {0, 1, 1}, {0, -1, 2}, {-1, 0, 3} };
    vector<vector<int>> BLOCK_NEIGHBORS = {
        {1, 0}, {0, 1}, {0, -1}, {-1, 0},
        {1, 1}, {1, -1}, {1, -1}, {-1, -1}
    };

    /** rows of maze. */
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

    /** Return true if coordinates and direction allow a corridor. */
    bool canPlaceCorridorDeep(Location o, int x, int y, int dirn);

    /** Makes connections around a location. */
    Location makeConnections(Location location);

    /** Returns Level cells as LevelCells. */
    vector<vector<LevelCell>> toSFML();

    /** Level draws itself on the window. */
    inline void draw(RenderWindow* window) {
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                LevelCell cell(Location(row, col), cells[row][col]);
                cell.draw(window);
            }
        }
    }

    /** Prints the level. */
    void print();
};

/** Generates a maze of given cols and rows. */
Level generateMaze(int r, int c, RenderWindow* window);

#endif