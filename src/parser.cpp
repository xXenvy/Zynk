#include "include/parser.hpp"
#include "include/errors.hpp"

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {};

ProgramNode* Parser::parse() {
	ProgramNode* programTree = new ProgramNode();

	while (!endOfFile()) {
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
		case TokenType::PRINT:
			return parsePrint(false);
		case TokenType::PRINTLN:
			return parsePrint(true);
		default:
			throw ZynkError{ ZynkErrorType::UnknownError, "Notimplemented " + current.value, &current.line};
	}
}

ASTNode* Parser::parseExpression(int priority = 0) {
	return parseBinaryOperation(priority);
}

ASTNode* Parser::parseBinaryOperation(int priority) {
	ASTNode* left = parsePrimary();

	while (!endOfFile() && isOperator(currentToken().type) && getOperatorPriority(currentToken().type) >= priority) {
		Token op = currentToken();
		moveForward();

		ASTNode* right = parseExpression(getOperatorPriority(op.type) + 1);

		left = new BinaryOperationNode(left, op.value, right);
	}

	return left;
}

ASTNode* Parser::parsePrimary() {
	const Token current = currentToken();
	moveForward();

	switch (current.type) {
		case TokenType::INT:
		case TokenType::FLOAT:
		case TokenType::STRING:
		case TokenType::BOOL:
			return new ValueNode(current.value);
		case TokenType::IDENTIFIER:
			return new VariableNode(current.value);
		default:
			throw ZynkError{ ZynkErrorType::SyntaxError, "Invalid expression", &current.line };
	}
}

ASTNode* Parser::parseFunction() {
	FunctionNode* function = new FunctionNode;
	const size_t line = currentToken().line;

	consume({ TokenType::DEF, "def", line });
	function->name = currentToken().value; // Function name.

	consume({ TokenType::IDENTIFIER, function->name, line });
	consume({ TokenType::LBRACKET, "(", line });
	consume({ TokenType::RBRACKET, ")", line });
	consume({ TokenType::LBRACE, "{", line});

	while (currentToken().type != TokenType::RBRACE && !endOfFile()) {
		function->body.push_back(parseCurrent());
	}
	consume({ TokenType::RBRACE, "}", currentToken().line });
	return function;
}

ASTNode* Parser::parseVarDeclaration() {
	VariableDeclarationNode* var = new VariableDeclarationNode{};
	const size_t currentLine = currentToken().line;
	var->name = currentToken().value;

	consume({ TokenType::IDENTIFIER, var->name, currentLine});
	consume({ TokenType::COLON, ":", currentLine });

	const Token varType = currentToken();

	switch (varType.type) {
		case TokenType::INT: 
		case TokenType::FLOAT: 
		case TokenType::STRING:
		case TokenType::BOOL:
			var->type = varType.value;
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
	var->value = parseExpression();
	consume({ TokenType::SEMICOLON, ";", currentLine });
	return var;
}

ASTNode* Parser::parsePrint(bool newLine) {
	PrintNode* print = new PrintNode;
	const size_t currentLine = currentToken().line;
	print->newLine = newLine;

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
			print->expression = parseExpression();
			break;
		default:
			throw ZynkError{
				ZynkErrorType::InvalidTypeError,
				"Invalid print value. Expected: String, bool, float or int.",
				&currentLine,
			};
	}
	consume({ TokenType::RBRACKET, ")", currentLine });
	consume({ TokenType::SEMICOLON, ";", currentLine });
	return print;
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

int Parser::getOperatorPriority(TokenType type) const {
	switch (type) {
		case TokenType::ADD:
		case TokenType::SUBTRACT:
			return 1;
		case TokenType::MULTIPLY:
		case TokenType::DIVIDE:
			return 2;
		default:
			return 0;
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