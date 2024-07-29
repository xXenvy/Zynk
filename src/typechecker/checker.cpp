#include "include/checker.hpp"
#include "../common/include/errors.hpp"

TypeChecker::TypeChecker(RuntimeEnvironment& env) : env(env) {};

ASTValueType TypeChecker::determineType(ASTBase* expression) {
    switch (expression->type) {
        case ASTType::Value: {
            return static_cast<ASTValue*>(expression)->type;
        }
        case ASTType::Variable: {
            ASTVariable* var = static_cast<ASTVariable*>(expression);
            ASTVariableDeclaration* declaration = env.getVariable(var->name);
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
        default:
            throw ZynkError{ ZynkErrorType::TypeError, "Cannot determine type for given AST type." };
        }
}

void TypeChecker::checkType(const ASTValueType& declared, ASTBase* value) {
    ASTValueType valueType = determineType(value);
    if (declared != valueType) {
        throw ZynkError{
            ZynkErrorType::TypeError,
            "Type mismatch. Declared type is " + typeToString(declared) + 
            ", but assigned value is of type " + typeToString(valueType) + "."
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