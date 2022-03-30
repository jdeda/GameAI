#ifndef A_STAR
#define A_STAR

#include "search.h"

class AStar : public Search
{
    private:
    const Heuristic* heuristic;

    public:

    /** Default constructor. */
    AStar(const Graph& graph, const Location& start, const Location& end, const Heuristic& heuristic);

    /** Returns path from start location to end location in graph. */
    Path search() const;
};

#endif