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
    visited,
    failed
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
    void setEstimatedTotalCost(float est);
    void setEdge(Edge newEdge);

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

    int exploredNodes = 0; // TODO: Make private

    /** Returns size of path. */
    int size() const;

    /** Returns true if path is empty. */
    bool isEmpty() const;

    /** Returns the path list. */
    vector<GraphNodeRecord> getPathList() const;

    /** Returns GraphNodeRecord with smallest CSF.*/
    GraphNodeRecord getSmallestCSF() const;

    /** Returns GraphNodeRecord with smallest EST.*/
    GraphNodeRecord getSmallestEST() const;

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

    /** Returns the index of the future position with respect to the current index on the path. */
    int getIndex(const Vector2f& futurePosition, int currentIndexOnPath) const;

    /** Returns the index of the future position with respect to the current index on the path. */
    Vector2f getPosition(int index) const;

    Location getLast() const;

    inline vector<LevelCell> toSFML() const {
        vector<LevelCell> sfml;
        if (path.size() == 0) { return sfml; }
        int end = path.size() - 1;

        Location start(path[0].getLocation().y, path[0].getLocation().x);
        sfml.push_back(LevelCell(start, 1));
        for (int i = 1; i < end; i++) {
            sfml.push_back(LevelCell(Location(path[i].getLocation().y, path[i].getLocation().x), 2));
        }
        Location endd(path[end].getLocation().y, path[end].getLocation().x);
        sfml.push_back(LevelCell(endd, 3));
        return sfml;
    }


    /** Prints path. */
    void print() const;

    /** Draws the path on the window. */
    inline void draw(RenderWindow* window) const {
        if (path.size() == 0) { return; }

        // Draw start.
        Location startLocation = Location(path[0].getLocation().y, path[0].getLocation().x);
        LevelCell startCell(startLocation, 1);
        startCell.draw(window);

        // Draw inbetween.
        for (int i = 1; i < path.size() - 1; i++) {
            Location location(path[i].getLocation().y, path[i].getLocation().x);
            LevelCell cell(location, 2);
            cell.draw(window);
        }

        // Draw end.
        Location endLocation = Location(path[path.size() - 1].getLocation().y, path[path.size() - 1].getLocation().x);
        LevelCell endCell(endLocation, 3);
        endCell.draw(window);
    }
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

/**
 * Heuristic is function that returns a value representing a cost
 * from one location to a set goal location (all of which is in a graph).
*/
class Heuristic
{

    private:
    Location goalLocation;

    public:

    /** Initializes class with the */
    Heuristic(const Location& goal);

    /** Returns heuristic value of location in the graph. */
    virtual float compute(const Location& location) const = 0;

    /** Getters. */
    Location getGoalLocation() const;
};

/** Manhattan distance heuristic. */
class ManhattanHeuristic : public Heuristic
{

    public:

    /** Default constructor. */
    ManhattanHeuristic(const Location& goal);

    /** Returns heuristic value of location to goal location. */
    float compute(const Location& location) const;
};

/** Euclidean distance heuristic. */
class EuclideanHeuristic : public Heuristic
{
    public:

    /** Default constructor. */
    EuclideanHeuristic(const Location& goal);

    /** Returns heuristic value of location to goal location. */
    float compute(const Location& location) const;
};


/** Weighted distance heuristic. */
class CustomHeuristic : public Heuristic
{
    public:

    /** Default constructor. */
    CustomHeuristic(const Location& goal);

    /** Returns heuristic value of location to goal location. */
    float compute(const Location& location) const;
};
#endif