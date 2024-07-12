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
            std::cout << "VarDeclaration: " << varNode->name << " (" << varNode->type << ") = " << varNode->value << std::endl;
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