#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <vector>
#include "token.hpp"

class BaseInterpreter {
public:
    virtual std::vector<Token> interpret(const std::string& source) = 0;
    virtual std::vector<Token> interpret_file(const std::string& file_path) = 0;
};


class ZynkInterpreter : public BaseInterpreter {
public:
    std::vector<Token> interpret(const std::string& source) override;
    std::vector<Token> interpret_file(const std::string& file_path) override;
};

#endif // INTERPRETER_H