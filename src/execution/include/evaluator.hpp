#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "../../parsing/include/ast.hpp"
#include "../../parsing/include/lexer.hpp"
#include "../../parsing/include/parser.hpp"
#include "../typechecker/include/checker.hpp"
#include "runtime.hpp"

class Evaluator {
public:
    Evaluator();

    RuntimeEnvironment env;
    void evaluate(std::unique_ptr<ASTBase> ast);
private:
    TypeChecker typeChecker;

    std::string evaluateExpression(std::unique_ptr<ASTBase> expression);
    std::string evaluateExpression(const std::string& expression, size_t line);
    std::string evaluateReadInput(std::unique_ptr<ASTReadInput> read);
    std::string evaluateTypeCast(std::unique_ptr<ASTTypeCast> typeCast);
    std::string evaluateFString(std::unique_ptr<ASTFString> fString);
    std::string evaluateBinaryOperation(std::unique_ptr<ASTBinaryOperation> operation);
    std::string evaluateComparisonOperation(std::unique_ptr<ASTComparisonOperation> operation);
    std::string evaluateAndOperation(std::unique_ptr<ASTAndOperation> operation);
    std::string evaluateOrOperation(std::unique_ptr<ASTOrOperation> operation);
    std::string evaluateFunctionCall(std::unique_ptr<ASTFunctionCall> functionCall);

    std::unique_ptr<ASTBase> evaluateCondition(std::unique_ptr<ASTCondition> condition);
    std::unique_ptr<ASTBase> evaluateWhile(std::unique_ptr<ASTWhile> loop);

    inline void evaluateVariableDeclaration(std::unique_ptr<ASTVariableDeclaration> variable);
    inline void evaluateVariableModify(std::unique_ptr<ASTVariableModify> variableModify);
    inline void evaluateProgram(std::unique_ptr<ASTProgram> program);
    inline void evaluateFunctionDeclaration(std::unique_ptr<ASTFunction> function);
    inline void evaluatePrint(std::unique_ptr<ASTPrint> print);
};

inline bool stringToBool(const std::string& value);
std::string calculate(const float left, const float right, const std::string& op);
std::string calculateString(const std::string& left_value, const std::string& right_value, const std::string& op);

#endif // EVALUATOR_H