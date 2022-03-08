#include <vector>
#include <unordered_map>
#include <stack>
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
};