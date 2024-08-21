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
    While,
    Break,
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
    virtual ~ASTBase() = default;
    virtual std::unique_ptr<ASTBase> clone() const = 0;
};

struct ASTProgram : public ASTBase {
    ASTProgram(const size_t line = 1) : ASTBase(ASTType::Program, line) {}
    std::vector<std::unique_ptr<ASTBase>> body;

    std::unique_ptr<ASTBase> clone() const override {
        auto newProgram = std::make_unique<ASTProgram>(line);
        for (const auto& stmt : body) {
            newProgram->body.push_back(stmt->clone());
        }
        return newProgram;
    }
};

struct ASTFunction : public ASTBase {
    ASTFunction(const std::string& name, const ASTValueType returnType, size_t line)
        : ASTBase(ASTType::FunctionDeclaration, line), name(name), returnType(returnType) {}
    const std::string name;
    const ASTValueType returnType;

    std::vector<std::unique_ptr<ASTBase>> arguments;
    std::vector<std::unique_ptr<ASTBase>> body;

    std::unique_ptr<ASTBase> clone() const override {
        auto newFunction = std::make_unique<ASTFunction>(name, returnType, line);
        for (const auto& arg : arguments) {
            newFunction->arguments.push_back(arg->clone());
        }
        for (const auto& stmt : body) {
            newFunction->body.push_back(stmt->clone());
        }
        return newFunction;
    }
};

struct ASTFunctionArgument : public ASTBase {
    ASTFunctionArgument(const std::string& name, const ASTValueType valueType, size_t line)
        : ASTBase(ASTType::FunctionArgument, line), name(name), valueType(valueType) {}
    const std::string name;
    const ASTValueType valueType;

    std::unique_ptr<ASTBase> clone() const override {
        return std::make_unique<ASTFunctionArgument>(name, valueType, line);
    }
};

struct ASTFunctionCall : public ASTBase {
    ASTFunctionCall(const std::string& name, size_t line)
        : ASTBase(ASTType::FunctionCall, line), name(name) {}
    const std::string name;
    std::vector<std::unique_ptr<ASTBase>> arguments;

    std::unique_ptr<ASTBase> clone() const override {
        auto newFuncCall = std::make_unique<ASTFunctionCall>(name, line);
        for (const auto& arg : arguments) {
            newFuncCall->arguments.push_back(arg->clone());
        }
        return newFuncCall;
    }
};

struct ASTReturn : public ASTBase {
    ASTReturn(std::unique_ptr<ASTBase> value, size_t line)
        : ASTBase(ASTType::Return, line), value(std::move(value)) {}
    std::unique_ptr<ASTBase> value;

    std::unique_ptr<ASTBase> clone() const override {
        return std::make_unique<ASTReturn>(value ? value->clone() : nullptr, line);
    }
};

struct ASTPrint : public ASTBase {
    ASTPrint(std::unique_ptr<ASTBase> expr, bool newLine, size_t line)
        : ASTBase(ASTType::Print, line), newLine(newLine), expression(std::move(expr)) {}
    const bool newLine;
    std::unique_ptr<ASTBase> expression;

    std::unique_ptr<ASTBase> clone() const override {
        return std::make_unique<ASTPrint>(expression ? expression->clone() : nullptr, newLine, line);
    }
};

struct ASTVariableDeclaration : public ASTBase {
    ASTVariableDeclaration(const std::string& name, ASTValueType type, std::unique_ptr<ASTBase> value, size_t line)
        : ASTBase(ASTType::VariableDeclaration, line), name(name), varType(type), value(std::move(value)) {}
    const std::string name;
    const ASTValueType varType;
    std::unique_ptr<ASTBase> value;

    std::unique_ptr<ASTBase> clone() const override {
        return std::make_unique<ASTVariableDeclaration>(name, varType, value ? value->clone() : nullptr, line);
    }
};

struct ASTVariableModify : public ASTBase {
    ASTVariableModify(const std::string& name, std::unique_ptr<ASTBase> value, size_t line)
        : ASTBase(ASTType::VariableModify, line), name(name), value(std::move(value)) {}
    const std::string name;
    std::unique_ptr<ASTBase> value;

    std::unique_ptr<ASTBase> clone() const override {
        return std::make_unique<ASTVariableModify>(name, value ? value->clone() : nullptr, line);
    }
};

struct ASTFString : public ASTBase {
    ASTFString(const std::string& value, size_t line)
        : ASTBase(ASTType::FString, line), value(value) {}
    const std::string value;

    std::unique_ptr<ASTBase> clone() const override {
        return std::make_unique<ASTFString>(value, line);
    }
};

struct ASTValue : public ASTBase {
    ASTValue(const std::string& value, ASTValueType type, size_t line)
        : ASTBase(ASTType::Value, line), value(value), valueType(type) {}
    std::string value;
    const ASTValueType valueType;

    std::unique_ptr<ASTBase> clone() const override {
        return std::make_unique<ASTValue>(value, valueType, line);
    }
};

struct ASTVariable : public ASTBase {
    ASTVariable(const std::string& name, size_t line)
        : ASTBase(ASTType::Variable, line), name(name) {}
    const std::string name;

