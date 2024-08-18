#include <gtest/gtest.h>
#include <string>
#include <vector>

#include "../src/parsing/include/lexer.hpp"

TEST(LexerTokenizeTest, PrintlnKeyword) {
	Lexer lexer("println(10);\nprintln(\"TEST\");\nprintln(1.5);");
	const std::vector<Token> tokens = lexer.tokenize();
	size_t keywords = 0;

	for (const Token& token : tokens) {
		if (token.type == TokenType::PRINTLN) {
			keywords++;
			ASSERT_TRUE(keywords == token.line);
		}
	}
	EXPECT_TRUE(keywords == 3);
	EXPECT_TRUE(tokens.size() == 16);
	EXPECT_TRUE(tokens.front().type == TokenType::PRINTLN);
	EXPECT_TRUE(tokens.back().type == TokenType::END_OF_FILE);
}

TEST(LexerTokenizeTest, PrintKeyword) {
	Lexer lexer("print(10);\nprint(\"TEST\");\nprint(1.5);");
	const std::vector<Token> tokens = lexer.tokenize();
	size_t keywords = 0;

	for (const Token& token : tokens) {
		if (token.type == TokenType::PRINT) keywords++;
	}
	EXPECT_TRUE(keywords == 3);
	EXPECT_TRUE(tokens.size() == 16);
	EXPECT_TRUE(tokens.front().type == TokenType::PRINT);
	EXPECT_TRUE(tokens.back().type == TokenType::END_OF_FILE);
}

TEST(LexerTokenizeTest, EmptySource) {
	Lexer lexer("");
	const std::vector<Token> tokens = lexer.tokenize();

	EXPECT_TRUE(tokens.size() == 1);
	EXPECT_TRUE(tokens.front().type == TokenType::END_OF_FILE);
	EXPECT_TRUE(tokens.front().line == 1);
}

TEST(LexerTokenizeTest, SimpleFunctionDefinition) {
	Lexer lexer("def main() {\n\n}\n");
	const std::vector<Token> tokens = lexer.tokenize();

	EXPECT_TRUE(tokens.size() == 7);
	EXPECT_TRUE(tokens.front().type == TokenType::DEF);
	EXPECT_TRUE(tokens.back().type == TokenType::END_OF_FILE);
	EXPECT_TRUE(tokens.front().line == 1);
	EXPECT_TRUE(tokens.back().line == 4);
}

TEST(LexerTokenizeTest, MultipleFunctionDefinitions) {
	std::string base = "def testmain0() {\n\n}\n";
	for(int i=1; i < 10; i++) {
		base += "def testmain" + std::to_string(i) + "() {\n\n}\n";
	}

	Lexer lexer(base);
	const std::vector<Token> tokens = lexer.tokenize();

	EXPECT_TRUE(tokens.size() == 61);
	EXPECT_TRUE(tokens.front().type == TokenType::DEF);
	EXPECT_TRUE(tokens.back().type == TokenType::END_OF_FILE);
}

TEST(LexerTokenizeTest, IntVariableDefinitions) {
	const std::string source = "var a: int = 10;\nvar b: int = 01;\nvar c: int = 321321321323232;";
	Lexer lexer(source);
	const std::vector<Token> tokens = lexer.tokenize();
	size_t type_counter = 0;

	for (const Token& token : tokens) {
		if (token.value == "int") {
			EXPECT_TRUE(token.type == TokenType::INT);
			type_counter++;
		}
	}
	EXPECT_TRUE(tokens.size() == 22);
	EXPECT_TRUE(type_counter == 3 && tokens.back().line == 3);
	EXPECT_TRUE(tokens.front().type == TokenType::VARIABLE);
	EXPECT_TRUE(tokens.back().type == TokenType::END_OF_FILE);
}

TEST(LexerTokenizeTest, FloatVariableDefinitions) {
	const std::string source = "var a: float = 10.0;\nvar b: float = 0.1;\nvar c: float = 32132132.1323232;";
	Lexer lexer(source);
	const std::vector<Token> tokens = lexer.tokenize();
	size_t type_counter = 0;

	for (const Token& token : tokens) {
		if (token.value == "float") {
			EXPECT_TRUE(token.type == TokenType::FLOAT);
			type_counter++;
		}
	}
	EXPECT_TRUE(tokens.size() == 22);
	EXPECT_TRUE(type_counter == 3);
	EXPECT_TRUE(tokens.front().type == TokenType::VARIABLE);
	EXPECT_TRUE(tokens.back().type == TokenType::END_OF_FILE);
}

