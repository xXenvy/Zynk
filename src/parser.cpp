#include "include/parser.hpp"
#include <iostream>

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {};

ProgramNode* Parser::parse() {
	auto program = new ProgramNode();

	while (!eof()) {
		program->nodes.push_back(parseCurrent());
		moveForward();
	}

	return program;
}

ASTNode* Parser::parseCurrent() {
	const TokenType current = currentToken().type;
	std::cout << "Current: " << static_cast<int>(current) << std::endl;

	switch (current) {
		case TokenType::DEF:
			return parseFunction();
		case TokenType::IDENTIFIER:
			return parseVarDeclaration();
		default:
			throw std::runtime_error("test");
	}
}

ASTNode* Parser::parseFunction() {
	FunctionNode* node = new FunctionNode;
	moveForward();

	// todo: check for errors. For now we expect a proper code.
	node->name = currentToken().value;
	consume(TokenType::IDENTIFIER);
	consume(TokenType::LBRACKET);
	consume(TokenType::RBRACKET);

	consume(TokenType::LBRACE);

	while (currentToken().type != TokenType::RBRACE) {
		node->body.push_back(parseCurrent());
	}
	return node;
}

ASTNode* Parser::parseVarDeclaration() {
	VariableDeclarationNode* node = new VariableDeclarationNode;

	// todo: check for errors. For now we expect a proper code.
	node->name = currentToken().value;
	consume(TokenType::IDENTIFIER);
	consume(TokenType::COLON);

	switch (currentToken().type) {
		case TokenType::INT:
			node->type = "int";
			consume(TokenType::INT);
			break;
		case TokenType::STRING:
			node->type = "String";
			consume(TokenType::STRING);
			break;
		case TokenType::FLOAT:
			node->type = "float";
			consume(TokenType::FLOAT);
			break;
		default:
			std::cout << 10 << std::endl;
			throw std::runtime_error("Unexpected token");
	}
	consume(TokenType::EQUAL);
	node->value = currentToken().value;
	moveForward();
	consume(TokenType::SEMICOLON);
	return node;
}

bool Parser::eof() const {
	return position > tokens.size() || tokens[position].type == TokenType::END_OF_FILE;
}

void Parser::moveForward() {
	if (!eof()) position++;
}

Token Parser::currentToken() const {
    if (!eof()) return tokens[position];
    return { TokenType::END_OF_FILE, "" };
}

Token Parser::consume(TokenType expected) {
	const Token current = currentToken();
	if (current.type == expected) {
		moveForward();
		return current;
	};
	std::cout << static_cast<int>(expected) << std::endl;
	std::cout << static_cast<int>(current.type) << std::endl;
	throw std::runtime_error("Unexpected token");
}