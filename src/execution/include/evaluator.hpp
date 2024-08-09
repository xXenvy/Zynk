#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "../../parsing/include/ast.hpp"
#include "runtime.hpp"

class Evaluator {
public:
    RuntimeEnvironment env;
    void evaluate(ASTBase* ast);
private:
    std::string evaluateExpression(ASTBase* expression);
    void evaluateVariableDeclaration(ASTVariableDeclaration* variable);
    void evaluateVariableModify(ASTVariableModify* variableModify);
    void evaluateProgram(ASTProgram* program);
    void evaluateFunctionDeclaration(ASTFunction* function);
    void evaluateFunctionCall(ASTFunctionCall* functionCall);
    void evaluatePrint(ASTPrint* print);
    void evaluateCondition(ASTCondition* condition);
};

std::string calculate(const float left, const float right, const std::string& op);
std::string calculateString(const std::string& left_value, const std::string& right_value, const std::string& op);

#endif // EVALUATOR_H
