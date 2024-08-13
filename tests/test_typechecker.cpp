#include <gtest/gtest.h>

#include "../src/typechecker/include/checker.hpp"
#include "../src/common/include/errors.hpp"

TEST(TypeCheckerTest, DetermineTypeIntegerValueNode) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto valueNodeInt = std::make_unique<ASTValue>("42", ASTValueType::Integer, 1);
    ASSERT_EQ(typeChecker.determineType(valueNodeInt.get()), ASTValueType::Integer);
}

TEST(TypeCheckerTest, DetermineTypeFloatValueNode) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto valueNodeFloat = std::make_unique<ASTValue>("3.14", ASTValueType::Float, 1);
    ASSERT_EQ(typeChecker.determineType(valueNodeFloat.get()), ASTValueType::Float);
}

TEST(TypeCheckerTest, DetermineTypeStringValueNode) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto valueNodeString = std::make_unique<ASTValue>("hello", ASTValueType::String, 1);
    ASSERT_EQ(typeChecker.determineType(valueNodeString.get()), ASTValueType::String);
}

TEST(TypeCheckerTest, DetermineTypeBooleanValueNode) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto valueNodeBool = std::make_unique<ASTValue>("true", ASTValueType::Bool, 1);
    ASSERT_EQ(typeChecker.determineType(valueNodeBool.get()), ASTValueType::Bool);
}

TEST(TypeCheckerTest, DetermineTypeZeroIntegerValueNode) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto valueNodeZeroInt = std::make_unique<ASTValue>("0", ASTValueType::Integer, 1);
    ASSERT_EQ(typeChecker.determineType(valueNodeZeroInt.get()), ASTValueType::Integer);
}

TEST(TypeCheckerTest, DetermineTypeZeroFloatValueNode) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto valueNodeZeroFloat = std::make_unique<ASTValue>("0.0", ASTValueType::Float, 1);
    ASSERT_EQ(typeChecker.determineType(valueNodeZeroFloat.get()), ASTValueType::Float);
}

TEST(TypeCheckerTest, DetermineTypeEmptyStringValueNode) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto valueNodeEmptyString = std::make_unique<ASTValue>("", ASTValueType::String, 1);
    ASSERT_EQ(typeChecker.determineType(valueNodeEmptyString.get()), ASTValueType::String);
}

TEST(TypeCheckerTest, DetermineTypeFalseBooleanValueNode) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto valueNodeFalseBool = std::make_unique<ASTValue>("false", ASTValueType::Bool, 1);
    ASSERT_EQ(typeChecker.determineType(valueNodeFalseBool.get()), ASTValueType::Bool);
}

TEST(TypeCheckerTest, DetermineTypeIntegerVariableNode) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);
    env.enterNewBlock();

    auto varValue = std::make_unique<ASTValue>("42", ASTValueType::Integer, 1);
    auto varDeclaration = std::make_unique<ASTVariableDeclaration>("x", ASTValueType::Integer, std::move(varValue), 1);
    env.declareVariable("x", varDeclaration.get());

    auto variableNode = std::make_unique<ASTVariable>("x", 1);
    ASSERT_EQ(typeChecker.determineType(variableNode.get()), ASTValueType::Integer);
    env.exitCurrentBlock();
}

TEST(TypeCheckerTest, DetermineTypeFloatVariableNode) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);
    env.enterNewBlock();

    auto varValue = std::make_unique<ASTValue>("3.14", ASTValueType::Float, 1);
    auto varDeclaration = std::make_unique<ASTVariableDeclaration>("y", ASTValueType::Float, std::move(varValue), 1);
    env.declareVariable("y", varDeclaration.get());

    auto variableNode = std::make_unique<ASTVariable>("y", 1);
    ASSERT_EQ(typeChecker.determineType(variableNode.get()), ASTValueType::Float);
    env.exitCurrentBlock();
}

TEST(TypeCheckerTest, DetermineTypeStringVariableNode) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);
    env.enterNewBlock();

    auto varValue = std::make_unique<ASTValue>("Hello", ASTValueType::String, 1);
    auto varDeclaration = std::make_unique<ASTVariableDeclaration>("greeting", ASTValueType::String, std::move(varValue), 1);
    env.declareVariable("greeting", varDeclaration.get());

    auto variableNode = std::make_unique<ASTVariable>("greeting", 1);
    ASSERT_EQ(typeChecker.determineType(variableNode.get()), ASTValueType::String);
    env.exitCurrentBlock();
}

