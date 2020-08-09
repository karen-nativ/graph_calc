#ifndef GCALC_H
#define GCALC_H

#include "Graph.h"
#include <iostream>
#include <map>


Graph parseGraph(const std::string& full_graph);

void printGraph(const Graph& G, std::ostream& output);

void saveGraph(const Graph& G, const std::string& filename);

Graph loadGraph(const std::string& filename);

void printAllVariables(const std::map<std::string, Graph>& variables, std::ostream& output);

Graph execute(const std::string& command, std::map<std::string, Graph> variables);

void executeKnownCommand(const std::string& known_command, 
    std::map<std::string, Graph>& variables, std::ostream& output);

bool readCommand(std::istream& input, std::map<std::string, Graph>& variables, std::ostream& output);

#endif //GCALC_H

