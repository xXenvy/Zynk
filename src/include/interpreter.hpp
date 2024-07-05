#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "lexer.hpp"
#include <string>

class BaseInterpreter {
public:
    virtual void interpret(const std::string& source) = 0;
    virtual void interpret_file(const std::string& file_path) = 0;
};


class ZynkInterpreter : public BaseInterpreter {
public:
    void interpret(const std::string& source) override;
    void interpret_file(const std::string& file_path) override;
};

#endif // INTERPRETER_H