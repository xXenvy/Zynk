#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "runtime.hpp"
#include "ast.hpp"
#include <string>

class Evaluator {
public:
    Evaluator(RuntimeEnvironment& env);
    void evaluate(const std::shared_ptr<ASTBase> ast);
    size_t variablesCount() const;
private:
    RuntimeEnvironment env;

    std::string evaluateExpression(const std::shared_ptr<ASTBase> expression);
    void evaluateVariableDeclaration(const std::shared_ptr<ASTVariableDeclaration> variable);
    void evaluateVariableModify(const std::shared_ptr<ASTVariableModify> variableModify);
    void evaluateProgram(const std::shared_ptr<ASTProgram> program);
    void evaluateFunctionDeclaration(const std::shared_ptr<ASTFunction> function);
    void evaluateFunctionCall(const std::shared_ptr<ASTFunctionCall> functionCall);
    void evaluatePrint(const std::shared_ptr<ASTPrint> print);
};

template <typename T>
T calculate(const T& left_value, const T& right_value, const std::string& op);

template <>
std::string calculate<std::string>(const std::string& left_value, const std::string& right_value, const std::string& op);

#endif // EVALUATOR_H
