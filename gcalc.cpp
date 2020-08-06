#include <iostream>
#include <string>
#include <map>
#include <set>
#include "Graph.h"
#include <fstream>
#include <regex>
#include <cctype>
#include "Exceptions.h"

using std::string;
using std::set;
using std::pair;
using std::map;
using std::cin;
using std::cout;
using std::endl;
using std::istream;
using std::ostream;
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
    if(!regex_match(full_graph, regex(full_regex))) {
        throw InvalidInitialization();
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

void printGraph(Graph G, ostream& output)
{
    for(const string& vertice : G.getVertices()) {
        output << vertice << endl;
    }
    output << "$" << endl;
    for(const pair<string, string>& edge : G.getEdges()) {
        output << edge.first << " " << edge.second << endl;
    }
}

void printAllVariables(const map<string, Graph>& variables, ostream& output)
{
    for(pair<string, Graph> variable : variables) {
        output << variable.first << endl;
    }
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
                throw UndefinedVariable(value_match[3]);
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
        return Graph();
    }
}

void executeKnownCommand(const string& known_command, map<string, Graph>& variables, ostream& output)
{
    smatch var_match;
    if(known_command == "reset") {
        //Do I need to free the graph?
        variables.clear();
    }
    else if(known_command == "who") {
        printAllVariables(variables, output);
    }
    else if(regex_match(known_command, var_match,
        regex("delete" + space_regex + "\\(" + space_regex + "(.*)" + space_regex + "\\)"))) {
        string var = trim(var_match[1], " ");
        if(variables.find(var) != variables.end()) {
            variables.erase(var);
        }
        else {
            throw UndefinedVariable(var);
        }
    }

    else if(regex_match(known_command, var_match,
        regex("print" + space_regex + "\\(" + space_regex + "(.*)" + space_regex + "\\)"))) {
        string var = trim(var_match[1], " ");
        printGraph(execute(var, variables), output);
    }
    else {
        throw UnrecognizedCommand(known_command);
    }
}

static void ValidateVariableName(const string& var_name, set<string>& known_commands)
{
    if(!isalpha(var_name[0])) {
        throw IllegalVariableName(var_name);
    }
    for(int i = 0; i < var_name.length(); i++) {
        if(!isalnum(var_name[i])) {
            throw IllegalVariableName(var_name);
        }
    }
    if(known_commands.find(var_name) != known_commands.end()) {
        //Variable name is a known command
        throw IllegalVariableName(var_name);
    }

}

bool readCommand(istream& input, map<string, Graph>& variables, ostream& output)
{
    const string exit = "quit";
    set<string> known_commands = { "reset", "who", "delete" , "print", exit };
    string command;
    if(!getline(input, command)) {
        return true;
    }
    command = trim(command, " ");
    if(command == exit) {
        return true;
    }
    size_t equals = command.find_first_of("=");
    if(equals == string::npos) {
        //A saved word is used
        executeKnownCommand(command, variables, output);
    }
    else {
        //A variable is declared
        string variable = trim(command.substr(0, equals), " ");
        string value = trim(command.substr(equals + 1), " ");
        ValidateVariableName(variable, known_commands);
        //If variable already exists in variables do we need to free memory??
        variables[variable] = execute(value, variables);

    }
    return false;
}

void main(int argc, char* argv[])
{
    map<string, Graph> variables;
    bool stop = false;
    bool from_console = (argc == 1);
    ifstream input_file;
    ofstream output_file;
    if(!from_console) {
        input_file.open(argv[1]);
        output_file.open(argv[2]);
    }
    while(!stop) {
        if(from_console) {
            //Read from console
            cout << "Gcalc> ";
        }
        try {
            stop = from_console ? readCommand(cin, variables, cout) : readCommand(input_file, variables, output_file);
        }
        catch(const Exception& exc) {
            if(from_console) {
                cout << exc.what() << endl;
            }
            else {
                output_file << exc.what() << endl;
            }

        }
        catch(...) {
            if(from_console) {
                cout << "Error: Unknown error occured" << endl;
            }
            else {
                output_file << "Error: Unknown error occured" << endl;
            }

        }
    }
}
