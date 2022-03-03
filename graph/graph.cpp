#include <vector>
#include <unordered_map>
#include <stack>
#include "../id/id.h"
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
    

    Graph::Graph(const unordered_map<int, vector<Edge>>& e, const vector<vector<graph::Vertex>>& v) {
        edges = e;
        verticies = v;
    }

    VertexRecord::VertexRecord(const Vertex& v, const VertexState& s) {
        vertex = v;
        state = s;
    }

    VertexState VertexRecord::getState() const { return state; }

    void VertexRecord::setState(VertexState newState) {
        state = newState;
    }
};