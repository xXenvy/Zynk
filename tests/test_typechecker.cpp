#include <gtest/gtest.h>

#include "../src/typechecker/include/checker.hpp"
#include "../src/common/include/errors.hpp"

TEST(TypeCheckerTest, DetermineTypeIntegerValueNode) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto valueNodeInt = std::make_unique<ASTValue>("42", ASTValueType::Integer);
    ASSERT_EQ(typeChecker.determineType(valueNodeInt.get()), ASTValueType::Integer);
}

TEST(TypeCheckerTest, DetermineTypeFloatValueNode) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto valueNodeFloat = std::make_unique<ASTValue>("3.14", ASTValueType::Float);
    ASSERT_EQ(typeChecker.determineType(valueNodeFloat.get()), ASTValueType::Float);
}

TEST(TypeCheckerTest, DetermineTypeStringValueNode) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto valueNodeString = std::make_unique<ASTValue>("hello", ASTValueType::String);
    ASSERT_EQ(typeChecker.determineType(valueNodeString.get()), ASTValueType::String);
}

TEST(TypeCheckerTest, DetermineTypeBooleanValueNode) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto valueNodeBool = std::make_unique<ASTValue>("true", ASTValueType::Bool);
    ASSERT_EQ(typeChecker.determineType(valueNodeBool.get()), ASTValueType::Bool);
}

TEST(TypeCheckerTest, DetermineTypeZeroIntegerValueNode) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto valueNodeZeroInt = std::make_unique<ASTValue>("0", ASTValueType::Integer);
    ASSERT_EQ(typeChecker.determineType(valueNodeZeroInt.get()), ASTValueType::Integer);
}

TEST(TypeCheckerTest, DetermineTypeZeroFloatValueNode) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto valueNodeZeroFloat = std::make_unique<ASTValue>("0.0", ASTValueType::Float);
    ASSERT_EQ(typeChecker.determineType(valueNodeZeroFloat.get()), ASTValueType::Float);
}

TEST(TypeCheckerTest, DetermineTypeEmptyStringValueNode) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto valueNodeEmptyString = std::make_unique<ASTValue>("", ASTValueType::String);
    ASSERT_EQ(typeChecker.determineType(valueNodeEmptyString.get()), ASTValueType::String);
}

TEST(TypeCheckerTest, DetermineTypeFalseBooleanValueNode) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto valueNodeFalseBool = std::make_unique<ASTValue>("false", ASTValueType::Bool);
    ASSERT_EQ(typeChecker.determineType(valueNodeFalseBool.get()), ASTValueType::Bool);
}

TEST(TypeCheckerTest, DetermineTypeIntegerVariableNode) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);
    env.enterNewBlock();

    auto varValue = std::make_unique<ASTValue>("42", ASTValueType::Integer);
    auto varDeclaration = std::make_unique<ASTVariableDeclaration>("x", ASTValueType::Integer, std::move(varValue));
    env.declareVariable("x", varDeclaration.get());

    auto variableNode = std::make_unique<ASTVariable>("x");
    ASSERT_EQ(typeChecker.determineType(variableNode.get()), ASTValueType::Integer);
    env.exitCurrentBlock();
}

TEST(TypeCheckerTest, DetermineTypeFloatVariableNode) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);
    env.enterNewBlock();

    auto varValue = std::make_unique<ASTValue>("3.14", ASTValueType::Float);
    auto varDeclaration = std::make_unique<ASTVariableDeclaration>("y", ASTValueType::Float, std::move(varValue));
    env.declareVariable("y", varDeclaration.get());

    auto variableNode = std::make_unique<ASTVariable>("y");
    ASSERT_EQ(typeChecker.determineType(variableNode.get()), ASTValueType::Float);
    env.exitCurrentBlock();
}

TEST(TypeCheckerTest, DetermineTypeStringVariableNode) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);
    env.enterNewBlock();

    auto varValue = std::make_unique<ASTValue>("Hello", ASTValueType::String);
    auto varDeclaration = std::make_unique<ASTVariableDeclaration>("greeting", ASTValueType::String, std::move(varValue));
    env.declareVariable("greeting", varDeclaration.get());

    auto variableNode = std::make_unique<ASTVariable>("greeting");
    ASSERT_EQ(typeChecker.determineType(variableNode.get()), ASTValueType::String);
    env.exitCurrentBlock();
}

