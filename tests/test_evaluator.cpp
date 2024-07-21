#include <gtest/gtest.h>

#include "../src/include/evaluator.hpp"
#include "../src/include/runtime.hpp"
#include "../src/include/parser.hpp"
#include "../src/include/lexer.hpp"
#include "../src/include/errors.hpp"

TEST(EvaluatorTest, VariableInitializationTest) {
	const std::string code = "var a: int = 10;\nvar b: float = 1.0;";
	Lexer lexer(code);
	const std::vector<Token> tokens = lexer.tokenize();

	Parser parser(tokens);
	const auto program = parser.parse();

	RuntimeEnvironment env;
	Evaluator evaluator(env);
	evaluator.evaluate(program);

	ASSERT_EQ(evaluator.variablesCount(), 2);
}

TEST(EvaluatorTest, VariableAssignmentAndArithmeticTest) {
	const std::string code = "var a: int = 10;\nvar b: int = a * 100;";
	Lexer lexer(code);
	const std::vector<Token> tokens = lexer.tokenize();

	Parser parser(tokens);
	const auto program = parser.parse();

	RuntimeEnvironment env;
	Evaluator evaluator(env);
	evaluator.evaluate(program);

	ASSERT_EQ(evaluator.variablesCount(), 2);
}