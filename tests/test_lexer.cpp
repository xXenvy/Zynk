#include <gtest/gtest.h>
#include <string>
#include <vector>

#include "../src/include/lexer.hpp"


TEST(LexerTest, EmptySource) {
	Lexer lexer("");
	const std::vector<Token> tokens = lexer.tokenize();

	EXPECT_TRUE(tokens.size() == 1);
	EXPECT_TRUE(tokens.front().type == TokenType::END_OF_FILE);
}