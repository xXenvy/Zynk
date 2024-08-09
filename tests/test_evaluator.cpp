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
    
    testing::internal::CaptureStdout();
    Evaluator evaluator;
	evaluator.evaluate(program.get());
	ASSERT_EQ(testing::internal::GetCapturedStdout(), "Hello, World!\n");
}

TEST(EvaluatorTest, EvaluateVariableDeclarationAndPrint) {
	const std::string code = "var x: int = 42; println(x);";
	Lexer lexer(code);
	const std::vector<Token> tokens = lexer.tokenize();

	Parser parser(tokens);
	const auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
	evaluator.evaluate(program.get());
	ASSERT_EQ(testing::internal::GetCapturedStdout(), "42\n");
}

TEST(EvaluatorTest, EvaluateFunctionDeclaration) {
	const std::string code = "def myFunction(){\nprintln(\"Inside function\");\n}";
	Lexer lexer(code);
	const std::vector<Token> tokens = lexer.tokenize();

	Parser parser(tokens);
	const auto program = parser.parse();
       
    testing::internal::CaptureStdout();
    Evaluator evaluator;
	evaluator.evaluate(program.get());
	// We do not call the function, so nothing should show up.
	ASSERT_EQ(testing::internal::GetCapturedStdout(), "");
}

TEST(EvaluatorTest, EvaluateFunctionCall) {
	const std::string code = R"(
        def myFunction(){
			var x: string = "Inside function.";
			println(x);
		}
        myFunction();
    )";
	Lexer lexer(code);
	const std::vector<Token> tokens = lexer.tokenize();

	Parser parser(tokens);
	const auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
	evaluator.evaluate(program.get());
	ASSERT_EQ(testing::internal::GetCapturedStdout(), "Inside function.\n");
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

    testing::internal::CaptureStdout();
	Evaluator evaluator;
	evaluator.evaluate(program.get());
	ASSERT_EQ(testing::internal::GetCapturedStdout(), "Outer function\nInside inner function\n");
}

TEST(EvaluatorTest, EvaluateBinaryOperationAddition) {
    const std::string code = "println(5 + 3);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(program.get());
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "8\n");
}

TEST(EvaluatorTest, EvaluateBinaryOperationMultiply) {
    const std::string code = "println(6 * 7);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(program.get());
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "42\n");
}

TEST(EvaluatorTest, EvaluateBinaryOperationSubtraction) {
    const std::string code = "println(10 - 4);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(program.get());
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "6\n");
}

TEST(EvaluatorTest, EvaluateBinaryOperationDivision) {
    const std::string code = "println(12 / 4);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(program.get());
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "3\n");
}

TEST(EvaluatorTest, EvaluateUndefinedFunctionCall) {
    const std::string code = "undefinedFunction();";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto program = parser.parse();

    Evaluator evaluator;
    ASSERT_THROW(evaluator.evaluate(program.get()), ZynkError);
}

TEST(EvaluatorTest, EvaluateUndefinedVariableUsage) {
    const std::string code = "println(x);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto program = parser.parse();

    Evaluator evaluator;
    ASSERT_THROW(evaluator.evaluate(program.get()), ZynkError);
}

TEST(EvaluatorTest, EvaluateFloatVariableDeclarationAndPrint) {
    const std::string code = "var x: float = 3.14;\nprintln(x);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(program.get());
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "3.14\n");
}

TEST(EvaluatorTest, EvaluateBinaryOperationFloatAddition) {
    const std::string code = "println(2.5 + 1.5);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(program.get());
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "4.000000\n");
}

TEST(EvaluatorTest, DuplicateVariableDeclaration) {
    const std::string code = "var x: int = 42;\nvar x: int = 43;";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto program = parser.parse();

    Evaluator evaluator;
    ASSERT_THROW(evaluator.evaluate(program.get()), ZynkError);
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

    Evaluator evaluator;
    ASSERT_THROW(evaluator.evaluate(program.get()), ZynkError);
}

TEST(EvaluatorTest, EvaluateVariableInExpression) {
    const std::string code = "var x: int = 5;\nprintln(x + 10);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(program.get());
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "15\n");
}

TEST(EvaluatorTest, EvaluateBinaryOperationFloatMultiplication) {
    const std::string code = "println(2.5 * 2);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(program.get());
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "5.000000\n");
}

TEST(EvaluatorTest, EvaluateDivisionByZero) {
    const std::string code = "println(1 / 0);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto program = parser.parse();

    Evaluator evaluator;
    ASSERT_THROW(evaluator.evaluate(program.get()), ZynkError);
}

TEST(EvaluatorTest, EvaluateEmptyProgram) {
    const std::string code = "";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(program.get());
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "");
}

TEST(EvaluatorTest, EvaluateVariableModifyInteger) {
    const std::string code = "var a: int = 10;\na = 20;\nprintln(a);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(program.get());
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "20\n");
}

TEST(EvaluatorTest, EvaluateVariableModifyWithExpression) {
    const std::string code = "var y: int = 5;\ny = y + 10;\nprintln(y);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(program.get());
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "15\n");
}

TEST(EvaluatorTest, EvaluateVariableModifyMultipleTimes) {
    const std::string code = R"(
        var z: int = 1;
        println(z);
        z = 2;
        println(z);
        z = 3;
        println(z);
    )";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(program.get());
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "1\n2\n3\n");
}

TEST(EvaluatorTest, EvaluateIfStatementTrueCondition) {
    const std::string code = R"(
        var x: int = 1;
        if (x) {
            println("Condition is true");
        }
        x = 0;
        if (x) println("Condition is false");
    )";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(program.get());
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "Condition is true\n");
}

TEST(EvaluatorTest, EvaluateIfElseStatement) {
    const std::string code = R"(
        var x: bool = false;
        if (x) {
            println("This should not be printed");
        } else {
            println("Condition is false, so this is printed");
        }
    )";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(program.get());
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "Condition is false, so this is printed\n");
}

TEST(EvaluatorTest, EvaluateShortIfStatement) {
    const std::string code = R"(
        var x: int = 0;
        if (x) println(x);
        else println(x + 1);
    )";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    const auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(program.get());
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "1\n");
}

