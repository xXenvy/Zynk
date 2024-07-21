#include "include/runtime.hpp"
#include "include/errors.hpp"

void RuntimeEnvironment::declareVariable(const std::string& name, std::shared_ptr<ASTVariableDeclaration> variable) {
	variables[name] = variable;
}

std::shared_ptr<ASTVariableDeclaration> RuntimeEnvironment::getVariable(const std::string& name) {
    const auto iterator = variables.find(name);
    if (iterator != variables.end()) {
        return iterator->second;
    }
    throw ZynkError{
        ZynkErrorType::NotDefined,
        "Variable '" + name + "' is not defined."
    };
}

void RuntimeEnvironment::declareFunction(const std::string& name, std::shared_ptr<ASTFunction> function) {
    functions[name] = function;
}

std::shared_ptr<ASTFunction> RuntimeEnvironment::getFunction(const std::string& name) {
    const auto iterator = functions.find(name);
    if (iterator != functions.end()) {
        return iterator->second;
    }
    throw ZynkError{
        ZynkErrorType::NotDefined,
        "Function '" + name + "' is not defined."
    };
}

size_t RuntimeEnvironment::size() const {
    return variables.size();
}