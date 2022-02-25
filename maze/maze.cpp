#include <vector>
#include <algorithm>
#include <stack>
#include "maze.h"
#include "../debug/debug.h"

using namespace std;

Location::Location(int a, int b) {
    x = a;
    y = b;
}

Level::Level(int w, int h) {
    width = w;
    height = h;
    for(int row = 0; row < h; row++) {
        vector<Connections> v;
        for(int col = 0; col < w; col++) { v.push_back(Connections()); }
        cells.push_back(v);
    }
}

void Level::startAt(Location location) {
    cells[location.x][location.y].inMaze = true;
}

bool Level::canPlaceCorridor(int x, int y, int dirn) {
    return 0 <= x < width && 0 <= y < height && !cells[x][y].inMaze;
}

Location Level::makeConnections(Location location) {
    vector<vector<int>> neighbors = NEIGHBORS;
    random_shuffle(neighbors.begin(), neighbors.end());
    int x = location.x;
    int y = location.y;
    for (int i = 0; i < neighbors.size(); i++) {

        // Pluck triplet.
        int dx = neighbors[i][0];
        int dy = neighbors[i][1];
        int dirn = neighbors[i][2];

        // Do.
        int nx = x + dx;
        int ny = y + dy;
        int fromDirn = 3 - dirn; // TODO: Why the three?
        if (canPlaceCorridor(nx, ny, fromDirn)) {
            cells[x][y].directions[dirn] = true;
            cells[nx][ny].inMaze = true;
            cells[nx][ny].directions[fromDirn] = true;
            return Location(nx, ny);
        }
    }
    return Location(-1, -1); // None of the neighbors were vlaid.
}

/**
 * @brief Returns a randomly generated maze. 
 * 
 * @param w width of the maze
 * @param h height of the maze
 * @return generated maze as a graph
 */
void generateMaze(int w, int h) {

    // Generate maze as a level.
    maze(Level(w,h), Location(0,0));

    // Convert level to graph.

    // Return.
}

void maze(Level level, Location start) {
    stack<Location> locations;
    locations.push(start);
    level.startAt(start); // Here's you're problem (seggy = no memory init).
    sanity();

    while (!locations.empty()) {
        Location current = locations.top();
        Location next = level.makeConnections(current);
        ((next.x == -1) and (next.y = -1)) ? locations.push(next) : locations.pop();
    }
}