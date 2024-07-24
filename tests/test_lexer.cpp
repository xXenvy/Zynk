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
	const std::string source = "a: int = 10;\nb: int = 01;\nc: int = 321321321323232;";
	Lexer lexer(source);
	const std::vector<Token> tokens = lexer.tokenize();
	size_t type_counter = 0;

	for (const Token& token : tokens) {
		if (token.value == "int") {
			EXPECT_TRUE(token.type == TokenType::INT);
			type_counter++;
		}
	}
	EXPECT_TRUE(tokens.size() == 19);
	EXPECT_TRUE(type_counter == 3 && tokens.back().line == 3);
	EXPECT_TRUE(tokens.front().type == TokenType::IDENTIFIER);
	EXPECT_TRUE(tokens.back().type == TokenType::END_OF_FILE);
}

TEST(LexerTokenizeTest, FloatVariableDefinitions) {
	const std::string source = "a: float = 10.0;\nb: float = 0.1;\nc: float = 32132132.1323232;";
	Lexer lexer(source);
	const std::vector<Token> tokens = lexer.tokenize();
	size_t type_counter = 0;

	for (const Token& token : tokens) {
		if (token.value == "float") {
			EXPECT_TRUE(token.type == TokenType::FLOAT);
			type_counter++;
		}
	}
	EXPECT_TRUE(tokens.size() == 19);
	EXPECT_TRUE(type_counter == 3);
	EXPECT_TRUE(tokens.front().type == TokenType::IDENTIFIER);
	EXPECT_TRUE(tokens.back().type == TokenType::END_OF_FILE);
}

TEST(LexerTokenizeTest, StringVariableDefinitions) {
	const std::string source = "a: String = \"Test\";\nb: String = \"AB123#@\";\n";
	Lexer lexer(source);
	const std::vector<Token> tokens = lexer.tokenize();
	size_t type_counter = 0;

	for (const Token& token : tokens) {
		if (token.value == "String") {
			EXPECT_TRUE(token.type == TokenType::STRING);
			type_counter++;
		}
	}
	EXPECT_TRUE(tokens.size() == 13);
	EXPECT_TRUE(type_counter == 2);
	EXPECT_TRUE(tokens.front().type == TokenType::IDENTIFIER);
	EXPECT_TRUE(tokens.back().type == TokenType::END_OF_FILE);
}

TEST(LexerTokenizeTest, BoolVariableDefinitions) {
	const std::string source = "a: bool = false;\nb: bool = true;\nc: bool = false;";
	Lexer lexer(source);
	const std::vector<Token> tokens = lexer.tokenize();
	size_t type_counter = 0;

	for (const Token& token : tokens) {
		if (token.value == "bool") {
			EXPECT_TRUE(token.type == TokenType::BOOL);
			type_counter++;
		}
	}
	EXPECT_TRUE(tokens.size() == 19);
	EXPECT_TRUE(type_counter == 3);
	EXPECT_TRUE(tokens.front().type == TokenType::IDENTIFIER);
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
		if (token.type == TokenType::EQUALITY) operators++;
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