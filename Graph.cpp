#include "graph.h"
#include <algorithm>
#include <iterator>
#include <cctype>

using namespace std;

/**
* Creates a map of all possible edges between given vertices.
* @param vertices The vertices from which we create all edges.
* @return A map of edges of the full graph of vertices
*/
static map<string, string> createFullEdges(set<string> vertices);

/**
* Validates that the given string follows requirements for vertice names.
* @param vertice_name The string that is validated.
* @return true if the name is a valid vertice name, otherwise false.
*/
static bool isValidName(const std::string& vertice_name);

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

Graph::Graph(const set<string>& new_vertices, const map<string, string>& new_edges) :
    vertices(new_vertices), edges(new_edges)
{
    //remove_if(vertices.begin(), vertices.end(), !(isValidName));
    ValidEdge isValid(vertices);
    //remove_if(edges.begin(), edges.end(), isValid);
}

Graph::Graph(const Graph& graph) : vertices(graph.vertices), edges(graph.edges)
{
}

Graph Graph::operator-(const Graph& graph) const
{
    set<string> difference_vertices;
    map<string, string> difference_edges;
    set_difference(vertices.begin(), vertices.end(), graph.vertices.begin(),
        graph.vertices.end(), inserter(difference_vertices, difference_vertices.begin()));

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
    map<string, string> all_edges = createFullEdges(vertices);
    map<string, string> complement_edges;
    set_difference(all_edges.begin(), all_edges.end(), edges.begin(),
        edges.end(), inserter(all_edges, complement_edges.begin()));
    return Graph(vertices, complement_edges);
}

Graph operator+(const Graph& graph1, const Graph& graph2)
{
    set<string> union_vertices;
    set_union(graph1.vertices.begin(), graph1.vertices.end(),
        graph2.vertices.begin(), graph2.vertices.end(), union_vertices.begin());

    map<string, string> union_edges;
    set_union(graph1.edges.begin(), graph1.edges.end(),
        graph2.edges.begin(), graph2.edges.end(), union_edges.begin());

    return Graph(union_vertices, union_edges);
}

Graph operator^(const Graph& graph1, const Graph& graph2)
{
    set<string> intersection_vertices;
    set_intersection(graph1.vertices.begin(), graph1.vertices.end(),
        graph2.vertices.begin(), graph2.vertices.end(), intersection_vertices.begin());

    map<string, string> intersection_edges;
    set_intersection(graph1.edges.begin(), graph1.edges.end(),
        graph2.edges.begin(), graph2.edges.end(), intersection_edges.begin());

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

    map<string, string> product_edges;
    for(const pair<string, string>& first_edge : graph1.edges) {
        for(const pair<string, string>& second_edge : graph2.edges) {
            product_edges.insert({ '[' + first_edge.first + ';' + second_edge.first + ']',
                '[' + first_edge.second + ';' + second_edge.second + ']' });
        }
    }

    return Graph(product_vertices, product_edges);
}

static map<string, string> createFullEdges(set<string> vertices)
{
    map<string, string> edges;
    for(const string& first_vertice : vertices) {
        for(const string& second_vertice : vertices) {
            if(first_vertice != second_vertice) {
                edges.insert({ first_vertice, second_vertice });
            }

        }
    }
    return edges;
}

static bool isValidName(const std::string& vertice_name)
{
    int parentheses = 0;
    for(const char& character : vertice_name) {
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
    }
    return (parentheses == 0);
}



