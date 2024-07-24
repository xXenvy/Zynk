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

	std::shared_ptr<ASTBase> parseFunctionDeclaration();
	std::shared_ptr<ASTBase> parseFunctionCall();
	std::shared_ptr<ASTBase> parseVariableDeclaration();
	std::shared_ptr<ASTBase> parseVariableModify();
	std::shared_ptr<ASTBase> parsePrint(bool newLine);
	std::shared_ptr<ASTBase> parseExpression(int priority);
	std::shared_ptr<ASTBase> parsePrimaryExpression();
public:
	Parser(const std::vector<Token>& tokens);
	std::shared_ptr<ASTProgram> parse();
	std::shared_ptr<ASTBase> parseCurrent();
};
#endif // PARSER_H