TEST(TypeCheckerTest, DetermineTypeUndeclaredVariableNode) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);
    env.enterNewBlock();

    auto variableNode = std::make_unique<ASTVariable>("undeclaredVariable");
    ASSERT_THROW(typeChecker.determineType(variableNode.get()), ZynkError);
    env.exitCurrentBlock();
}

TEST(TypeCheckerTest, DetermineTypeIntegerBinaryOperationNode) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto leftValue = std::make_unique<ASTValue>("5", ASTValueType::Integer);
    auto rightValue = std::make_unique<ASTValue>("3", ASTValueType::Integer);
    auto operationNode = std::make_unique<ASTBinaryOperation>(
        std::move(leftValue), "+", std::move(rightValue));
    ASSERT_EQ(typeChecker.determineType(operationNode.get()), ASTValueType::Integer);
}

TEST(TypeCheckerTest, DetermineTypeFloatBinaryOperationNode) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto leftValue = std::make_unique<ASTValue>("5", ASTValueType::Integer);
    auto rightValue = std::make_unique<ASTValue>("3.14", ASTValueType::Float);
    auto operationNode = std::make_unique<ASTBinaryOperation>(
        std::move(leftValue), "*", std::move(rightValue));
    ASSERT_EQ(typeChecker.determineType(operationNode.get()), ASTValueType::Float);
}

TEST(TypeCheckerTest, DetermineTypeMismatchedBinaryOperationNode) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto leftValue = std::make_unique<ASTValue>("Hello", ASTValueType::String);
    auto rightValue = std::make_unique<ASTValue>("3.14", ASTValueType::Float);
    auto operationNode = std::make_unique<ASTBinaryOperation>(
        std::move(leftValue), "+", std::move(rightValue));

    ASSERT_EQ(typeChecker.determineType(operationNode.get()), ASTValueType::String);
}

TEST(TypeCheckerTest, CheckTypeMatchingTypes) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto valueNode = std::make_unique<ASTValue>("true", ASTValueType::Bool);
    ASSERT_NO_THROW(typeChecker.checkType(ASTValueType::Bool, valueNode.get()));
}

TEST(TypeCheckerTest, CheckTypeMismatchedTypes) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto valueNode = std::make_unique<ASTValue>("42", ASTValueType::Integer);
    ASSERT_THROW(typeChecker.checkType(ASTValueType::Float, valueNode.get()), ZynkError);
}

TEST(TypeCheckerTest, CheckTypeBinaryOperation_MatchingTypes) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto leftValue = std::make_unique<ASTValue>("5", ASTValueType::Integer);
    auto rightValue = std::make_unique<ASTValue>("3", ASTValueType::Integer);
    auto operationNode = std::make_unique<ASTBinaryOperation>(
        std::move(leftValue), "+", std::move(rightValue));

    ASSERT_NO_THROW(typeChecker.checkType(ASTValueType::Integer, operationNode.get()));
}

TEST(TypeCheckerTest, CheckTypeUndeclaredVariableNode) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);
    env.enterNewBlock();

    auto variableNode = std::make_unique<ASTVariable>("undeclaredVariable");
    ASSERT_THROW(typeChecker.checkType(ASTValueType::Integer, variableNode.get()), ZynkError);
    env.exitCurrentBlock();
}

TEST(TypeCheckerTest, CheckTypeMixedTypesInBinaryOperation) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto leftValue = std::make_unique<ASTValue>("5", ASTValueType::Integer);
    auto rightValue = std::make_unique<ASTValue>("true", ASTValueType::Bool);
    auto operationNode = std::make_unique<ASTBinaryOperation>(
        std::move(leftValue), "+", std::move(rightValue));

    ASSERT_THROW(typeChecker.checkType(ASTValueType::Integer, operationNode.get()), ZynkError);
}