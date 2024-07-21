#ifndef AST_H
#define AST_H

#include <vector>
#include <string>
#include <memory>


enum class ASTType {
    Program,
    FunctionDeclaration,
    FunctionCall,
    VariableDeclaration,
    VariableModify,
    Print,
    Value,
    Variable,
    BinaryOperation
};

struct ASTBase {
    ASTType type;
    ASTBase(ASTType type) : type(type) {}
    virtual ~ASTBase() = default;
};

struct ASTProgram : public ASTBase {
    ASTProgram() : ASTBase(ASTType::Program) {}
    std::vector<std::shared_ptr<ASTBase>> body;
};

struct ASTFunction : public ASTBase {
    ASTFunction(const std::string& name) 
        : ASTBase(ASTType::FunctionDeclaration), name(name) {}
    std::string name;
    std::vector<std::shared_ptr<ASTBase>> body;
};

struct ASTFunctionCall : public ASTBase {
    ASTFunctionCall(const std::string& name) : ASTBase(ASTType::FunctionCall), name(name) {}
    std::string name; // Currently we do not support function arguments.
};

struct ASTPrint : public ASTBase {
    ASTPrint(std::shared_ptr<ASTBase> expr, bool newLine) :
        ASTBase(ASTType::Print), newLine(newLine), expression(expr) {}
    bool newLine;
    std::shared_ptr<ASTBase> expression;
};

struct ASTVariableDeclaration : public ASTBase {
    ASTVariableDeclaration(const std::string& name, const std::string& type, std::shared_ptr<ASTBase> value)
        : ASTBase(ASTType::VariableDeclaration), name(name), type(type), value(value) {}
    std::string name;
    std::string type;
    std::shared_ptr<ASTBase> value;
};

struct ASTVariableModify: public ASTBase {
    ASTVariableModify(const std::string& name, std::shared_ptr<ASTBase> value)
        : ASTBase(ASTType::VariableModify), name(name), value(value) {}
    std::string name;
    std::shared_ptr<ASTBase> value;
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
    ASTBinaryOperation(std::shared_ptr<ASTBase> left, const std::string& op, std::shared_ptr<ASTBase> right)
        : ASTBase(ASTType::BinaryOperation), left(left), op(op), right(right) {}
    std::shared_ptr<ASTBase> left;
    std::string op;
    std::shared_ptr<ASTBase> right;
};

#endif // AST_H