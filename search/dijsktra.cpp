#include "dijsktra.h"
#include "../graph/graph.h"
#include "search.h"
#include "dijsktra.h"

#include <iostream>
using namespace std;

Dijkstra::Dijkstra(const Graph& graph, const Location& start, const Location& end) : Search(graph, start, end) {
}

Path Dijkstra::search() {

    // Setup open and closed list.
    GraphNodeRecord start(Search::quantize(Search::getStart()), GraphNodeRecordState::visited);
    Path openList;
    openList.add(start);
    Path closedList;


    // Run through open list till empty.
    GraphNodeRecord current = openList.getSmallestCSF();
    while(!openList.isEmpty()) {

        // Find smallest element in open list.
        GraphNodeRecord current = openList.getSmallestCSF();

        // If current is stored at goal (location), terminate.
        if(current.getLocation() == Search::getEnd()) { break; }

        // Otherwise get its outgoing edges. // TODO: could fail.
        vector<Edge> edges = Search::getGraph().getOutgoingEdges(current.getNode().getVertex());

        // Find best edge to traverse.
        for(const auto& edge: edges) {

            // Get end node and it's cost so far (CSF).
            cout << current.getNode().getVertex().getID() << endl;
            // These to and from nodes are not in the graph... So...
            // When creating graph, if ID doesn't exist, create new, else and linked, set ID?
            cout  << edge.getToVertex().getID() << endl;
            GraphNode endNode = Search::getGraph().getNode(edge.getToVertex()); // TODO: could fail. <-- This is failing.
            float endNodeCSF = current.getCostSoFar() + edge.getCost();

            // Skip nodes on closed list.
            if(closedList.contains(endNode)) { 
                continue;
            }
            
            // On open means worse route.
            else if(openList.contains(endNode)) {

                // If current route yields worse CSF, skip this edge.
                GraphNodeRecord endNodeRecord = openList.find(endNode);
                if(endNodeRecord.getCostSoFar() <= endNodeCSF) {
                    continue;
                }
            }

            // Otherwise know node is unvisited, thus make new record.
            else {
                GraphNodeRecord endNodeRecord(endNode, GraphNodeRecordState::visited);
                endNodeRecord.setCostSoFar(endNodeCSF);
                endNodeRecord.setEdge(edge);
                if(!openList.contains(endNode)) { openList.add(endNodeRecord); }
            }
        }

        // Finished viewing edges for current.
        openList.remove(current);
        closedList.add(current);
    }

    // Goal not found.
    if(current.getLocation() == Search::getEnd()) { return Path(); }

    // Goal found.
    vector<GraphNodeRecord> pathList;
    while(!(current.getLocation() == Search::getStart())) {
        pathList.push_back(current);
        current = closedList.find(current.getEdge().getFromVertex()); // Closed list is essentially the path...
        // Can't store records in record class, not possible.
    }

    // Return reversed path lsit as a Path.
    Path path;
    for(int i = pathList.size(); i > 0; i++) { path.add(pathList[i]); }
    return path;
}
