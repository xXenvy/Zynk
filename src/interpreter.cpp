#include "include/interpreter.hpp"
#include "include/lexer.hpp"
#include "include/parser.hpp"
#include "include/ast.hpp"
#include "include/errors.hpp"
#include "include/evaluator.hpp"
#include "include/runtime.hpp"

#include <fstream>
#include <sstream>

void displayAST(ASTBase* node, int indent = 0) {
    if (node == nullptr) return;

    for (int i = 0; i < indent; ++i) {
        std::cout << "  ";
    }

    std::cout << "Node type(" << static_cast<int>(node->type) << ") ";

    switch (node->type) {
    case ASTType::Program: {
        ASTProgram* programNode = static_cast<ASTProgram*>(node);
        std::cout << "Program: " << std::endl;
        for (ASTBase* child : programNode->body) {
            displayAST(child, indent + 1);
        }
        break;
    }
    case ASTType::Function: {
        ASTFunction* funcNode = static_cast<ASTFunction*>(node);
        std::cout << "Function: " << funcNode->name << std::endl;
        for (ASTBase* stmt : funcNode->body) {
            displayAST(stmt, indent + 1);
        }
        break;
    }
    case ASTType::VariableDeclaration: {
        ASTVariableDeclaration* varNode = static_cast<ASTVariableDeclaration*>(node);
        std::cout << "VarDeclaration: " << varNode->name << " (" << varNode->type << ")" << std::endl;
        displayAST(varNode->value, indent + 1);
        break;
    }
    case ASTType::Print: {
        ASTPrint* printNode = static_cast<ASTPrint*>(node);
        std::cout << "PrintStatement: " << "(newLine=" << printNode->newLine << ")" << std::endl;
        displayAST(printNode->expression, indent + 1);
        break;
    }
    case ASTType::BinaryOperation: {
        ASTBinaryOperation* binOpNode = static_cast<ASTBinaryOperation*>(node);
        std::cout << "BinaryOperation: " << binOpNode->op << std::endl;
        displayAST(binOpNode->left, indent + 1);
        displayAST(binOpNode->right, indent + 1);
        break;
    }
    case ASTType::Value: {
        ASTValue* valueNode = static_cast<ASTValue*>(node);
        std::cout << "Value: " << valueNode->value << std::endl;
        break;
    }
    case ASTType::Variable: {
        ASTVariable* varNode = static_cast<ASTVariable*>(node);
        std::cout << "Variable: " << varNode->name << std::endl;
        break;
    }
    default:
        std::cout << "Unknown node type" << std::endl;
        break;
    }
}

const ASTProgram* ZynkInterpreter::interpret(const std::string& source) {
    Lexer lexer(source);
    const std::vector<Token> tokens = lexer.tokenize();

    for (const Token& token : tokens) {
        std::cout << "Token(" << static_cast<int>(token.type) << ", \"" << token.value << "\")\n";
    }

    Parser parser(tokens);
    ASTProgram* program = parser.parse();
    
    RuntimeEnvironment env;
    Evaluator evaluator(env);
    evaluator.evaluate(program);

    displayAST(program);

    return program;
}

const ASTProgram* ZynkInterpreter::interpret_file(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw ZynkError{ ZynkErrorType::FileOpenError, "Failed to open a file " + filePath };
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return interpret(buffer.str());
}