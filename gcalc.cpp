#include <map>
#include <set>
#include <fstream>
#include <iostream>
#include <regex>
#include <cctype>
#include "Exceptions.h"
#include "gcalc.h"

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

const string space_regex = "\\s*";
const string vertice_regex = "([\\w\\[;\\]]+)";
const string vertice_space = space_regex + vertice_regex + space_regex;

static string trim(string s, const string& trim_chars = " ");


static string trim(string s, const string& trim_chars)
{
    s.erase(0, s.find_first_not_of(trim_chars));
    s.erase(s.find_last_not_of(trim_chars) + 1);
    return s;
}

Graph parseGraph(const string& full_graph)
{
    const string edge = "<" + vertice_space + "," + vertice_space + ">";
    const string regex_edges = "^(" + vertice_space + ",)*" + vertice_space
        + "\\|(" + space_regex + "<" + vertice_space + "," + vertice_space + ">" + space_regex + ",)*"
        + space_regex + "<" + vertice_space + "," + vertice_space + ">" + space_regex + "$";
    const string regex_no_edges = "^((" + vertice_space + ",)*" + vertice_space + ")?" + space_regex + "(\\|)?"
        + space_regex + "$";
    smatch vertice_match;
    set<string> parsed_vertices;
    set<pair<string, string>> parsed_edges;
    string vertice_string;
    if(regex_match(full_graph, regex(regex_edges))) {
        //Graph with edges
        smatch split, edge_match;
        regex_search(full_graph, split, regex("\\|"));
        vertice_string = split.prefix();

        string edge_string = split.suffix();
        while(regex_search(edge_string, edge_match, regex(edge))) {

            //CHECK IF EDGES ARE VALID?
            if(parsed_edges.insert({ edge_match[1], edge_match[2] }).second) {
                edge_string = edge_match.suffix();
            }
            else {
                //Trying to enter duplicate
                throw InvalidInitialization();
            }

        }
    }
    else if(regex_match(full_graph, regex(regex_no_edges))) {
        //Graph without edges
        vertice_string = full_graph;
    }
    else {
        throw InvalidInitialization();
    }

    while(regex_search(vertice_string, vertice_match, regex(vertice_regex))) {
        //CHECK IF VERTICE IS VALID
        if(parsed_vertices.insert(vertice_match[0]).second) {
            vertice_string = vertice_match.suffix();
        }
        else {
            throw InvalidInitialization();
        }
    }
    return Graph(parsed_vertices, parsed_edges);
}

void printGraph(const Graph& G, ostream& output)
{
    for(const string& vertice : G.getVertices()) {
        output << vertice << endl;
    }
    output << "$" << endl;
    for(const pair<string, string>& edge : G.getEdges()) {
        output << edge.first << " " << edge.second << endl;
    }
}

static void writeVerticetoFile(const string& vertice, ostream& outfile)
{
    unsigned int vertice_length = vertice.length();
    outfile.write((char*)&vertice_length, sizeof(vertice_length));
    outfile.write((char*)&vertice[0], vertice_length);
}

void saveGraph(const Graph& G, const string& filename)
{
    ofstream outfile(filename, std::ios_base::binary);
    if(!outfile) {
        throw OpenFileError(filename);
    }
    set<string> vertices = G.getVertices();
    unsigned int num_vertices = vertices.size();
    outfile.write((char*)&num_vertices, sizeof(num_vertices));

    set<pair<string, string>> edges = G.getEdges();
    unsigned int num_edges = edges.size();
    outfile.write((char*)&num_edges, sizeof(num_edges));

    for(const string& vertice : vertices) {
        writeVerticetoFile(vertice, outfile);
    }
    for(const pair<string, string>& edge : edges) {
        writeVerticetoFile(edge.first, outfile);
        writeVerticetoFile(edge.second, outfile);
    }
}

static string readVerticefromFile(istream& infile)
{
    unsigned int vertice_length;
    infile.read((char*)&vertice_length, sizeof(vertice_length));
    if(!infile) {
        throw OpenFileError();
    }
    string vertice;
    vertice.resize(vertice_length);
    infile.read((char*)&vertice[0], vertice_length);
    if(!infile) {
        throw OpenFileError();
    }
    return vertice;
}

Graph loadGraph(const string& filename)
{
    set<string> parsed_vertices;
    set<pair<string, string>> parsed_edges;
    ifstream infile(filename, std::ios_base::binary);
    if(!infile) {
        throw OpenFileError(filename);
    }
    try {
        unsigned int num_vertices;
        infile.read((char*)&num_vertices, sizeof(num_vertices));
        if(!infile) {
            throw OpenFileError(filename);
        }

        unsigned int num_edges;
        infile.read((char*)&num_edges, sizeof(num_edges));
        if(!infile) {
            throw OpenFileError(filename);
        }

        for(unsigned int i = 0; i < num_vertices; i++) {
            string vertice = readVerticefromFile(infile);
            if(!parsed_vertices.insert(vertice).second) {
                throw InvalidInitialization();
            }
        }
        for(unsigned i = 0; i < num_edges; i++) {
            string vertice1 = readVerticefromFile(infile);
            string vertice2 = readVerticefromFile(infile);
            if(!parsed_edges.insert({ vertice1, vertice2 }).second) {
                throw InvalidInitialization();
            }
        }
    }
    catch(const OpenFileError& exc) {
        throw OpenFileError(filename);
    }
    catch(const std::length_error& exc) {
        throw OpenFileError(filename);
    }

    return Graph(parsed_vertices, parsed_edges);
}

void printAllVariables(const map<string, Graph>& variables, ostream& output)
{
    for(pair<string, Graph> variable : variables) {
        output << variable.first << endl;
    }
}

