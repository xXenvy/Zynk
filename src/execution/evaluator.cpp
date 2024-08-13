#include "../common/include/errors.hpp"
#include "include/evaluator.hpp"
#include <memory>
#include <cassert>

Evaluator::Evaluator() : typeChecker(env) {};

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
        case ASTType::ReadLine:
            evaluateReadLine(static_cast<ASTReadLine*>(ast));
            break;
        case ASTType::Condition:
            evaluateCondition(static_cast<ASTCondition*>(ast));
            break;
        default:
            throw ZynkError(
                ZynkErrorType::RuntimeError,
                "Unknown AST type encountered during evaluation.",
                ast->line
            );
    }
}

void Evaluator::evaluateProgram(ASTProgram* program) {
    env.enterNewBlock(); // Main program code block.
    for (const std::unique_ptr<ASTBase>& child : program->body) {
        if(child.get() != nullptr) evaluate(child.get());
    }
    env.exitCurrentBlock(); // We need to do that, cuz gc need to free memory on main block.
}

void Evaluator::evaluateFunctionDeclaration(ASTFunction* function) {
    env.declareFunction(function->name, function);
}

void Evaluator::evaluateFunctionCall(ASTFunctionCall* functionCall) {
    ASTFunction* func = env.getFunction(functionCall->name, functionCall->line);
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

std::string Evaluator::evaluateReadLine(ASTReadLine* read) {
    std::string input;
    if (read->out.get() != nullptr) {
        std::cout << evaluateExpression(read->out.get());
    }
    std::getline(std::cin, input);
    return input;
}

void Evaluator::evaluateVariableDeclaration(ASTVariableDeclaration* declaration) {
    if (declaration->value.get() != nullptr) {
        typeChecker.checkType(declaration->varType, declaration->value.get());
        ASTValue* varValue = new ASTValue(
            evaluateExpression(declaration->value.get()),
            declaration->varType,
            declaration->line
        );
        declaration->value.reset(varValue);
    }
    env.declareVariable(declaration->name, declaration);
}

void Evaluator::evaluateVariableModify(ASTVariableModify* variableModify) {
    ASTVariableDeclaration* declaration = env.getVariable(variableModify->name, variableModify->line);
    typeChecker.checkType(declaration->varType, variableModify->value.get());

    ASTValue* newValue = new ASTValue(
        evaluateExpression(variableModify->value.get()),
        declaration->varType,
        declaration->line
    );
    declaration->value.reset(newValue);
}

std::string Evaluator::evaluateTypeCast(ASTTypeCast* typeCast) {
    std::string base = evaluateExpression(typeCast->value.get());

    switch (typeCast->castType) {
        case ASTValueType::Integer:
            try {
                return std::to_string(std::stoi(base));
            } catch (const std::invalid_argument&) {
                throw ZynkError(
                    ZynkErrorType::TypeCastError, 
                    "Invalid argument: unable to convert the provided value to an integer.",
                    typeCast->line
                );
            }
        case ASTValueType::Float:
            try {
                return std::to_string(std::stof(base));
            } catch (const std::invalid_argument&) {
                throw ZynkError(
                    ZynkErrorType::TypeCastError,
                    "Invalid argument. Unable to convert the provided value to an float.",
                    typeCast->line
                );
            }
        case ASTValueType::String:
            return base; // Expressions by default are always strings.
        case ASTValueType::Bool: {
            if (!stringToBool(base)) {
                return "false";
            }
            return "true";
        }
        default: {
            // This should never happen, but whatever
            throw ZynkError(
                ZynkErrorType::RuntimeError, 
                "Invalid type cast encountered.",
                typeCast->line
            );
        }
    }
}

void Evaluator::evaluateCondition(ASTCondition* condition) {
    const std::string value = evaluateExpression(condition->expression.get());

    env.enterNewBlock();
    for (const std::unique_ptr<ASTBase>& child : stringToBool(value) ? condition->body : condition->elseBody) {
        evaluate(child.get());
    }
    env.exitCurrentBlock();
}

std::string Evaluator::evaluateExpression(ASTBase* expression) {
    if (expression == nullptr) return "null";
    switch (expression->type) {
        case ASTType::Value:
            return static_cast<ASTValue*>(expression)->value;
        case ASTType::Variable: {
            const auto var = static_cast<ASTVariable*>(expression);
            return evaluateExpression(env.getVariable(var->name, var->line)->value.get());
        };
        case ASTType::ReadLine: {
            return evaluateReadLine(static_cast<ASTReadLine*>(expression));
        };
        case ASTType::TypeCast: {
            return evaluateTypeCast(static_cast<ASTTypeCast*>(expression));
        };
        case ASTType::BinaryOperation: {
            const auto operation = static_cast<ASTBinaryOperation*>(expression);
            ASTValueType valueTypes[2] = {
                typeChecker.determineType(operation->left.get()),
                typeChecker.determineType(operation->right.get())
            };
            for (ASTValueType& valueType : valueTypes) {
                if (valueType != ASTValueType::Integer && valueType != ASTValueType::Float) {
                    throw ZynkError(
                        ZynkErrorType::ExpressionError,
                        "Cannot perform BinaryOperation on '" + typeChecker.typeToString(valueType) + "' type.",
                        operation->line
                    );
                }
            }
            const std::string left = evaluateExpression(operation->left.get());
            const std::string right = evaluateExpression(operation->right.get());
            try {
                return calculateString(left, right, operation->op);
            } catch (const ZynkError& err) {
                throw ZynkError(err.base_type, err.what(), operation->line);
            }
        };
        case ASTType::OrOperation: {
            const auto operation = static_cast<ASTOrOperation*>(expression);
            const std::string left = evaluateExpression(operation->left.get());
            const std::string right = evaluateExpression(operation->right.get());
            if (stringToBool(left)) return left;
            return right;
        };
        case ASTType::AndOperation: {
            const auto operation = static_cast<ASTAndOperation*>(expression);
            const std::string left = evaluateExpression(operation->left.get());
            const std::string right = evaluateExpression(operation->right.get());
            if (!stringToBool(left)) return left;
            return right;
        };
        default:
            throw ZynkError(
                ZynkErrorType::RuntimeError,
                "Invalid expression type encountered during evaluation.",
                expression->line
            );
    }
}

std::string calculate(const float left, const float right, const std::string& op) {
    if (op == "*") return std::to_string(left * right);
    if (op == "-") return std::to_string(left - right);
    if (op == "+") return std::to_string(left + right);
    if (op == "/") {
        if (right == 0) throw ZynkError(ZynkErrorType::RuntimeError, "Division by zero." );
        return std::to_string(left / right);
    }
    throw ZynkError(ZynkErrorType::RuntimeError, "Invalid operator: " + op + ".");
}

std::string calculateString(const std::string& left_value, const std::string& right_value, const std::string& op) {
    // I don't like the way it's done, but I don't know how to do it better right now.
    if (op == ">") return left_value > right_value ? "true" : "false";
    if (op == ">=") return left_value >= right_value ? "true" : "false";
    if (op == "<") return left_value < right_value ? "true" : "false";
    if (op == "<=") return left_value <= right_value ? "true" : "false";
    if (op == "==") return left_value == right_value ? "true" : "false";
    if (op == "!=") return left_value != right_value ? "true" : "false";
    if (op == "or") {
        return (stringToBool(left_value) == true || stringToBool(right_value) == true) ? "true" : "false";
    }
    if (op == "and") {
        return (stringToBool(left_value) == true && stringToBool(right_value) == true) ? "true" : "false";
    }

    const bool leftIsFloat = left_value.find('.') != std::string::npos;
    const bool rightIsFloat = right_value.find('.') != std::string::npos;

    const float left = std::stof(left_value);
    const float right = std::stof(right_value);
    std::string result = calculate(left, right, op);

    if (leftIsFloat || rightIsFloat) return result;

    size_t dotPosition = result.find('.');
    if (dotPosition != std::string::npos) {
        result = result.substr(0, dotPosition);
    }
    return result;
}

inline bool stringToBool(const std::string& value) {
    return value != "0" && !value.empty() && value != "null" && value != "false";
}