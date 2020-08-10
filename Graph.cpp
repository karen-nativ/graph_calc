#include "Graph.h"
#include <algorithm>
#include <iterator>
#include <cctype>
#include "Exceptions.h"

using namespace std;

/**
* A functor that validates that the given edge connects two different existing vertices.
*/
class ValidEdge
{
    /** A set of the existing vertices */
    const set<string> vertices;
public:
    ValidEdge(const set<string>& new_vertices) : vertices(new_vertices)
    {
    }
    bool operator()(const pair<string, string>& edge) const
    {
        return ((edge.first != edge.second) && (vertices.find(edge.first) != vertices.end()) &&
            (vertices.find(edge.second) != vertices.end()));
    }
};

Graph::Graph(const set<string>& new_vertices, const set<pair<string, string>>& new_edges)
{
    ValidEdge isValidEdge(new_vertices);
    if(all_of(new_vertices.begin(), new_vertices.end(), isValidName) && all_of(new_edges.begin(), new_edges.end(), isValidEdge)) {
        copy(new_vertices.begin(), new_vertices.end(), inserter(vertices, vertices.begin()));
        copy(new_edges.begin(), new_edges.end(), inserter(edges, edges.begin()));
    }
    else {
        //Invalid edge or vertice
        throw InvalidInitialization();
    }




    /* Without algorithm
    for(const string& new_vertice : new_vertices) {
        if(isValidName(new_vertice)) {
            vertices.insert(new_vertice);
        }
    }
    ValidEdge isValid(vertices);

    for(const pair<string, string>& new_edge : new_edges) {
        if(isValid(new_edge)) {
            edges.insert(new_edge);
        }
    }*/

}

Graph& Graph::operator=(const Graph& graph)
{
    if(this == &graph) {
        return *this;
    }
    vertices = graph.vertices;
    edges = graph.edges;
    return *this;
}

const set<string> Graph::getVertices() const
{
    return vertices;
}

const set<pair<string, string>> Graph::getEdges() const
{
    return edges;
}

void Graph::addNewVertice(const string& vertice_name)
{
    if(!isValidName(vertice_name) || !vertices.insert(vertice_name).second) {
        throw IllegalVerticeName(vertice_name);
    }
}

void Graph::addNewEdge(const pair<string, string>& edge)
{
    ValidEdge isValidEdge(vertices);
    if(!isValidEdge(edge) || !edges.insert(edge).second) {
        throw IllegalEdge(edge);
    }
}

Graph Graph::operator-(const Graph& graph) const
{
    /*if(this == &graph) {
        //if they are the same the difference is an empty graph
        return Graph();
    }*/

    set<string> difference_vertices;
    set<pair<string, string>> difference_edges;


    set_difference(vertices.begin(), vertices.end(), graph.vertices.begin(),
        graph.vertices.end(), inserter(difference_vertices, difference_vertices.begin()));

    /* Withough algorithm
    for(const string& vertice : vertices) {
        if(graph.vertices.find(vertice) == graph.vertices.end()) {
            difference_vertices.insert(vertice);
        }
    }
    */

    for(const pair<string, string>& edge : edges) {
        ValidEdge isValid(difference_vertices);
        if(isValid(edge)) {
            difference_edges.insert(edge);
        }
    }
    Graph difference(difference_vertices, difference_edges);
    return difference;
}

Graph Graph::operator!() const
{
    set<pair<string, string>> all_edges = createFullEdges(vertices);
    set<pair<string, string>> complement_edges;

    set_difference(all_edges.begin(), all_edges.end(), edges.begin(),
        edges.end(), inserter(complement_edges, complement_edges.begin()));

    /* Without algorithm
    for(const pair<string, string>& edge : all_edges) {
        if(edges.find(edge) == edges.end()) {
            complement_edges.insert(edge);
        }
    }
    */
    return Graph(vertices, complement_edges);
}

