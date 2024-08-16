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
        case ASTType::ReadInput:
            evaluateReadInput(static_cast<ASTReadInput*>(ast));
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

inline void Evaluator::evaluateProgram(ASTProgram* program) {
    env.enterNewBlock(); // Main program code block.
    for (const std::unique_ptr<ASTBase>& child : program->body) {
        if(child.get() != nullptr) evaluate(child.get());
    }
    env.exitCurrentBlock(); // We need to do that, cuz gc need to free memory on main block.
}

inline void Evaluator::evaluateFunctionDeclaration(ASTFunction* function) {
    env.declareFunction(function->name, function);
}

inline void Evaluator::evaluateFunctionCall(ASTFunctionCall* functionCall) {
    ASTFunction* func = env.getFunction(functionCall->name, functionCall->line);

    if (env.isRecursionDepthExceeded()) {
        throw ZynkError{
            ZynkErrorType::RecursionError,
            "Exceeded maximum recursion depth of " + std::to_string(env.MAX_DEPTH) + ".",
            functionCall->line,
        };
    }

    env.enterNewBlock(true);
    for (const std::unique_ptr<ASTBase>& child : func->body) {
        evaluate(child.get());
    }
    env.exitCurrentBlock(true);
}

inline void Evaluator::evaluatePrint(ASTPrint* print) {
    const std::string value = evaluateExpression(print->expression.get());
    std::cout << value << (print->newLine ? "\n" : "");
}

inline void Evaluator::evaluateVariableDeclaration(ASTVariableDeclaration* declaration) {
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

inline void Evaluator::evaluateVariableModify(ASTVariableModify* variableModify) {
    ASTVariableDeclaration* declaration = env.getVariable(variableModify->name, variableModify->line, true);
    typeChecker.checkType(declaration->varType, variableModify->value.get());

    ASTValue* newValue = new ASTValue(
        evaluateExpression(variableModify->value.get()),
        declaration->varType,
        declaration->line
    );
    declaration->value.reset(newValue);
}

inline void Evaluator::evaluateCondition(ASTCondition* condition) {
    const std::string value = evaluateExpression(condition->expression.get());

    env.enterNewBlock();
    for (const std::unique_ptr<ASTBase>& child : stringToBool(value) ? condition->body : condition->elseBody) {
        evaluate(child.get());
    }
    env.exitCurrentBlock();
}

std::string Evaluator::evaluateReadInput(ASTReadInput* read) {
    std::string input;
    if (read->out.get() != nullptr) {
        std::cout << evaluateExpression(read->out.get());
    }
    std::getline(std::cin, input);
    return input;
}

std::string Evaluator::evaluateFString(ASTFString* fString) {
    std::string result;
    size_t start = 0;
    std::string value = fString->value;

    while (start < value.size()) {
        size_t braceOpen = value.find('{', start);
        if (braceOpen == std::string::npos) {
            // There is no further brackets.
            result += value.substr(start);
            break;
        }

        result += value.substr(start, braceOpen - start);
        size_t braceClose = value.find('}', braceOpen);
        if (braceClose == std::string::npos) {
            throw ZynkError(
                ZynkErrorType::RuntimeError,
                "Unclosed '{' in f-string.",
                fString->line
            );
        }

        std::string expression = value.substr(braceOpen + 1, braceClose - braceOpen - 1);
        result += evaluateExpression(expression, fString->line);
        start = braceClose + 1;
    }
    return result;
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
                    "Invalid argument. Unable to convert the provided value to an integer.",
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
            return stringToBool(base) ? "true" : "false";
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

std::string Evaluator::evaluateBinaryOperation(ASTBinaryOperation* operation) {
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
}

std::string Evaluator::evaluateComparisonOperation(ASTComparisonOperation* operation) {
    const std::string left = evaluateExpression(operation->left.get());
    const std::string right = evaluateExpression(operation->right.get());
    const std::string& op = operation->op;

    auto compare = [&](auto a, auto b) -> std::string {
        if (op == "==") return a == b ? "true" : "false";
        if (op == "!=") return a != b ? "true" : "false";
        if (op == ">") return a > b ? "true" : "false";
        if (op == ">=") return a >= b ? "true" : "false";
        if (op == "<") return a < b ? "true" : "false";
        if (op == "<=") return a <= b ? "true" : "false";
        throw ZynkError(
            ZynkErrorType::RuntimeError,
            "Invalid operator '" + op + "' in ComparisonOperation.",
            operation->line
        );
    };

    try {
        return compare(std::stof(left), std::stof(right));
    } catch (const std::invalid_argument&) {
        return compare(left, right);
    }
}

std::string Evaluator::evaluateOrOperation(ASTOrOperation* operation) {
    const std::string left = evaluateExpression(operation->left.get());
    if (stringToBool(left)) return left;
    return evaluateExpression(operation->right.get());
}

std::string Evaluator::evaluateAndOperation(ASTAndOperation* operation) {
    const std::string left = evaluateExpression(operation->left.get());
    if (!stringToBool(left)) return left;
    return evaluateExpression(operation->right.get());
}

std::string Evaluator::evaluateExpression(const std::string& expression, size_t realLine) {
    Lexer lexer(expression);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto astExpression = parser.parseExpression(0);

    // We set the line number manually here, because the parser doesn't know where
    // this expression came from. Without this, line in error message would be wrong.
    astExpression.get()->line = realLine;
    return evaluateExpression(astExpression.get());
}

std::string Evaluator::evaluateExpression(ASTBase* expression) {
    if (expression == nullptr) return "null";
    switch (expression->type) {
        case ASTType::Value:
            return static_cast<ASTValue*>(expression)->value;
        case ASTType::Variable: {
            const auto var = static_cast<ASTVariable*>(expression);
            return evaluateExpression(env.getVariable(var->name, var->line, true)->value.get());
        };
        case ASTType::ReadInput:
            return evaluateReadInput(static_cast<ASTReadInput*>(expression));
        case ASTType::TypeCast:
            return evaluateTypeCast(static_cast<ASTTypeCast*>(expression));
        case ASTType::FString:
            return evaluateFString(static_cast<ASTFString*>(expression));
        case ASTType::BinaryOperation:
            return evaluateBinaryOperation(static_cast<ASTBinaryOperation*>(expression));
        case ASTType::ComparisonOperation:
            return evaluateComparisonOperation(static_cast<ASTComparisonOperation*>(expression));
        case ASTType::OrOperation:
            return evaluateOrOperation(static_cast<ASTOrOperation*>(expression));
        case ASTType::AndOperation: 
            return evaluateAndOperation(static_cast<ASTAndOperation*>(expression));
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