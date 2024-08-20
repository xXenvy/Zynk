#include "../errors/include/errors.hpp"
#include "include/evaluator.hpp"

#include <memory>
#include <cassert>
#include <unordered_map>

Evaluator::Evaluator() : typeChecker(env) {};

void Evaluator::evaluate(std::unique_ptr<ASTBase> ast) {
    assert(ast != nullptr && "Ast should not be nullptr");

    switch (ast->type) {
        case ASTType::Program: {
            std::unique_ptr<ASTProgram> programPtr(
                static_cast<ASTProgram*>(ast.release())
            );
            evaluateProgram(std::move(programPtr));
            break;
        }
        case ASTType::FunctionDeclaration: {
            std::unique_ptr<ASTFunction> funcDeclPtr(
                static_cast<ASTFunction*>(ast.release())
            );
            evaluateFunctionDeclaration(std::move(funcDeclPtr));
            break;
        }
        case ASTType::FunctionCall: {
            std::unique_ptr<ASTFunctionCall> funcCallPtr(
                static_cast<ASTFunctionCall*>(ast.release())
            );
            evaluateFunctionCall(std::move(funcCallPtr));
            break;
        }
        case ASTType::VariableDeclaration: {
            std::unique_ptr<ASTVariableDeclaration> varDeclPtr(
                static_cast<ASTVariableDeclaration*>(ast.release())
            );
            evaluateVariableDeclaration(std::move(varDeclPtr));
            break;
        }
        case ASTType::VariableModify: {
            std::unique_ptr<ASTVariableModify> varModPtr(
                static_cast<ASTVariableModify*>(ast.release())
            );
            evaluateVariableModify(std::move(varModPtr));
            break;
        }
        case ASTType::Print: {
            std::unique_ptr<ASTPrint> printPtr(
                static_cast<ASTPrint*>(ast.release())
            );
            evaluatePrint(std::move(printPtr));
            break;
        }
        case ASTType::ReadInput: {
            std::unique_ptr<ASTReadInput> readInputPtr(
                static_cast<ASTReadInput*>(ast.release())
            );
            evaluateReadInput(std::move(readInputPtr));
            break;
        }
        case ASTType::Condition: {
            std::unique_ptr<ASTCondition> conditionPtr(
                static_cast<ASTCondition*>(ast.release())
            );
            evaluateCondition(std::move(conditionPtr));
            break;
        }
        case ASTType::While: {
            std::unique_ptr<ASTWhile> whilePtr(
                static_cast<ASTWhile*>(ast.release())
            );
            evaluateWhile(std::move(whilePtr));
            break;
        }
        case ASTType::Variable:
        case ASTType::Value: {
            evaluateExpression(std::move(ast));
            break;
        }
        default:
            throw std::runtime_error("Unknown AST type encountered during evaluation.");
    }
}

inline void Evaluator::evaluateProgram(std::unique_ptr<ASTProgram> program) {
    env.enterNewBlock(); // Main program code block.
    for (std::unique_ptr<ASTBase>& child : program->body) {
        if (child != nullptr) evaluate(std::move(child));
    }
    env.exitCurrentBlock();
}

inline void Evaluator::evaluateFunctionDeclaration(std::unique_ptr<ASTFunction> function) {
    env.declareFunction(std::move(function));
}

inline void Evaluator::evaluatePrint(std::unique_ptr<ASTPrint> print) {
    const std::string value = evaluateExpression(std::move(print->expression));
    std::cout << value << (print->newLine ? "\n" : "");
}

inline void Evaluator::evaluateVariableDeclaration(std::unique_ptr<ASTVariableDeclaration> declaration) {
    if (declaration->value.get() != nullptr) {
        typeChecker.checkType(declaration->varType, declaration->value.get());

        std::unique_ptr<ASTValue> varValue = std::make_unique<ASTValue>(
            evaluateExpression(std::move(declaration->value)),
            declaration->varType,
            declaration->line
        );
        env.declareVariable(declaration->name, std::move(varValue));
        return;
    }
    // nullptr is here, because it allows to declare a variable, without specifying a value. 
    // The variable in that time will be `null`.
    env.declareVariable(declaration->name, nullptr);
}

