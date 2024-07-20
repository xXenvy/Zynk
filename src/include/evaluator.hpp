#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "runtime.hpp"
#include "ast.hpp"
#include <string>

class Evaluator {
public:
    Evaluator(RuntimeEnvironment& env);
    void evaluate(ASTBase* ast);

private:
    RuntimeEnvironment env;

    std::string evaluateExpression(ASTBase* expression);

    void evaluateVariableDeclaration(ASTVariableDeclaration* variable);
    void evaluateProgram(ASTProgram* program);
    void evaluateFunction(ASTFunction* function);
    void evaluatePrint(ASTPrint* print);
};

#endif // EVALUATOR_H
