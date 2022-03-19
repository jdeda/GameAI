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

    void Vertex::setID(int id) { Vertex::id.setID(id); }

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

    void GraphNode::setEdges(const vector<Edge>& edges) { GraphNode::edges = edges; }

    void GraphNode::appendEdges(const vector<Edge>& edges) {
        for (const auto& edge : edges) {
            GraphNode::edges.push_back(edge);
        }
    }


    Graph::Graph(int rows, int cols, const unordered_map<int, GraphNode>& nodes) {
        Graph::rows = rows;
        Graph::cols = cols;
        Graph::nodes = nodes;
        unordered_map<Location, GraphNode> localizer;

        // TODO: Id here...may be bad.
        for (const auto& kv : nodes) { localizer.insert({ kv.second.getLocation(), kv.second }); }
        Graph::localizer = localizer;
    }

    Graph::Graph(const Graph& graph) {
        nodes = graph.nodes;
        localizer = graph.localizer;
        rows = graph.rows;
        cols = graph.cols;
    }


    int Graph::getRows() { return rows; }
    int Graph::getCols() { return cols; }
    void Graph::print() {
        cout << rows << endl;
        cout << cols << endl;
        for (int i = 0; i < rows; i++) {
            cout << "row " << i << endl;
            for (int j = 0; j < cols; j++) {
                cout << "(" << i << ", " << j << "): " << quantize(Location(i, j)).getVertex().getID() << endl;
            }
            cout << endl << endl;
        }
    }

    void Graph::printy() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cout << "===========================================" << endl;
                cout << "from " << "(" << i << ", " << j << ")" << endl << endl;
                GraphNode node = quantize(Location(i, j));
                for(const auto& edge: node.getEdges()) {
                    Location from = localize(edge.getFromVertex());
                    Location to = localize(edge.getToVertex());
                    cout << "\tto " << "(" << to.x << ", " << to.y << ")";
                    cout << "\tcost = " << edge.getCost() << endl << endl;
                }
                cout << "===========================================" << endl << endl;
            }
            cout << endl << endl;
        }
    }
};