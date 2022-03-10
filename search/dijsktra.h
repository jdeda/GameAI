#ifndef DIJSKTRA_H
#define DIJSKTRA_H

#include "search.h"

/** Represents Dijkstra's search algorithm. */
class Dijkstra: Search {

    /** Returns path from start location to end location in graph. */
    Path search();


};

#endif