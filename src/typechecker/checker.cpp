#include "include/checker.hpp"
#include "../common/include/errors.hpp"

TypeChecker::TypeChecker(RuntimeEnvironment& env) : env(env) {};

ASTValueType TypeChecker::determineType(ASTBase* expression) {
    switch (expression->type) {
        case ASTType::TypeCast:
            return static_cast<ASTTypeCast*>(expression)->castType;
        case ASTType::ReadLine:
            return ASTValueType::String;
        case ASTType::Value: {
            return static_cast<ASTValue*>(expression)->valueType;
        }
        case ASTType::Variable: {
            ASTVariable* var = static_cast<ASTVariable*>(expression);
            ASTVariableDeclaration* declaration = env.getVariable(var->name, var->line);
            return determineType(declaration->value.get());
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
        throw ZynkError{
            ZynkErrorType::TypeError,
            "Type mismatch. Declared type is " + typeToString(declared) + 
            ", but assigned value is of type " + typeToString(valueType) + ".",
            value->line
        };
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
        default:
            return "Unknown Type";
    }
}