
#include <vector>
#include <unordered_map>
#include "../id/id.h"
#include "graph.h"

using namespace std;

Vertex::Vertex()
{
    id = ID(); // TODO: Make sure this works... Also this is really a side-effect.
}

int Vertex::getID() const { return id.getID(); }

Edge::Edge(float w, float c, Vertex f, Vertex t)
{
    weight = w;
    cost = c;
    from = f;
    to = t;
}

float Edge::getCost() const
{
    return cost;
}

// TODO: How do you initialize this?
Graph::Graph(const vector<Edge> &e)
{
    edges = e; // TODO: what about lookupTable?
}

VertexRecord::VertexRecord(const Vertex &v, const VertexState &s)
{
    vertex = v;
    state = s;
}

VertexState VertexRecord::getState() const { return state; }

void VertexRecord::setState(VertexState newState)
{
    state = newState;
}