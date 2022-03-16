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
    setFillColor(inLevel ? sf::Color::White : sf::Color::Black);
    setSize(LevelCell::dims);
}

LevelCell::LevelCell(const Location& location, int status) {
    setPosition((location.x * LevelCell::dims.x) / 1.f, (location.y * LevelCell::dims.y) / 1.f);
    // Start.
    if(status == 1) {
        setFillColor(sf::Color{ 255, 255, 0, 120 });
    }

    // Default.
    if(status == 2) {
        setFillColor(sf::Color{ 0, 0, 255, 120 });

    }

    // End.
    if(status == 3) {
                setFillColor(sf::Color{ 0, 255, 0, 120 });
    }
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

        // TRANSPOSE!: this is really: down, right, left, up

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

int Level::getRows() { return rows; }

int Level::getCols() { return cols; }

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
            std::cout << cells[r][c].inLevel << " ";
        }
        std::cout << "\n";
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
    unordered_map<Location, GraphNode> nodesByLocation;

    // Init nodes (without edges).
    for (int i = 0; i < level.rows; i++) {
        for (int j = 0; j < level.cols; j++) {
            Location location(i, j);
            graph::Vertex vertex;
            vector<Edge> edges;
            GraphNode node = GraphNode(location, vertex, edges);
            nodes.insert({ node.getVertex().getID(), node });
            nodesByLocation.insert({node.getLocation(), node });
        }
    }

    // Init nodes' edges.
    for (int i = 0; i < level.rows; i++) {
        for (int j = 0; j < level.cols; j++) {

            // Connection not in level has no edges.
            Connections connections = level.cells[i][j];
            if (!connections.inLevel) { continue; }

            // Get self and list of edges to append.
            GraphNode node = nodesByLocation.at(Location(i, j));
            vector<Edge> edges;

            // Connection in level has edges (for those that are marked as true).
            auto directions = connections.directions;
            for (int k = 0; k < 4; k++) {
                if (!directions[k]) { continue; }
                int nx = i + level.NEIGHBORS[k][0];
                int ny = j + level.NEIGHBORS[k][1];
                if (!level.inBounds(nx, ny)) { continue; }
                if (!level.cells[nx][ny].inLevel) { continue; }

                // Add edge to neighbor.
                GraphNode neighborNode = nodesByLocation.at(Location(nx, ny));
                edges.push_back(Edge(1.0, 1.0, node.getVertex(), neighborNode.getVertex()));
            }

            // Update node and its reference in the nodes.
            node.appendEdges(edges);
            auto iteratorNodes = nodes.find(node.getVertex().getID());
            iteratorNodes->second = node;
        }
    }

    // Return the graph.
    return Graph(level.rows, level.cols, nodes);
}
