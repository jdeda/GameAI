#include "../graph/graph.h"
#include "../level/level.h"
#include "search.h"

// TODO: Does this constructor cause index problems and what does it really do?
GraphNodeRecord::GraphNodeRecord(const graph::GraphNode& n, const GraphNodeRecordState& s) : node(n), state(s) {}

GraphNodeRecordState GraphNodeRecord::getState() const { return state; }

void GraphNodeRecord::setState(GraphNodeRecordState newState) {
    state = newState;
    costSoFar = 0;
    estimatedTotalCost = 0;
}

Search::Search(const Graph& _graph, const Location& _start, const Location& _end) :
graph(_graph), start(_start), end(_end) { }

Graph Search::getGraph() { return graph; }
Location Search::getStart() { return start; }
Location Search::getEnd() { return end; }