#include <vector>
#include <iostream>
#include <cmath>
#include "../graph/graph.h"
#include "../level/level.h"
#include "search.h"

using namespace std;

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

vector<GraphNodeRecord> Path::getPathList() const { return path; }

GraphNodeRecord Path::getSmallestCSF() const {
    GraphNodeRecord smallest = path[0];
    for (const auto& record : path) {
        if (record.getCostSoFar() < smallest.getCostSoFar()) {
            smallest = record;
        }
    }
    return smallest;
}

GraphNodeRecord Path::getSmallestEST() const {
    GraphNodeRecord smallest = path[0];
    for (const auto& record : path) {
        if (record.getEstimatedTotalCost() < smallest.getEstimatedTotalCost()) {
            smallest = record;
        }
    }
    return smallest;
}

bool Path::contains(const GraphNode& node) const {
    for (const auto& in : path) {
        if (in.getLocation() == node.getLocation()) { return true; }
    }
    return false;
}

GraphNodeRecord Path::find(const GraphNode& node) const {
    for (const auto& in : path) {
        if (in.getLocation() == node.getLocation()) { return in; }
    }
    return GraphNodeRecord(node, GraphNodeRecordState::failed); // TODO: fail is a band-aid fix.
}

GraphNodeRecord Path::find(const graph::Vertex& vertex) const {
    for (const auto& in : path) {
        if (in.getNode().getVertex().getID() == vertex.getID()) { return in; }
    }
    return GraphNodeRecord(path[0].getNode(), GraphNodeRecordState::failed); // TODO: total band-aid.
}

void Path::add(const GraphNodeRecord& record) {
    path.push_back(record);
}

void Path::remove(const GraphNodeRecord& record) {
    for (auto it = begin(path); it != end(path); ++it) {
        if (it->getLocation() == record.getLocation()) {
            path.erase(it);
            break;
        }
    }
}

int Path::getIndex(const Vector2f& futurePosition, int currentIndexOnPath) const {
    Location futureLocation = mapToLevel(22, 29.0909, futurePosition); // TODO: Hack
    for (int i = 0; i < path.size(); i++) {
        if (path[i].getLocation() == futureLocation) {
            return i;
        }
    }
    // cout << "OH NO" << endl;
    // You could just return the next index to be safe?
    return currentIndexOnPath + 1; // Cause segfault?
}

Vector2f Path::getPosition(int index) const {
    auto temp = mapToWindow(29.0909, path[index].getLocation());
    return Vector2f(temp.y, temp.x);
}

Location Path::getLast() const {
    return path.back().getLocation();
}

void Path::print() const {
    cout << "Path:" << endl;
    for (const auto& record : path) {
        Location l = record.getLocation();
        cout << l.x << " " << l.y << endl;
    }

}

Search::Search(const Graph& _graph, const Location& _start, const Location& _end) :
    graph(_graph), start(_start), end(_end) {
}

Graph Search::getGraph() const { return graph; }
Location Search::getStart() const { return start; }
Location Search::getEnd() const { return end; }

Location Heuristic::getGoalLocation() const { return goalLocation; }

Heuristic::Heuristic(const Location& goal) : goalLocation(goal) {}

ManhattanHeuristic::ManhattanHeuristic(const Location& goal) : Heuristic(goal) {}

float ManhattanHeuristic::compute(const Location& location) const {
    return abs(getGoalLocation().x - location.x) + abs(getGoalLocation().y - location.y);
}

EuclideanHeuristic::EuclideanHeuristic(const Location& goal) : Heuristic(goal) {}

float EuclideanHeuristic::compute(const Location& location) const {
    return sqrt(pow(getGoalLocation().x - location.x, 2) + pow(getGoalLocation().y - location.y, 2));
}

CustomHeuristic::CustomHeuristic(const Location& goal) : Heuristic(goal) {}

float CustomHeuristic::compute(const Location& location) const {
    return 10.f * sqrt(pow(getGoalLocation().x - location.x, 2) + pow(getGoalLocation().y - location.y, 2));
}