#include <gtest/gtest.h>

#include "../src/execution/include/evaluator.hpp"
#include "../src/execution/include/runtime.hpp"
#include "../src/parsing/include/parser.hpp"
#include "../src/parsing/include/lexer.hpp"
#include "../src/errors/include/errors.hpp"

TEST(EvaluatorTest, EvaluatePrintStatement) {
    const std::string code = "println(\"Hello, World!\");";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "Hello, World!\n");
}

TEST(EvaluatorTest, EvaluateVariableDeclarationAndPrint) {
    const std::string code = "var x: int = 42; println(x);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "42\n");
}

TEST(EvaluatorTest, EvaluateFunctionDeclaration) {
    const std::string code = R"(
        def myFunction() -> null {
            println("Inside function");
        }
    )";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    // We do not call the function, so nothing should show up.
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "");
}

TEST(EvaluatorTest, EvaluateFunctionCall) {
    const std::string code = R"(
        def myFunction() -> null {
            var x: string = "Inside function.";
            println(x);
        }
        myFunction();
    )";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "Inside function.\n");
}

TEST(EvaluatorTest, EvaluateNestedFunctionCalls) {
    const std::string code = R"(
        def outerFunction() -> null {
            def innerFunction() -> null { 
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
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "Outer function\nInside inner function\n");
}

TEST(EvaluatorTest, EvaluateBinaryOperationAddition) {
    const std::string code = "println(5 + 3);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "8\n");
}

TEST(EvaluatorTest, EvaluateBinaryOperationMultiply) {
    const std::string code = "println(6 * 7);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "42\n");
}

TEST(EvaluatorTest, EvaluateBinaryOperationSubtraction) {
    const std::string code = "println(10 - 4);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "6\n");
}

TEST(EvaluatorTest, EvaluateBinaryOperationDivision) {
    const std::string code = "println(12 / 4);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "3\n");
}

TEST(EvaluatorTest, EvaluateUndefinedFunctionCall) {
    const std::string code = "undefinedFunction();";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    Evaluator evaluator;
    ASSERT_THROW(evaluator.evaluate(std::move(program)), ZynkError);
}

TEST(EvaluatorTest, EvaluateUndefinedVariableUsage) {
    const std::string code = "println(x);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    Evaluator evaluator;
    ASSERT_THROW(evaluator.evaluate(std::move(program)), ZynkError);
}

TEST(EvaluatorTest, EvaluateFloatVariableDeclarationAndPrint) {
    const std::string code = "var x: float = 3.14;\nprintln(x);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "3.14\n");
}

TEST(EvaluatorTest, EvaluateBinaryOperationFloatAddition) {
    const std::string code = "println(2.5 + 1.5);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "4.000000\n");
}

TEST(EvaluatorTest, DuplicateVariableDeclaration) {
    const std::string code = "var x: int = 42;\nvar x: int = 43;";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    Evaluator evaluator;
    ASSERT_THROW(evaluator.evaluate(std::move(program)), ZynkError);
}

TEST(EvaluatorTest, DuplicateFunctionDeclaration) {
    const std::string code = R"(
        def myFunction() -> null {}
        def myFunction() -> null {}
    )";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    Evaluator evaluator;
    ASSERT_THROW(evaluator.evaluate(std::move(program)), ZynkError);
}

TEST(EvaluatorTest, EvaluateVariableInExpression) {
    const std::string code = "var x: int = 5;\nprintln(x + 10);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "15\n");
}

TEST(EvaluatorTest, EvaluateBinaryOperationFloatMultiplication) {
    const std::string code = "println(2.5 * 2);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "5.000000\n");
}

TEST(EvaluatorTest, EvaluateDivisionByZero) {
    const std::string code = "println(1 / 0);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    Evaluator evaluator;
    ASSERT_THROW(evaluator.evaluate(std::move(program)), ZynkError);
}

TEST(EvaluatorTest, EvaluateEmptyProgram) {
    const std::string code = "";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "");
}

TEST(EvaluatorTest, EvaluateVariableModifyInteger) {
    const std::string code = "var a: int = 10;\na = 20;\nprintln(a);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "20\n");
}

TEST(EvaluatorTest, EvaluateVariableModifyWithExpression) {
    const std::string code = "var y: int = 5;\ny = y + 10;\nprintln(y);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
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
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
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
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
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
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
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
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "1\n");
}

