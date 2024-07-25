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