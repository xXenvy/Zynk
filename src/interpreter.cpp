#include "include/interpreter.hpp"
#include "include/lexer.hpp"
#include "include/parser.hpp"
#include "include/ast.hpp"
#include "include/errors.hpp"
#include "include/evaluator.hpp"
#include "include/runtime.hpp"

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