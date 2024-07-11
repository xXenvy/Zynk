#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <vector>
#include "token.hpp"

class BaseInterpreter {
public:
    virtual const std::vector<Token> interpret(const std::string& source) = 0;
    virtual const std::vector<Token> interpret_file(const std::string& file_path) = 0;
};


class ZynkInterpreter : public BaseInterpreter {
public:
    const std::vector<Token> interpret(const std::string& source) override;
    const std::vector<Token> interpret_file(const std::string& file_path) override;
};

#endif // INTERPRETER_H