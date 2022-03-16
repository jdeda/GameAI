#ifndef DIJSKTRA_H
#define DIJSKTRA_H

#include "search.h"

/** Represents Dijkstra's search algorithm. */
class Dijkstra: Search {

    public:
    /** Default constructor. */
    Dijkstra(const Graph& graph, const Location& start, const Location& end);

    /** Returns path from start location to end location in graph. */
    Path search() const;


};

#endif