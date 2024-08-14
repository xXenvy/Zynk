#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "../../parsing/include/ast.hpp"
#include "../../parsing/include/lexer.hpp"
#include "../../parsing/include/parser.hpp"
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
    std::string evaluateExpression(const std::string& expression, size_t line);
    std::string evaluateReadLine(ASTReadLine* read);
    std::string evaluateTypeCast(ASTTypeCast* typeCast);
    std::string evaluateFString(ASTFString* fString);
    std::string evaluateBinaryOperation(ASTBinaryOperation* operation);
    std::string evaluateAndOperation(ASTAndOperation* operation);
    std::string evaluateOrOperation(ASTOrOperation* operation);

    inline void evaluateVariableDeclaration(ASTVariableDeclaration* variable);
    inline void evaluateVariableModify(ASTVariableModify* variableModify);
    inline void evaluateProgram(ASTProgram* program);
    inline void evaluateFunctionDeclaration(ASTFunction* function);
    inline void evaluateFunctionCall(ASTFunctionCall* functionCall);
    inline void evaluatePrint(ASTPrint* print);
    inline void evaluateCondition(ASTCondition* condition);
};

inline bool stringToBool(const std::string& value);
std::string calculate(const float left, const float right, const std::string& op);
std::string calculateString(const std::string& left_value, const std::string& right_value, const std::string& op);

#endif // EVALUATOR_H
