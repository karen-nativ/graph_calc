#ifndef GCALC_H
#define GCALC_H

#include "Graph.h"
#include <iostream>
#include <map>


Graph parseGraph(const std::string& full_graph);

void printGraph(const Graph& G, std::ostream& output);

void saveGraph(const Graph& G, const std::string& filename);

Graph loadGraph(const std::string& filename);

void printAllVariables(const std::map<std::string, Graph>& variables, ostream& output);

Graph execute(const std::string& command, map<std::string, Graph> variables);

void executeKnownCommand(const std::string& known_command, map<std::string, Graph>& variables, ostream& output);

bool readCommand(istream& input, std::map<std::string, Graph>& variables, ostream& output);

static std::string trim(std::string s, std::string trim_chars = " ");

static void writeVerticetoFile(const std::string& vertice, ostream& outfile);

static std::string readVerticefromFile(istream& infile);

static int getOpeningParentheses(const std::string& command);

static void ValidateVariableName(const std::string& var_name, set<std::string>& known_commands);

#endif //GCALC_H

