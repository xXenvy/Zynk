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
    const size_t line;

    ASTBase(ASTType type, size_t line) : type(type), line(line) {}
};

struct ASTProgram : public ASTBase {
    ASTProgram(const size_t line) : ASTBase(ASTType::Program, line) {}
    std::vector<std::unique_ptr<ASTBase>> body;
};

struct ASTFunction : public ASTBase {
    ASTFunction(const std::string& name, size_t line)
        : ASTBase(ASTType::FunctionDeclaration, line), name(name) {}
    const std::string name;
    std::vector<std::unique_ptr<ASTBase>> body;
};

struct ASTFunctionCall : public ASTBase {
    ASTFunctionCall(const std::string& name, size_t line)
        : ASTBase(ASTType::FunctionCall, line), name(name) {}
    const std::string name; // Currently we do not support function arguments.
};

struct ASTPrint : public ASTBase {
    ASTPrint(std::unique_ptr<ASTBase> expr, bool newLine, size_t line)
        : ASTBase(ASTType::Print, line), newLine(newLine), expression(std::move(expr)) {}
    const bool newLine;
    std::unique_ptr<ASTBase> expression;
};

struct ASTVariableDeclaration : public ASTBase {
    ASTVariableDeclaration(const std::string& name, ASTValueType type, std::unique_ptr<ASTBase> value, size_t line)
        : ASTBase(ASTType::VariableDeclaration, line), name(name), type(type), value(std::move(value)) {}
    const std::string name;
    const ASTValueType type;
    std::unique_ptr<ASTBase> value;
};

struct ASTVariableModify : public ASTBase {
    ASTVariableModify(const std::string& name, std::unique_ptr<ASTBase> value, size_t line)
        : ASTBase(ASTType::VariableModify, line), name(name), value(std::move(value)) {}
    const std::string name;
    std::unique_ptr<ASTBase> value;
};

struct ASTValue : public ASTBase {
    ASTValue(const std::string& value, ASTValueType type, size_t line)
        : ASTBase(ASTType::Value, line), value(value), type(type) {}
    const std::string value;
    const ASTValueType type;
};

struct ASTVariable : public ASTBase {
    ASTVariable(const std::string& name, size_t line)
        : ASTBase(ASTType::Variable, line), name(name) {}
    const std::string name;
};

struct ASTBinaryOperation : public ASTBase {
    ASTBinaryOperation(std::unique_ptr<ASTBase> left, const std::string& op, std::unique_ptr<ASTBase> right, size_t line)
        : ASTBase(ASTType::BinaryOperation, line), left(std::move(left)), op(op), right(std::move(right)) {}
    std::unique_ptr<ASTBase> left;
    const std::string op;
    std::unique_ptr<ASTBase> right;
};

struct ASTCondition : public ASTBase {
    ASTCondition(std::unique_ptr<ASTBase> expression, size_t line)
        : ASTBase(ASTType::Condition, line), expression(std::move(expression)) {}
    std::unique_ptr<ASTBase> expression;
    std::vector<std::unique_ptr<ASTBase>> body;
    std::vector<std::unique_ptr<ASTBase>> elseBody;
};

struct ASTReadLine : public ASTBase {
    ASTReadLine(std::unique_ptr<ASTBase> out, size_t line)
        : ASTBase(ASTType::ReadLine, line), out(std::move(out)) {}
    std::unique_ptr<ASTBase> out;
};

struct ASTTypeCast : public ASTBase {
    ASTTypeCast(std::unique_ptr<ASTBase> value, ASTValueType type, size_t line)
        : ASTBase(ASTType::TypeCast, line), value(std::move(value)), castType(type) {}
    std::unique_ptr<ASTBase> value;
    const ASTValueType castType;
};

#endif // AST_H
