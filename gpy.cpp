#include <iostream>
#include "gpy.h"
#include "gcalc.h"
#include "Exceptions.h"

using std::string;
using std::ostream;
using std::cout;
using std::endl;

using namespace gcalc;

Graph* create()
{
    try {
        Graph* new_graph = new Graph();
        return new_graph;
    }
    catch(std::bad_alloc& ba) {
        std::cerr << "Error: Bad allocation " << ba.what() << endl;
        return nullptr;
    }
}

void destroy(Graph* graph)
{
    delete graph;
}

Graph* addVertex(Graph* graph, const char* v)
{
    try {
        const string v_string(v);
        (*graph).addNewVertice(v_string);
        return graph;
    }
    catch(const GraphException& exc) {
        cout << exc.what() << endl;
    }
    catch(...) {
        cout << "Error: Unable to add vertice" << endl;
    }
    return nullptr;
}

Graph* addEdge(Graph* graph, const char* v1, const char* v2)
{
    try {
        const string v1_string(v1);
        const string v2_string(v2);

        (*graph).addNewEdge({ v1, v2 });
        return graph;
    }
    catch(const GraphException& exc) {
        cout << exc.what() << endl;
    }
    catch(...) {
        cout << "Error: Unable to add edge" << endl;
    }
    return nullptr;
}

void disp(Graph* graph)
{
    if(graph == nullptr) {
        cout << "Error: Unable to print null pointer" << endl;
    }
    else {
        try {
            printGraph(*graph, cout);
        }
        catch(const GraphException& exc) {
            cout << exc.what() << endl;
        }
        catch(...) {
            cout << "Error: Unable to print graph" << endl;
        }
    }
}

Graph* graphUnion(Graph* graph_in1, Graph* graph_in2, Graph* graph_out)
{
    try {
        *graph_out = (*graph_in1) + (*graph_in2);
        return graph_out;
    }
    catch(const GraphException& exc) {
        cout << exc.what() << endl;
    }
    catch(...) {
        cout << "Error: Unable to add graphs" << endl;
    }
    return nullptr;

}

Graph* graphInsersection(Graph* graph_in1, Graph* graph_in2, Graph* graph_out)
{
    try {
        *graph_out = (*graph_in1) ^ (*graph_in2);
        return graph_out;
    }
    catch(const GraphException& exc) {
        cout << exc.what() << endl;
    }
    catch(...) {
        cout << "Error: Unable to intersect graphs" << endl;
    }
    return nullptr;
}

Graph* graphDifference(Graph* graph_in1, Graph* graph_in2, Graph* graph_out)
{
    try {
        *graph_out = (*graph_in1) - (*graph_in2);
        return graph_out;
    }
    catch(const GraphException& exc) {
        cout << exc.what() << endl;
    }
    catch(...) {
        cout << "Error: Unable to subtract graphs" << endl;
    }
    return nullptr;
}

Graph* graphProduct(Graph* graph_in1, Graph* graph_in2, Graph* graph_out)
{
    try {
        *graph_out = (*graph_in1) * (*graph_in2);
        return graph_out;
    }
    catch(const GraphException& exc) {
        cout << exc.what() << endl;
    }
    catch(...) {
        cout << "Error: Unable to multiply graphs" << endl;
    }
    return nullptr;
}

Graph* graphComplement(Graph* graph_in, Graph* graph_out)
{
    try {
        *graph_out = !(*graph_in);
        return graph_out;
    }
    catch(const GraphException& exc) {
        cout << exc.what() << endl;
    }
    catch(...) {
        cout << "Error: Unable to calculate complement" << endl;
    }
    return nullptr;
}