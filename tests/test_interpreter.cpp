#include <gtest/gtest.h>
#include "../src/include/interpreter.hpp"

TEST(LexerTokenizeTest, Basic) {
	// For now there is no need to test the interpreter stronger, 
	// as most things are tested in the lexer anyway.
	ZynkInterpreter interpreter;
	const std::vector<Token> tokens = interpreter.interpret("def main() {\n\n}\n");

	EXPECT_TRUE(tokens.size() == 7);
	EXPECT_TRUE(tokens.front().type == TokenType::DEF);
	EXPECT_TRUE(tokens.back().type == TokenType::END_OF_FILE);
}

TEST(InterpreterTest, ShouldThrowRuntimeError) {
    ZynkInterpreter interpreter;
    EXPECT_THROW(interpreter.interpret_file("invalidfile.zk"), std::runtime_error);
}