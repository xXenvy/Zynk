#include <gtest/gtest.h>
#include "../src/include/interpreter.hpp"
#include "../src/include/errors.hpp"

TEST(InterpreterTest, Basic) {
	// For now there is no need to test the interpreter stronger, 
	// as most things are tested in the lexer anyway.
	ZynkInterpreter interpreter;
	const ProgramNode* program = interpreter.interpret("def main123() {}\n");

	EXPECT_TRUE(program->type == ASTNodeType::Program);
	EXPECT_TRUE(program->nodes.size() == 1);
}

TEST(InterpreterTest, ShouldThrowRuntimeError) {
    ZynkInterpreter interpreter;
	try {
		interpreter.interpret_file("invalidfile.zk");
		FAIL() << "Expected ZynkError thrown.";
	}
	catch (const ZynkError& error) {
		EXPECT_EQ(error.type, ZynkErrorType::FileOpenError);
	}
	catch (const std::exception& error) {
		FAIL() << "Unexpected exception type: " << error.what();
	}
}