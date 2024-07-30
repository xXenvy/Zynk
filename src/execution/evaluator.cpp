#include "../common/include/errors.hpp"
#include "include/evaluator.hpp"
#include <memory>
#include <cassert>


void Evaluator::evaluate(ASTBase* ast) {
    assert(ast != nullptr && "Ast should not be nullptr");

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
        case ASTType::VariableModify:
            evaluateVariableModify(static_cast<ASTVariableModify*>(ast));
            break;
        case ASTType::Print:
            evaluatePrint(static_cast<ASTPrint*>(ast));
            break;
        case ASTType::Condition:
            evaluateCondition(static_cast<ASTCondition*>(ast));
            break;
        default:
            throw ZynkError{ ZynkErrorType::RuntimeError, "Unknown AST type." };
    }
}

void Evaluator::evaluateProgram(ASTProgram* program) {
    env.enterNewBlock(); // Main program code block.
    for (const std::unique_ptr<ASTBase>& child : program->body) {
        evaluate(child.get());
    }
    env.exitCurrentBlock(); // We need to do that, cuz gc need to free memory on main block.
}

void Evaluator::evaluateFunctionDeclaration(ASTFunction* function) {
    env.declareFunction(function->name, function);
}

void Evaluator::evaluateFunctionCall(ASTFunctionCall* functionCall) {
    ASTFunction* func = env.getFunction(functionCall->name);
    env.enterNewBlock();
    for (const std::unique_ptr<ASTBase>& child : func->body) {
        evaluate(child.get());
    }
    env.exitCurrentBlock();
}

void Evaluator::evaluatePrint(ASTPrint* print) {
    const std::string value = evaluateExpression(print->expression.get());
    std::cout << value << (print->newLine ? "\n" : "");
}

void Evaluator::evaluateVariableDeclaration(ASTVariableDeclaration* declaration) {
    env.declareVariable(declaration->name, declaration);
}

void Evaluator::evaluateVariableModify(ASTVariableModify* variableModify) {
    ASTVariableDeclaration* declaration = env.getVariable(variableModify->name);
    ASTValue* newValue = new ASTValue(evaluateExpression(variableModify->value.get()));
    // We need to calculate the new value of the variable already at this point.
    declaration->value.reset(newValue);
}

std::string Evaluator::evaluateExpression(ASTBase* expression) {
    if (expression == nullptr) return "null";
    switch (expression->type) {
        case ASTType::Value:
            return static_cast<ASTValue*>(expression)->value;
        case ASTType::Variable: {
            const auto var = static_cast<ASTVariable*>(expression);
            return evaluateExpression(env.getVariable(var->name)->value.get());
        };
        case ASTType::BinaryOperation: {
            const auto operation = static_cast<ASTBinaryOperation*>(expression);
            const std::string left = evaluateExpression(operation->left.get());
            const std::string right = evaluateExpression(operation->right.get());
            std::cout << left << " : " << right << std::endl;
            try {
                return calculateString(left, right, operation->op);
            } catch (const std::invalid_argument&) {
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

void Evaluator::evaluateCondition(ASTCondition* condition) {
    const std::string value = evaluateExpression(condition->expression.get());
    bool result = true;
    if (value == "0" || value.empty() || value == "null" || value == "false") result = false;
    if (result) {
        env.enterNewBlock();
        for (const std::unique_ptr<ASTBase>& child : condition->body) {
            evaluate(child.get());
        }
        env.exitCurrentBlock();
    } else {
        env.enterNewBlock();
        for (const std::unique_ptr<ASTBase>& child : condition->elseBody) {
            evaluate(child.get());
        }
        env.exitCurrentBlock();
    }
}

std::string calculate(const float left, const float right, const std::string& op) {
    if (op == "*") return std::to_string(left * right);
    if (op == "-") return std::to_string(left - right);
    if (op == "+") return std::to_string(left + right);
    if (op == "/") {
        if (right == 0) throw ZynkError{ ZynkErrorType::RuntimeError, "division by zero" };
        return std::to_string(left / right);
    }
    throw ZynkError{ ZynkErrorType::RuntimeError, "Invalid operator: " + op + "." };
}

std::string calculateString(const std::string& left_value, const std::string& right_value, const std::string& op) {
    // I don't like the way it's done, but I don't know how to do it now.
    if (op == ">") return left_value > right_value ? "true" : "false";
    if (op == ">=") return left_value >= right_value ? "true" : "false";
    if (op == "<") return left_value < right_value ? "true" : "false";
    if (op == "<=") return left_value <= right_value ? "true" : "false";
    if (op == "==") return left_value == right_value ? "true" : "false";
    if (op == "!=") return left_value != right_value ? "true" : "false";

    const bool leftIsFloat = left_value.find('.') != std::string::npos;
    const bool rightIsFloat = right_value.find('.') != std::string::npos;

    const float left = std::stof(left_value);
    const float right = std::stof(right_value);
    std::string result = calculate(left, right, op);

    if (leftIsFloat || rightIsFloat) {
        return result;
    }
    size_t dotPosition = result.find('.');
    if (dotPosition != std::string::npos) {
        result = result.substr(0, dotPosition);
    }
    return result;
}