#include <gtest/gtest.h>

#include "../src/include/parser.hpp"
#include "../src/include/lexer.hpp"
#include "../src/include/errors.hpp"

TEST(ParserTest, parseVariableDeclaration) {
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

TEST(ParserTest, parseMultipleVariableDeclarations) {
	Lexer lexer("a: int = 1;\nb: float = 1.0;\nc: bool = true;\nd: String = \"Test\";");
	const std::vector<Token> tokens = lexer.tokenize();

	Parser parser(tokens);
	const ASTProgram* program = parser.parse();

	ASSERT_TRUE(program->type == ASTType::Program);
	ASSERT_TRUE(program->body.size() == 4);

	for (const ASTBase* var : program->body) {
		ASSERT_EQ(var->type, ASTType::VariableDeclaration);
	}

	delete program;
}

TEST(ParserTest, parseFunction) {
	Lexer lexer("def main(){\n    println(10);\n}");
	const std::vector<Token> tokens = lexer.tokenize();

	Parser parser(tokens);
	const ASTProgram* program = parser.parse();

	ASSERT_TRUE(program->type == ASTType::Program);
	ASSERT_TRUE(program->body.size() == 1);
	ASSERT_TRUE(program->body.front()->type == ASTType::Function);

	const ASTFunction* function = static_cast<ASTFunction*>(program->body.front());
	ASSERT_TRUE(function->name == "main");
	ASSERT_TRUE(function->body.size() == 1);
	ASSERT_TRUE(function->body.front()->type == ASTType::Print);

	const ASTPrint* print = static_cast<ASTPrint*>(function->body.front());
	ASSERT_TRUE(print->newLine);
	const ASTValue* printValue = static_cast<ASTValue*>(print->expression);
	ASSERT_TRUE(printValue->type == ASTType::Value);
	ASSERT_TRUE(printValue->value == "10");

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

TEST(ParserTest, parseVariableDeclarationWithBinaryOperation) {
	// 'b' is not defined, but parser doesn't have that context.
	Lexer lexer("a: float = b + 1.0;");
	const std::vector<Token> tokens = lexer.tokenize();

	Parser parser(tokens);
	const ASTProgram* program = parser.parse();

	ASSERT_TRUE(program->type == ASTType::Program);
	ASSERT_TRUE(program->body.size() == 1);
	ASSERT_TRUE(program->body.front()->type == ASTType::VariableDeclaration);

	const ASTVariableDeclaration* var = static_cast<ASTVariableDeclaration*>(program->body.front());
	ASSERT_TRUE(var->name == "a");
	ASSERT_TRUE(var->type == "float");
	const ASTBinaryOperation* operation = static_cast<ASTBinaryOperation*>(var->value);
	ASSERT_TRUE(operation->op == "+");
	
	const ASTVariable* left = static_cast<ASTVariable*>(operation->left);
	const ASTValue* right = static_cast<ASTValue*>(operation->right);
	ASSERT_TRUE(left->name == "b" && right->value == "1.0");
	delete program;
}

TEST(ParserTest, parseVariableDeclarationWithComplexExpression) {
	// 'b' is not defined, but parser doesn't have that context.
	Lexer lexer("a: float = 1 + 5 * b;");
	const std::vector<Token> tokens = lexer.tokenize();

	Parser parser(tokens);
	const ASTProgram* program = parser.parse();

	ASSERT_TRUE(program->type == ASTType::Program);
	ASSERT_TRUE(program->body.size() == 1);
	ASSERT_TRUE(program->body.front()->type == ASTType::VariableDeclaration);

	const ASTVariableDeclaration* var = static_cast<ASTVariableDeclaration*>(program->body.front());
	ASSERT_TRUE(var->name == "a");
	ASSERT_TRUE(var->type == "float");

	const ASTBinaryOperation* operation = static_cast<ASTBinaryOperation*>(var->value);
	ASSERT_TRUE(operation->op == "+");

	const ASTValue* leftValue = static_cast<ASTValue*>(operation->left);
	const ASTBinaryOperation* rightOperation = static_cast<ASTBinaryOperation*>(operation->right);

	ASSERT_TRUE(leftValue->value == "1");
	ASSERT_TRUE(rightOperation->op == "*");

	const ASTValue* rightLeftValue = static_cast<ASTValue*>(rightOperation->left);
	const ASTVariable* rightRightVariable = static_cast<ASTVariable*>(rightOperation->right);

	ASSERT_TRUE(rightLeftValue->value == "5");
	ASSERT_TRUE(rightRightVariable->name == "b");

	delete program;
}

TEST(ParserTest, parsePrintAndPrintlnCalls) {
	Lexer lexer("print(0);\nprintln(true);");
	const std::vector<Token> tokens = lexer.tokenize();

	Parser parser(tokens);
	const ASTProgram* program = parser.parse();

	ASSERT_TRUE(program->type == ASTType::Program);
	ASSERT_TRUE(program->body.size() == 2);
	
	int newLine = 0;
	for (ASTBase* printBase : program->body) {
		const ASTPrint* print = static_cast<ASTPrint*>(printBase);
		ASSERT_EQ(print->newLine, newLine);
		newLine++;
	}

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
