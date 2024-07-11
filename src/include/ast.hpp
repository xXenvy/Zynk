#ifndef AST_H
#define AST_H

#include <vector>
#include <string>

enum class ASTNodeType {
    Program,
    Function,
    VariableDeclaration,
    PrintStatement,
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
};

struct PrintStatementNode : public ASTNode {
    PrintStatementNode() : ASTNode(ASTNodeType::PrintStatement) {}
    std::string message;
};

struct VariableDeclarationNode : public ASTNode {
    VariableDeclarationNode() : ASTNode(ASTNodeType::VariableDeclaration) {}
    std::string name;
    std::string type;
    std::string value;
};

#endif