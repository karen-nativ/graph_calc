#ifndef GRAPH_H
#define GRAPH_H

#include <set> 
#include <string>

/**
* Class representing a graph containing vertices and edges.
*/
class Graph
{
    std::set<std::string> vertices;
    std::set<std::pair<std::string, std::string> > edges;
private:
    /**
    * Creates a set of all possible edges between given vertices.
    * @param vertices The vertices from which we create all edges.
    * @return A set of edges of the full graph of vertices
    */
    static set<pair<string, string>> createFullEdges(set<string> vertices);

    /**
    * Validates that the given string follows requirements for vertice names.
    * @param vertice_name The string that is validated.
    * @return true if the name is a valid vertice name, otherwise false.
    */
    static bool isValidName(const std::string& vertice_name);
public:
    /**
     * Default C'tor. DO I NEED THIS?
     */
    explicit Graph() = default;

    /**
     * C'tor.
     * @param vertices The vertices of the graph.
     * @param edges The edges of the graph.
     */
    explicit Graph(const std::set<std::string>& vertices, const std::set<std::pair<std::string, std::string> >& edges);

    /**
    * Copy c'tor.
    * @param graph The graph to copy.
    */
    Graph(const Graph& graph) = default;

    /**
    * Default D'tor
    */
    ~Graph() = default; // DO I NEED TO DELETE (FREE) SETS?

    /**
     * Copies the given graph to the current one.
     * @param graph The graph to copy.
     * @return The updated graph.
     */
    Graph& operator=(const Graph& graph);

    /**
    * Get function for vertices of a graph.
    * @return A set of the vertices in the graph, in string format.
    */
    const std::set<std::string> getVertices() const;

    /**
    * Get function for edges of a graph.
    * @return A set of the edges in the graph, in string format.
    */
    const std::set<std::pair<std::string, std::string> > getEdges() const;

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

    void addNewVertice(const std::string& vertice_name);
    void addNewEdge(const std::pair<std::string, std::string>& edge);

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

#endif //GRAPH_H
