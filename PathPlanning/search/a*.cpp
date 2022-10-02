#include "../graph/graph.h"
#include "search.h"
#include "a*.h"

#include <iostream>
using namespace std;

AStar::AStar(const Graph& graph, const Location& start, const Location& end, const Heuristic& heuristic) : Search(graph, start, end), heuristic(&heuristic) {}

Path AStar::search() const {

    // Setup open and closed list.
    GraphNodeRecord start(quantize(getStart()), GraphNodeRecordState::visited);
    start.setEstimatedTotalCost(heuristic->compute(getStart()));
    GraphNodeRecord end(quantize(getEnd()), GraphNodeRecordState::unvisited);
    Path openList;
    openList.add(start);
    Path closedList;

    // Run through open list till empty.
    while (!openList.isEmpty()) {

        // Find smallest element in open list.
        GraphNodeRecord current = openList.getSmallestEST();

        // Found the end.
        if (current.getLocation() == getEnd()) {
            vector<GraphNodeRecord> pathList;
            while (!(current.getLocation() == getStart())) {
                pathList.push_back(current);
                current = closedList.find(current.getEdge().getFromVertex()); // Closed list is essentially the path...
            }

            // Return reversed path list as a Path.
            Path path;
            path.add(start);
            for (int i = pathList.size() - 1; i > 0; i--) { path.add(pathList[i]); }
            path.add(end);
            path.exploredNodes = closedList.size();
            return path;
        }

        // Otherwise get its outgoing edges.
        vector<Edge> edges = getGraph().getOutgoingEdges(current.getNode().getVertex());

        // Find best edge to traverse.
        for (const auto& edge : edges) {

            // Get end node and it's cost so far (CSF).
            GraphNode endNode = getGraph().getNode(edge.getToVertex());
            float endNodeCSF = current.getCostSoFar() + edge.getCost(); // Get CSF or EST? i think CSF.
            float endNodeHeuristic = heuristic->compute(endNode.getLocation()); // TODO: Bad?

            // Skip nodes on closed list or remove it.
            if (closedList.contains(endNode)) {
                GraphNodeRecord endNodeRecord = closedList.find(endNode);

                // If shorter route not found, skip.
                if (endNodeRecord.getCostSoFar() <= endNodeCSF) { continue; }

                // If shorter route found, remove from closedList.
                closedList.remove(endNodeRecord);

                // Use compute heurisitic for end node (via old values).
                float endNodeHeuristic = endNodeRecord.getEstimatedTotalCost() - endNodeRecord.getCostSoFar();
            }

            // On open means worse route.
            else if (openList.contains(endNode)) {

                // If current route yields worse CSF, skip this edge.
                GraphNodeRecord endNodeRecord = openList.find(endNode);
                if (endNodeRecord.getCostSoFar() <= endNodeCSF) { continue; }
                float endNodeHeuristic = endNodeRecord.getEstimatedTotalCost() - endNodeRecord.getCostSoFar();
            }

            // Otherwise know node is unvisited, thus make new record.
            else {
                GraphNodeRecord endNodeRecord(endNode, GraphNodeRecordState::visited);
                endNodeRecord.setCostSoFar(endNodeCSF);
                endNodeRecord.setEdge(edge);
                endNodeRecord.setEstimatedTotalCost(endNodeCSF + endNodeHeuristic);
                if (!openList.contains(endNode)) { openList.add(endNodeRecord); }
            }
        }

        // Finished viewing edges for current.
        openList.remove(current);
        closedList.add(current);
    }
    return Path();
}