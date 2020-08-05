#include <iostream>
#include <string>
#include <map>
#include <set>
#include "Graph.h"
#include <fstream>
#include <regex>
#include <cctype>

using std::string;
using std::set;
using std::pair;
using std::map;
using std::cin;
using std::cout;
using std::endl;
using std::istream;
using std::ifstream;
using std::ofstream;
using std::cerr;
using std::regex;
using std::regex_match;
using std::smatch;
using std::unary_function;

const string space_regex = "\\\s*";
const string vertice_regex = "([\\\w\\[;\\]]+)";

string trim(string s, string trim_chars = " ")
{
    s.erase(0, s.find_first_not_of(trim_chars));
    s.erase(s.find_last_not_of(trim_chars) + 1);
    return s;
}

Graph parseGraph(const string& full_graph)
{
    const string vertice_space = space_regex + vertice_regex + space_regex;
    const string edge = "<" + vertice_space + "," + vertice_space + ">";
    const string full_regex = "(?:" + vertice_space + ",)*" + vertice_space 
        + "(\\|)(?:" + space_regex + "<" + vertice_space + "," + vertice_space + ">" + space_regex + ",)*"
        + space_regex + "<" + vertice_space + "," + vertice_space + ">" + space_regex;

    smatch split, vertice_match, edge_match;
    if(regex_match(full_graph, regex(full_regex))) {
        //Exception - incorrect syntax
    }
    regex_search(full_graph, split, regex("\\|"));

    set<string> parsed_vertices;

    string vertice_string = split.prefix();
    while(regex_search(vertice_string, vertice_match, regex(vertice_regex))) {
        //CHECK IF VERTICE IS VALID
        parsed_vertices.insert(vertice_match[0]);
        vertice_string = vertice_match.suffix();
    }

    set<pair<string, string>> parsed_edges;
    string edge_string = split.suffix();
    while(regex_search(edge_string, edge_match, regex(edge))) {

        //CHECK IF EDGES ARE VALID?
        parsed_edges.insert({ edge_match[1], edge_match[2] });
        edge_string = edge_match.suffix();
    }
    return Graph(parsed_vertices, parsed_edges);
}

/*Graph parseGraph(const string& full_graph)
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
}*/

void printGraph(Graph G)
{
    for(const string& vertice : G.getVertices()) {
        cout << vertice << endl;
    }
    cout << "$" << endl;
    for(const pair<string, string>& edge : G.getEdges()) {
        cout << edge.first << " " << edge.second << endl;
    }
}

void printAllVariables(const map<string, Graph>& variables)
{
    for(pair<string, Graph> variable : variables) {
        cout << variable.first << endl;
    }
}

void readCommand(istream& input)
{
    //Logic of command execution
}

Graph execute(const string& command, map<string, Graph> variables)
{
    smatch value_match;
    string ops_regex = space_regex + "(!?)" + space_regex + "(?:\\{(.*)\\}|" + vertice_regex + "|\\((.*)\\))" + 
        space_regex + "(?:(\\+|-|\\*|^)(.+))?" + space_regex;
    Graph left_operand;
    Graph right_operand;
    if(regex_match(command, value_match, regex(ops_regex))) {
        bool has_complement = value_match[1] != "";
        if(value_match[2] != "") {
            //Initialization
            left_operand = has_complement ? !(parseGraph(trim(value_match[2]))) : parseGraph(trim(value_match[2]));
        }
        else if(value_match[3] != "") {
            //Copy c'tor
            if(variables.find(value_match[3]) != variables.end()) {
                //variables exists - good
                left_operand = has_complement ? !(variables[value_match[3]]) : variables[value_match[3]];
            }
            else {
                cout << "Error: Undefined variable '" + value_match[3].str() + "'"<< endl;
            }
        }
        else {
            //We remain with ()
            left_operand = has_complement ? !(execute(trim(value_match[4]), variables)) :
                execute(trim(value_match[4]), variables);
        }
        if(value_match[5] != "") {
            //There is a math operator
            right_operand = execute(trim(value_match[6]), variables);
            if(value_match[5] == "+") {
                return left_operand + right_operand;
            }
            else if(value_match[5] == "-") {
                return left_operand - right_operand;
            }
            else if(value_match[5] == "*") {
                return left_operand * right_operand;
            }
            else if(value_match[5] == "^") {
                return left_operand ^ right_operand;
            }
            else {
                //Error - incorrect regex operator...
                return Graph();
            }  
        }
        else {
            //There is only a left operand
            return left_operand;
        }
    }
    else {
        //Error - incorrect rvalue?
        cout << "ERROR!! Incorrect rvalue? (Line 160)" << endl;
        return Graph();
    }
}

void executeKnownCommand(const string& known_command, map<string, Graph>& variables)
{
    smatch var_match;
    if(known_command == "reset") {
        //Do I need to free the graph?
        variables.clear();
    }
    else if(known_command == "who") {
        printAllVariables(variables);
    }
    else if(regex_match(known_command, var_match, regex("delete" + space_regex + "(.*)"))) {
        string var = trim(var_match[0], " ");
        if(variables.find(var) != variables.end()) {
            variables.erase(var);
        }
        else {
            //Error - trying to delete a variable that doesn't exist!
        }
    }

    else if(regex_match(known_command, var_match,
        regex("print" + space_regex + "\\(" + space_regex + "(.*)" + space_regex + "\\)" + space_regex))) {
        string var = trim(var_match[1], " ");
        printGraph(execute(var, variables));
        }
    else {
        //Error - unknown command
    }
}

static bool isValidVariableName(const string& var_name)
{
    if(!isalpha(var_name[0])) {
        return false;
    }
    for(int i = 0; i < var_name.length(); i++) {
        if(!isalnum(var_name[i])) {
            return false;
        }
    }
    return true;
}

void main(int argc, char* argv[])
{
    /* switch(argc) {
         case 1: //Read from console
         {
             break;
         }
         case 3: //Read from file
             break;
         default: //Error
             break;
     }*/

    map<string, Graph> variables;
    const string exit = "quit";
    set<string> known_commands = { "reset", "who", "delete" , "print" };
    string command;
    cout << "Gcalc> ";
    getline(cin, command);
    command = trim(command, " ");
    while(command != exit) {
        size_t equals = command.find_first_of("=");
        if(equals == string::npos) {
            //A saved word is used
            executeKnownCommand(command, variables);
        }
        else {
            //A variable is declared
            string variable = trim(command.substr(0, equals), " ");
            string value = trim(command.substr(equals + 1), " ");
            if(known_commands.find(variable) == known_commands.end()) {
                //If variable already exists in variables do we need to free memory??
                if(isValidVariableName(variable)) {
                    variables[variable] = execute(value, variables);
                }
                else {
                    //Error - invalid variable name!
                }
            }
            else {
                //Error - the variable name is already a known command
            }
        }
        cout << "Gcalc> ";
        getline(cin, command);
    }
}
