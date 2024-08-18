#ifndef AST_H
#define AST_H

#include <vector>
#include <string>
#include <memory>

enum class ASTType {
    Program,
    FunctionDeclaration,
    FunctionCall,
    FunctionArgument,
    VariableDeclaration,
    VariableModify,
    Print,
    ReadInput,
    Value,
    Variable,
    FString,
    BinaryOperation,
    ComparisonOperation,
    Condition,
    TypeCast,
    AndOperation,
    OrOperation,
    Return,
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
    size_t line;

    ASTBase(ASTType type, size_t line) : type(type), line(line) {}
};

struct ASTProgram : public ASTBase {
    ASTProgram(const size_t line = 1) : ASTBase(ASTType::Program, line) {}
    std::vector<std::unique_ptr<ASTBase>> body;
};

struct ASTFunction : public ASTBase {
    ASTFunction(const std::string& name, const ASTValueType returnType, size_t line)
        : ASTBase(ASTType::FunctionDeclaration, line), name(name), returnType(returnType) {}
    const std::string name;
    const ASTValueType returnType;
    std::vector<std::unique_ptr<ASTBase>> arguments;
    std::vector<std::unique_ptr<ASTBase>> body;
};

struct ASTFunctionArgument : public ASTBase {
    ASTFunctionArgument(const std::string& name, const ASTValueType valueType, size_t line)
        : ASTBase(ASTType::FunctionArgument, line), name(name), valueType(valueType) {}
    const std::string name;
    const ASTValueType valueType;
};

struct ASTFunctionCall : public ASTBase {
    ASTFunctionCall(const std::string& name, size_t line)
        : ASTBase(ASTType::FunctionCall, line), name(name) {}
    const std::string name;
    std::vector<std::unique_ptr<ASTBase>> arguments;
};

struct ASTReturn : public ASTBase {
    ASTReturn(std::unique_ptr<ASTBase> value, size_t line)
        : ASTBase(ASTType::Return, line), value(std::move(value)) {}
    std::unique_ptr<ASTBase> value;
};

struct ASTPrint : public ASTBase {
    ASTPrint(std::unique_ptr<ASTBase> expr, bool newLine, size_t line)
        : ASTBase(ASTType::Print, line), newLine(newLine), expression(std::move(expr)) {}
    const bool newLine;
    std::unique_ptr<ASTBase> expression;
};

struct ASTVariableDeclaration : public ASTBase {
    ASTVariableDeclaration(const std::string& name, ASTValueType type, std::unique_ptr<ASTBase> value, size_t line)
        : ASTBase(ASTType::VariableDeclaration, line), name(name), varType(type), value(std::move(value)) {}
    const std::string name;
    const ASTValueType varType;
    std::unique_ptr<ASTBase> value;
};

struct ASTVariableModify : public ASTBase {
    ASTVariableModify(const std::string& name, std::unique_ptr<ASTBase> value, size_t line)
        : ASTBase(ASTType::VariableModify, line), name(name), value(std::move(value)) {}
    const std::string name;
    std::unique_ptr<ASTBase> value;
};

struct ASTFString : public ASTBase {
    ASTFString(const std::string& value, size_t line)
        : ASTBase(ASTType::FString, line), value(value) {}
    const std::string value;
};

struct ASTValue : public ASTBase {
    ASTValue(const std::string& value, ASTValueType type, size_t line)
        : ASTBase(ASTType::Value, line), value(value), valueType(type) {}
    const std::string value;
    const ASTValueType valueType;
};

struct ASTVariable : public ASTBase {
    ASTVariable(const std::string& name, size_t line)
        : ASTBase(ASTType::Variable, line), name(name) {}
    const std::string name;
};

struct ASTCondition : public ASTBase {
    ASTCondition(std::unique_ptr<ASTBase> expression, size_t line)
        : ASTBase(ASTType::Condition, line), expression(std::move(expression)) {}
    std::unique_ptr<ASTBase> expression;
    std::vector<std::unique_ptr<ASTBase>> body;
    std::vector<std::unique_ptr<ASTBase>> elseBody;
};

struct ASTReadInput : public ASTBase {
    ASTReadInput(std::unique_ptr<ASTBase> out, size_t line)
        : ASTBase(ASTType::ReadInput, line), out(std::move(out)) {}
    std::unique_ptr<ASTBase> out;
};

struct ASTTypeCast : public ASTBase {
    ASTTypeCast(std::unique_ptr<ASTBase> value, ASTValueType type, size_t line)
        : ASTBase(ASTType::TypeCast, line), value(std::move(value)), castType(type) {}
    std::unique_ptr<ASTBase> value;
    const ASTValueType castType;
};

struct ASTBinaryOperation : public ASTBase {
    ASTBinaryOperation(std::unique_ptr<ASTBase> left, const std::string& op, std::unique_ptr<ASTBase> right, size_t line)
        : ASTBase(ASTType::BinaryOperation, line), left(std::move(left)), op(op), right(std::move(right)) {}
    std::unique_ptr<ASTBase> left;
    const std::string op;
    std::unique_ptr<ASTBase> right;
};

struct ASTComparisonOperation : public ASTBase {
    ASTComparisonOperation(std::unique_ptr<ASTBase> left, const std::string& op, std::unique_ptr<ASTBase> right, size_t line)
        : ASTBase(ASTType::ComparisonOperation, line), left(std::move(left)), op(op), right(std::move(right)) {}

    std::unique_ptr<ASTBase> left;
    const std::string op;
    std::unique_ptr<ASTBase> right;
};

struct ASTAndOperation : public ASTBase {
    ASTAndOperation(std::unique_ptr<ASTBase> left, std::unique_ptr<ASTBase> right, size_t line)
        : ASTBase(ASTType::AndOperation, line), left(std::move(left)), right(std::move(right)) {}
    std::unique_ptr<ASTBase> left;
    std::unique_ptr<ASTBase> right;
};

struct ASTOrOperation : public ASTBase {
    ASTOrOperation(std::unique_ptr<ASTBase> left, std::unique_ptr<ASTBase> right, size_t line)
        : ASTBase(ASTType::OrOperation, line), left(std::move(left)), right(std::move(right)) {}
    std::unique_ptr<ASTBase> left;
    std::unique_ptr<ASTBase> right;
};

#endif // AST_H