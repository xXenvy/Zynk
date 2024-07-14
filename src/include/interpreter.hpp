#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include "ast.hpp"

class ZynkInterpreter {
public:
    const ASTProgram* interpret(const std::string& source);
    const ASTProgram* interpret_file(const std::string& file_path);
};

#endif // INTERPRETER_H