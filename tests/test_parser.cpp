#include <gtest/gtest.h>

#include "../src/include/parser.hpp"
#include "../src/include/lexer.hpp"
#include "../src/include/errors.hpp"

TEST(ParserTest, parseVariable) {
	Lexer lexer("a: int = 1;");
	const std::vector<Token> tokens = lexer.tokenize();
	
	Parser parser(tokens);
	const ASTProgram* program = parser.parse();
	
	ASSERT_TRUE(program->type == ASTType::Program);
	ASSERT_TRUE(program->body.size() == 1);
	ASSERT_TRUE(program->body.front()->type == ASTType::VariableDeclaration);

	const ASTVariableDeclaration* variable = static_cast<ASTVariableDeclaration*>(program->body.front());
	ASSERT_TRUE(variable->type == "int");
	ASSERT_TRUE(variable->name == "a");
	const ASTValue* value = static_cast<ASTValue*>(variable->value);
	ASSERT_TRUE(value->value == "1");

	delete program;
}

TEST(ParserTest, parseEmptyFunction) {
	Lexer lexer("def main(){\n}");
	const std::vector<Token> tokens = lexer.tokenize();

	Parser parser(tokens);
	const ASTProgram* program = parser.parse();

	ASSERT_TRUE(program->type == ASTType::Program);
	ASSERT_TRUE(program->body.size() == 1);
	ASSERT_TRUE(program->body.front()->type == ASTType::Function);

	const ASTFunction* function = static_cast<ASTFunction*>(program->body.front());
	ASSERT_TRUE(function->name == "main");
	ASSERT_TRUE(function->body.size() == 0);

	delete program;
}

TEST(ParserTest, ShouldThrowSyntaxError) {
	Lexer lexer("def main()\n}"); // Missing '{'
	const std::vector<Token> tokens = lexer.tokenize();

	Parser parser(tokens);
	try {
		parser.parse();
		FAIL() << "Expected ZynkError thrown.";
	}
	catch (const ZynkError& error) {
		EXPECT_EQ(error.base_type, ZynkErrorType::SyntaxError);
	}
	catch (const std::exception& error) {
		FAIL() << "Unexpected exception type: " << error.what();
	}
}
TEST(ParserTest, ShouldThrowInvalidTypeError) {
	Lexer lexer("a: abc = 10;"); // Invalid type 'abc'
	const std::vector<Token> tokens = lexer.tokenize();

	Parser parser(tokens);
	try {
		parser.parse();
		FAIL() << "Expected ZynkError thrown.";
	}
	catch (const ZynkError& error) {
		EXPECT_EQ(error.base_type, ZynkErrorType::InvalidTypeError);
	}
	catch (const std::exception& error) {
		FAIL() << "Unexpected exception type: " << error.what();
	}
}
