#include <gtest/gtest.h>

#include "../src/execution/include/evaluator.hpp"
#include "../src/execution/include/runtime.hpp"
#include "../src/parsing/include/parser.hpp"
#include "../src/parsing/include/lexer.hpp"
#include "../src/common/include/errors.hpp"

TEST(EvaluatorTest, EvaluatePrintStatement) {
	const std::string code = "println(\"Hello, World!\");";
	Lexer lexer(code);
	const std::vector<Token> tokens = lexer.tokenize();

	Parser parser(tokens);
	const auto program = parser.parse();

	RuntimeEnvironment env;
	Evaluator evaluator(env);

	testing::internal::CaptureStdout();
	evaluator.evaluate(program);
	const std::string output = testing::internal::GetCapturedStdout();
	ASSERT_EQ(output, "Hello, World!\n");
}

TEST(EvaluatorTest, EvaluateVariableDeclarationAndPrint) {
	const std::string code = "var x: int = 42; println(x);";
	Lexer lexer(code);
	const std::vector<Token> tokens = lexer.tokenize();

	Parser parser(tokens);
	const auto program = parser.parse();

	RuntimeEnvironment env;
	Evaluator evaluator(env);

	testing::internal::CaptureStdout();
	evaluator.evaluate(program);
	std::string output = testing::internal::GetCapturedStdout();
	ASSERT_EQ(output, "42\n");
}

TEST(EvaluatorTest, EvaluateFunctionDeclaration) {
	const std::string code = "def myFunction(){\nprintln(\"Inside function\");\n}";
	Lexer lexer(code);
	const std::vector<Token> tokens = lexer.tokenize();

	Parser parser(tokens);
	const auto program = parser.parse();

	RuntimeEnvironment env;
	Evaluator evaluator(env);

	evaluator.evaluate(program);

	// We do not call the function, so nothing should show up.
	testing::internal::CaptureStdout();
	std::string output = testing::internal::GetCapturedStdout();
	ASSERT_EQ(output, "");
}

TEST(EvaluatorTest, EvaluateFunctionCall) {
	const std::string code = R"(
        def myFunction(){
			var x: String = "Inside function.";
			println(x);
		}
        myFunction();
    )";
	Lexer lexer(code);
	const std::vector<Token> tokens = lexer.tokenize();

	Parser parser(tokens);
	const auto program = parser.parse();

	RuntimeEnvironment env;
	Evaluator evaluator(env);

	testing::internal::CaptureStdout();
	evaluator.evaluate(program);
	std::string output = testing::internal::GetCapturedStdout();
	ASSERT_EQ(output, "Inside function.\n");
}

TEST(EvaluatorTest, EvaluateNestedFunctionCalls) {
	const std::string code = R"(
        def outerFunction() {
            def innerFunction() { 
                println("Inside inner function"); 
            }
			println("Outer function");
            innerFunction();
        }
        outerFunction();
    )";
	Lexer lexer(code);
	const std::vector<Token> tokens = lexer.tokenize();

	Parser parser(tokens);
	const auto program = parser.parse();

	RuntimeEnvironment env;
	Evaluator evaluator(env);

	testing::internal::CaptureStdout();
	evaluator.evaluate(program);
	std::string output = testing::internal::GetCapturedStdout();

	ASSERT_EQ(output, "Outer function\nInside inner function\n");
}

TEST(EvaluatorTest, EvaluateBinaryOperationAddition) {
    const std::string code = "println(5 + 3);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto program = parser.parse();

    RuntimeEnvironment env;
    Evaluator evaluator(env);

    testing::internal::CaptureStdout();
    evaluator.evaluate(program);
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_EQ(output, "8\n");
}

TEST(EvaluatorTest, EvaluateBinaryOperationMultiply) {
    const std::string code = "println(6 * 7);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto program = parser.parse();

    RuntimeEnvironment env;
    Evaluator evaluator(env);

    testing::internal::CaptureStdout();
    evaluator.evaluate(program);
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_EQ(output, "42\n");
}

