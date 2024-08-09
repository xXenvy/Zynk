#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>

class ZynkInterpreter {
public:
    void interpret(const std::string& source);
    void interpretFile(const std::string& file_path);
};

#endif // INTERPRETER_H