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
    ReadLine,
    Value,
    Variable,
    BinaryOperation,
    Condition,
    TypeCast
};

enum class ASTValueType {
    String,
    Integer,
    Float,
    Bool,
    None,
};

struct ASTBase {
    const ASTType type;
    ASTBase(ASTType type) : type(type) {}
    virtual ~ASTBase() = default;
};

struct ASTProgram : public ASTBase {
    ASTProgram() : ASTBase(ASTType::Program) {}
    std::vector<std::unique_ptr<ASTBase>> body;
};

struct ASTFunction : public ASTBase {
    ASTFunction(const std::string& name)
        : ASTBase(ASTType::FunctionDeclaration), name(name) {}
    const std::string name;
    std::vector<std::unique_ptr<ASTBase>> body;
};

struct ASTFunctionCall : public ASTBase {
    ASTFunctionCall(const std::string& name) : ASTBase(ASTType::FunctionCall), name(name) {}
    const std::string name; // Currently we do not support function arguments.
};

struct ASTPrint : public ASTBase {
    ASTPrint(std::unique_ptr<ASTBase> expr, bool newLine) :
        ASTBase(ASTType::Print), newLine(newLine), expression(std::move(expr)) {}
    const bool newLine;
    std::unique_ptr<ASTBase> expression;
};

struct ASTVariableDeclaration : public ASTBase {
    ASTVariableDeclaration(const std::string& name, const ASTValueType type, std::unique_ptr<ASTBase> value)
        : ASTBase(ASTType::VariableDeclaration), name(name), type(type), value(std::move(value)) {}
    const std::string name;
    const ASTValueType type;
    std::unique_ptr<ASTBase> value;
};

struct ASTVariableModify : public ASTBase {
    ASTVariableModify(const std::string& name, std::unique_ptr<ASTBase> value)
        : ASTBase(ASTType::VariableModify), name(name), value(std::move(value)) {}
    const std::string name;
    std::unique_ptr<ASTBase> value;
};

struct ASTValue : public ASTBase {
    ASTValue(const std::string& value, const ASTValueType type) 
        : ASTBase(ASTType::Value), value(value), type(type) {}
    const std::string value;
    const ASTValueType type;
};

struct ASTVariable : public ASTBase {
    ASTVariable(const std::string& name) : ASTBase(ASTType::Variable), name(name) {}
    const std::string name;
};

struct ASTBinaryOperation : public ASTBase {
    ASTBinaryOperation(std::unique_ptr<ASTBase> left, const std::string& op, std::unique_ptr<ASTBase> right)
        : ASTBase(ASTType::BinaryOperation), left(std::move(left)), op(op), right(std::move(right)) {}
    std::unique_ptr<ASTBase> left;
    const std::string op;
    std::unique_ptr<ASTBase> right;
};

struct ASTCondition : public ASTBase {
    ASTCondition(std::unique_ptr<ASTBase> expression)
        : ASTBase(ASTType::Condition), expression(std::move(expression)) {}
    std::unique_ptr<ASTBase> expression;
    std::vector<std::unique_ptr<ASTBase>> body;
    std::vector<std::unique_ptr<ASTBase>> elseBody;
};

struct ASTReadLine : public ASTBase {
    ASTReadLine(std::unique_ptr<ASTBase> out) 
        : ASTBase(ASTType::ReadLine), out(std::move(out)) {}
    std::unique_ptr<ASTBase> out;
};

struct ASTTypeCast : public ASTBase {
    ASTTypeCast(std::unique_ptr<ASTBase> value, const ASTValueType type)
        : ASTBase(ASTType::TypeCast), value(std::move(value)), castType(type) {}
    std::unique_ptr<ASTBase> value;
    const ASTValueType castType;
};

#endif // AST_H
