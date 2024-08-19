#include "include/checker.hpp"
#include "../../errors/include/errors.hpp"

TypeChecker::TypeChecker(RuntimeEnvironment& env) : env(env) {};

ASTValueType TypeChecker::determineType(ASTBase* expression) {
    if (expression == nullptr) return ASTValueType::None;
    switch (expression->type) {
        case ASTType::TypeCast:
            return static_cast<ASTTypeCast*>(expression)->castType;
        case ASTType::Value: 
            return static_cast<ASTValue*>(expression)->valueType;
        case ASTType::Return:
            return determineType(static_cast<ASTReturn*>(expression)->value.get());
        case ASTType::ComparisonOperation:
            return ASTValueType::Bool;
        case ASTType::FString:
        case ASTType::ReadInput:
            return ASTValueType::String;
        case ASTType::FunctionCall: {
            ASTFunctionCall* funcCall = static_cast<ASTFunctionCall*>(expression);
            ASTFunction* func = env.getFunction(funcCall->name, expression->line);
            return func->returnType;
        }
        case ASTType::Variable: {
            ASTVariable* var = static_cast<ASTVariable*>(expression);
            ASTValue* varValue = env.getVariable(var->name, var->line, true);
            return determineType(varValue);
        }
        case ASTType::BinaryOperation: {
            ASTBinaryOperation* operation = static_cast<ASTBinaryOperation*>(expression);
            ASTValueType leftType = determineType(operation->left.get());
            ASTValueType rightType = determineType(operation->right.get());

            if (!isNumber(leftType)) return leftType;
            if (!isNumber(rightType)) return rightType;
            if (leftType == ASTValueType::Float || rightType == ASTValueType::Float) {
                return ASTValueType::Float;
            }
            return ASTValueType::Integer;
        }
        case ASTType::OrOperation: {
            ASTOrOperation* operation = static_cast<ASTOrOperation*>(expression);
            ASTValueType leftType = determineType(operation->left.get());
            ASTValueType rightType = determineType(operation->right.get());
            if (leftType != rightType) {
                throw ZynkError(
                    ZynkErrorType::TypeError,
                    "Operands of the 'or' operation in variable declarations must be of the same type.",
                    operation->line
                );
            }
            return leftType;
        }
        case ASTType::AndOperation: {
            ASTAndOperation* operation = static_cast<ASTAndOperation*>(expression);
            ASTValueType leftType = determineType(operation->left.get());
            ASTValueType rightType = determineType(operation->right.get());
            if (leftType != rightType) {
                throw ZynkError(
                    ZynkErrorType::TypeError,
                    "Operands of the 'and' operation in variable declarations must be of the same type.",
                    operation->line
                );
            }
            return leftType;
        }
        default:
            throw ZynkError(
                ZynkErrorType::TypeError, 
                "Cannot determine type for given AST type.",
                expression->line
            );
        }
}

void TypeChecker::checkType(const ASTValueType& declared, ASTBase* value) {
    ASTValueType valueType = determineType(value);
    if (declared != valueType) {
        throw ZynkError(
            ZynkErrorType::TypeError,
            "Type mismatch. Declared type is " + typeToString(declared) + 
            ", but assigned value is of type " + typeToString(valueType) + ".",
            value->line
        );
    }
}

void TypeChecker::checkType(ASTFunction* func, ASTBase* value) {
    ASTValueType returnType = determineType(value);
    if (returnType != func->returnType) {
        throw ZynkError(
            ZynkErrorType::TypeError,
            "Function '" + func->name + "' does not return a value of type " +
            typeToString(func->returnType) + ". Instead, it returned " + typeToString(returnType) + " type.",
            value->line
        );
    }
}

bool TypeChecker::isNumber(const ASTValueType& type) {
    return type == ASTValueType::Float || type == ASTValueType::Integer;
}

std::string TypeChecker::typeToString(const ASTValueType& type) {
    switch (type) {
        case ASTValueType::String:
            return "string";
        case ASTValueType::Integer:
            return "int";
        case ASTValueType::Float:
            return "float";
        case ASTValueType::Bool:
            return "bool";
        case ASTValueType::None:
            return "null";
        default:
            return "Unknown Type";
    }
}