TEST(EvaluatorTest, EvaluateReadStatementWithPrompt) {
    const std::string code = R"(
        var input: string = readInput("Enter your name: ");
        println(input);
    )";
    std::istringstream input("Alice\n");
    std::streambuf* originalCinStreamBuf = std::cin.rdbuf(input.rdbuf());
    testing::internal::CaptureStdout();

    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    Evaluator evaluator;
    evaluator.evaluate(std::move(program));

    std::cin.rdbuf(originalCinStreamBuf);
    std::string captured_output = testing::internal::GetCapturedStdout();
    ASSERT_TRUE(captured_output.find("Enter your name: ") != std::string::npos);
    ASSERT_EQ(captured_output.substr(captured_output.find("Alice")), "Alice\n");
}

TEST(EvaluatorTest, EvaluateReadStatementWithoutPrompt) {
    const std::string code = R"(
        var input: string = readInput();
        println(input);
    )";
    std::istringstream input("Bob\n");
    std::streambuf* originalCinStreamBuf = std::cin.rdbuf(input.rdbuf());
    testing::internal::CaptureStdout();

    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    Evaluator evaluator;
    evaluator.evaluate(std::move(program));

    std::cin.rdbuf(originalCinStreamBuf);
    std::string captured_output = testing::internal::GetCapturedStdout();
    ASSERT_EQ(captured_output, "Bob\n");
}

TEST(EvaluatorTest, EvaluateStringToIntCast) {
    const std::string code = "var x: int = int(\"42\");\nprintln(x + 1);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "43\n");
}

TEST(EvaluatorTest, EvaluateIntToFloatCast) {
    const std::string code = "var x: float = float(42);\nprintln(x);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "42.000000\n");
}

TEST(EvaluatorTest, EvaluateFloatToIntCast) {
    const std::string code = "var x: int = int(42.99);\nprintln(x);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "42\n");
}

TEST(EvaluatorTest, EvaluateStringToFloatCast) {
    const std::string code = "var x: float = float(\"42.50\");\nprintln(x);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "42.500000\n");
}

TEST(EvaluatorTest, EvaluateStringToBoolCast) {
    const std::string code = "var x: bool = bool(\"0\");\nprintln(x);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "false\n");
}

TEST(EvaluatorTest, EvaluateInvalidStringToIntCast) {
    const std::string code = "var x: int = int(\"not_a_number\");\nprintln(x);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();
    Evaluator evaluator;

    EXPECT_THROW(evaluator.evaluate(std::move(program)), ZynkError);
}

TEST(EvaluatorTest, EvaluateCommentedPrintStatement) {
    const std::string code = R"(
        // println("This should not be printed");
        var x: int = 5;
        println(x);
    )";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "5\n");
}

TEST(EvaluatorTest, EvaluateNegativeInteger) {
    const std::string code = "println(-42);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "-42\n");
}

TEST(EvaluatorTest, EvaluateNegativeFloat) {
    const std::string code = "println(-3.14);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "-3.14\n");
}

TEST(EvaluatorTest, EvaluateAdditionWithNegativeInteger) {
    const std::string code = "println(10 + -4);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "6\n");
}

TEST(EvaluatorTest, EvaluateMultiplicationWithNegativeInteger) {
    const std::string code = "println(-5.5 * 3);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "-16.500000\n");
}

TEST(EvaluatorTest, EvaluateLogicalAndTrueTrue) {
    const std::string code = "println(true && true);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "true\n");
}

TEST(EvaluatorTest, EvaluateLogicalAndTrueFalse) {
    const std::string code = "println(true && false);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "false\n");
}

TEST(EvaluatorTest, EvaluateLogicalOrTrueFalse) {
    const std::string code = "println(true || false);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "true\n");
}

TEST(EvaluatorTest, EvaluateLogicalOrFalseFalse) {
    const std::string code = "println(false || false);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "false\n");
}

TEST(EvaluatorTest, EvaluateFStringWithMultipleExpressions) {
    const std::string code = R"(
        var x: int = 42;
        var y: float = 3.5;
        println(f"x: {x}, y: {y}, sum: {x + y}");
    )";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "x: 42, y: 3.5, sum: 45.500000\n");
}

TEST(EvaluatorTest, EvaluatePrintExpressionWithParentheses) {
    const std::string code = "println((1 + 2) * 5);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "15\n");
}

TEST(EvaluatorTest, EvaluatePrintNestedParentheses) {
    const std::string code = "println(((3 + 4) * (2 + 1)));";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "21\n");
}

TEST(EvaluatorTest, EvaluateVarNestedParentheses) {
    const std::string code = R"(
        var x: int = ((3 + 4) * (2 + 1));
        println(x);
    )";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "21\n");
}

