#include "include/parser.hpp"
#include "include/errors.hpp"

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {};

ProgramNode* Parser::parse() {
	ProgramNode* programTree = new ProgramNode();

	while (!end_of_file()) {
		programTree->nodes.push_back(parseCurrent());
	}
	return programTree;
}

ASTNode* Parser::parseCurrent() {
	const Token current = currentToken();

	switch (current.type) {
		case TokenType::DEF:
			return parseFunction();
		case TokenType::IDENTIFIER:
			return parseVarDeclaration();
		default:
			throw ZynkError{ ZynkErrorType::UnknownError, "Notimplemented.", &current.line};
	}
}

ASTNode* Parser::parseFunction() {
	FunctionNode* node = new FunctionNode;
	moveForward(); // To skip 'def' keyword

	node->name = currentToken().value;
	const size_t line = currentToken().line;

	consume({ TokenType::IDENTIFIER, node->name, line });
	consume({ TokenType::LBRACKET, "(", line });
	consume({ TokenType::RBRACKET, ")", line });
	consume({ TokenType::LBRACE, "{", line});

	while (currentToken().type != TokenType::RBRACE && !end_of_file()) {
		node->body.push_back(parseCurrent());
	}
	consume({ TokenType::RBRACE, "}", currentToken().line });
	return node;
}

ASTNode* Parser::parseVarDeclaration() {
	VariableDeclarationNode* node = new VariableDeclarationNode;
	const size_t currentLine = currentToken().line;
	node->name = currentToken().value;

	consume({ TokenType::IDENTIFIER, node->name, currentLine});
	consume({ TokenType::COLON, ":", currentLine });

	const Token varType = currentToken();

	switch (varType.type) {
		case TokenType::INT: 
		case TokenType::FLOAT: 
		case TokenType::STRING:
		case TokenType::BOOL:
			node->type = varType.value;
			consume(varType);
			break;
		default:
			throw ZynkError{ 
				ZynkErrorType::InvalidTypeError,
				"Expected: String, bool, float or int. Found: '" + varType.value + "' instead.",
				&currentLine,
			};
	}
	consume({ TokenType::EQUAL, "=", currentLine });
	node->value = currentToken().value;
	moveForward();
	consume({ TokenType::SEMICOLON, ";", currentLine });
	return node;
}

bool Parser::end_of_file() const {
	return position > tokens.size() || tokens[position].type == TokenType::END_OF_FILE;
}

void Parser::moveForward() {
	if (!end_of_file()) position++;
}

Token Parser::currentToken() const {
    if (!end_of_file()) return tokens[position];
    return { TokenType::END_OF_FILE, "EOF", tokens.back().line};
}

Token Parser::consume(Token expected) {
	const Token current = currentToken();
	if (current.type == expected.type) {
		moveForward();
		return current;
	};
	throw ZynkError{
		ZynkErrorType::SyntaxError,
		"Expected '" + expected.value + "', found: '" + current.value + "' instead.\n",
		&expected.line,
	};
}