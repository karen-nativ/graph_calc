#include "Exceptions.h"
#include <exception>

using std::string;
using std::pair;

GraphException::GraphException(const string& name) : message("Error: " + name)
{
}

const char* GraphException::what() const noexcept
{
    return message.data();
}

IllegalVerticeName::IllegalVerticeName(const string& vertice_name) : 
    GraphException("Illegal vertice name '" + vertice_name + "'")
{
}

IllegalEdge::IllegalEdge(const pair<string, string>& edge) :
    GraphException("Illegal edge '<" + edge.first + " " + edge.second + ">'")
{
}

IllegalVariableName::IllegalVariableName(const string& var_name) : 
    GraphException("Illegal variable name '" + var_name + "'")
{
}

UndefinedVariable::UndefinedVariable(const string& var_name) : 
    GraphException("Undefined variable '" + var_name + "'")
{
}

UnrecognizedCommand::UnrecognizedCommand(const string& command) :
    GraphException("Unrecognized command '" + command + "'")
{
}

InvalidInitialization::InvalidInitialization() : GraphException("Invalid graph initialization")
{
}

OpenFileError::OpenFileError(const string& filename) : GraphException("Unable to open '" + filename + "'")
{
}

SyntaxError::SyntaxError() : GraphException("Syntax error")
{
}

