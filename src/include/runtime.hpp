#ifndef RUNTIME_H
#define RUNTIME_H

#include <unordered_map>
#include <string>

class RuntimeEnvironment {
private:
    std::unordered_map<std::string, std::string> variables;
};


#endif // RUNTIME_H