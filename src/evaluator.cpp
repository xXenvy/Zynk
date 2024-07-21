#include "include/evaluator.hpp"
#include "include/errors.hpp"
#include <memory>

Evaluator::Evaluator(RuntimeEnvironment& env) : env(env) {};

void Evaluator::evaluate(const std::shared_ptr<ASTBase> ast) {
    switch (ast->type) {
        case ASTType::Program:
            evaluateProgram(std::dynamic_pointer_cast<ASTProgram>(ast));
            break;
        case ASTType::FunctionDeclaration:
            evaluateFunctionDeclaration(std::dynamic_pointer_cast<ASTFunction>(ast));
            break;
        case ASTType::FunctionCall:
            evaluateFunctionCall(std::dynamic_pointer_cast<ASTFunctionCall>(ast));
            break;
        case ASTType::VariableDeclaration:
            evaluateVariableDeclaration(std::dynamic_pointer_cast<ASTVariableDeclaration>(ast));
            break;
        case ASTType::VariableModify:
            evaluateVariableModify(std::dynamic_pointer_cast<ASTVariableModify>(ast));
            break;
        case ASTType::Print:
            evaluatePrint(std::dynamic_pointer_cast<ASTPrint>(ast));
            break;
        default:
            throw ZynkError{ ZynkErrorType::RuntimeError, "Unknown AST type." };
    }
}

void Evaluator::evaluateProgram(const std::shared_ptr<ASTProgram> program) {
    for (const std::shared_ptr<ASTBase> child : program->body) {
        evaluate(child);
    }
}

void Evaluator::evaluateFunctionDeclaration(const std::shared_ptr<ASTFunction> function) {
    env.declareFunction(function->name, function);
}

void Evaluator::evaluateFunctionCall(std::shared_ptr<ASTFunctionCall> functionCall) {
    const auto func = std::dynamic_pointer_cast<ASTFunction>(env.getFunction(functionCall->name));
    for (std::shared_ptr<ASTBase> child : func->body) {
        evaluate(child);
    }
}

void Evaluator::evaluatePrint(std::shared_ptr<ASTPrint> print) {
    const std::string value = evaluateExpression(print->expression);
    std::cout << value << (print->newLine ? "\n" : "");
}

void Evaluator::evaluateVariableDeclaration(const std::shared_ptr<ASTVariableDeclaration> declaration) {
    env.declareVariable(declaration->name, declaration);
}

void Evaluator::evaluateVariableModify(const std::shared_ptr<ASTVariableModify> variableModify) {
    const auto declaration = env.getVariable(variableModify->name);
    declaration->value = variableModify->value;
}

std::string Evaluator::evaluateExpression(const std::shared_ptr<ASTBase> expression) {
    switch (expression->type) {
        case ASTType::Value:
            return std::dynamic_pointer_cast<ASTValue>(expression)->value;
        case ASTType::Variable: {
            const auto var = std::dynamic_pointer_cast<ASTVariable>(expression);
            return evaluateExpression(env.getVariable(var->name)->value);
        };
        case ASTType::BinaryOperation: {
            const auto operation = std::dynamic_pointer_cast<ASTBinaryOperation>(expression);
            const std::string left = evaluateExpression(operation->left);
            const std::string right = evaluateExpression(operation->right);
            try {
                return calculate<std::string>(left, right, operation->op);
            } catch (const std::invalid_argument) {
                throw ZynkError{ 
                    ZynkErrorType::ExpressionError, 
                    "Invalid expression. Cannot perform BinaryOperation on that type."
                };
            }
        };
        default:
            throw ZynkError{ZynkErrorType::RuntimeError, "Invalid expression."};
    }
}

size_t Evaluator::variablesCount() const {
    return env.size();
}

template <typename T>
T calculate(const T& left, const T& right, const std::string& op) {
    if (op == "*") return left * right;
    if (op == "/") return left / right;
    if (op == "-") return left - right;
    if (op == "+") return left + right;
    throw ZynkError{ZynkErrorType::RuntimeError, "Invalid operator: " + op + "."};
}

template <>
std::string calculate<std::string>(const std::string& left_value, const std::string& right_value, const std::string& op) {
    const bool leftIsFloat = left_value.find('.') != std::string::npos;
    const bool rightIsFloat = right_value.find('.') != std::string::npos;

    if (leftIsFloat || rightIsFloat) {
        const float left = std::stof(left_value);
        const float right = std::stof(right_value);
        return std::to_string(calculate(left, right, op));
    }
    const int left = std::stoi(left_value);
    const int right = std::stoi(right_value);
    return std::to_string(calculate(left, right, op));
}