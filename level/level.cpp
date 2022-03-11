#include <vector>
#include <iostream>
#include <algorithm>
#include <stack>
#include "../debug/debug.h"
#include "../graph/graph.h"
#include "location.h"
#include "level.h"

using namespace std;
using namespace sf;
using namespace graph;

LevelCell::LevelCell(const Location& location, bool inLevel) {
    setPosition((location.x * LevelCell::dims.x) / 1.f, (location.y * LevelCell::dims.y) / 1.f);
    setFillColor(inLevel ? sf::Color::Yellow : sf::Color::Black);
    setSize(LevelCell::dims);
}


Level::Level(const Level& level) {
    rows = level.rows;
    cols = level.cols;
    cells = level.cells;
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
    cells[location.x][location.y].inLevel = true;
}

int Level::getDirIdx(Location o, int x, int y) const {
    int dx = x - o.x;
    int dy = y - o.y;

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

bool Level::inBounds(int x, int y) const {
    return (x >= 0 && x < rows) && (y >= 0 && y < cols);
}

bool Level::canPlaceCorridor(int x, int y) const {
    return inBounds(x, y) && (cells[x][y].inLevel == false);
}

bool Level::canPlaceCorridorDeep(Location o, int x, int y, int fromDirIdx) const {

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
            cells[nx][ny].inLevel = true;
            cells[nx][ny].directions[fromDirIdx] = true;
            return Location(nx, ny);
        }

    }
    return Location(-1, -1); // None of the neighbors were vlaid.
}

Level generateMaze(int w, int h) {

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
        ((next.x != -1) && (next.y != -1)) ? locations.push(next) : locations.pop();
    }

    // Return level that is now a maze.
    return level;
}

void Level::print() {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            cout << cells[r][c].inLevel << " ";
        }
        cout << "\n";
    }
}

vector<vector<LevelCell>> Level::toSFML() {
    vector<vector<LevelCell>> cellsSFML;
    for (int row = 0; row < rows; row++) {
        vector<LevelCell> v;
        for (int col = 0; col < cols; col++) {
            v.push_back(LevelCell(Location(row, col), cells[row][col].inLevel));
        }
        cellsSFML.push_back(v);
    }
    return cellsSFML;
}

Graph levelToGraph(const Level& level) {

    unordered_map<int, GraphNode> nodes;
    for (int i = 0; i < level.rows; i++) {
        for (int j = 0; j < level.cols; j++) {

            // Make vertex and location for this point in the level.
            graph::Vertex vertex;
            Location location(i, j);

            // Makes edges for this vertex for the graph (take directions marked as true).
            vector<Edge> edges;
            Connections connections = level.cells[i][j];

            // Connection not in level has no edges.
            // What might happen here is vertex is passed, copied, and the IDs are actually now mismatched...
            if (!connections.inLevel) {
                // Does this graph node get a copy of the vertex...or does it construct one and increment id?
                GraphNode node = GraphNode(location, vertex, edges);
                nodes.insert({ node.getVertex().getID(), node });
                continue;
            }

            // Connection in level has edges (for those that are marked as true).
            bool directions[4] = { connections.directions };
            for (int k = 0; k < 4; k++) {  // This is dangerous, assume 4 directions.
                if (!directions[k]) { continue; } // Not marked true, continue.
                int nx = i + level.NEIGHBORS[k][0]; // i + dx
                int ny = j + level.NEIGHBORS[k][0]; // j + dy

                // TODO: ID property may not work as expected.
                if (!level.inBounds(nx, ny)) { continue; }
                graph::Vertex vertexNeighbor;
                Edge e = Edge(1.0, 1.0, vertex, vertexNeighbor); // weight=cost=1.0
                edges.push_back(e);
            }
            GraphNode node = GraphNode(location, vertex, edges);
            nodes.insert({ node.getVertex().getID(), node });

        }
    }
    return Graph(nodes);
}