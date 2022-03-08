#ifndef SEARCH_H
#define SEARCH_H

#include "../graph/graph.h"
#include "../level/level.h"    

/** Any GraphNode in search is either open, closed, unvisited, or visited. */
enum GraphNodeRecordState
{
    open,
    closed,
    unvisited,
    visited
};

/** Used for path finding algorithms. Stores a GraphNode and its relative state for pathfinding purposes. */
class GraphNodeRecord
{

    private:
    /** GraphNode to record.*/
    GraphNode node;

    /** GraphNode state. */
    GraphNodeRecordState state;

    public:
    /* Construct a GraphNodeRecord with all its fields. */
    GraphNodeRecord(const GraphNode& n, const GraphNodeRecordState& s);

    /** Returns state of vertex.*/
    GraphNodeRecordState getState() const;

    /** Sets state of vertex. */
    void setState(GraphNodeRecordState newState);
};

/** Represents the list of GraphRecords that complete a path. */
class Path
{
    private:


    /** The list of GraphNodeRecords in the path (order matters). */
    vector<GraphNodeRecord> path;

    public:

    bool contains();

    void add(const GraphNodeRecords& record);

};

/** Abstract class representing Search algorithms. */
class Search
{

    private:

    /** The graph to search. */
    const graph::Graph graph;

    /** The start location in the graph. */
    const Location start;

    /** The end location in the graph. */
    const Location end;

    public:

    /** Default constructor for Search class. */
    Search(const Graph& graph, const Location& start, const Location& end);

    /**
     * @brief Returns a path from the start to end location
     *
     * @param graph the graph to perform search on
     * @param start the start location to search
     * @param end the end location of the search
     * @return Path the path from the start to end location
     */
    virtual Path search() const = 0;

    /** Maps vertex in graph to location in level. */
    inline Location localize(const graph::Vertex& vertex) const {
        return graph.localize(vertex);
    }

    /** Maps location in level to GraphNode in graph. */
    inline GraphNode quantize(const Location& location) const {
        return graph.quantize(location);
    }

    /** Getters. */
    Graph getGraph();
    Location getStart();
    Location getEnd();
};

#endif