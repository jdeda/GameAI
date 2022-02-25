#include <vector>
#include "maze.h"

using namespace std;

Location::Location(int a, int b) {
    x = a;
    y = b;
}

Level::Level(int w, int h) {
    width = w;
    heigth = h;
}

void Level::startAt(Location location) {
    cells[location.x][location.y].inMaze = true;
}

bool Level::canPlaceCorridor(int x, int y, int dirn) {
    return 0 <= x < width && 0 <= y < heigth && !cells[x][y].inMaze;
}

vector<vector<int>> Level::shuffle(vector<vector<int>> n) {
    return n;
}

Location Level::makeConnections(Location location) {
    vector<vector<int>> neighbors = shuffle(NEIGHBORS);
    int x = location.x;
    int y = location.y;
    for (int i = 0; i < neighbors.size() i++) {

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

void maze(Level level, Location start) {
    stack<Location> locations;
    locations.push(start);
    level.startAt(start);

    while (!locations.empty()) {
        Location current = locations.top();
        Location next = level.makeConnections(current);
        ((next.x == -1) and (next.y = -1)) ? locations.push(next) : locations.pop();
    }
}