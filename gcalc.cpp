#include <iostream>
#include <string>
#include <map>
#include "Graph.h"

using std::string;
using std::set;
using std::pair;
using std::map;
using std::cin;
using std::cout;

string trim(string s, string trim_chars)
{
    s.erase(0, s.find_first_not_of(trim_chars));
    s.erase(s.find_last_not_of(trim_chars) + 1);
    return s;
}

set<string> split(string source, string deliminator)
{
    set<string> split_string;
    size_t deliminator_index = source.find(deliminator);
    while(deliminator_index != string::npos) {
        split_string.insert(trim(source.substr(0, deliminator_index), " <>"));
        source = source.substr(deliminator_index + deliminator.length());
        deliminator_index = source.find(deliminator);
    }
    split_string.insert(trim(source, " <>"));
    return split_string;
}

Graph parseGraph(const string& full_graph)
{
    int delimiter_index = full_graph.find("|");
    string vertices = trim(full_graph.substr(0, delimiter_index - 1), " ");
    string edges = trim(full_graph.substr(delimiter_index + 1), " ");

    set<string> parsed_vertices = split(vertices, ",");
    set<string> edge_set = split(edges, ">,");
    set<pair<string, string>> parsed_edges;
    
    for(const string& edge : edge_set) {
        size_t seperator_index = edge.find(",");
        string first = edge.substr(0, seperator_index);
        string second = edge.substr(seperator_index + 1, edge.length() - seperator_index - 1);
        parsed_edges.insert({ first, second });
    }
    return Graph(parsed_vertices, parsed_edges);
}

void main()
{
    map<string, string> variables;
    cout << "Gcalc> ";
    string command;
    getline(cin, command);
    size_t equals = command.find_first_of("=");
    string variable = command.substr(0, equals);
    string value = command.substr(equals + 1);

    if(value.front() == '{') {
        size_t closing = value.find_first_of("}");
        string full_graph = value.substr(1, closing - 1);
        Graph G1 = parseGraph(full_graph);
    }


}
