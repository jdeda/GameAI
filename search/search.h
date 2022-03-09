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

    /** Accumulated cost of the node (CSF). */
    float costSoFar;

    /** Accumulated cost of the node plus heuristic of node (ETC = CSF + H). */
    float estimatedTotalCost;

    /** Connection between this record to another. */
    Edge edge;

    public:
    /* Construct a GraphNodeRecord with all its fields. */
    GraphNodeRecord(const GraphNode& n, const GraphNodeRecordState& s);

    /** Setters. */
    void setState(GraphNodeRecordState newState);
    void setCostSoFar(float csf);
    void setEstimatedTotalCost();
    void setEdge(Edge edge); // TODO: set from and to records!

    /** Getters. */
    GraphNode getNode() const;
    GraphNodeRecordState getState() const;
    float getCostSoFar() const;
    float getEstimatedTotalCost() const;
    Location getLocation() const;
    Edge getEdge() const;
};

/** Represents the list of GraphRecords that complete a path. */
class Path
{
    private:
    /** The list of GraphNodeRecords in the path (order matters). */
    vector<GraphNodeRecord> path;

    public:

    /** Returns size of path. */
    int size() const;

    /** Returns true if path is empty. */
    bool isEmpty() const;

    /** Returns GraphNodeRecord with smallest CSF.*/
    GraphNodeRecord getSmallestCSF() const;

    /** Return true if node is in Path. */
    bool contains(const GraphNode& node) const;

    /** Return GraphNodeRecord in Path of corresponding GraphNode. */
    GraphNodeRecord find(const GraphNode& node) const;

    /** Return GraphNodeRecord in Path of corresponding Vertex. */
    GraphNodeRecord find(const graph::Vertex& vertex) const;

    /** Adds GraphNodeRecord to Path. */
    void add(const GraphNodeRecord& record);
    
    /** Removes GraphNodeRecord to Path. */
    void remove(const GraphNodeRecord& record);
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
    Graph getGraph() const;
    Location getStart() const;
    Location getEnd() const;
};

#endif