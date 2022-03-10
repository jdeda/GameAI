#include <vector>
#include "../graph/graph.h"
#include "../level/level.h"
#include "search.h"

// TODO: Does this constructor cause index problems and what does it really do?
// TODO: Edge constructor is a band-aid fix.
GraphNodeRecord::GraphNodeRecord(const graph::GraphNode& n, const GraphNodeRecordState& s) :
    node(n), state(s), edge(1.0, 1.0, n.getVertex(), n.getVertex()) {
        costSoFar = 0;
        estimatedTotalCost = 0;
}

void GraphNodeRecord::setState(GraphNodeRecordState newState) { state = newState; }
void GraphNodeRecord::setCostSoFar(float csf) { costSoFar = csf; }
void GraphNodeRecord::setEstimatedTotalCost(float est) { estimatedTotalCost = est; }
void GraphNodeRecord::setEdge(Edge newEdge) { edge = newEdge; }

GraphNode GraphNodeRecord::getNode() const { return node; }
GraphNodeRecordState GraphNodeRecord::getState() const { return state; }
float GraphNodeRecord::getCostSoFar() const { return costSoFar; }
float GraphNodeRecord::getEstimatedTotalCost() const { return estimatedTotalCost; }
Location GraphNodeRecord::getLocation() const { return node.getLocation(); }
Edge GraphNodeRecord::getEdge() const { return edge; }

int Path::size() const { return path.size(); }

bool Path::isEmpty() const { return path.size() == 0; }

vector<GraphNodeRecord> Path::getPathList() { return path; }

GraphNodeRecord Path::getSmallestCSF() const {
    GraphNodeRecord smallest = path[0];
    for(const auto& record : path) {
        if(record.getCostSoFar() < smallest.getCostSoFar()) { 
            smallest = record;
        }
    }
    return smallest;
}

bool Path::contains(const GraphNode& node) const {
    for(const auto& in : path) {
        if(in.getLocation() == node.getLocation()) { return true; }
    }
    return false;
}

GraphNodeRecord Path::find(const GraphNode& node) const {
    for(const auto& in : path) {
        if(in.getLocation() == node.getLocation()) { return in; }
    }
    return GraphNodeRecord(node, GraphNodeRecordState::fail); // TODO: fail is a band-aid fix.
}

GraphNodeRecord Path::find(const graph::Vertex& vertex) const {
    for(const auto& in : path) {
        if(in.getNode().getVertex().getID() == vertex.getID()) { return in; }
    }
    return GraphNodeRecord(path[0].getNode(), GraphNodeRecordState::fail); // TODO: total band-aid.
}

void Path::add(const GraphNodeRecord& record) {
    path.push_back(record);
}

void Path::remove(const GraphNodeRecord& record) {
    for (auto it = begin (path); it != end (path); ++it) {
        if (it->getLocation() == record.getLocation()) {
            path.erase(it);
            break;
        }
    }
}

Search::Search(const Graph& _graph, const Location& _start, const Location& _end) :
    graph(_graph), start(_start), end(_end) {
}

Graph Search::getGraph() const { return graph; }
Location Search::getStart() const { return start; }
Location Search::getEnd() const { return end; }