#ifndef DIJSKTRA_H
#define DIJSKTRA_H

#include "dijsktra.h"
#include "../graph/graph.h"
#include "search.h"
#include "dijsktra.h"

Path Dijkstra::search() {

    // Initialize lists.
    Path path;
    Path openList;
    Path closedList;


    GraphNodeRecord start(Search::quantize(Search::getStart()), GraphNodeRecordState::visited);
    path.add(start);
    return path;

}

#endif