Graph operator+(const Graph& graph1, const Graph& graph2)
{
    set<string> union_vertices;
    set_union(graph1.vertices.begin(), graph1.vertices.end(),
        graph2.vertices.begin(), graph2.vertices.end(), inserter(union_vertices, union_vertices.begin()));

    /*Without algorithm
    for(const string& vertice : graph2.vertices) {
        union_vertices.insert(vertice);
    }*/

    set<pair<string, string>> union_edges;
    set_union(graph1.edges.begin(), graph1.edges.end(),
        graph2.edges.begin(), graph2.edges.end(), inserter(union_edges, union_edges.begin()));

    /* Without algorithm
    for(const pair<string, string>& edge : graph2.edges) {
        union_edges.insert(edge);
    }
    */
    return Graph(union_vertices, union_edges);
}

Graph operator^(const Graph& graph1, const Graph& graph2)
{
    set<string> intersection_vertices;
    set_intersection(graph1.vertices.begin(), graph1.vertices.end(), graph2.vertices.begin(),
        graph2.vertices.end(), inserter(intersection_vertices, intersection_vertices.begin()));

    /* Without algorithm
    for(const string& vertice : graph1.vertices) {
        if(graph2.vertices.find(vertice) != graph2.vertices.end()) {
            intersection_vertices.insert(vertice);
        }
    }*/

    set<pair<string, string>> intersection_edges;
    set_intersection(graph1.edges.begin(), graph1.edges.end(),
        graph2.edges.begin(), graph2.edges.end(), inserter(intersection_edges, intersection_edges.begin()));

    /*Without algorithm
    for(const pair<string, string>& edge : graph1.edges) {
        if(graph2.edges.find(edge) != graph2.edges.end()) {
            intersection_edges.insert(edge);
        }
    }
    */
    return Graph(intersection_vertices, intersection_edges);
}

Graph operator*(const Graph& graph1, const Graph& graph2)
{
    set<string> product_vertices;
    for(const string& first_vertice : graph1.vertices) {
        for(const string& second_vertice : graph2.vertices) {
            product_vertices.insert('[' + first_vertice + ';' + second_vertice + ']');
        }
    }

    set<pair<string, string>> product_edges;
    for(const pair<string, string>& first_edge : graph1.edges) {
        for(const pair<string, string>& second_edge : graph2.edges) {
            product_edges.insert({ '[' + first_edge.first + ';' + second_edge.first + ']',
                '[' + first_edge.second + ';' + second_edge.second + ']' });
        }
    }

    return Graph(product_vertices, product_edges);
}

set<pair<string, string>> Graph::createFullEdges(set<string> vertices)
{
    set<pair<string, string>> edges;
    for(const string& first_vertice : vertices) {
        for(const string& second_vertice : vertices) {
            if(first_vertice != second_vertice) {
                edges.insert({ first_vertice, second_vertice });
            }

        }
    }
    return edges;
}

bool Graph::isValidName(const std::string& vertice_name)
{
    int parentheses = 0;
    for(size_t i = 0; i < vertice_name.length(); i++) {
        switch(vertice_name[i]) {
            case '[':
                parentheses++;
                break;
            case ']':
                if(parentheses < 1) {
                    return false;
                }
                parentheses--;
                break;
            case ';':
                if(parentheses < 1) {
                    return false;
                }
                break;
            default:
                if(!isalnum(vertice_name[i])) {
                    return false;
                }
                break;
        }
    }
    /*for(const char& character : vertice_name) {
        switch(character) {
            case '[':
                parentheses++;
                break;
            case ']':
                if(parentheses < 1) {
                    return false;
                }
                parentheses--;
                break;
            case ';':
                if(parentheses < 1) {
                    return false;
                }
                break;
            default:
                if(!isalnum(character)) {
                    return false;
                }
                break;
        }
    }*/
    return (parentheses == 0) && (vertice_name.length() > 0);
}