TEST(TypeCheckerTest, DetermineTypeUndeclaredVariableNode) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);
    env.enterNewBlock();

    auto variableNode = std::make_unique<ASTVariable>("undeclaredVariable", 1);
    ASSERT_THROW(typeChecker.determineType(variableNode.get()), ZynkError);
    env.exitCurrentBlock();
}

TEST(TypeCheckerTest, DetermineTypeIntegerBinaryOperationNode) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto leftValue = std::make_unique<ASTValue>("5", ASTValueType::Integer, 1);
    auto rightValue = std::make_unique<ASTValue>("3", ASTValueType::Integer, 1);
    auto operationNode = std::make_unique<ASTBinaryOperation>(
        std::move(leftValue), "+", std::move(rightValue), 1
    );
    ASSERT_EQ(typeChecker.determineType(operationNode.get()), ASTValueType::Integer);
}

TEST(TypeCheckerTest, DetermineTypeFloatBinaryOperationNode) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto leftValue = std::make_unique<ASTValue>("5", ASTValueType::Integer, 1);
    auto rightValue = std::make_unique<ASTValue>("3.14", ASTValueType::Float, 1);
    auto operationNode = std::make_unique<ASTBinaryOperation>(
        std::move(leftValue), "*", std::move(rightValue), 1
    );
    ASSERT_EQ(typeChecker.determineType(operationNode.get()), ASTValueType::Float);
}

TEST(TypeCheckerTest, DetermineTypeMismatchedBinaryOperationNode) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto leftValue = std::make_unique<ASTValue>("Hello", ASTValueType::String, 1);
    auto rightValue = std::make_unique<ASTValue>("3.14", ASTValueType::Float, 1);
    auto operationNode = std::make_unique<ASTBinaryOperation>(
        std::move(leftValue), "+", std::move(rightValue), 1
    );
    ASSERT_EQ(typeChecker.determineType(operationNode.get()), ASTValueType::String);
}

TEST(TypeCheckerTest, CheckTypeMatchingTypes) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto valueNode = std::make_unique<ASTValue>("true", ASTValueType::Bool, 1);
    ASSERT_NO_THROW(typeChecker.checkType(ASTValueType::Bool, valueNode.get()));
}

TEST(TypeCheckerTest, CheckTypeMismatchedTypes) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto valueNode = std::make_unique<ASTValue>("42", ASTValueType::Integer, 1);
    ASSERT_THROW(typeChecker.checkType(ASTValueType::Float, valueNode.get()), ZynkError);
}

TEST(TypeCheckerTest, CheckTypeBinaryOperation_MatchingTypes) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto leftValue = std::make_unique<ASTValue>("5", ASTValueType::Integer, 1);
    auto rightValue = std::make_unique<ASTValue>("3", ASTValueType::Integer, 1);
    auto operationNode = std::make_unique<ASTBinaryOperation>(
        std::move(leftValue), "+", std::move(rightValue), 1
    );
    ASSERT_NO_THROW(typeChecker.checkType(ASTValueType::Integer, operationNode.get()));
}

TEST(TypeCheckerTest, CheckTypeUndeclaredVariableNode) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    env.enterNewBlock();
    auto variableNode = std::make_unique<ASTVariable>("undeclaredVariable", 1);
    ASSERT_THROW(typeChecker.checkType(ASTValueType::Integer, variableNode.get()), ZynkError);
    env.exitCurrentBlock();
}

TEST(TypeCheckerTest, CheckTypeMixedTypesInBinaryOperation) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto leftValue = std::make_unique<ASTValue>("5", ASTValueType::Integer, 1);
    auto rightValue = std::make_unique<ASTValue>("true", ASTValueType::Bool, 1);
    auto operationNode = std::make_unique<ASTBinaryOperation>(
        std::move(leftValue), "+", std::move(rightValue), 1
    );
    ASSERT_THROW(typeChecker.checkType(ASTValueType::Integer, operationNode.get()), ZynkError);
}

TEST(TypeCheckerTest, DetermineTypeCastFromFloatToInt) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto valueNodeFloat = std::make_unique<ASTValue>("3.14", ASTValueType::Float, 1);
    auto castNode = std::make_unique<ASTTypeCast>(std::move(valueNodeFloat), ASTValueType::Integer, 1);
    ASSERT_EQ(typeChecker.determineType(castNode.get()), ASTValueType::Integer);
}

TEST(TypeCheckerTest, DetermineTypeCastFromStringToFloat) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto valueNodeString = std::make_unique<ASTValue>("3.14", ASTValueType::String, 1);
    auto castNode = std::make_unique<ASTTypeCast>(std::move(valueNodeString), ASTValueType::Float, 1);
    ASSERT_EQ(typeChecker.determineType(castNode.get()), ASTValueType::Float);
}

