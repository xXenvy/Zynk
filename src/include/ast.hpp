#ifndef AST_H
#define AST_H

#include <vector>
#include <string>

enum class ASTNodeType {
    Program,
    Function,
    VariableDeclaration,
    Print,
    Value,
    Variable,
    BinaryOperation
};

struct ASTNode {
    ASTNodeType type;
    ASTNode(ASTNodeType type) : type(type) {}
};

struct ProgramNode : public ASTNode {
    ProgramNode() : ASTNode(ASTNodeType::Program) {}

    std::vector<ASTNode*> nodes;

    ~ProgramNode() {
        for (ASTNode* node : nodes) {
            delete node;
        }
    }
};

struct FunctionNode : public ASTNode {
    FunctionNode() : ASTNode(ASTNodeType::Function) {}

    std::string name;
    std::vector<ASTNode*> body;

    ~FunctionNode() {
        for (ASTNode* node : body) {
            delete node;
        }
    }
};

struct PrintNode : public ASTNode {
    PrintNode() : ASTNode(ASTNodeType::Print), newLine(true) {}
    bool newLine;
    ASTNode* expression;

    ~PrintNode() {
        delete expression;
    }
};

struct VariableDeclarationNode : public ASTNode {
    VariableDeclarationNode() : ASTNode(ASTNodeType::VariableDeclaration) {}
    std::string name;
    std::string type;
    ASTNode* value;

    ~VariableDeclarationNode() {
        delete value;
    }
};

struct ValueNode : public ASTNode {
    ValueNode(const std::string& value) : ASTNode(ASTNodeType::Value), value(value) {}
    std::string value;
};

struct VariableNode : public ASTNode {
    VariableNode(const std::string& name) : ASTNode(ASTNodeType::Variable), name(name) {}
    std::string name;
};

struct BinaryOperationNode : public ASTNode {
    BinaryOperationNode(ASTNode* left, const std::string& op, ASTNode* right)
        : ASTNode(ASTNodeType::BinaryOperation), left(left), op(op), right(right) {}

    ASTNode* left;
    std::string op;
    ASTNode* right;

    ~BinaryOperationNode() {
        delete left;
        delete right;
    }
};

#endif