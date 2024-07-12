#include "include/interpreter.hpp"
#include "include/lexer.hpp"
#include "include/parser.hpp"
#include "include/ast.hpp"
#include "include/errors.hpp"

#include <fstream>
#include <sstream>

void displayAST(ASTNode* node, int indent = 0) {
    if (node == nullptr) return;

    for (int i = 0; i < indent; ++i) {
        std::cout << "  ";
    }

    std::cout << "Node type(" << static_cast<int>(node->type) << ") ";

    switch (node->type) {
        case ASTNodeType::Program: {
            ProgramNode* programNode = static_cast<ProgramNode*>(node);
            std::cout << "Program: " << std::endl;
            for (ASTNode* child : programNode->nodes) {
                displayAST(child, indent + 1);
            }
            break;
        }
        case ASTNodeType::Function: {
            FunctionNode* funcNode = static_cast<FunctionNode*>(node);
            std::cout << "Function: " << funcNode->name << std::endl;
            for (ASTNode* stmt : funcNode->body) {
                displayAST(stmt, indent + 1);
            }
            break;
        }
        case ASTNodeType::VariableDeclaration: {
            VariableDeclarationNode* varNode = static_cast<VariableDeclarationNode*>(node);
            std::cout << "VarDeclaration: " << varNode->name << " (" << varNode->type << ")" << std::endl;
            displayAST(varNode->value, indent + 1);
            break;
        }
        case ASTNodeType::Print: {
            PrintNode* printNode = static_cast<PrintNode*>(node);
            std::cout << "PrintStatement: " << "(newLine=" << printNode->newLine << ")" << std::endl;
            displayAST(printNode->expression, indent + 1);
            break;
        }
        case ASTNodeType::BinaryOperation: {
            BinaryOperationNode* binOpNode = static_cast<BinaryOperationNode*>(node);
            std::cout << "BinaryOperation: " << binOpNode->op << std::endl;
            displayAST(binOpNode->left, indent + 1);
            displayAST(binOpNode->right, indent + 1);
            break;
        }
        case ASTNodeType::Value: {
            ValueNode* valueNode = static_cast<ValueNode*>(node);
            std::cout << "Value: " << valueNode->value << std::endl;
            break;
        }
        case ASTNodeType::Variable: {
            VariableNode* varNode = static_cast<VariableNode*>(node);
            std::cout << "Variable: " << varNode->name << std::endl;
            break;
        }
        default:
            std::cout << "Unknown node type" << std::endl;
            break;
        }
}

const ProgramNode* ZynkInterpreter::interpret(const std::string& source) {
	Lexer lexer(source);
	const std::vector<Token> tokens = lexer.tokenize();

    for (const Token& token : tokens) {
        std::cout << "Token(" << static_cast<int>(token.type) << ", \"" << token.value << "\")\n";
    }

	Parser parser(tokens);
    ProgramNode* node = parser.parse();
    displayAST(node);

    return node;
}

const ProgramNode* ZynkInterpreter::interpret_file(const std::string& filePath) {
	std::ifstream file(filePath);
    if (!file.is_open()) {
        throw ZynkError{ ZynkErrorType::FileOpenError, "Failed to open a file " + filePath};
    }
        
	std::stringstream buffer;
	buffer << file.rdbuf();
	return interpret(buffer.str());
}