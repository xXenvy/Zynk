#ifndef PARSER_H
#define PARSER_H

#include "token.hpp"
#include "ast.hpp"

class Parser {
private:
	const std::vector<Token> tokens;
	size_t position = 0;

	void moveForward();

	bool endOfFile() const;
	bool isOperator(TokenType type) const;

	Token consume(Token expected);
	Token currentToken() const;

	ASTBase* parseFunction();
	ASTBase* parseVarDeclaration();
	ASTBase* parsePrint(bool newLine);
	ASTBase* parseExpression();
public:
	Parser(const std::vector<Token>& tokens);
	ASTProgram* parse();
	ASTBase* parseCurrent();
};
#endif // PARSER_H