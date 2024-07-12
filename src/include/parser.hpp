#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "token.hpp"
#include "ast.hpp"

class Parser {
private:
	const std::vector<Token> tokens;
	size_t position = 0;
public:
	Parser(const std::vector<Token>& tokens);
	ProgramNode* parse();
	ASTNode* parseCurrent();

	bool end_of_file() const;
	void moveForward();

	Token consume(Token expected);
	Token currentToken() const;

	ASTNode* parseFunction();
	ASTNode* parseVarDeclaration();

};

#endif