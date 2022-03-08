#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <unordered_map>
#include "../id/id.h"
#include "../level/location.h"

using namespace std;

namespace graph {

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
        /** Constructs an Edge with all its fields. */
        Edge(float w, float c, Vertex f, Vertex t);

        /** Returns non-negative cost of traversing edge. */
        float getCost() const;
    };

    /** Represents node in graph (location, vertex, edges). */
    class GraphNode
    {

        private:

        /** Represents (x,y) coordinates in tile graph. */
        Location location;

        /** Represents vertex in graph. */
        Vertex vertex;

        /** Represents outgoing edges associated with vertex in graph. */
        vector<Edge> edges;

        public:

        /** Constructs a GraphNode with all of its fields. */
        GraphNode(const Location& location, const Vertex& vertex, const vector<Edge>& edges);

        /** Getters. */
        Vertex getVertex() const;
        Location getLocation() const;
        vector<Edge> getEdges() const;
    };

    /** Represents a tile directed weighted graph. */
    class Graph
    {
        private:

        /** Index into map given a vertex's unique id and return associated GraphNode. */
        unordered_map<int, GraphNode> nodes; // Also called quantizer.

        /** Index into map given a location and return associated GraphNode. */
        unordered_map<Location, GraphNode> localizer;

        /** Number of rows to represent graph. */
        int rows;

        /** Number of columns to represent graph. */
        int cols;

        public:

        /** Constructs a new graph initializing all its fields given the map of edges.*/
        Graph(const unordered_map<int, GraphNode>& nodes);

        /** Returns the list of outgoing edges from the given vertex. */
        inline vector<Edge> getOutgoingEdges(const Vertex& of) {
            return nodes.at(of.getID()).getEdges();
        }

        /** Maps vertex in graph to location in level. */
        inline Location localize(const graph::Vertex& vertex) {
            return nodes.at(vertex.getID()).getLocation();

        }

        /** Maps location in level to GraphNode in graph. */
        inline GraphNode quantize(const Location& location) {
            return localizer.at(location);
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
        VertexRecord(const Vertex& v, const VertexState& s);

        /** Returns state of vertex.*/
        VertexState getState() const;

        /** Sets state of vertex. */
        void setState(VertexState newState);
    };

};

#endif