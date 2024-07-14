#include "include/parser.hpp"
#include "include/errors.hpp"

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {};

ASTProgram* Parser::parse() {
	// Process to parse ProgramNode from provided tokens.
	ASTProgram* programTree = new ASTProgram();
	while (!endOfFile()) {
		programTree->body.push_back(parseCurrent());
	}
	return programTree;
}

ASTBase* Parser::parseCurrent() {
	// Parses current token.
	const Token current = currentToken();
	switch (current.type) {
		case TokenType::DEF:
			return parseFunction();
		case TokenType::IDENTIFIER:
			return parseVarDeclaration();
		case TokenType::PRINT:
			return parsePrint(false);
		case TokenType::PRINTLN:
			return parsePrint(true);
		default:
			throw ZynkError{ ZynkErrorType::UnknownError, "Notimplemented: " + current.value, &current.line};
	}
}

ASTBase* Parser::parseFunction() {
	const size_t currentLine = currentToken().line;
	consume({ TokenType::DEF, "def", currentLine });

	const std::string functionName = currentToken().value;
	ASTFunction* function = new ASTFunction(functionName);

	// Function name should be an identifier.
	consume({ TokenType::IDENTIFIER, functionName, currentLine });
	consume({ TokenType::LBRACKET, "(", currentLine });
	consume({ TokenType::RBRACKET, ")", currentLine });
	consume({ TokenType::LBRACE, "{", currentLine });

	while (currentToken().type != TokenType::RBRACE) {
		if (endOfFile()) break;
		function->body.push_back(parseCurrent());
	}
	consume({ TokenType::RBRACE, "}", currentToken().line });
	return function;
}

ASTBase* Parser::parseVarDeclaration() {
	const size_t currentLine = currentToken().line;
	const std::string varName = currentToken().value;
	std::string varType;

	consume({ TokenType::IDENTIFIER, varName, currentLine});
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
	ASTVariableDeclaration* varDeclaration = new ASTVariableDeclaration(
		varName, varType, parseExpression()
	);
	consume({ TokenType::SEMICOLON, ";", currentLine });
	return varDeclaration;
}

ASTBase* Parser::parsePrint(bool newLine) {
	const size_t currentLine = currentToken().line;
	ASTPrint* print;

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
			print = new ASTPrint(parseExpression(), newLine);
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

ASTBase* Parser::parseExpression() {
	const Token current = currentToken();
	ASTBase* primary;
	moveForward();

	switch (current.type) {
		case TokenType::INT:
		case TokenType::FLOAT:
		case TokenType::STRING:
		case TokenType::BOOL:
			primary = new ASTValue(current.value);
			break;
		case TokenType::IDENTIFIER:
			primary = new ASTVariable(current.value);
			break;
		default:
			throw ZynkError{ 
				ZynkErrorType::ExpressionError, 
				"Invalid expression. Expected value or variable, found: '" + current.value + "' instead.",
				&current.line };
	}

	while (isOperator(currentToken().type)) {
		const Token op = currentToken();
		moveForward();

		if (current.type == TokenType::STRING || currentToken().type == TokenType::STRING) {
			throw ZynkError( // todo: string concatenation.
				ZynkErrorType::ExpressionError,
				"Invalid expression. Cannot perform BinaryOperation on string type.",
				&current.line
			);
		}
		ASTBase* right = parseExpression();
		return new ASTBinaryOperation(primary, op.value, right);
	}
	return primary;
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

Token Parser::currentToken() const {
    if (!endOfFile()) return tokens[position];
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