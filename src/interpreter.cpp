#include "include/interpreter.hpp"
#include "include/lexer.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

std::vector<Token> ZynkInterpreter::interpret(const std::string& source) {
	Lexer lexer(source);
	std::vector<Token> tokens = lexer.tokenize();

	for (const Token& token : tokens) {
		std::cout << "Token(" << static_cast<int>(token.type) << ", \"" << token.value << "\")\n";
	}
	return tokens;
}

std::vector<Token> ZynkInterpreter::interpret_file(const std::string& filePath) {
	std::ifstream file(filePath);
	if (!file.is_open()) throw std::runtime_error("Failed to open a file: " + filePath);

	std::stringstream buffer;
	buffer << file.rdbuf();
	return interpret(buffer.str());
}