TEST(TypeCheckerTest, DetermineTypeCastFromIntToBool) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto valueNodeInt = std::make_unique<ASTValue>("1", ASTValueType::Integer, 1);
    auto castNode = std::make_unique<ASTTypeCast>(std::move(valueNodeInt), ASTValueType::Bool, 1);
    ASSERT_EQ(typeChecker.determineType(castNode.get()), ASTValueType::Bool);
}

TEST(TypeCheckerTest, DetermineTypeAndOperationNode) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto leftValue = std::make_unique<ASTValue>("true", ASTValueType::Bool, 1);
    auto rightValue = std::make_unique<ASTValue>("false", ASTValueType::Bool, 1);
    auto operationNode = std::make_unique<ASTAndOperation>(
        std::move(leftValue), std::move(rightValue), 1
    );
    ASSERT_EQ(typeChecker.determineType(operationNode.get()), ASTValueType::Bool);
}

TEST(TypeCheckerTest, DetermineTypeOrOperationNode) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto leftValue = std::make_unique<ASTValue>("true", ASTValueType::Bool, 1);
    auto rightValue = std::make_unique<ASTValue>("false", ASTValueType::Bool, 1);
    auto operationNode = std::make_unique<ASTOrOperation>(
        std::move(leftValue), std::move(rightValue), 1
    );
    ASSERT_EQ(typeChecker.determineType(operationNode.get()), ASTValueType::Bool);
}

TEST(TypeCheckerTest, DetermineTypeMismatchedAndOperationNode) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto leftValue = std::make_unique<ASTValue>("true", ASTValueType::Bool, 1);
    auto rightValue = std::make_unique<ASTValue>("5", ASTValueType::Integer, 1);
    auto operationNode = std::make_unique<ASTAndOperation>(
        std::move(leftValue), std::move(rightValue), 1
    );
    ASSERT_THROW(typeChecker.determineType(operationNode.get()), ZynkError);
}

TEST(TypeCheckerTest, DetermineTypeMismatchedOrOperationNode) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto leftValue = std::make_unique<ASTValue>("true", ASTValueType::Bool, 1);
    auto rightValue = std::make_unique<ASTValue>("3.14", ASTValueType::Float, 1);
    auto operationNode = std::make_unique<ASTOrOperation>(
        std::move(leftValue), std::move(rightValue), 1
    );
    ASSERT_THROW(typeChecker.determineType(operationNode.get()), ZynkError);
}

TEST(TypeCheckerTest, CheckTypeAndOperation) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto leftValue = std::make_unique<ASTValue>("true", ASTValueType::Bool, 1);
    auto rightValue = std::make_unique<ASTValue>("false", ASTValueType::Bool, 1);
    auto operationNode = std::make_unique<ASTAndOperation>(
        std::move(leftValue), std::move(rightValue), 1
    );
    ASSERT_NO_THROW(typeChecker.checkType(ASTValueType::Bool, operationNode.get()));
}

TEST(TypeCheckerTest, CheckTypeOrOperation) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto leftValue = std::make_unique<ASTValue>("false", ASTValueType::Bool, 1);
    auto rightValue = std::make_unique<ASTValue>("true", ASTValueType::Bool, 1);
    auto operationNode = std::make_unique<ASTOrOperation>(
        std::move(leftValue), std::move(rightValue), 1
    );
    ASSERT_NO_THROW(typeChecker.checkType(ASTValueType::Bool, operationNode.get()));
}

TEST(TypeCheckerTest, CheckTypeMismatchedAndOperation) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto leftValue = std::make_unique<ASTValue>("false", ASTValueType::Bool, 1);
    auto rightValue = std::make_unique<ASTValue>("42", ASTValueType::Integer, 1);
    auto operationNode = std::make_unique<ASTAndOperation>(
        std::move(leftValue), std::move(rightValue), 1
    );
    ASSERT_THROW(typeChecker.checkType(ASTValueType::Bool, operationNode.get()), ZynkError);
}

TEST(TypeCheckerTest, CheckTypeMismatchedOrOperation) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto leftValue = std::make_unique<ASTValue>("true", ASTValueType::Bool, 1);
    auto rightValue = std::make_unique<ASTValue>("Hello", ASTValueType::String, 1);
    auto operationNode = std::make_unique<ASTOrOperation>(
        std::move(leftValue), std::move(rightValue), 1
    );
    ASSERT_THROW(typeChecker.checkType(ASTValueType::Bool, operationNode.get()), ZynkError);
}