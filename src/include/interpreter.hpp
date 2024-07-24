#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include "ast.hpp"

class ZynkInterpreter {
public:
    void interpret(const std::string& source);
    void interpret_file(const std::string& file_path);
};

#endif // INTERPRETER_H