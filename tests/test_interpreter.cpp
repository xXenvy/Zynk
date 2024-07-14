#include <gtest/gtest.h>
#include "../src/include/interpreter.hpp"
#include "../src/include/errors.hpp"

TEST(InterpreterTest, Basic) {
	// For now there is no need to test the interpreter stronger, 
	// as most things are tested in the lexer anyway.
	ZynkInterpreter interpreter;
	const ASTProgram* program = interpreter.interpret("def main123() {}\n");

	EXPECT_TRUE(program->type == ASTType::Program);
	EXPECT_TRUE(program->body.size() == 1);
}

TEST(InterpreterTest, ShouldThrowRuntimeError) {
    ZynkInterpreter interpreter;
	try {
		interpreter.interpret_file("invalidfile.zk");
		FAIL() << "Expected ZynkError thrown.";
	}
	catch (const ZynkError& error) {
		EXPECT_EQ(error.base_type, ZynkErrorType::FileOpenError);
	}
	catch (const std::exception& error) {
		FAIL() << "Unexpected exception type: " << error.what();
	}
}