TEST(EvaluatorTest, EvaluateFStringWithIntegers) {
    const std::string code = R"(
        var x: int = 42;
        println(f"Value of x is {x}");
    )";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "Value of x is 42\n");
}

TEST(EvaluatorTest, EvaluateFStringWithBooleans) {
    const std::string code = R"(
        var is_valid: bool = true;
        println(f"Is the input valid? {is_valid}");
    )";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "Is the input valid? true\n");
}

TEST(EvaluatorTest, EvaluateFStringWithStringInterpolation) {
    const std::string code = R"(
        var name: string = "Alice";
        var age: int = 30;
        println(f"My name is {name} and I am {age} years old.");
    )";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "My name is Alice and I am 30 years old.\n");
}

TEST(EvaluatorTest, EvaluateFStringWithUndefinedVariable) {
    const std::string code = "println(f\"{abc}\");";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    Evaluator evaluator;
    ASSERT_THROW(evaluator.evaluate(std::move(program)), ZynkError);
}

TEST(EvaluatorTest, EvaluateFStringWithUnclosedBracket) {
    const std::string code = "println(f\"{abc\");";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    Evaluator evaluator;
    ASSERT_THROW(evaluator.evaluate(std::move(program)), ZynkError);
}

TEST(EvaluatorTest, EvaluateEqualOperation) {
    const std::string code = "println(5 == 5);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "true\n");
}

TEST(EvaluatorTest, EvaluateGreaterThanOperation) {
    const std::string code = "println(7 > 4);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "true\n");
}

TEST(EvaluatorTest, EvaluateStringEqualityOperation) {
    const std::string code = R"(
        var a: string = "hello";
        var b: string = "hello";
        println(a == b);
    )";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "true\n");
}

TEST(EvaluatorTest, EvaluateLessThanOrEqualOperation) {
    const std::string code = "println(5 <= 5);";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "true\n");
}

TEST(EvaluatorTest, FunctionCallWithRecursiveLoopThrowsError) {
    const std::string code = R"(
        var x: int = 0;

        def main() -> null {
            x = x + 1;
            if(x <= 1001) main();
        }
        main();
    )";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    Evaluator evaluator;
    ASSERT_THROW(evaluator.evaluate(std::move(program)), ZynkError);
}

TEST(EvaluatorTest, EvaluateFunctionReturningWrongType) {
    const std::string code = R"(
        def myFunction() -> int {
            return "string"; // Funkcja deklaruje int, ale zwraca string
        }
        myFunction();
    )";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    Evaluator evaluator;
    ASSERT_THROW(evaluator.evaluate(std::move(program)), ZynkError);
}

TEST(EvaluatorTest, EvaluateFunctionReturningNullWhenValueExpected) {
    const std::string code = R"(
        def myFunction() -> int {
            return 1;
        }
        var x: int = myFunction();
        println(x);
    )";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "1\n");
}

TEST(EvaluatorTest, EvaluateFunctionWithIntegerArgument) {
    const std::string code = R"(
        def myFunction(x: int) -> null {
            println(x + 1);
        }
        myFunction(10);
    )";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "11\n");
}

TEST(EvaluatorTest, EvaluateFunctionWithMultipleArguments) {
    const std::string code = R"(
        def add(a: int, b: int) -> int {
            return a + b;
        }
        println(add(3, 4));
    )";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "7\n");
}

TEST(EvaluatorTest, EvaluateFunctionWithStringArgument) {
    const std::string code = R"(
        def greet(name: string) -> null {
            println(f"Hello, {name}!");
        }
        greet("Alice");
    )";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "Hello, Alice!\n");
}

TEST(EvaluatorTest, EvaluateFunctionWithBooleanArgument) {
    const std::string code = R"(
        def checkStatus(active: bool) -> null {
            if (active) {
                println("Active");
            } else {
                println("Inactive");
            }
        }
        checkStatus(true);
    )";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "Active\n");
}

TEST(EvaluatorTest, EvaluateWhileLoopBasic) {
    const std::string code = R"(
        var x: int = 0;
        while (x < 5) {
            println(x);
            x = x + 1;
        }
    )";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "0\n1\n2\n3\n4\n");
}

TEST(EvaluatorTest, EvaluateWhileLoopWithBreak) {
    const std::string code = R"(
        var x: int = 0;
        while (true) {
            println(x);
            x = x + 1;
            if (x >= 3) {
                break;
            }
        }
        println("Loop ended");
    )";
    Lexer lexer(code);
    const std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto program = parser.parse();

    testing::internal::CaptureStdout();
    Evaluator evaluator;
    evaluator.evaluate(std::move(program));
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "0\n1\n2\nLoop ended\n");
}