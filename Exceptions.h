#ifndef GRAPH_EXCEPTIONS_H
#define GRAPH_EXCEPTIONS_H

#include <exception>
#include <string>

/**
 * A generic exception class for the project.
 */
class Exception : public std::exception
{
};

/**
  * A generic exception class for the graph.
  */
class GraphException : public Exception
{
    const std::string message;

public:
    /**
     * @return Explanation about the exception, in a format that includes its name.
     */
    const char* what() const noexcept override;

protected:
    /**
     * Creates an exception with the given name.
     * @param name The exception's name.
     */
    explicit GraphException(const std::string& name);
};


/**
 * Thrown if a vertice name is illegal.
 */
class IllegalVerticeName : public GraphException
{
public:
    /**
     * C'tor
     * @param vertice_name The illegal vertice name.
     */
    IllegalVerticeName(const std::string& vertice_name);
};

/**
 * Thrown if a variable name is illegal.
 */
class IllegalVariableName : public GraphException
{
public:
    /**
     * C'tor
     * @param var_name The illegal variable name.
     */
    IllegalVariableName(const std::string& var_name);
};

/**
 * Thrown if a variable is undefined.
 */
class UndefinedVariable : public GraphException
{
public:
    /**
     * C'tor
     * @param var_name The name of the undefined variable.
     */
   UndefinedVariable(const std::string& var_name);
};

/**
 * Thrown if a command is unrecognized.
 */
class UnrecognizedCommand : public GraphException
{
public:
    /**
     * C'tor
     * @param command The undefined command.
     */
    UnrecognizedCommand(const std::string& command);
};

/**
 * Thrown if a graph initialization is invalid.
 */
class InvalidInitialization : public GraphException
{
public:
    /**
     * Default c'tor.
     */
    InvalidInitialization();
};

#endif //GRAPH_EXCEPTIONS_H