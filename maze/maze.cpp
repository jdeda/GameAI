#include <vector>
#include <iostream>
#include <algorithm>
#include <stack>
#include "maze.h"
#include "../debug/debug.h"

using namespace std;
using namespace sf;

Location::Location(int a, int b) {
    x = a;
    y = b;
}

LevelCell::LevelCell(const Location& location, const Connections& connections) {
    // Font font;
    // if (!font.loadFromFile("arial.ttf")) { exit(99); }
    // auto text = Text{ id, font };
    setPosition((location.x * LevelCell::dims.x) / 2.f, (location.y * LevelCell::dims.y) / 2.f);
    setFillColor(connections.inMaze ? sf::Color::Yellow : sf::Color::Black);
    setSize(LevelCell::dims);
}

Level::Level(int w, int h) {
    cols = w;
    rows = h;
    for (int r = 0; r < rows; r++) {
        vector<Connections> v;
        for (int c = 0; c < cols; c++) {
            v.push_back(Connections());
        }
        cells.push_back(v);
    }
}

void Level::startAt(Location location) {
    cells[location.x][location.y].inMaze = true;
}

int Level::getDirIdx(Location o, int x, int y) {
    int dx = x - o.x;
    int dy = y - o.y;
    cout << dx << " " << dy << endl;

    // Right.
    if (dx == 1 && dy == 0) {
        return 0;
    }

    // Up.
    else if (dx == 0 && dy == 1) {
        return 1;
    }

    // Down.
    else if (dx == 0 && dy == -1) {
        return 2;
    }

    // Left.
    else if (dx == -1 && dy == 0) {
        return 3;
    }

    // Fail.
    else {
        exit(34);
    }
}


bool Level::canPlaceCorridor(int x, int y) {
    return (x > 0 && x < rows - 1) && (y > 0 && y < cols - 1) && (cells[x][y].inMaze == false);
}

bool Level::canPlaceCorridorDeep(Location o, int x, int y, int fromDirIdx) {

    // Make sure can move right, up, down, left.
    for (auto neighbor : NEIGHBORS) {
        int dx = neighbor[0];
        int dy = neighbor[1];
        int nx = x + dx;
        int ny = y + dy;

        // If not location to branch and has block neighbor in maze, fail.
        if ((nx != o.x || ny != o.y) && canPlaceCorridor(nx, ny) == false) {
            return false;
        }
    }

    // Make sure direction of expansion doesn't rub into a corner.
    int idx = getDirIdx(o, x, y);
    for (auto neighbor : BLOCK_NEIGHBORS[idx]) {
        int dx = neighbor[0];
        int dy = neighbor[1];
        int nx = x + dx;
        int ny = y + dy;
        // cout << idx << endl;

        // If not location to branch and has block neighbor in maze, fail.
        if ((nx != o.x || ny != o.y) && canPlaceCorridor(nx, ny) == false) {
            return false;
        }
    }

    // Passed all tests so it must be a valid corridor.
    return true;
}

Location Level::makeConnections(Location location) {
    vector<vector<int>> neighbors = NEIGHBORS;
    random_shuffle(neighbors.begin(), neighbors.end());
    int x = location.x;
    int y = location.y;
    for (auto neighbor : neighbors) {

        // Get neighbor.
        int dx = neighbor[0];
        int dy = neighbor[1];
        int dirIdx = neighbor[2];
        int nx = x + dx;
        int ny = y + dy;
        int fromDirIdx = 3 - dirIdx;

        // Make sure it is not a cell that already is in the level.
        if (canPlaceCorridor(nx, ny) && canPlaceCorridorDeep(location, nx, ny, dirIdx)) {
            cells[x][y].directions[dirIdx] = true;
            cells[nx][ny].inMaze = true;
            cells[nx][ny].directions[fromDirIdx] = true;
            return Location(nx, ny);
        }

    }
    return Location(-1, -1); // None of the neighbors were vlaid.
}

Level generateMaze(int w, int h, RenderWindow* window) {

    // Initialize level and start.
    Level level = Level(w, h);
    Location start = Location(2, 2);

    // Prepare DFS.
    stack<Location> locations;
    locations.push(start);
    level.startAt(start);

    // Perform DFS.
    while (!locations.empty()) {
        Location current = locations.top();
        Location next = level.makeConnections(current);
        window->clear(Color(255, 255, 255));
        level.draw(window);
        window->display();
        ((next.x != -1) && (next.y != -1)) ? locations.push(next) : locations.pop();
    }

    // Return level that is now a maze.
    return level;
}

void Level::print() {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            cout << cells[r][c].inMaze << " ";
        }
        cout << "\n";
    }
}

vector<vector<LevelCell>> Level::toSFML() {

    vector<vector<LevelCell>> cellsSFML;
    for (int row = 0; row < rows; row++) {
        vector<LevelCell> v;
        for (int col = 0; col < cols; col++) {
            v.push_back(LevelCell(Location(row, col), cells[row][col]));
        }
        cellsSFML.push_back(v);
    }
    return cellsSFML;
}