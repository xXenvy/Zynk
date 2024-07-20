#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "runtime.hpp"
#include "ast.hpp"
#include <string>

class Evaluator {
public:
    Evaluator(RuntimeEnvironment& env);
    void evaluate(ASTBase* ast);
    size_t variablesCount() const;
private:
    RuntimeEnvironment env;

    std::string evaluateExpression(ASTBase* expression);
    void evaluateVariableDeclaration(ASTVariableDeclaration* variable);
    void evaluateProgram(ASTProgram* program);
    void evaluateFunctionDeclaration(ASTFunction* function);
    void evaluateFunctionCall(ASTFunctionCall* functionCall);
    void evaluatePrint(ASTPrint* print);
};

template <typename T>
T calculate(T left_value, T right_value, const std::string& op);

template <>
std::string calculate<std::string>(std::string left_value, std::string right_value, const std::string& op);

#endif // EVALUATOR_H
