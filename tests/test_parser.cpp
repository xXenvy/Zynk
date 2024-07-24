#include <gtest/gtest.h>

#include "../src/include/parser.hpp"
#include "../src/include/lexer.hpp"
#include "../src/include/errors.hpp"

TEST(ParserTest, parseVariableDeclaration) {
	Lexer lexer("var a: int = 1;");
	const std::vector<Token> tokens = lexer.tokenize();
	
	Parser parser(tokens);
	const auto program = parser.parse();
	
	ASSERT_TRUE(program->type == ASTType::Program);
	ASSERT_TRUE(program->body.size() == 1);
	ASSERT_TRUE(program->body.front()->type == ASTType::VariableDeclaration);

	const auto variable = std::static_pointer_cast<ASTVariableDeclaration>(program->body.front());

	ASSERT_TRUE(variable->type == "int");
	ASSERT_TRUE(variable->name == "a");
	ASSERT_TRUE(std::static_pointer_cast<ASTValue>(variable->value)->value == "1");
}

TEST(ParserTest, parseMultipleVariableDeclarations) {
	Lexer lexer("var a: int = 1;\nvar b: float = 1.0;\n"
		"var c: bool = true; \nvar d: String = \"Test\";");
	const std::vector<Token> tokens = lexer.tokenize();

	Parser parser(tokens);
	const auto program = parser.parse();

	ASSERT_TRUE(program->type == ASTType::Program);
	ASSERT_TRUE(program->body.size() == 4);

	for (const std::shared_ptr<ASTBase> var : program->body) {
		ASSERT_EQ(var->type, ASTType::VariableDeclaration);
	}
}

TEST(ParserTest, parseFunctionDeclaration) {
	Lexer lexer("def main(){\n    println(10);\n}");
	const std::vector<Token> tokens = lexer.tokenize();

	Parser parser(tokens);
	const auto program = parser.parse();

	ASSERT_TRUE(program->type == ASTType::Program);
	ASSERT_TRUE(program->body.size() == 1);
	ASSERT_TRUE(program->body.front()->type == ASTType::FunctionDeclaration);

	const auto function = std::static_pointer_cast<ASTFunction>(program->body.front());
	ASSERT_TRUE(function->name == "main");
	ASSERT_TRUE(function->body.size() == 1);
	ASSERT_TRUE(function->body.front()->type == ASTType::Print);

	const auto print = std::static_pointer_cast<ASTPrint>(function->body.front());
	ASSERT_TRUE(print->newLine);
	const auto printValue = std::static_pointer_cast<ASTValue>(print->expression);
	ASSERT_TRUE(printValue->type == ASTType::Value);
	ASSERT_TRUE(printValue->value == "10");
}

TEST(ParserTest, parseEmptyFunction) {
	Lexer lexer("def main(){\n}");
	const std::vector<Token> tokens = lexer.tokenize();

	Parser parser(tokens);
	const auto program = parser.parse();

	ASSERT_TRUE(program->type == ASTType::Program);
	ASSERT_TRUE(program->body.size() == 1);
	ASSERT_TRUE(program->body.front()->type == ASTType::FunctionDeclaration);

	const auto function = std::static_pointer_cast<ASTFunction>(program->body.front());
	ASSERT_TRUE(function->name == "main");
	ASSERT_TRUE(function->body.size() == 0);
}

TEST(ParserTest, parseVariableDeclarationWithBinaryOperation) {
	// 'b' is not defined, but parser doesn't have that context.
	Lexer lexer("var a: float = b + 1.0;");
	const std::vector<Token> tokens = lexer.tokenize();

	Parser parser(tokens);
	const auto program = parser.parse();

	ASSERT_TRUE(program->type == ASTType::Program);
	ASSERT_TRUE(program->body.size() == 1);
	ASSERT_TRUE(program->body.front()->type == ASTType::VariableDeclaration);

	const auto var = std::static_pointer_cast<ASTVariableDeclaration>(program->body.front());
	ASSERT_TRUE(var->name == "a");
	ASSERT_TRUE(var->type == "float");
	const auto operation = std::static_pointer_cast<ASTBinaryOperation>(var->value);
	ASSERT_TRUE(operation->op == "+");
	
	const auto left = std::static_pointer_cast<ASTVariable>(operation->left);
	const auto right = std::static_pointer_cast<ASTValue>(operation->right);
	ASSERT_TRUE(left->name == "b" && right->value == "1.0");
}