TEST(LexerTokenizeTest, StringVariableDefinitions) {
	const std::string source = "var a: string = \"Test\";\nvar b: string = \"AB123#@\";\n";
	Lexer lexer(source);
	const std::vector<Token> tokens = lexer.tokenize();
	size_t type_counter = 0;

	for (const Token& token : tokens) {
		if (token.value == "string") {
			EXPECT_TRUE(token.type == TokenType::STRING);
			type_counter++;
		}
	}
	EXPECT_TRUE(tokens.size() == 15);
	EXPECT_TRUE(type_counter == 2);
	EXPECT_TRUE(tokens.front().type == TokenType::VARIABLE);
	EXPECT_TRUE(tokens.back().type == TokenType::END_OF_FILE);
}

TEST(LexerTokenizeTest, BoolVariableDefinitions) {
	const std::string source = "var a: bool = false;\nvar b: bool = true;\nvar c: bool = false;";
	Lexer lexer(source);
	const std::vector<Token> tokens = lexer.tokenize();
	size_t type_counter = 0;

	for (const Token& token : tokens) {
		if (token.value == "bool") {
			EXPECT_TRUE(token.type == TokenType::BOOL);
			type_counter++;
		}
	}
	EXPECT_TRUE(tokens.size() == 22);
	EXPECT_TRUE(type_counter == 3);
	EXPECT_TRUE(tokens.front().type == TokenType::VARIABLE);
	EXPECT_TRUE(tokens.back().type == TokenType::END_OF_FILE);
}

TEST(LexerTokenizeTest, ManySemicolons) {
	Lexer lexer(";;;\n;;;\n;;;;;;");
	const std::vector<Token> tokens = lexer.tokenize();
	size_t semicolons = 0;

	for (const Token& token : tokens) {
		if (token.type == TokenType::SEMICOLON) semicolons++;
	}
	EXPECT_TRUE(semicolons == 12);
	EXPECT_TRUE(tokens.size() == 13);
	EXPECT_TRUE(tokens.front().type == TokenType::SEMICOLON);
	EXPECT_TRUE(tokens.back().type == TokenType::END_OF_FILE);
}

TEST(LexerTokenizeTest, NotEqualOperator) {
	Lexer lexer("10 != 50;\n\"Test\" != \"ABC\";\n1 !! 2;");
	const std::vector<Token> tokens = lexer.tokenize();
	size_t operators = 0;

	for (const Token& token : tokens) {
		if (token.type == TokenType::NOT_EQUAL) operators++;
	}
	EXPECT_TRUE(operators == 2);
	EXPECT_TRUE(tokens.size() == 14);
	EXPECT_TRUE(tokens.front().type == TokenType::INT);
	EXPECT_TRUE(tokens.back().type == TokenType::END_OF_FILE);
}

TEST(LexerTokenizeTest, EqualityOperator) {
	Lexer lexer("10 == 50;\n\"Test\" == \"ABC\";\n");
	const std::vector<Token> tokens = lexer.tokenize();
	size_t operators = 0;

	for (const Token& token : tokens) {
		if (token.type == TokenType::EQUAL) operators++;
	}
	EXPECT_TRUE(operators == 2);
	EXPECT_TRUE(tokens.size() == 9);
	EXPECT_TRUE(tokens.front().type == TokenType::INT);
	EXPECT_TRUE(tokens.back().type == TokenType::END_OF_FILE);
}

TEST(LexerTokenizeTest, AddOperator) {
	Lexer lexer("10 + 50;\n5.1 + 5;\n");
	const std::vector<Token> tokens = lexer.tokenize();
	size_t operators = 0;

	for (const Token& token : tokens) {
		if (token.type == TokenType::ADD) operators++;
	}
	EXPECT_TRUE(operators == 2);
	EXPECT_TRUE(tokens.size() == 9);
	EXPECT_TRUE(tokens.front().type == TokenType::INT);
	EXPECT_TRUE(tokens.back().type == TokenType::END_OF_FILE);
}

TEST(LexerTokenizeTest, SubtractOperator) {
	Lexer lexer("10 - 50;\n5.1 - 5;\n");
	const std::vector<Token> tokens = lexer.tokenize();
	size_t operators = 0;

	for (const Token& token : tokens) {
		if (token.type == TokenType::SUBTRACT) operators++;
	}
	EXPECT_TRUE(operators == 2);
	EXPECT_TRUE(tokens.size() == 9);
	EXPECT_TRUE(tokens.front().type == TokenType::INT);
	EXPECT_TRUE(tokens.back().type == TokenType::END_OF_FILE);
}

