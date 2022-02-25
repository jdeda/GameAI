#ifndef MAZE_H
#define MAZE_H

#include <vector>
using namespace std;

class Location {
    public:

    Location(int a, int b);
    int x;
    int y;
};

class Connections {
    public:
    bool inMaze = false;
    char directions[4] = {false, false, false, false};
};

class Level {

    public:

    // Adjacency list? What does this do? // 4x3
    vector<vector<int>> NEIGHBORS = {{1, 0, 0}, {0, 1, 1}, {0, -1, 2}, {-1, 0, 3}};

    int width;
    int heigth;
    vector<vector<Connections>> cells; // TODO: Make this fixed size...

    Level(int w, int h);
    void startAt(Location location);

    bool canPlaceCorridor(int x, int y, int dirn);

    vector<vector<int>> shuffle(vector<vector<int>> n);

    Location makeConnections(Location location);
};

void maze(Level level, Location start);

#endif