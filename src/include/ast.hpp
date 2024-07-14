#ifndef AST_H
#define AST_H

#include <vector>
#include <string>

enum class ASTType {
    Program,
    Function,
    VariableDeclaration,
    Print,
    Value,
    Variable,
    BinaryOperation
};

struct ASTBase {
    ASTType type;
    ASTBase(ASTType type) : type(type) {}
};

struct ASTProgram : public ASTBase {
    ASTProgram() : ASTBase(ASTType::Program) {}

    std::vector<ASTBase*> body;

    ~ASTProgram() {
        for (ASTBase* ast : body) {
            delete ast;
        }
    }
};

struct ASTFunction : public ASTBase {
    ASTFunction(const std::string& name) 
        : ASTBase(ASTType::Function), name(name) {}

    std::string name;
    std::vector<ASTBase*> body;

    ~ASTFunction() {
        for (ASTBase* node : body) {
            delete node;
        }
    }
};

struct ASTPrint : public ASTBase {
    ASTPrint(ASTBase* expr, bool newLine) : 
        ASTBase(ASTType::Print), newLine(newLine), expression(expr) {}
    bool newLine;
    ASTBase* expression;

    ~ASTPrint() {
        delete expression;
    }
};

struct ASTVariableDeclaration : public ASTBase {
    ASTVariableDeclaration(const std::string& name, const std::string& type, ASTBase* value) 
        : ASTBase(ASTType::VariableDeclaration), name(name), type(type), value(value) {}
    std::string name;
    std::string type;
    ASTBase* value;

    ~ASTVariableDeclaration() {
        delete value;
    }
};

struct ASTValue : public ASTBase {
    ASTValue(const std::string& value) : ASTBase(ASTType::Value), value(value) {}
    std::string value;
};

struct ASTVariable : public ASTBase {
    ASTVariable(const std::string& name) : ASTBase(ASTType::Variable), name(name) {}
    std::string name;
};

struct ASTBinaryOperation : public ASTBase {
    ASTBinaryOperation(ASTBase* left, const std::string& op, ASTBase* right)
        : ASTBase(ASTType::BinaryOperation), left(left), op(op), right(right) {}

    ASTBase* left;
    std::string op;
    ASTBase* right;

    ~ASTBinaryOperation() {
        delete left;
        delete right;
    }
};

#endif // AST_H