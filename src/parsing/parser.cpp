#include "../include/errors.hpp"
#include "include/parser.hpp"

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {};

std::shared_ptr<ASTProgram> Parser::parse() {
	// Process to parse ProgramNode from provided tokens.
	std::shared_ptr<ASTProgram> programTree = std::make_shared<ASTProgram>();
	while (!endOfFile()) {
		programTree->body.push_back(parseCurrent());
	}
	return programTree;
}

std::shared_ptr<ASTBase> Parser::parseCurrent() {
	// Parses current token.
	const Token current = currentToken();
	switch (current.type) {
		case TokenType::DEF:
			return parseFunctionDeclaration();
		case TokenType::VARIABLE:
			return parseVariableDeclaration();
		case TokenType::PRINT:
			return parsePrint(false);
		case TokenType::PRINTLN:
			return parsePrint(true);
		case TokenType::IDENTIFIER:
			moveForward();
			if (currentToken().type == TokenType::LBRACKET) return parseFunctionCall();
			if (currentToken().type == TokenType::EQUAL) return parseVariableModify();
		default:
			throw ZynkError{ ZynkErrorType::UnknownError, "Notimplemented: " + current.value, &current.line };
	}
}

std::shared_ptr<ASTBase> Parser::parseFunctionDeclaration() {
	const size_t currentLine = currentToken().line;
	consume({ TokenType::DEF, "def", currentLine });

	const std::string functionName = currentToken().value;
	std::shared_ptr<ASTFunction> function = std::make_shared<ASTFunction>(functionName);

	// Function name should be an identifier.
	consume({ TokenType::IDENTIFIER, functionName, currentLine });
	consume({ TokenType::LBRACKET, "(", currentLine });
	consume({ TokenType::RBRACKET, ")", currentLine });
	consume({ TokenType::LBRACE, "{", currentLine });

	while (currentToken().type != TokenType::RBRACE && !endOfFile()) {
		function->body.push_back(parseCurrent());
	}
	consume({ TokenType::RBRACE, "}", currentToken().line });
	return function;
}

std::shared_ptr<ASTBase> Parser::parseFunctionCall() {
	position--; // We had to jump one position to see if it was a function call.
	const Token current = currentToken();

	consume({ TokenType::IDENTIFIER, current.value, current.line });
	consume({ TokenType::LBRACKET, "(", current.line });
	consume({ TokenType::RBRACKET, ")", current.line });
	consume({ TokenType::SEMICOLON, ";", current.line });
	return std::make_shared<ASTFunctionCall>(current.value);
}

std::shared_ptr<ASTBase> Parser::parseVariableDeclaration() {
	const size_t currentLine = currentToken().line;
	consume({ TokenType::VARIABLE, "var", currentLine });

	const std::string varName = currentToken().value;
	std::string varType;

	consume({ TokenType::IDENTIFIER, varName, currentLine });
	consume({ TokenType::COLON, ":", currentLine });

	const Token varTypeToken = currentToken();
	switch (varTypeToken.type) {
		case TokenType::INT:
		case TokenType::FLOAT:
		case TokenType::STRING:
		case TokenType::BOOL:
			varType = varTypeToken.value;
			consume(varTypeToken);
			break;
		default:
			throw ZynkError{
				ZynkErrorType::InvalidTypeError,
				"Expected: String, bool, float or int. Found: '" + varTypeToken.value + "' instead.",
				&currentLine,
			};
	}
	consume({ TokenType::EQUAL, "=", currentLine });
	const auto varDeclaration = std::make_shared<ASTVariableDeclaration>(
		varName, varType, parseExpression(0)
	);
	consume({ TokenType::SEMICOLON, ";", currentLine });
	return varDeclaration;
}

std::shared_ptr<ASTBase> Parser::parseVariableModify() {
	position--; // We had to jump one position to see if it was a var modify.
	const Token current = currentToken();

	consume({ TokenType::IDENTIFIER, current.value, current.line});
	consume({ TokenType::EQUAL, "=", current.line });
	std::shared_ptr<ASTBase> newValue = parseExpression(0);
	consume({ TokenType::SEMICOLON, ";", current.line });

	return std::make_shared<ASTVariableModify>(current.value, newValue);
}

std::shared_ptr<ASTBase> Parser::parsePrint(bool newLine) {
	const size_t currentLine = currentToken().line;
	std::shared_ptr<ASTPrint> print;

	if (newLine) {
		consume({ TokenType::PRINTLN, "println", currentLine });
	} else {
		consume({ TokenType::PRINT, "print", currentLine });
	}
	consume({ TokenType::LBRACKET, "(", currentLine });
	switch (currentToken().type) {
		case TokenType::STRING:
		case TokenType::INT:
		case TokenType::FLOAT:
		case TokenType::BOOL:
		case TokenType::IDENTIFIER:
			print = std::make_shared<ASTPrint>(parseExpression(0), newLine);
			break;
		default:
			throw ZynkError{
				ZynkErrorType::ExpressionError,
				"Invalid print expression. Expected value or variable, found: '" + currentToken().value + "' instead.",
				&currentLine,
			};
	}
	consume({ TokenType::RBRACKET, ")", currentLine });
	consume({ TokenType::SEMICOLON, ";", currentLine });
	return print;
}

std::shared_ptr<ASTBase> Parser::parseExpression(int priority) {
	const Token leftToken = currentToken();
	std::shared_ptr<ASTBase> left = parsePrimaryExpression();

	while (!endOfFile() && isOperator(currentToken().type)) {
		Token op = currentToken();
		int opPriority = getPriority(op.type);
		if (opPriority <= priority) break;
		moveForward();

		if (leftToken.type == TokenType::STRING || currentToken().type == TokenType::STRING) {
			throw ZynkError(
				ZynkErrorType::ExpressionError,
				"Invalid expression. Cannot perform BinaryOperation on string type.",
				&leftToken.line
			);
		}
		std::shared_ptr<ASTBase> right = parseExpression(opPriority);
		left = std::make_shared<ASTBinaryOperation>(left, op.value, right);
	}
	return left;
}

std::shared_ptr<ASTBase> Parser::parsePrimaryExpression() {
	const Token current = currentToken();
	moveForward();

	switch (current.type) {
		case TokenType::INT:
		case TokenType::FLOAT:
		case TokenType::STRING:
		case TokenType::BOOL:
			return std::make_shared<ASTValue>(current.value);
		case TokenType::IDENTIFIER:
			return std::make_shared<ASTVariable>(current.value);
		default:
			throw ZynkError{
				ZynkErrorType::ExpressionError,
				"Invalid expression. Expected value or variable, found: '" + current.value + "' instead.",
				&current.line
			};
	}
}

bool Parser::endOfFile() const {
	return position > tokens.size() || tokens[position].type == TokenType::END_OF_FILE;
}

bool Parser::isOperator(TokenType type) const {
	switch (type) {
		case TokenType::ADD:
		case TokenType::SUBTRACT:
		case TokenType::MULTIPLY:
		case TokenType::DIVIDE:
			return true;
		default:
			return false;
	}
}

void Parser::moveForward() {
	if (!endOfFile()) position++;
}

int Parser::getPriority(TokenType type) const {
	switch (type) {
		case TokenType::MULTIPLY:
		case TokenType::DIVIDE:
			return 2;
		case TokenType::ADD:
		case TokenType::SUBTRACT:
			return 1;
		default:
			return 0;
	}
}

Token Parser::currentToken() const {
	if (!endOfFile()) return tokens[position];
	return { TokenType::END_OF_FILE, "EOF", tokens.back().line };
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