static int getOpeningParentheses(const string& command)
{
    int counter = 1;
    if(command.back() == ')') {
        for(int i = command.length() - 2; i >= 0; i--) {
            if(command[i] == ')') {
                counter++;
            }
            else if(command[i] == '(') {
                if(counter == 1) {
                    return i;
                }
                counter--;
            }
        }

    }
    //We have arrived to the beginning and haven't matched all parentheses, or the last character is not parentheses
    throw SyntaxError();
}

Graph execute(const string& command, map<string, Graph> variables)
{
    if(command.length() == 0) {
        throw SyntaxError();
    }
    Graph left_operand, right_operand;
    string non_const_command = trim(command);
    string load = "load";
    string load_regex = load + space_regex + "\\(" + space_regex + "([^,\\(\\)]*?)" + space_regex + "\\)";
    bool parentheses = (command.length() > 0) && (command.back() == ')');
    if(parentheses) {
        int opening = getOpeningParentheses(command);
        string right_operand_str = command.substr((opening + 1), static_cast<int>(command.length()) - opening - 2);
        string left_part = trim(command.substr(0, opening));
        if((left_part.length() >= load.length()) &&
            (left_part.compare(left_part.length() - load.length(), load.length(), load) == 0)) {
            //We have load command
            parentheses = false;
        }
        else {
            //We have parentheses
            non_const_command = left_part;
            right_operand = execute(trim(right_operand_str), variables);
        }
    }
    smatch value_match;
    const string no_parentheses_regex = "^" + space_regex + "(?:(.+?)(\\+|-|\\*|\\^))??" + space_regex +
        "((?:!" + space_regex + ")*)(?:(?:\\{(?!.*\\{.*)(.*)\\})|(\\w+)|" + load_regex + ")" + space_regex + "$";
    const string parentheses_regex = "^" + space_regex + "(?:(.+?)(\\+|-|\\*|\\^))??" + space_regex +
        "((?:!" + space_regex + ")*)$";

    if((!parentheses) && regex_match(non_const_command, value_match, regex(no_parentheses_regex))) {
        if(value_match[4] != "") {
            //Initialization on right operand
            right_operand = parseGraph(trim(value_match[4]));
        }
        else if(value_match[5] != "") {
            //Copy c'tor on right operand
            if(variables.find(value_match[5]) != variables.end()) {
                //variables exists - good
                right_operand = variables[value_match[5]];
            }
            else {
                throw UndefinedVariable(value_match[5]);
            }
        }
        else if(value_match[6] != "") {
            //We have load
            right_operand = loadGraph(trim(value_match[6]));
        }
        else {
            throw SyntaxError();
        }
    }
    else if(!((parentheses) && regex_match(non_const_command, value_match, regex(parentheses_regex)))) {
        throw SyntaxError();
    }

    string complements = value_match[3];
    bool right_complement = (count(complements.begin(), complements.end(), '!') % 2 != 0);
    if(right_complement) {
        right_operand = !right_operand;
    }
    if(value_match[2] != "") {
        //There is a math operator, therefore also left operand
        left_operand = execute(trim(value_match[1]), variables);
        if(value_match[2] == "+") {
            return left_operand + right_operand;
        }
        else if(value_match[2] == "-") {
            return left_operand - right_operand;
        }
        else if(value_match[2] == "*") {
            return left_operand * right_operand;
        }
        else if(value_match[2] == "^") {
            return left_operand ^ right_operand;
        }
        else {
            throw SyntaxError();
        }
    }
    else {
        //There is no left operand, complement has already been checked
        return right_operand;
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
    else if(regex_match(known_command, var_match, regex("save" + space_regex +
        "\\(" + space_regex + "(.*?)" + space_regex + "," + space_regex + "([^,]*)" + space_regex + "\\)"))) {
        //Save graph to file
        Graph to_save = execute(var_match[1], variables);
        saveGraph(to_save, var_match[2]);
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
    for(size_t i = 0; i < var_name.length(); i++) {
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
    set<string> known_commands = { "reset", "who", "delete" , "print", "save", "load", exit };
    string command;
    if(!getline(input, command)) {
        return true;
    }
    command = trim(command, " ");
    if(command == exit) {
        return true;
    }
    //output << endl << command << endl;
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

int main(int argc, char* argv[])
{
    map<string, Graph> variables;
    bool stop = false;
    bool from_console = false;
    ifstream input_file;
    ofstream output_file;
    switch(argc) {
        case 1:
        {
            from_console = true;
            break;
        }
        case 3:
        {
            input_file.open(argv[1]);
            output_file.open(argv[2]);
            if(!input_file || !output_file) {
                cerr << "Error: Failure while trying to open file" << endl;
                stop = true;
                return -1;
                //throw OpenFileError();
            }
            break;
        }
        default:
        {
            stop = true;
            cerr << "Error: Illegal argument amount: " << argc << endl;
            return -1;
        }
    }
    while(!stop) {
        if(from_console) {
            //Read from console
            cout << "Gcalc> ";
        }
        try {
            stop = from_console ? readCommand(cin, variables, cout) : readCommand(input_file, variables, output_file);
            if(stop) {
                return 0;
            }
        }
        catch(const GraphException& exc) {
            if(from_console) {
                cout << exc.what() << endl;
            }
            else {
                output_file << exc.what() << endl;
            }
        }
        catch(const std::exception& exc) { //Fatal Error
            cerr << "Error: " << exc.what() << endl;
            stop = true;
            return -1;
        }
        catch(...) {
            cerr << "Error: Unknown error occured" << endl;
            stop = true;
            return -1;
        }
    }
    return -1;
}
