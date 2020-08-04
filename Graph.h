#ifndef GRAPH_H
#define GRAPH_H

#include <set> 
#include <string>
#include <map>

/**
* Class representing a graph containing vertices and edges.
*/
class Graph
{
    std::set<std::string> vertices;
    std::map<std::string, std::string> edges;

public:
    /**
     * C'tor.
     * @param vertices The vertices of the graph.
     * @param edges The edges of the graph.
     */
    explicit Graph(const std::set<std::string>& vertices, const std::map<std::string, std::string>& edges);

    /**
    * Copy c'tor.
    * @param graph The graph to copy.
    */
    Graph(const Graph& graph);

    /**
    * Creates a copy of the graph to the left of the minus,
    *   removing all vertices that exist on the graph to the right, and all edges that are connected to them.
    * @param graph Matrix to copy.
    * @return A reference to a new graph containing the vertices unique to the left graph,
    *           and the edges from the left graph that connect these vertices.
    */
    Graph operator-(const Graph& graph) const;

    /**
    * Creates a copy of the graph, with all possible edges connecting two vertices, not including the ones that
    *   exist on the given graph.
    * @return A reference to the graph created, which is the complement of the given graph.
    */
    Graph operator!() const;

    friend Graph operator+(const Graph& graph1, const Graph& graph2);
    friend Graph operator^(const Graph& graph1, const Graph& graph2);
    friend Graph operator*(const Graph& graph1, const Graph& graph2);
};


/**
* Calculates the union of two graphs.
* @param graph1 First graph to add.
* @param graph2 Second graph to add.
* @return A reference to a new graph which is the union of the given graphs.
*/
Graph operator+(const Graph& graph1, const Graph& graph2);

/**
* Calculates the intersection of two graphs.
* @param graph1 First graph to intersect.
* @param graph2 Second graph to intersect.
* @return A reference to a new graph which is the intersection of the given graphs.
*/
Graph operator^(const Graph& graph1, const Graph& graph2);

/**
* Calculates the product of two graphs, which is a graph that contains vertices that are all combinations of the
*   given graphs, and edges that connect between two vertices: [v1, u1] and [v2, u2] if in the given graphs the edges
*   <v1, v2> and <u1, u2> exist.
* @param graph1 First graph to multiply.
* @param graph2 Second graph to multiply.
* @return A reference to a new graph which is the product of the given graphs.
*/
Graph operator*(const Graph& graph1, const Graph& graph2);

#endif GRAPH_H