    std::unique_ptr<ASTBase> clone() const override {
        return std::make_unique<ASTVariable>(name, line);
    }
};

struct ASTCondition : public ASTBase {
    ASTCondition(std::unique_ptr<ASTBase> expression, size_t line)
        : ASTBase(ASTType::Condition, line), expression(std::move(expression)) {}
    std::unique_ptr<ASTBase> expression;
    std::vector<std::unique_ptr<ASTBase>> body;
    std::vector<std::unique_ptr<ASTBase>> elseBody;

    std::unique_ptr<ASTBase> clone() const override {
        auto newCondition = std::make_unique<ASTCondition>(expression ? expression->clone() : nullptr, line);
        for (const auto& stmt : body) {
            newCondition->body.push_back(stmt->clone());
        }
        for (const auto& stmt : elseBody) {
            newCondition->elseBody.push_back(stmt->clone());
        }
        return newCondition;
    }
};

struct ASTReadInput : public ASTBase {
    ASTReadInput(std::unique_ptr<ASTBase> out, size_t line)
        : ASTBase(ASTType::ReadInput, line), out(std::move(out)) {}
    std::unique_ptr<ASTBase> out;

    std::unique_ptr<ASTBase> clone() const override {
        return std::make_unique<ASTReadInput>(out ? out->clone() : nullptr, line);
    }
};

struct ASTWhile : public ASTBase {
    ASTWhile(std::unique_ptr<ASTBase> value, size_t line)
        : ASTBase(ASTType::While, line), value(std::move(value)) {}
    std::unique_ptr<ASTBase> value;
    std::vector<std::unique_ptr<ASTBase>> body;

    std::unique_ptr<ASTBase> clone() const override {
        auto newWhile = std::make_unique<ASTWhile>(value ? value->clone() : nullptr, line);
        for (const auto& stmt : body) {
            newWhile->body.push_back(stmt->clone());
        }
        return newWhile;
    }
};

struct ASTBreak : public ASTBase {
    ASTBreak(size_t line) : ASTBase(ASTType::Break, line) {}

    std::unique_ptr<ASTBase> clone() const override {
        return std::make_unique<ASTBreak>(line);
    }
};

struct ASTTypeCast : public ASTBase {
    ASTTypeCast(std::unique_ptr<ASTBase> value, ASTValueType type, size_t line)
        : ASTBase(ASTType::TypeCast, line), value(std::move(value)), castType(type) {}
    std::unique_ptr<ASTBase> value;
    const ASTValueType castType;

    std::unique_ptr<ASTBase> clone() const override {
        return std::make_unique<ASTTypeCast>(value ? value->clone() : nullptr, castType, line);
    }
};

struct ASTBinaryOperation : public ASTBase {
    ASTBinaryOperation(std::unique_ptr<ASTBase> left, const std::string& op, std::unique_ptr<ASTBase> right, size_t line)
        : ASTBase(ASTType::BinaryOperation, line), left(std::move(left)), op(op), right(std::move(right)) {}
    std::unique_ptr<ASTBase> left;
    const std::string op;
    std::unique_ptr<ASTBase> right;

    std::unique_ptr<ASTBase> clone() const override {
        return std::make_unique<ASTBinaryOperation>(
            left ? left->clone() : nullptr,
            op,
            right ? right->clone() : nullptr,
            line
        );
    }
};

struct ASTComparisonOperation : public ASTBase {
    ASTComparisonOperation(std::unique_ptr<ASTBase> left, const std::string& op, std::unique_ptr<ASTBase> right, size_t line)
        : ASTBase(ASTType::ComparisonOperation, line), left(std::move(left)), op(op), right(std::move(right)) {}

    std::unique_ptr<ASTBase> left;
    const std::string op;
    std::unique_ptr<ASTBase> right;

    std::unique_ptr<ASTBase> clone() const override {
        return std::make_unique<ASTComparisonOperation>(
            left ? left->clone() : nullptr,
            op,
            right ? right->clone() : nullptr,
            line
        );
    }
};

struct ASTAndOperation : public ASTBase {
    ASTAndOperation(std::unique_ptr<ASTBase> left, std::unique_ptr<ASTBase> right, size_t line)
        : ASTBase(ASTType::AndOperation, line), left(std::move(left)), right(std::move(right)) {}
    std::unique_ptr<ASTBase> left;
    std::unique_ptr<ASTBase> right;

    std::unique_ptr<ASTBase> clone() const override {
        return std::make_unique<ASTAndOperation>(
            left ? left->clone() : nullptr,
            right ? right->clone() : nullptr,
            line
        );
    }
};

struct ASTOrOperation : public ASTBase {
    ASTOrOperation(std::unique_ptr<ASTBase> left, std::unique_ptr<ASTBase> right, size_t line)
        : ASTBase(ASTType::OrOperation, line), left(std::move(left)), right(std::move(right)) {}
    std::unique_ptr<ASTBase> left;
    std::unique_ptr<ASTBase> right;

    std::unique_ptr<ASTBase> clone() const override {
        return std::make_unique<ASTOrOperation>(
            left ? left->clone() : nullptr,
            right ? right->clone() : nullptr,
            line
        );
    }
};


#endif // AST_H