#include "include/evaluator.hpp"
#include "include/errors.hpp"

Evaluator::Evaluator(RuntimeEnvironment& env) : env(env) {};

void Evaluator::evaluate(ASTBase* ast) {
    switch (ast->type) {
        case ASTType::Program:
            evaluateProgram(static_cast<ASTProgram*>(ast));
            break;
        case ASTType::Function:
            evaluateFunction(static_cast<ASTFunction*>(ast));
            break;
        case ASTType::VariableDeclaration:
            evaluateVariableDeclaration(static_cast<ASTVariableDeclaration*>(ast));
            break;
        case ASTType::Print:
            evaluatePrint(static_cast<ASTPrint*>(ast));
            break;
        default:
            throw ZynkError{ ZynkErrorType::RuntimeError, "Unknown AST type." };
    }
}

void Evaluator::evaluateProgram(ASTProgram* program) {
    for (ASTBase* child : program->body) {
        evaluate(child);
    }
}

void Evaluator::evaluateFunction(ASTFunction* function) {
    for (ASTBase* child : function->body) {
        evaluate(child);
    }
}

void Evaluator::evaluatePrint(ASTPrint* print) {
    const std::string value = evaluateExpression(print->expression);
    std::cout << value << (print->newLine ? "\n" : "");
}

void Evaluator::evaluateVariableDeclaration(ASTVariableDeclaration* declaration) {
    env.declareVariable(declaration->name, declaration);
}

std::string Evaluator::evaluateExpression(ASTBase* expression) {
    switch (expression->type) {
        case ASTType::Value:
            return static_cast<ASTValue*>(expression)->value;
        case ASTType::Variable: {
            const ASTVariable* var = static_cast<ASTVariable*>(expression);
            return evaluateExpression(env.getVariable(var->name)->value);
        };
        case ASTType::BinaryOperation: {
            ASTBinaryOperation* operation = static_cast<ASTBinaryOperation*>(expression);

            const int left_value = std::stoi(evaluateExpression(operation->left));
            const int right_value = std::stoi(evaluateExpression(operation->right));
            if (operation->op == "*") return std::to_string(left_value * right_value);
            if (operation->op == "/") return std::to_string(left_value / right_value);
            if (operation->op == "-") return std::to_string(left_value - right_value);
            return std::to_string(left_value + right_value);
        };
        default:
            throw std::runtime_error("Unsupported expression type");
    }
}