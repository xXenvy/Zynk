#include "../common/include/errors.hpp"
#include "include/parser.hpp"

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {};

std::unique_ptr<ASTProgram> Parser::parse() {
	// Process to parse Program AST from provided tokens.
	std::unique_ptr<ASTProgram> programTree = std::make_unique<ASTProgram>(1);
	while (!endOfFile()) {
		programTree->body.push_back(parseCurrent());
	}
	return programTree;
}

std::unique_ptr<ASTBase> Parser::parseCurrent() {
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
		case TokenType::READLINE:
			return parseRead();
		case TokenType::CONDITION:
			return parseIfStatement();
		case TokenType::END_OF_FILE:
			return nullptr;
		case TokenType::IDENTIFIER: {
			moveForward();
			if (currentToken().type == TokenType::LBRACKET) return parseFunctionCall();
			if (currentToken().type == TokenType::ASSIGN) return parseVariableModify();
			[[fallthrough]];
		}
		default:
			throw ZynkError(
				ZynkErrorType::SyntaxError,
				"Unexpected token: '" + current.value + "'.", 
				current.line
			);
	}
}

std::unique_ptr<ASTBase> Parser::parseFunctionDeclaration() {
	const size_t currentLine = currentToken().line;
	consume({ TokenType::DEF, "def", currentLine });

	const std::string functionName = currentToken().value;
	std::unique_ptr<ASTFunction> function = std::make_unique<ASTFunction>(functionName, currentLine);

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

std::unique_ptr<ASTBase> Parser::parseFunctionCall() {
	position--; // We had to jump one position to see if it was a function call.
	const Token current = currentToken();

	consume({ TokenType::IDENTIFIER, current.value, current.line });
	consume({ TokenType::LBRACKET, "(", current.line });
	consume({ TokenType::RBRACKET, ")", current.line });
	consume({ TokenType::SEMICOLON, ";", current.line });
	return std::make_unique<ASTFunctionCall>(current.value, current.line);
}

std::unique_ptr<ASTBase> Parser::parseVariableDeclaration() {
	const size_t currentLine = currentToken().line;
	consume({ TokenType::VARIABLE, "var", currentLine });

	const std::string varName = currentToken().value;
	ASTValueType varType;

	consume({ TokenType::IDENTIFIER, varName, currentLine });
	consume({ TokenType::COLON, ":", currentLine });

	const Token varTypeToken = currentToken();
	switch (varTypeToken.type) {
		case TokenType::INT:
			varType = ASTValueType::Integer;
			break;
		case TokenType::FLOAT:
			varType = ASTValueType::Float;
			break;
		case TokenType::STRING:
			varType = ASTValueType::String;
			break;
		case TokenType::BOOL:
			varType = ASTValueType::Bool;
			break;
		default:
			throw ZynkError(
				ZynkErrorType::TypeError,
				"Expected type 'int', 'float', 'string', or 'bool', but found: '" + varTypeToken.value + "'.",
				currentLine
			);
	}
	consume(varTypeToken);
	if (currentToken().type == TokenType::SEMICOLON) {
		consume({ TokenType::SEMICOLON, ";", currentLine });
		return std::make_unique<ASTVariableDeclaration>(
			varName, varType, nullptr, currentLine
		);
	}
	consume({ TokenType::ASSIGN, "=", currentLine });
	auto varDeclaration = std::make_unique<ASTVariableDeclaration>(
		varName, varType, parseExpression(0), currentLine
	);
	consume({ TokenType::SEMICOLON, ";", currentLine });
	return varDeclaration;
}

std::unique_ptr<ASTBase> Parser::parseVariableModify() {
	position--; // We had to jump one position to see if it was a var modify.
	const Token current = currentToken();

	consume({ TokenType::IDENTIFIER, current.value, current.line});
	consume({ TokenType::ASSIGN, "=", current.line });
	std::unique_ptr<ASTBase> newValue = parseExpression(0);
	consume({ TokenType::SEMICOLON, ";", current.line });
	return std::make_unique<ASTVariableModify>(current.value, std::move(newValue), current.line);
}

std::unique_ptr<ASTBase> Parser::parsePrint(bool newLine) {
	const size_t currentLine = currentToken().line;

	if (newLine) consume({ TokenType::PRINTLN, "println", currentLine });
	else consume({ TokenType::PRINT, "print", currentLine });
	consume({ TokenType::LBRACKET, "(", currentLine });

	auto print = std::make_unique<ASTPrint>(parseExpression(0), newLine, currentLine);
	consume({ TokenType::RBRACKET, ")", currentLine });
	consume({ TokenType::SEMICOLON, ";", currentLine });
	return print;
}

std::unique_ptr<ASTBase> Parser::parseRead() {
	const size_t currentLine = currentToken().line;
	consume({ TokenType::READLINE, "read", currentLine });
	consume({ TokenType::LBRACKET, "(", currentLine });

	std::unique_ptr<ASTReadLine> read;
	if (currentToken().type == TokenType::RBRACKET) {
		read = std::make_unique<ASTReadLine>(nullptr, currentLine);
	} else {
		read = std::make_unique<ASTReadLine>(parseExpression(0), currentLine);
	}
	consume({ TokenType::RBRACKET, ")", currentLine });
	consume({ TokenType::SEMICOLON, ";", currentLine });
	return read;
}

std::unique_ptr<ASTBase> Parser::parseExpression(int priority) {
	const Token leftToken = currentToken();
	std::unique_ptr<ASTBase> left = parsePrimaryExpression();

	while (!endOfFile() && isOperator(currentToken().type)) {
		Token op = currentToken();
		int opPriority = getPriority(op.type);
		if (opPriority <= priority) break;
		moveForward();

		if (leftToken.type == TokenType::STRING || currentToken().type == TokenType::STRING) {
			throw ZynkError(
				ZynkErrorType::ExpressionError,
				"Binary operations are not allowed with strings.",
				leftToken.line
			);
		}
		std::unique_ptr<ASTBase> right = parseExpression(opPriority);
		left = std::make_unique<ASTBinaryOperation>(
			std::move(left), op.value, std::move(right), op.line
		);
	}
	return left;
}

std::unique_ptr<ASTBase> Parser::parsePrimaryExpression() {
	const Token current = currentToken();
	const size_t currentLine = current.line;
	moveForward();
	const bool isTypeCast = currentToken().type == TokenType::LBRACKET;

	switch (current.type) {
		case TokenType::INT:
			if (isTypeCast) return parseTypeCast(TokenType::INT);
			return std::make_unique<ASTValue>(current.value, ASTValueType::Integer, currentLine);
		case TokenType::FLOAT:
			if (isTypeCast) return parseTypeCast(TokenType::FLOAT);
			return std::make_unique<ASTValue>(current.value, ASTValueType::Float, currentLine);
		case TokenType::STRING:
			if (isTypeCast) return parseTypeCast(TokenType::STRING);
			return std::make_unique<ASTValue>(current.value, ASTValueType::String, currentLine);
		case TokenType::BOOL:
			if (isTypeCast) return parseTypeCast(TokenType::BOOL);
			return std::make_unique<ASTValue>(current.value, ASTValueType::Bool, currentLine);
		case TokenType::NONE:
			return std::make_unique<ASTValue>(current.value, ASTValueType::None, currentLine);
		case TokenType::IDENTIFIER:
			return std::make_unique<ASTVariable>(current.value, currentLine);
		case TokenType::READLINE: {
			position--;
			std::unique_ptr<ASTBase> expr = parseRead();
			position--;
			return expr;
		}
		default:
			throw ZynkError(
				ZynkErrorType::ExpressionError,
				"Unexpected token '" + current.value + "' while parsing expression.",
				currentLine
			);
	}
}

std::unique_ptr<ASTBase> Parser::parseIfStatement() {
	size_t currentLine = currentToken().line;
	consume({ TokenType::CONDITION, "if", currentLine });
	consume({ TokenType::LBRACKET, "(", currentLine });

	auto condition = std::make_unique<ASTCondition>(parseExpression(0), currentLine);
	consume({ TokenType::RBRACKET, ")", currentLine });
	bool shortCondition = currentToken().type != TokenType::LBRACE;

	if(!shortCondition) consume({ TokenType::LBRACE, "{", currentLine });
	while (currentToken().type != TokenType::RBRACE && !endOfFile()) {
		condition->body.push_back(parseCurrent());
		if (shortCondition) break;
	}

	if (!shortCondition) consume({ TokenType::RBRACE, "}", currentLine });
	if (currentToken().type != TokenType::ELSE) return condition;
	currentLine = currentToken().line;

	// Parsing else block.
	consume({ TokenType::ELSE, "else", currentLine });
	bool shortElse = currentToken().type != TokenType::LBRACE;

	if (!shortElse) consume({ TokenType::LBRACE, "{", currentLine });
	while (currentToken().type != TokenType::RBRACE && !endOfFile()) {
		condition->elseBody.push_back(parseCurrent());
		if (shortElse) break;
	}
	if (!shortElse) consume({ TokenType::RBRACE, "}", currentLine });
	return condition;
}

std::unique_ptr<ASTBase> Parser::parseTypeCast(TokenType type) {
	const size_t currentLine = currentToken().line;
	consume({ TokenType::LBRACKET, "(", currentLine });
	std::unique_ptr<ASTBase> value = parseExpression(0);
	consume({ TokenType::RBRACKET, ")", currentLine });

	ASTValueType castType;
	switch (type) {
		case TokenType::INT:
			castType = ASTValueType::Integer;
			break;
		case TokenType::FLOAT:
			castType = ASTValueType::Float;
			break;
		case TokenType::STRING:
			castType = ASTValueType::String;
			break;
		case TokenType::BOOL:
			castType = ASTValueType::Bool;
			break;
		default:
			throw ZynkError(
				ZynkErrorType::SyntaxError,
				"Invalid type cast. Expected 'int', 'float', 'string', or 'bool', but found an unrecognized type.",
				currentLine
			);
	}
	return std::make_unique<ASTTypeCast>(std::move(value), castType, currentLine);
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
		case TokenType::GREATER_THAN:
		case TokenType::LESS_THAN:
		case TokenType::GREATER_OR_EQUAL:
		case TokenType::LESS_OR_EQUAL:
		case TokenType::EQUAL:
		case TokenType::NOT_EQUAL:
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
			if (isOperator(type)) return 1;
			return 0;
	}
}

Token Parser::currentToken() const {
	if (!endOfFile()) return tokens[position];
	return { TokenType::END_OF_FILE, "End of file", tokens.back().line };
}

Token Parser::consume(Token expected) {
	const Token current = currentToken();
	if (current.type == expected.type) {
		moveForward();
		return current;
	};
	throw ZynkError{
		ZynkErrorType::SyntaxError,
		"Expected '" + expected.value + "', found: '" + current.value + "' instead.",
		expected.line,
	};
}