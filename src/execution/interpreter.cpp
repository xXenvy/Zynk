#include "include/interpreter.hpp"

#include "../common/include/errors.hpp"
#include "../parsing/include/lexer.hpp"
#include "../parsing/include/parser.hpp"
#include "../parsing/include/ast.hpp"
#include "../execution/include/evaluator.hpp"
#include "../execution/include/runtime.hpp"

#include <fstream>
#include <sstream>

void ZynkInterpreter::interpret(const std::string& source) {
    // Processing the raw source into tokens.
    Lexer lexer(source);
    const std::vector<Token> tokens = lexer.tokenize();

    // Parsing the tokens into AST objects.
    Parser parser(tokens);
    const std::unique_ptr<ASTProgram> program = parser.parse();
    
    // Executing the program.
    Evaluator evaluator;
    evaluator.evaluate(program.get());
}

void ZynkInterpreter::interpretFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw ZynkError(ZynkErrorType::FileOpenError, "Failed to open a file.");
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    interpret(buffer.str());
}