TEST(EvaluatorTest, EvaluateBinaryOperationSubtraction) {
    const std::string code = "println(10 - 4);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto program = parser.parse();

    RuntimeEnvironment env;
    Evaluator evaluator(env);

    testing::internal::CaptureStdout();
    evaluator.evaluate(program);
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_EQ(output, "6\n");
}

TEST(EvaluatorTest, EvaluateBinaryOperationDivision) {
    const std::string code = "println(12 / 4);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto program = parser.parse();

    RuntimeEnvironment env;
    Evaluator evaluator(env);

    testing::internal::CaptureStdout();
    evaluator.evaluate(program);
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_EQ(output, "3\n");
}

TEST(EvaluatorTest, EvaluateUndefinedFunctionCall) {
    const std::string code = "undefinedFunction();";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto program = parser.parse();

    RuntimeEnvironment env;
    Evaluator evaluator(env);
    ASSERT_THROW(evaluator.evaluate(program), ZynkError);
}

TEST(EvaluatorTest, EvaluateUndefinedVariableUsage) {
    const std::string code = "println(x);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto program = parser.parse();

    RuntimeEnvironment env;
    Evaluator evaluator(env);
    ASSERT_THROW(evaluator.evaluate(program), ZynkError);
}

TEST(EvaluatorTest, EvaluateFloatVariableDeclarationAndPrint) {
    const std::string code = "var x: float = 3.14;\nprintln(x);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto program = parser.parse();

    RuntimeEnvironment env;
    Evaluator evaluator(env);

    testing::internal::CaptureStdout();
    evaluator.evaluate(program);
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_EQ(output, "3.14\n");
}

TEST(EvaluatorTest, EvaluateBinaryOperationFloatAddition) {
    const std::string code = "println(2.5 + 1.5);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto program = parser.parse();

    RuntimeEnvironment env;
    Evaluator evaluator(env);

    testing::internal::CaptureStdout();
    evaluator.evaluate(program);
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_EQ(output, "4.000000\n");
}

TEST(EvaluatorTest, DuplicateVariableDeclaration) {
    const std::string code = "var x: int = 42;\nvar x: int = 43;";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto program = parser.parse();

    RuntimeEnvironment env;
    Evaluator evaluator(env);
    ASSERT_THROW(evaluator.evaluate(program), ZynkError);
}

TEST(EvaluatorTest, DuplicateFunctionDeclaration) {
    const std::string code = R"(
        def myFunction() { }
        def myFunction() { }
    )";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto program = parser.parse();

    RuntimeEnvironment env;
    Evaluator evaluator(env);
    ASSERT_THROW(evaluator.evaluate(program), ZynkError);
}

TEST(EvaluatorTest, EvaluateVariableInExpression) {
    const std::string code = "var x: int = 5;\nprintln(x + 10);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto program = parser.parse();

    RuntimeEnvironment env;
    Evaluator evaluator(env);

    testing::internal::CaptureStdout();
    evaluator.evaluate(program);
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_EQ(output, "15\n");
}

TEST(EvaluatorTest, EvaluateBinaryOperationFloatMultiplication) {
    const std::string code = "println(2.5 * 2);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto program = parser.parse();

    RuntimeEnvironment env;
    Evaluator evaluator(env);

    testing::internal::CaptureStdout();
    evaluator.evaluate(program);
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_EQ(output, "5.000000\n");
}

TEST(EvaluatorTest, EvaluateDivisionByZero) {
    const std::string code = "println(1 / 0);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto program = parser.parse();

    RuntimeEnvironment env;
    Evaluator evaluator(env);
    ASSERT_THROW(evaluator.evaluate(program), ZynkError);
}

TEST(EvaluatorTest, EvaluateEmptyProgram) {
    const std::string code = "";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto program = parser.parse();

    RuntimeEnvironment env;
    Evaluator evaluator(env);

    testing::internal::CaptureStdout();
    evaluator.evaluate(program);
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_EQ(output, "");
}
