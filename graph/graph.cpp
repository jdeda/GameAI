#include <vector>
#include <unordered_map>
#include <stack>
#include <iostream>
#include "../id/id.h"
#include "../level/location.h"
#include "graph.h"

using namespace std;

namespace graph {

    Vertex::Vertex() {
        id = ID();
    }

    int Vertex::getID() const { return id.getID(); }

    Edge::Edge(float w, float c, Vertex f, Vertex t) {
        weight = w;
        cost = c;
        from = f;
        to = t;
    }

    float Edge::getCost() const {
        return cost;
    }
    graph::Vertex Edge::getFromVertex() const {
        return from;
    }

    graph::Vertex Edge::getToVertex() const {
        return to;
    }

    GraphNode::GraphNode(const Location& location, const Vertex& vertex, const vector<Edge>& edges) :
        location(location.x, location.y) {
        GraphNode::location = location;
        GraphNode::vertex = vertex;
        GraphNode::edges = edges;
    }

    Vertex GraphNode::getVertex() const { return vertex; }

    Location GraphNode::getLocation() const { return location; }

    vector<Edge> GraphNode::getEdges() const { return edges; }

    Graph::Graph(const unordered_map<int, GraphNode>& nodes) {
        Graph::nodes = nodes;
        unordered_map<Location, GraphNode> localizer;

        // TODO: Id here...may be bad.
        for (const auto& kv : nodes) { localizer.insert({ kv.second.getLocation(), kv.second }); }
        Graph::localizer = localizer;
    }

    int Graph::getRows() { return rows; }
    int Graph::getCols() { return cols; }
    void Graph::print() {
        int r = rows - 1;
        int c = cols - 1;
        cout << r << endl;
        cout << c << endl;
        for(int i = 2; i < r; i++) {
            cout << "row " << i << endl;
            for(int j = 2; j < c; j++) {
                cout << "(" << i << ", " << j << "): " << quantize(Location(i, j)).getVertex().getID() << endl;
            }
            cout << endl << endl;
        }
    }
};