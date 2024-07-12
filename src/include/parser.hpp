#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "token.hpp"
#include "ast.hpp"

class Parser {
private:
	const std::vector<Token> tokens;
	size_t position = 0;
public: // todo: consider moving some methods to private
	Parser(const std::vector<Token>& tokens);
	ProgramNode* parse();
	ASTNode* parseCurrent();
	void moveForward();

	bool endOfFile() const;
	bool isOperator(TokenType type) const;
	int getOperatorPriority(TokenType type) const;

	Token consume(Token expected);
	Token currentToken() const;

	ASTNode* parseFunction();
	ASTNode* parseVarDeclaration();
	ASTNode* parsePrint(bool newLine);

	ASTNode* parseExpression(int priority);
	ASTNode* parsePrimary();
	ASTNode* parseBinaryOperation(int priority);

};

#endif