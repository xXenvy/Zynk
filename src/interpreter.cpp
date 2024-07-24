#include "include/interpreter.hpp"
#include "include/lexer.hpp"
#include "include/parser.hpp"
#include "include/ast.hpp"
#include "include/errors.hpp"
#include "include/evaluator.hpp"
#include "include/runtime.hpp"

#include <fstream>
#include <sstream>

void displayAST(std::shared_ptr<ASTBase> node, int indent = 0) {
    if (node == nullptr) return;

    for (int i = 0; i < indent; ++i) {
        std::cout << "  ";
    }

    std::cout << "Node type(" << static_cast<int>(node->type) << ") ";

    switch (node->type) {
        case ASTType::Program: {
            auto programNode = std::dynamic_pointer_cast<ASTProgram>(node);
            std::cout << "Program: " << std::endl;
            for (auto child : programNode->body) {
                displayAST(child, indent + 1);
            }
            break;
        }
        case ASTType::FunctionDeclaration: {
            auto funcNode = std::dynamic_pointer_cast<ASTFunction>(node);
            std::cout << "Function: " << funcNode->name << std::endl;
            for (auto stmt : funcNode->body) {
                displayAST(stmt, indent + 1);
            }
            break;
        }
        case ASTType::FunctionCall: {
            auto funcCall = std::dynamic_pointer_cast<ASTFunctionCall>(node);
            std::cout << "FunctionCall: " << funcCall->name << std::endl;
            break;
        }
        case ASTType::VariableDeclaration: {
            auto varNode = std::dynamic_pointer_cast<ASTVariableDeclaration>(node);
            std::cout << "VarDeclaration: " << varNode->name << " (" << varNode->type << ")" << std::endl;
            displayAST(varNode->value, indent + 1);
            break;
        }
        case ASTType::Print: {
            auto printNode = std::dynamic_pointer_cast<ASTPrint>(node);
            std::cout << "PrintStatement: " << "(newLine=" << printNode->newLine << ")" << std::endl;
            displayAST(printNode->expression, indent + 1);
            break;
        }
        case ASTType::BinaryOperation: {
            auto binOpNode = std::dynamic_pointer_cast<ASTBinaryOperation>(node);
            std::cout << "BinaryOperation: " << binOpNode->op << std::endl;
            displayAST(binOpNode->left, indent + 1);
            displayAST(binOpNode->right, indent + 1);
            break;
        }
        case ASTType::Value: {
            auto valueNode = std::dynamic_pointer_cast<ASTValue>(node);
            std::cout << "Value: " << valueNode->value << std::endl;
            break;
        }
        case ASTType::Variable: {
            auto varNode = std::dynamic_pointer_cast<ASTVariable>(node);
            std::cout << "Variable: " << varNode->name << std::endl;
            break;
        }
        default:
            std::cout << "Unknown node type" << std::endl;
            break;
    }
}

void ZynkInterpreter::interpret(const std::string& source) {
    // Processing the raw source into tokens.
    Lexer lexer(source);
    const std::vector<Token> tokens = lexer.tokenize();

    // for (const Token& token : tokens) {
        // std::cout << "Token(" << static_cast<int>(token.type) << ", \"" << token.value << "\")\n";
    // }
    // displayAST(program);

    // Parsing the tokens into ASTObjects.
    Parser parser(tokens);
    const std::shared_ptr<ASTProgram> program = parser.parse();
    
    RuntimeEnvironment env;
    Evaluator evaluator(env);
    evaluator.evaluate(program);
}

void ZynkInterpreter::interpret_file(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw ZynkError{ ZynkErrorType::FileOpenError, "Failed to open a file " + filePath };
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    interpret(buffer.str());
}