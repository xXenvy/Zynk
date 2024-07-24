#include <gtest/gtest.h>
#include "../src/execution/include/interpreter.hpp"
#include "../src/common/include/errors.hpp"

TEST(InterpreterTest, ShouldThrowFileOpenError) {
	// For now there is no need to test the interpreter stronger, 
	// as most things are tested in other tests anyway.
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