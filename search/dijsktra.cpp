#include "../graph/graph.h"
#include "search.h"
#include "dijsktra.h"

#include <iostream>
using namespace std;

Dijkstra::Dijkstra(const Graph& graph, const Location& start, const Location& end) : Search(graph, start, end) {
}

Path Dijkstra::search() const {

    // Setup open and closed list.
    GraphNodeRecord start(Search::quantize(Search::getStart()), GraphNodeRecordState::visited);
    GraphNodeRecord end(Search::quantize(Search::getEnd()), GraphNodeRecordState::unvisited);
    Path openList;
    openList.add(start);
    Path closedList;

    // Run through open list till empty.
    GraphNodeRecord current = openList.getSmallestCSF();
    while (!openList.isEmpty()) {

        // Find smallest element in open list.
        GraphNodeRecord current = openList.getSmallestCSF();

        // Found the end.
        if (current.getLocation() == Search::getEnd()) {
            vector<GraphNodeRecord> pathList;
            while (!(current.getLocation() == Search::getStart())) {
                pathList.push_back(current);
                current = closedList.find(current.getEdge().getFromVertex()); // Closed list is essentially the path...
            }
            // cout << "PATHLIST SIZE: " << pathList.size() << endl;
            // for (const auto& record : pathList) { cout << record.getLocation().x << " " << record.getLocation().y << endl; }

            // Return reversed path list as a Path.
            // TODO: Why do I have to add start and end records?
            Path path;
            path.add(start);
            for (int i = pathList.size() - 1; i > 0; i--) { path.add(pathList[i]); }
            path.add(end);
            path.exploredNodes = closedList.size();
            return path;
        }

        // Otherwise get its outgoing edges. // TODO: could fail.
        vector<Edge> edges = Search::getGraph().getOutgoingEdges(current.getNode().getVertex());

        // cout << "\n\n" << current.getLocation().x << " " <<  current.getLocation().y << endl;
        // Find best edge to traverse.
        for (const auto& edge : edges) {

            auto l = Search::localize(edge.getToVertex());
            // cout << l.x << " " << l.y << endl;

            // Get end node and it's cost so far (CSF).
            GraphNode endNode = Search::getGraph().getNode(edge.getToVertex());
            float endNodeCSF = current.getCostSoFar() + edge.getCost();

            // Skip nodes on closed list.
            if (closedList.contains(endNode)) {
                continue;
            }

            // On open means worse route.
            else if (openList.contains(endNode)) {

                // If current route yields worse CSF, skip this edge.
                GraphNodeRecord endNodeRecord = openList.find(endNode);
                if (endNodeRecord.getCostSoFar() <= endNodeCSF) {
                    continue;
                }
            }

            // Otherwise know node is unvisited, thus make new record.
            else {
                GraphNodeRecord endNodeRecord(endNode, GraphNodeRecordState::visited);
                endNodeRecord.setCostSoFar(endNodeCSF);
                endNodeRecord.setEdge(edge);
                if (!openList.contains(endNode)) { openList.add(endNodeRecord); }
            }
        }

        // Finished viewing edges for current.
        openList.remove(current);
        closedList.add(current);
    }
    return Path();
}
