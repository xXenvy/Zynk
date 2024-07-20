#include "include/evaluator.hpp"
#include "include/errors.hpp"

Evaluator::Evaluator(RuntimeEnvironment& env) : env(env) {};

void Evaluator::evaluate(ASTBase* ast) {
    switch (ast->type) {
        case ASTType::Program:
            evaluateProgram(static_cast<ASTProgram*>(ast));
            break;
        case ASTType::FunctionDeclaration:
            evaluateFunctionDeclaration(static_cast<ASTFunction*>(ast));
            break;
        case ASTType::FunctionCall:
            evaluateFunctionCall(static_cast<ASTFunctionCall*>(ast));
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

void Evaluator::evaluateFunctionDeclaration(ASTFunction* function) {
    env.declareFunction(function->name, function);
}

void Evaluator::evaluateFunctionCall(ASTFunctionCall* functionCall) {
    ASTFunction* func = static_cast<ASTFunction*>(env.getFunction(functionCall->name));
    for (ASTBase* child : func->body) {
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
            std::string left = evaluateExpression(operation->left);
            std::string right = evaluateExpression(operation->right);
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
T calculate(T left, T right, const std::string& op) {
    if (op == "*") return left * right;
    if (op == "/") return left / right;
    if (op == "-") return left - right;
    if (op == "+") return left + right;
    throw ZynkError{ZynkErrorType::RuntimeError, "Invalid operator: " + op + "."};
}

template <>
std::string calculate<std::string>(std::string left_value, std::string right_value, const std::string& op) {
    const bool leftIsFloat = left_value.find('.') != std::string::npos;
    const bool rightIsFloat = right_value.find('.') != std::string::npos;

    if (leftIsFloat || rightIsFloat) {
        float left = std::stof(left_value);
        float right = std::stof(right_value);
        return std::to_string(calculate(left, right, op));
    }
    int left = std::stoi(left_value);
    int right = std::stoi(right_value);
    return std::to_string(calculate(left, right, op));
}