inline void Evaluator::evaluateVariableModify(std::unique_ptr<ASTVariableModify> variableModify) {
    ASTValue* variable = env.getVariable(variableModify->name, variableModify->line, true);

    typeChecker.checkType(variable->valueType, variableModify->value.get());
    variable->value = evaluateExpression(std::move(variableModify->value));
}

std::string Evaluator::evaluateReadInput(std::unique_ptr<ASTReadInput> read) {
    std::string input;
    if (read->out != nullptr) {
        std::cout << evaluateExpression(std::move(read->out));
    }
    std::getline(std::cin, input);
    return input;
}

std::string Evaluator::evaluateFString(std::unique_ptr<ASTFString> fString) {
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

std::string Evaluator::evaluateTypeCast(std::unique_ptr<ASTTypeCast> typeCast) {
    std::string base = evaluateExpression(std::move(typeCast->value));

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

std::string Evaluator::evaluateBinaryOperation(std::unique_ptr<ASTBinaryOperation> operation) {
    const ASTValueType valueTypes[2] = {
        typeChecker.determineType(operation->left.get()),
        typeChecker.determineType(operation->right.get())
    };

    for (const ASTValueType& valueType : valueTypes) {
        if (valueType != ASTValueType::Integer && valueType != ASTValueType::Float) {
            throw ZynkError(
                ZynkErrorType::ExpressionError,
                "Cannot perform BinaryOperation on '" + typeChecker.typeToString(valueType) + "' type.",
                operation->line
            );
        }
    }

    const std::string left = evaluateExpression(std::move(operation->left));
    const std::string right = evaluateExpression(std::move(operation->right));

    try {
        return calculateString(left, right, operation->op);
    } catch (const ZynkError& err) {
        throw ZynkError(err.base_type, err.what(), operation->line);
    }
}

std::string Evaluator::evaluateComparisonOperation(std::unique_ptr<ASTComparisonOperation> operation) {
    const std::string left = evaluateExpression(std::move(operation->left));
    const std::string right = evaluateExpression(std::move(operation->right));
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

std::unique_ptr<ASTBase> Evaluator::evaluateCondition(std::unique_ptr<ASTCondition> condition) {
    const bool status = stringToBool(evaluateExpression(std::move(condition->expression)));
    auto& body = status ? condition->body : condition->elseBody;
    
    env.enterNewBlock();
    for (std::unique_ptr<ASTBase>& child : body) {

        if (child->type == ASTType::Return) {
            ASTValueType resultType = typeChecker.determineType(child.get());
            const size_t childLine = child->line;
            std::string result = evaluateExpression(std::move(child));
            env.exitCurrentBlock();
            return std::make_unique<ASTValue>(result, resultType, childLine);
        }

        if (child->type == ASTType::Break) {
            env.exitCurrentBlock();
            return std::make_unique<ASTBreak>(child->line);
        }

        if (child->type == ASTType::Condition) {
            std::unique_ptr<ASTCondition> conditionPtr(
                static_cast<ASTCondition*>(child.release())
            );
            auto result = evaluateCondition(std::move(conditionPtr));
            if (result != nullptr) {
                env.exitCurrentBlock();
                return result;
            }
        }
        else evaluate(std::move(child));
    }
    env.exitCurrentBlock();
    return nullptr;
}

std::unique_ptr<ASTBase> Evaluator::evaluateWhile(std::unique_ptr<ASTWhile> loop) {
    env.enterNewBlock();
    bool shouldContinue = true;

    while (stringToBool(evaluateExpression(loop->value->clone())) && shouldContinue) {

        for (std::unique_ptr<ASTBase>& child : loop->body) {
            if (child == nullptr) continue;

            if (child->type == ASTType::Break) {
                shouldContinue = false;
                break;
            }

            if (child->type == ASTType::Return) {
                ASTValueType resultType = typeChecker.determineType(child.get());
                std::string result = evaluateExpression(std::move(child->clone()));
                env.exitCurrentBlock();
                return std::make_unique<ASTValue>(result, resultType, child->line);
            }

            if (child->type == ASTType::Condition) {
                std::unique_ptr<ASTCondition> conditionPtr(
                    static_cast<ASTCondition*>(child->clone().release())
                );
                auto maybeResult = evaluateCondition(std::move(conditionPtr));
                if (maybeResult == nullptr) continue;

                if (maybeResult->type == ASTType::Break) {
                    shouldContinue = false;
                    break;
                }
                if (maybeResult->type == ASTType::Value) {
                    env.exitCurrentBlock();
                    return maybeResult;
                }
            }
            evaluate(std::move(child->clone()));
        }
    }
    env.exitCurrentBlock();
    return nullptr;
}

std::string Evaluator::evaluateFunctionCall(std::unique_ptr<ASTFunctionCall> functionCall) {
    ASTFunction* func = env.getFunction(functionCall->name, functionCall->line);

    if (func->arguments.size() != functionCall->arguments.size()) {
        throw ZynkError(
            ZynkErrorType::RuntimeError,
            "Invalid number of arguments for function '" + functionCall->name + "'.",
            functionCall->line
        );
    }

    if (env.isRecursionDepthExceeded()) {
        throw ZynkError(
            ZynkErrorType::RecursionError,
            "Exceeded maximum recursion depth of " + std::to_string(env.MAX_DEPTH) + ".",
            functionCall->line
        );
    }

    std::unordered_map<std::string, std::unique_ptr<ASTValue>> functionArgs;

    for (size_t i = 0; i < func->arguments.size(); ++i) {
        std::unique_ptr<ASTBase> funcCallArg = std::move(functionCall->arguments[i]);
        auto funcArg = static_cast<ASTFunctionArgument*>(func->arguments[i].get());

        typeChecker.checkType(funcArg->valueType, funcCallArg.get());

        functionArgs.insert({
                funcArg->name,
                std::make_unique<ASTValue>(
                    evaluateExpression(std::move(funcCallArg)),
                    funcArg->valueType,
                    funcArg->line)
            }
        );
    }

    env.enterNewBlock(true);
    for (auto& argPair : functionArgs) {
        env.declareVariable(argPair.first, std::move(argPair.second));
    }
    std::string result = "null";

    for (std::unique_ptr<ASTBase>& child : func->body) {
        if (child == nullptr) continue;

        switch (child->type) {
            case ASTType::Return: {
                typeChecker.checkType(func, child.get());
                result = evaluateExpression(std::move(child->clone()));
                env.exitCurrentBlock(true);
                return result;
            }

            case ASTType::Condition: {
                std::unique_ptr<ASTCondition> conditionPtr(
                    static_cast<ASTCondition*>(child->clone().release())
                );
                auto maybeResult = evaluateCondition(std::move(conditionPtr));

                if (maybeResult != nullptr && maybeResult->type == ASTType::Value) {
                    typeChecker.checkType(func, maybeResult.get());
                    result = static_cast<ASTValue*>(maybeResult.get())->value;
                    env.exitCurrentBlock(true);
                    return result;
                }
                break;
            }

            case ASTType::While: {
                std::unique_ptr<ASTWhile> whilePtr(
                    static_cast<ASTWhile*>(child->clone().release())
                );
                auto maybeResult = evaluateWhile(std::move(whilePtr));

                if (maybeResult != nullptr && maybeResult->type == ASTType::Value) {
                    typeChecker.checkType(func, maybeResult.get());
                    result = static_cast<ASTValue*>(maybeResult.get())->value;
                    env.exitCurrentBlock(true);
                    return result;
                }
                break;
            }
            default:
                evaluate(std::move(child->clone()));
                break;
        }
    }
    env.exitCurrentBlock(true);

    if (result == "null" && func->returnType != ASTValueType::None) {
        throw ZynkError(
            ZynkErrorType::TypeError,
            "Function '" + functionCall->name + "' does not return a value of type " 
            + typeChecker.typeToString(func->returnType) + " in all control paths.",
            func->line
        );
    }
    return result;
}

std::string Evaluator::evaluateOrOperation(std::unique_ptr<ASTOrOperation> operation) {
    const std::string left = evaluateExpression(std::move(operation->left));
    if (stringToBool(left)) return left;
    return evaluateExpression(std::move(operation->right));
}

std::string Evaluator::evaluateAndOperation(std::unique_ptr<ASTAndOperation> operation) {
    const std::string left = evaluateExpression(std::move(operation->left));
    if (!stringToBool(left)) return left;
    return evaluateExpression(std::move(operation->right));
}

std::string Evaluator::evaluateExpression(const std::string& expression, size_t realLine) {
    Lexer lexer(expression);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    std::unique_ptr<ASTBase> astExpression = parser.parseExpression(0);

    // We set the line number manually here, because the parser doesn't know where
    // this expression came from. Without this, line in error message would be wrong.
    astExpression.get()->line = realLine;
    return evaluateExpression(std::move(astExpression));
}

std::string Evaluator::evaluateExpression(std::unique_ptr<ASTBase> expression) {
    if (expression == nullptr) return "null";

    switch (expression->type) {
        case ASTType::Value:
            return static_cast<ASTValue*>(expression.get())->value;
        case ASTType::Variable: {
            const auto var = static_cast<ASTVariable*>(expression.get());
            return env.getVariable(var->name, var->line, true)->value;
        };
        case ASTType::ReadInput: {
            std::unique_ptr<ASTReadInput> readPtr(
                static_cast<ASTReadInput*>(expression.release())
            );
            return evaluateReadInput(std::move(readPtr));
        }
        case ASTType::TypeCast: {
            std::unique_ptr<ASTTypeCast> castPtr(
                static_cast<ASTTypeCast*>(expression.release())
            );
            return evaluateTypeCast(std::move(castPtr));
        }
        case ASTType::FString: {
            std::unique_ptr<ASTFString> fStringPtr(
                static_cast<ASTFString*>(expression.release())
            );
            return evaluateFString(std::move(fStringPtr));
        }
        case ASTType::BinaryOperation: {
            std::unique_ptr<ASTBinaryOperation> binaryOpPtr(
                static_cast<ASTBinaryOperation*>(expression.release())
            );
            return evaluateBinaryOperation(std::move(binaryOpPtr));
        }
        case ASTType::ComparisonOperation: {
            std::unique_ptr<ASTComparisonOperation> compOpPtr(
                static_cast<ASTComparisonOperation*>(expression.release())
            );
            return evaluateComparisonOperation(std::move(compOpPtr));
        }
        case ASTType::OrOperation: {
            std::unique_ptr<ASTOrOperation> orOpPtr(
                static_cast<ASTOrOperation*>(expression.release())
            );
            return evaluateOrOperation(std::move(orOpPtr));
        }
        case ASTType::AndOperation: {
            std::unique_ptr<ASTAndOperation> andOpPtr(
                static_cast<ASTAndOperation*>(expression.release())
            );
            return evaluateAndOperation(std::move(andOpPtr));
        }
        case ASTType::FunctionCall: {
            std::unique_ptr<ASTFunctionCall> funcCallPtr(
                static_cast<ASTFunctionCall*>(expression.release())
            );
            return evaluateFunctionCall(std::move(funcCallPtr));
        }
        case ASTType::Return: {
            std::unique_ptr<ASTReturn> returnPtr(
                static_cast<ASTReturn*>(expression.release())
            );
            return evaluateExpression(std::move(returnPtr->value));
        }
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
    // todo: refactor this crap
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