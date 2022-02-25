
#include <vector>
#include <unordered_map>
#include "../id/id.h"

using namespace std;

/** Represents a general graph vertex. */
class Vertex
{

private:
    /** Unique ID for vertex. */
    ID id;

public:
    /** Default constructor for vertex. */
    Vertex();

    /** Returns the vertex's ID (as an int). */
    int getID() const;
};

/** Represents a directed weighted graph edge. */
class Edge
{

private:
    /** Non-negative weight of this edge. */
    float weight;

    /** Non-negative cost of traversing this edge. */
    float cost;

    /** Vertex edge starts. */
    Vertex from;

    /** Vertex edge leads to (ends). */
    Vertex to;

public:
    /**
     * @brief Construct an Edge initializing all its fields.
     *
     * @param w the weight
     * @param c the cost
     * @param f the from Vertex
     * @param t the to Vertex
     */
    Edge(float w, float c, Vertex f, Vertex t);

    /** Returns non-negative cost of traversing edge. */
    float getCost() const;
};

/** Represents a directed weighted graph. */
class Graph
{

    /** Index into map given a vertex's unique id, and return the list of its outgoing edges. */
    unordered_map<int, vector<Edge>> lookupTable;

    /** Graph is represented by a list of edgess. */
    vector<Edge> edges;

    /** Constructs a new graph initializing all its fields given the list of edges.*/
    Graph(const vector<Edge> &e);

    /**
     * @brief Returns the list of outgoing edges from the given vertex.
     *
     * @param from the vertex to get outgoing edges from the given vertex
     * @return vector<Edge> the outgoing edges from the given vetex
     */
    inline vector<Edge> getOutgoingEdges(const Vertex &of)
    {
        return lookupTable.at(of.getID());
    }
};

/** Any vertex in search is either open, closed, unvisited, or visited. */
enum VertexState
{
    open,
    closed,
    unvisited,
    visited
};

/** Used for path finding algorithms. Stores a vertex and its relative state for pathfinding purposes. */
class VertexRecord
{

private:
    /** Vertex to record.*/
    Vertex vertex;

    /** Vertex state. */
    VertexState state;

public:
    /* Construct a Vertex Record with all its fields. */
    VertexRecord(const Vertex &v, const VertexState &s);

    /** Returns state of vertex.*/
    VertexState getState() const;

    /** Sets state of vertex. */
    void setState(VertexState newState);
};
