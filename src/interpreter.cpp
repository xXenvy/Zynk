#include "include/interpreter.hpp"
#include "include/lexer.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

void ZynkInterpreter::interpret(const std::string& source) {
	Lexer lexer(source);
	std::vector<Token> tokens = lexer.tokenize();

	for (const Token& token : tokens) {
		std::cout << "Token(" << static_cast<int>(token.type) << ", \"" << token.value << "\")\n";
	}
}

void ZynkInterpreter::interpret_file(const std::string& filePath) {
	std::ifstream file(filePath);
	if (!file.is_open()) { // Todo: throw and handle errors.
		std::cerr << "Failed to open a file." << std::endl;
		return;
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	interpret(buffer.str());
}