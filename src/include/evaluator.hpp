#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "runtime.hpp"
#include "ast.hpp"
#include <string>

class Evaluator {
public:
    Evaluator(RuntimeEnvironment& env);
    void evaluate(std::shared_ptr<ASTBase> ast);
    size_t variablesCount() const;
private:
    RuntimeEnvironment env;

    std::string evaluateExpression(std::shared_ptr<ASTBase> expression);
    void evaluateVariableDeclaration(std::shared_ptr<ASTVariableDeclaration> variable);
    void evaluateVariableModify(std::shared_ptr<ASTVariableModify> variableModify);
    void evaluateProgram(std::shared_ptr<ASTProgram> program);
    void evaluateFunctionDeclaration(std::shared_ptr<ASTFunction> function);
    void evaluateFunctionCall(std::shared_ptr<ASTFunctionCall> functionCall);
    void evaluatePrint(std::shared_ptr<ASTPrint> print);
};

template <typename T>
T calculate(T left_value, T right_value, const std::string& op);

template <>
std::string calculate<std::string>(std::string left_value, std::string right_value, const std::string& op);

#endif // EVALUATOR_H