TEST(ParserTest, parseVariableDeclarationWithComplexExpression) {
	// 'b' is not defined, but parser doesn't have that context.
	Lexer lexer("var a: float = 1 + 5 * b;");
	const std::vector<Token> tokens = lexer.tokenize();

	Parser parser(tokens);
	const auto program = parser.parse();

	ASSERT_TRUE(program->type == ASTType::Program);
	ASSERT_TRUE(program->body.size() == 1);
	ASSERT_TRUE(program->body.front()->type == ASTType::VariableDeclaration);

	const auto var = std::static_pointer_cast<ASTVariableDeclaration>(program->body.front());
	ASSERT_TRUE(var->name == "a");
	ASSERT_TRUE(var->type == "float");

	const auto operation = std::static_pointer_cast<ASTBinaryOperation>(var->value);
	ASSERT_TRUE(operation->op == "+");

	const auto leftValue = std::static_pointer_cast<ASTValue>(operation->left);
	const auto rightOperation = std::static_pointer_cast<ASTBinaryOperation>(operation->right);

	ASSERT_TRUE(leftValue->value == "1");
	ASSERT_TRUE(rightOperation->op == "*");

	const auto rightLeftValue = std::static_pointer_cast<ASTValue>(rightOperation->left);
	const auto rightRightVariable = std::static_pointer_cast<ASTVariable>(rightOperation->right);

	ASSERT_TRUE(rightLeftValue->value == "5");
	ASSERT_TRUE(rightRightVariable->name == "b");
}

TEST(ParserTest, parsePrintAndPrintlnCalls) {
	Lexer lexer("print(0);\nprintln(true);");
	const std::vector<Token> tokens = lexer.tokenize();

	Parser parser(tokens);
	const auto program = parser.parse();

	ASSERT_TRUE(program->type == ASTType::Program);
	ASSERT_TRUE(program->body.size() == 2);
	
	int newLine = 0;
	for (const auto printBase : program->body) {
		const auto print = std::static_pointer_cast<ASTPrint>(printBase);
		ASSERT_EQ(print->newLine, newLine);
		newLine++;
	}
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
	Lexer lexer("var a: abc = 10;"); // Invalid type 'abc'
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

TEST(ParserTest, parseFunctionDeclarationWithMissingBracket) {
	Lexer lexer("def main({\n    println(10);"); // Missing closing bracket
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

TEST(ParserTest, parseFunctionWithInvalidExpression) {
	Lexer lexer("def main() {\n    println(10 + ;\n}"); // Invalid expression with missing operand
	const std::vector<Token> tokens = lexer.tokenize();

	Parser parser(tokens);
	try {
		parser.parse();
		FAIL() << "Expected ZynkError thrown.";
	}
	catch (const ZynkError& error) {
		EXPECT_EQ(error.base_type, ZynkErrorType::ExpressionError);
	}
	catch (const std::exception& error) {
		FAIL() << "Unexpected exception type: " << error.what();
	}
}

TEST(ParserTest, parseFunctionCallWithExtraTokens) {
	Lexer lexer("main(10);"); // Extra comma in function call
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

TEST(ParserTest, parseVariableDeclarationWithMissingColon) {
	Lexer lexer("var a int = 10;"); // Missing colon
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

TEST(ParserTest, parseExpressionWithInvalidOperator) {
	Lexer lexer("var a: int = 5 $ 10;"); // Invalid operator '$'
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

TEST(ParserTest, parseStringWithMissingClosingQuote) {
	Lexer lexer("var a: String = \"Unclosed string;");
	const std::vector<Token> tokens = lexer.tokenize();

	Parser parser(tokens);
	try {
		parser.parse();
		FAIL() << "Expected ZynkError thrown.";
	}
	catch (const ZynkError& error) {
		EXPECT_EQ(error.base_type, ZynkErrorType::ExpressionError);
	}
	catch (const std::exception& error) {
		FAIL() << "Unexpected exception type: " << error.what();
	}
}