TEST(LexerTokenizeTest, MultiplyOperator) {
	Lexer lexer("5 * 50;\n5.1 * 5;\n1.500 * 5;\n");
	const std::vector<Token> tokens = lexer.tokenize();
	size_t operators = 0;

	for (const Token& token : tokens) {
		if (token.type == TokenType::MULTIPLY) operators++;
	}
	EXPECT_TRUE(operators == 3);
	EXPECT_TRUE(tokens.size() == 13);
	EXPECT_TRUE(tokens.front().type == TokenType::INT);
	EXPECT_TRUE(tokens.back().type == TokenType::END_OF_FILE);
}

TEST(LexerTokenizeTest, DivideOperator) {
	Lexer lexer("50 / 5;\n5 / 5;\n100 / 10;");
	const std::vector<Token> tokens = lexer.tokenize();
	size_t operators = 0;

	for (const Token& token : tokens) {
		if (token.type == TokenType::DIVIDE) operators++;
	}
	EXPECT_TRUE(operators == 3);
	EXPECT_TRUE(tokens.size() == 13);
	EXPECT_TRUE(tokens.front().type == TokenType::INT);
	EXPECT_TRUE(tokens.back().type == TokenType::END_OF_FILE);
}

TEST(LexerTokenizeTest, Brackets) {
	Lexer lexer("(10);\n{10};");
	const std::vector<Token> tokens = lexer.tokenize();
	size_t lbrackets = 0;
	size_t rbrackets = 0;

	for (const Token& token : tokens) {
		if (token.type == TokenType::LBRACKET || token.type == TokenType::LBRACE) lbrackets++;
		if (token.type == TokenType::RBRACKET || token.type == TokenType::RBRACE) rbrackets++;
	}
	EXPECT_EQ(lbrackets, 2);
	EXPECT_EQ(rbrackets, 2);
	EXPECT_EQ(tokens.size(), 9);
	EXPECT_EQ(tokens.front().type, TokenType::LBRACKET);
	EXPECT_EQ(tokens.back().type, TokenType::END_OF_FILE);
}

TEST(LexerTokenizeTest, UnterminatedString) {
	Lexer lexer("\"Unfinished string;");
	const std::vector<Token> tokens = lexer.tokenize();

	EXPECT_EQ(tokens.size(), 2);
	EXPECT_EQ(tokens.front().type, TokenType::UNKNOWN);
	EXPECT_EQ(tokens.front().value, "Unterminated string");
	EXPECT_EQ(tokens.back().type, TokenType::END_OF_FILE);
}

TEST(LexerTokenizeTest, NullKeyword) {
	Lexer lexer("null");
	const std::vector<Token> tokens = lexer.tokenize();

	EXPECT_TRUE(tokens.size() == 2);
	EXPECT_TRUE(tokens.front().type == TokenType::NONE);
	EXPECT_TRUE(tokens.front().value == "null");
	EXPECT_TRUE(tokens.back().type == TokenType::END_OF_FILE);
}

TEST(LexerTokenizeTest, ConditionKeyword) {
	Lexer lexer("if (x == 10) { print(\"Yes\"); } else { print(\"No\"); }");
	const std::vector<Token> tokens = lexer.tokenize();
	size_t conditionals = 0;
	size_t elses = 0;

	for (const Token& token : tokens) {
		if (token.type == TokenType::CONDITION) conditionals++;
		if (token.type == TokenType::ELSE) elses++;
	}
	EXPECT_TRUE(conditionals == 1);
	EXPECT_TRUE(elses == 1);
	EXPECT_TRUE(tokens.front().type == TokenType::CONDITION);
	EXPECT_TRUE(tokens.back().type == TokenType::END_OF_FILE);
	EXPECT_TRUE(tokens.size() == 22);
}

TEST(LexerTokenizeTest, VariableKeyword) {
	Lexer lexer("var x = 10;");
	const std::vector<Token> tokens = lexer.tokenize();

	EXPECT_TRUE(tokens.size() == 6);
	EXPECT_TRUE(tokens.front().type == TokenType::VARIABLE);
	EXPECT_TRUE(tokens.front().value == "var");
	EXPECT_TRUE(tokens.back().type == TokenType::END_OF_FILE);
}

TEST(LexerTokenizeTest, ComparisonOperators) {
	Lexer lexer("x < 10; y > 20; a <= b; c >= d;");
	const std::vector<Token> tokens = lexer.tokenize();

	size_t operators = 0;
	for (const Token& token : tokens) {
		if (token.type == TokenType::LESS_THAN || token.type == TokenType::GREATER_THAN ||
			token.type == TokenType::LESS_OR_EQUAL || token.type == TokenType::GREATER_OR_EQUAL) {
			operators++;
		}
	}
	EXPECT_TRUE(operators == 4);
	EXPECT_TRUE(tokens.front().type == TokenType::IDENTIFIER);
	EXPECT_TRUE(tokens.back().type == TokenType::END_OF_FILE);
	EXPECT_TRUE(tokens.size() == 17);
}

