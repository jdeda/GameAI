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

        /** This is very dangerous, but hacking my way to make things work. */
        void setID(int id);
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

        // Getters.
        float getCost() const;
        Vertex getFromVertex() const;
        Vertex getToVertex() const;
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

        /** Setters. */
        void setEdges(const vector<Edge>& edges); // TODO: This is dangerous but I am hacking...
        void appendEdges(const vector<Edge>& edges); // TODO: This is dangerous but I am hacking...

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
        Graph(int rows, int cols, const unordered_map<int, GraphNode>& nodes);

        /** Copy constructor. */
        Graph(const Graph& graph);

        /** Returns the list of outgoing edges from the given vertex. */
        inline vector<Edge> getOutgoingEdges(const Vertex& of) const {
            return nodes.at(of.getID()).getEdges();
        }

        inline GraphNode getNode(const graph::Vertex& vertex) const {
            return nodes.at(vertex.getID());
        }

        /** Maps vertex in graph to location in level. */
        inline Location localize(const graph::Vertex& vertex) const {
            return nodes.at(vertex.getID()).getLocation();
        }

        /** Maps location in level to GraphNode in graph. */
        inline GraphNode quantize(const Location& location) const {
            return localizer.at(location);
        }

        /** Getters. */
        int getRows();
        int getCols();

        /** Prints IDs of verticies in graph. */
        void print();

        void printy();
    };
};

#endif