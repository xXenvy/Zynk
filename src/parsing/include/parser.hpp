#ifndef PARSER_H
#define PARSER_H

#include "token.hpp"
#include "ast.hpp"

class Parser {
private:
	const std::vector<Token> tokens;
	size_t position = 0;

	void moveForward();
	int getPriority(TokenType type) const;

	bool endOfFile() const;
	bool isOperator(TokenType type) const;

	Token consume(Token expected);
	Token currentToken() const;

	ASTValueType parseValueType() const;
	std::unique_ptr<ASTBase> parseFunctionDeclaration();
	std::unique_ptr<ASTBase> parseFunctionCall(bool isFinalInstruction = true);
	std::unique_ptr<ASTBase> parseVariableDeclaration();
	std::unique_ptr<ASTBase> parseVariableModify();
	std::unique_ptr<ASTBase> parsePrint(bool newLine);
	std::unique_ptr<ASTBase> parseRead(bool isFinalInstruction = true);
	std::unique_ptr<ASTBase> parseTypeCast(TokenType type);
	std::unique_ptr<ASTBase> parsePrimaryExpression();
	std::unique_ptr<ASTBase> parseIfStatement();
	std::unique_ptr<ASTBase> parseReturnStatement();
public:
	Parser(const std::vector<Token>& tokens);
	std::unique_ptr<ASTProgram> parse();
	std::unique_ptr<ASTBase> parseCurrent();
	std::unique_ptr<ASTBase> parseExpression(int priority);
};
#endif // PARSER_H