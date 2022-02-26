#include <vector>
#include <iostream>
#include <algorithm>
#include <stack>
#include "maze.h"
#include "../debug/debug.h"

using namespace std;


Location::Location(int a, int b) {
    x = a;
    y = b;
}

// LevelCell::LevelCell(const Location& location, const Connections& connections) {
//     setPosition(location.x, location.y);
    
// }

Level::Level(int w, int h) {
    cols = w;
    rows = h;
    for(int r = 0; r < rows; r++) {
        vector<Connections> v;
        for(int c = 0; c < cols; c++) { v.push_back(Connections()); }
        cells.push_back(v);
    }
}

void Level::startAt(Location location) {
    cells[location.x][location.y].inMaze = true;
}

// TODO: This is very odd...
bool Level::canPlaceCorridor(int x, int y, int dirn) {
    cout << "Oh fuck" << endl;
    cout << x << " " << y << endl;
    return (x >= 0 && x < rows) && (y >= 0 && y < cols) && (!cells[x][y].inMaze);
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

Level generateMaze(int w, int h) {

    // Initialize level and start.
    Level level = Level(w,h);
    Location start = Location(1,1);

    // Prepare DFS.
    stack<Location> locations;
    locations.push(start);
    level.startAt(start);

    // Perform DFS.
    while (!locations.empty()) {
        Location current = locations.top();
        Location next = level.makeConnections(current);
        ((next.x != -1) && (next.y != -1)) ? locations.push(next): locations.pop();
        level.print();
        cout << endl;
    }

    // Return level that is now a maze.
    return level;
}

void Level::print() {
    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) { 
            int i = cells[r][c].inMaze ? 1 : 0;
            cout << i << " ";
        }
        cout << "\n";
    }
}

// vector<vector<LevelCell>> Level::toSFML() {

//     vector<vector<LevelCell>> cellsSFML;
//     for(int row = 0; row < rows; row++) {
//         vector<LevelCell> v;
//         for(int col = 0; col < cols; col++) { 
//             v.push_back(LevelCell(Location(row, col), cells[row][col]));
//         }
//         cellsSFML.push_back(v);
//     }
//     return cellsSFML;
// }
