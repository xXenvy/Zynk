#include "include/runtime.hpp"
#include "include/errors.hpp"

void RuntimeEnvironment::declareVariable(const std::string& name, ASTVariableDeclaration* variable) {
	variables[name] = variable;
}

ASTVariableDeclaration* RuntimeEnvironment::getVariable(const std::string& name) {
    const auto iterator = variables.find(name);
    if (iterator != variables.end()) {
        return iterator->second;
    }
    throw ZynkError{
        ZynkErrorType::VariableNotDefined,
        "Variable '" + name + "' is not defined."
    };
}

void RuntimeEnvironment::show() {
    for (const auto& pair : variables) {
        std::cout << pair.first << std::endl;
        std::cout << pair.second->type << std::endl;
        std::cout << static_cast<ASTValue*>(pair.second->value)->value << std::endl;
        std::cout << std::endl;
    }
}