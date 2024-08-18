#include "../common/include/errors.hpp"
#include "include/parser.hpp"

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {};

std::unique_ptr<ASTProgram> Parser::parse() {
	// Process to parse Program AST from provided tokens.
	std::unique_ptr<ASTProgram> programTree = std::make_unique<ASTProgram>();
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
		case TokenType::PRINTLN:
			return parsePrint(current.type == TokenType::PRINTLN);
		case TokenType::READINPUT:
			return parseRead();
		case TokenType::CONDITION:
			return parseIfStatement();
		case TokenType::END_OF_FILE:
			return nullptr;
		case TokenType::RETURN:
			return parseReturnStatement();
		case TokenType::IDENTIFIER: {
			moveForward();
			if (currentToken().type == TokenType::LBRACKET) return parseFunctionCall();
			if (currentToken().type == TokenType::ASSIGN) return parseVariableModify();
			return std::make_unique<ASTVariable>(current.value, current.line);
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
	std::vector<std::unique_ptr<ASTBase>> funcArgs;

	// Function name should be an identifier.
	consume({ TokenType::IDENTIFIER, functionName, currentLine });
	consume({ TokenType::LBRACKET, "(", currentLine });

	while (currentToken().type != TokenType::RBRACKET && !endOfFile()) {
		funcArgs.push_back(parseFunctionArgument());
		if(currentToken().type != TokenType::RBRACKET) consume({ TokenType::COMMA, ",", currentLine });
	}

	consume({ TokenType::RBRACKET, ")", currentLine });
	consume({ TokenType::SUBTRACT, "-", currentLine });
	consume({ TokenType::GREATER_THAN, ">", currentLine });

	std::unique_ptr<ASTFunction> function = std::make_unique<ASTFunction>(
		functionName, parseValueType(), currentLine
	);
	function->arguments = std::move(funcArgs);

	moveForward();
	consume({ TokenType::LBRACE, "{", currentLine });

	while (currentToken().type != TokenType::RBRACE && !endOfFile()) {
		function->body.push_back(parseCurrent());
	}
	consume({ TokenType::RBRACE, "}", currentToken().line });
	return function;
}

std::unique_ptr<ASTBase> Parser::parseFunctionCall(bool isFinalInstruction) {
	position--; // We had to jump one position to see if it was a function call.
	const Token current = currentToken();
	std::vector<std::unique_ptr<ASTBase>> args;

	consume({ TokenType::IDENTIFIER, current.value, current.line });
	consume({ TokenType::LBRACKET, "(", current.line });

	while (currentToken().type != TokenType::RBRACKET && !endOfFile()) {
		args.push_back(parseExpression(0));
		if (currentToken().type != TokenType::RBRACKET) {
			consume({ TokenType::COMMA, ",", current.line });
		}
	}

	consume({ TokenType::RBRACKET, ")", current.line });
	if(isFinalInstruction) consume({ TokenType::SEMICOLON, ";", current.line });

	auto funcCall = std::make_unique<ASTFunctionCall>(current.value, current.line);
	funcCall->arguments = std::move(args);
	return funcCall;
}

std::unique_ptr<ASTBase> Parser::parseFunctionArgument() {
	const size_t currentLine = currentToken().line;
	const std::string argumentName = currentToken().value;

	consume({ TokenType::IDENTIFIER, argumentName, currentLine });
	consume({ TokenType::COLON, ":", currentLine });
	const ASTValueType argumentType = parseValueType();

	moveForward();
	return std::make_unique<ASTFunctionArgument>(argumentName, argumentType, currentLine);
}

std::unique_ptr<ASTBase> Parser::parseReturnStatement() {
	moveForward();

	const size_t currentLine = currentToken().line;
	if (currentToken().type == TokenType::SEMICOLON) {
		consume({ TokenType::SEMICOLON, ";", currentLine });
		return std::make_unique<ASTReturn>(nullptr, currentLine);
	}

	auto returnAST = std::make_unique<ASTReturn>(parseExpression(0), currentLine);
	consume({ TokenType::SEMICOLON, ";", currentLine });
	return returnAST;
}

std::unique_ptr<ASTBase> Parser::parseVariableDeclaration() {
	const size_t currentLine = currentToken().line;
	consume({ TokenType::VARIABLE, "var", currentLine });
	const std::string varName = currentToken().value;

	consume({ TokenType::IDENTIFIER, varName, currentLine });
	consume({ TokenType::COLON, ":", currentLine });

	const ASTValueType varType = parseValueType();
	moveForward();

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

std::unique_ptr<ASTBase> Parser::parseRead(bool isFinalInstruction) {
	const size_t currentLine = currentToken().line;
	consume({ TokenType::READINPUT, "readInput", currentLine });
	consume({ TokenType::LBRACKET, "(", currentLine });

	std::unique_ptr<ASTReadInput> read;
	if (currentToken().type == TokenType::RBRACKET) {
		read = std::make_unique<ASTReadInput>(nullptr, currentLine);
	} else {
		read = std::make_unique<ASTReadInput>(parseExpression(0), currentLine);
	}
	consume({ TokenType::RBRACKET, ")", currentLine });
	if (isFinalInstruction) consume({ TokenType::SEMICOLON, ";", currentLine });
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

		std::unique_ptr<ASTBase> right = parseExpression(opPriority);
		switch (op.type) {
			case TokenType::EQUAL:
			case TokenType::NOT_EQUAL:
			case TokenType::GREATER_THAN:
			case TokenType::LESS_THAN:
			case TokenType::GREATER_OR_EQUAL:
			case TokenType::LESS_OR_EQUAL:
				left = std::make_unique<ASTComparisonOperation>(
					std::move(left), op.value, std::move(right), op.line
				);
				break;
			case TokenType::OR:
				left = std::make_unique<ASTOrOperation>(
					std::move(left), std::move(right), op.line
				);
				break;
			case TokenType::AND:
				left = std::make_unique<ASTAndOperation>(
					std::move(left), std::move(right), op.line
				);
				break;
			default: {
				if (leftToken.type == TokenType::STRING || currentToken().type == TokenType::STRING) {
					throw ZynkError(
						ZynkErrorType::ExpressionError,
						"Binary operations are not allowed with strings.",
						leftToken.line
					);
				}
				left = std::make_unique<ASTBinaryOperation>(
					std::move(left), op.value, std::move(right), op.line
				);
				break;
			}
		}
	}
	return left;
}

std::unique_ptr<ASTBase> Parser::parsePrimaryExpression() {
	const Token current = currentToken();
	const size_t currentLine = current.line;

	if (current.type == TokenType::LBRACKET) {
		moveForward();
		std::unique_ptr<ASTBase> expr = parseExpression(0);
		consume({ TokenType::RBRACKET, ")", currentLine });
		return expr;
	}

	// Handles case for negative number.
	if (current.type == TokenType::SUBTRACT) {
		moveForward();
		const Token numberToken = currentToken();

		if (numberToken.type == TokenType::INT || numberToken.type == TokenType::FLOAT) {
			moveForward();
			return std::make_unique<ASTValue>(
				"-" + numberToken.value,
				numberToken.type == TokenType::INT ? ASTValueType::Integer : ASTValueType::Float,
				currentLine
			);
		}
		throw ZynkError(
			ZynkErrorType::SyntaxError,
			"Expected a number after '-' for negative value.",
			currentLine
		);
	}
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
		case TokenType::IDENTIFIER: {
			if (currentToken().type == TokenType::STRING && current.value == "f") {
				const std::string fStringValue = currentToken().value;
				moveForward();
				return std::make_unique<ASTFString>(fStringValue, currentLine);
			}
			if (currentToken().type == TokenType::LBRACKET) {
				return parseFunctionCall(false);
			}
			return std::make_unique<ASTVariable>(current.value, currentLine);
		}
		case TokenType::READINPUT: {
			position--;
			std::unique_ptr<ASTBase> expr = parseRead(false);
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

void Parser::moveForward() {
	if (!endOfFile()) position++;
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
		case TokenType::OR:
		case TokenType::AND:
			return true;
		default:
			return false;
	}
}

int Parser::getPriority(TokenType type) const {
	switch (type) {
		case TokenType::MULTIPLY:
		case TokenType::DIVIDE:
			return 2;
		default:
			return isOperator(type) ? 1 : 0;
	}
}

ASTValueType Parser::parseValueType() const {
	const Token current = currentToken();
	switch (current.type) {
		case TokenType::INT:
			return ASTValueType::Integer;
		case TokenType::FLOAT:
			return ASTValueType::Float;
		case TokenType::STRING:
			return ASTValueType::String;
		case TokenType::BOOL:
			return ASTValueType::Bool;
		case TokenType::NONE:
			return ASTValueType::None;
		default:
			throw ZynkError(
				ZynkErrorType::TypeError,
				"Expected type 'int', 'float', 'string', 'null, or 'bool', but found: '" + current.value + "'.",
				current.line
			);
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