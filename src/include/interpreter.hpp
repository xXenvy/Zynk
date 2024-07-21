#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include "ast.hpp"

class ZynkInterpreter {
public:
    std::shared_ptr<ASTProgram> interpret(const std::string& source);
    std::shared_ptr<ASTProgram> interpret_file(const std::string& file_path);
};

#endif // INTERPRETER_H