TEST(LexerTokenizeTest, UnknownTokens) {
	Lexer lexer("@#$$%^&");
	const std::vector<Token> tokens = lexer.tokenize();

	size_t unknownCount = 0;
	for (const Token& token : tokens) {
		if (token.type == TokenType::UNKNOWN) unknownCount++;
	}
	EXPECT_TRUE(unknownCount == 7);
	EXPECT_TRUE(tokens.size() == 8);
	EXPECT_TRUE(tokens.back().type == TokenType::END_OF_FILE);
}

TEST(LexerTokenizeTest, ReadInputKeyword) {
	Lexer lexer("readInput(1)\n;\nreadInput();\nreadInput(\"Input\");");
	const std::vector<Token> tokens = lexer.tokenize();

	size_t readKeywords = 0;
	for (const Token& token : tokens) {
		if (token.type == TokenType::READINPUT) {
			readKeywords++;
		}
	}
	EXPECT_TRUE(readKeywords == 3);
	EXPECT_TRUE(tokens.size() == 15);
	EXPECT_TRUE(tokens.front().type == TokenType::READINPUT);
	EXPECT_TRUE(tokens.back().type == TokenType::END_OF_FILE);
}

TEST(LexerTokenizeTest, CommentAtEndOfFile) {
	Lexer lexer("var x: int = 10; // Comment at end of file");
	const std::vector<Token> tokens = lexer.tokenize();

	EXPECT_TRUE(tokens.size() == 8);
	EXPECT_TRUE(tokens[0].type == TokenType::VARIABLE);
	EXPECT_TRUE(tokens[1].type == TokenType::IDENTIFIER);
	EXPECT_TRUE(tokens[2].type == TokenType::COLON);
	EXPECT_TRUE(tokens[3].type == TokenType::INT);
	EXPECT_TRUE(tokens[4].type == TokenType::ASSIGN);
	EXPECT_TRUE(tokens[5].type == TokenType::INT);
	EXPECT_TRUE(tokens.back().type == TokenType::END_OF_FILE);
}

TEST(LexerTokenizeTest, CommentVarDeclararation) {
	Lexer lexer("// var x: int = 10;");
	const std::vector<Token> tokens = lexer.tokenize();

	EXPECT_TRUE(tokens.size() == 1);
	EXPECT_TRUE(tokens.back().type == TokenType::END_OF_FILE);
}

TEST(LexerTokenizeTest, MultipleComments) {
	Lexer lexer("// Test comment\n1 + 1;\n// Test comment 2.");
	const std::vector<Token> tokens = lexer.tokenize();

	EXPECT_TRUE(tokens.size() == 5);
	EXPECT_TRUE(tokens.back().type == TokenType::END_OF_FILE);
}

TEST(LexerTokenizeTest, LogicalAndOperator) {
	Lexer lexer("true && false;\n1 && 0;");
	const std::vector<Token> tokens = lexer.tokenize();
	size_t andCount = 0;

	for (const Token& token : tokens) {
		if (token.type == TokenType::AND) {
			andCount++;
		}
	}
	EXPECT_TRUE(andCount == 2);
	EXPECT_TRUE(tokens.size() == 9);
	EXPECT_TRUE(tokens.front().type == TokenType::BOOL);
	EXPECT_TRUE(tokens.back().type == TokenType::END_OF_FILE);
}

TEST(LexerTokenizeTest, LogicalOrOperator) {
	Lexer lexer("true || false;\n1 || 0;");
	const std::vector<Token> tokens = lexer.tokenize();
	size_t orCount = 0;

	for (const Token& token : tokens) {
		if (token.type == TokenType::OR) {
			orCount++;
		}
	}
	EXPECT_TRUE(orCount == 2);
	EXPECT_TRUE(tokens.size() == 9);
	EXPECT_TRUE(tokens.front().type == TokenType::BOOL);
	EXPECT_TRUE(tokens.back().type == TokenType::END_OF_FILE);
}

TEST(LexerTokenizeTest, ReturnKeyword) {
	Lexer lexer("return 1;");
	const std::vector<Token> tokens = lexer.tokenize();

	EXPECT_TRUE(tokens.size() == 4);
	EXPECT_TRUE(tokens.front().type == TokenType::RETURN);
	EXPECT_TRUE(tokens.back().type == TokenType::END_OF_FILE);
}

TEST(LexerTokenizeTest, Commas) {
	Lexer lexer(",, , () ,");
	const std::vector<Token> tokens = lexer.tokenize();

	EXPECT_TRUE(tokens.size() == 7);
	EXPECT_TRUE(tokens.front().type == TokenType::COMMA);
	EXPECT_TRUE(tokens.back().type == TokenType::END_OF_FILE);
}