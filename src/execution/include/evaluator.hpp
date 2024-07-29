#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "../../parsing/include/ast.hpp"
#include "../../typechecker/include/checker.hpp"
#include "runtime.hpp"

class Evaluator {
public:
    Evaluator();

    RuntimeEnvironment env;
    void evaluate(ASTBase* ast);
private:
    TypeChecker typeChecker;
    std::string evaluateExpression(ASTBase* expression);
    void evaluateVariableDeclaration(ASTVariableDeclaration* variable);
    void evaluateVariableModify(ASTVariableModify* variableModify);
    void evaluateProgram(ASTProgram* program);
    void evaluateFunctionDeclaration(ASTFunction* function);
    void evaluateFunctionCall(ASTFunctionCall* functionCall);
    void evaluatePrint(ASTPrint* print);
};

template <typename T>
T calculate(const T& left_value, const T& right_value, const std::string& op);

template <>
std::string calculate<std::string>(const std::string& left_value, const std::string& right_value, const std::string& op);

#endif // EVALUATOR_H
