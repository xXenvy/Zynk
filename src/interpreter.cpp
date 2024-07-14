#include "include/interpreter.hpp"
#include "include/lexer.hpp"
#include "include/parser.hpp"
#include "include/ast.hpp"
#include "include/errors.hpp"

#include <fstream>
#include <sstream>

const ASTProgram* ZynkInterpreter::interpret(const std::string& source) {
	Lexer lexer(source);
	const std::vector<Token> tokens = lexer.tokenize();

	Parser parser(tokens);
    ASTProgram* node = parser.parse();

    return node;
}

const ASTProgram* ZynkInterpreter::interpret_file(const std::string& filePath) {
	std::ifstream file(filePath);
    if (!file.is_open()) {
        throw ZynkError{ ZynkErrorType::FileOpenError, "Failed to open a file " + filePath};
    }
        
	std::stringstream buffer;
	buffer << file.rdbuf();
	return interpret(buffer.str());
}