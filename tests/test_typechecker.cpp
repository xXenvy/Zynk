#include <gtest/gtest.h>

#include "../src/typechecker/include/checker.hpp"
#include "../src/common/include/errors.hpp"

TEST(TypeCheckerTest, DetermineTypeIntegerValue) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto ASTValueInt = std::make_unique<ASTValue>("42", ASTValueType::Integer, 1);
    ASSERT_EQ(typeChecker.determineType(ASTValueInt.get()), ASTValueType::Integer);
}

TEST(TypeCheckerTest, DetermineTypeFloatValue) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto ASTValueFloat = std::make_unique<ASTValue>("3.14", ASTValueType::Float, 1);
    ASSERT_EQ(typeChecker.determineType(ASTValueFloat.get()), ASTValueType::Float);
}

TEST(TypeCheckerTest, DetermineTypeStringValue) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto ASTValueString = std::make_unique<ASTValue>("hello", ASTValueType::String, 1);
    ASSERT_EQ(typeChecker.determineType(ASTValueString.get()), ASTValueType::String);
}

TEST(TypeCheckerTest, DetermineTypeBooleanValue) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto ASTValueBool = std::make_unique<ASTValue>("true", ASTValueType::Bool, 1);
    ASSERT_EQ(typeChecker.determineType(ASTValueBool.get()), ASTValueType::Bool);
}

TEST(TypeCheckerTest, DetermineTypeZeroIntegerValue) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto ASTValueZeroInt = std::make_unique<ASTValue>("0", ASTValueType::Integer, 1);
    ASSERT_EQ(typeChecker.determineType(ASTValueZeroInt.get()), ASTValueType::Integer);
}

TEST(TypeCheckerTest, DetermineTypeZeroFloatValue) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto ASTValueZeroFloat = std::make_unique<ASTValue>("0.0", ASTValueType::Float, 1);
    ASSERT_EQ(typeChecker.determineType(ASTValueZeroFloat.get()), ASTValueType::Float);
}

TEST(TypeCheckerTest, DetermineTypeEmptyStringValue) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto ASTValueEmptyString = std::make_unique<ASTValue>("", ASTValueType::String, 1);
    ASSERT_EQ(typeChecker.determineType(ASTValueEmptyString.get()), ASTValueType::String);
}

TEST(TypeCheckerTest, DetermineTypeFalseBooleanValue) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto ASTValueFalseBool = std::make_unique<ASTValue>("false", ASTValueType::Bool, 1);
    ASSERT_EQ(typeChecker.determineType(ASTValueFalseBool.get()), ASTValueType::Bool);
}

TEST(TypeCheckerTest, DetermineTypeIntegerVariable) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);
    env.enterNewBlock();

    auto ASTValueInt = std::make_unique<ASTValue>("42", ASTValueType::Integer, 1);
    auto ASTVarDeclaration = std::make_unique<ASTVariableDeclaration>("x", ASTValueType::Integer, std::move(ASTValueInt), 1);
    env.declareVariable("x", ASTVarDeclaration.get());

    auto ASTVariableInt = std::make_unique<ASTVariable>("x", 1);
    ASSERT_EQ(typeChecker.determineType(ASTVariableInt.get()), ASTValueType::Integer);
    env.exitCurrentBlock();
}

TEST(TypeCheckerTest, DetermineTypeFloatVariable) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);
    env.enterNewBlock();

    auto ASTValueFloat = std::make_unique<ASTValue>("3.14", ASTValueType::Float, 1);
    auto ASTVarDeclaration = std::make_unique<ASTVariableDeclaration>("y", ASTValueType::Float, std::move(ASTValueFloat), 1);
    env.declareVariable("y", ASTVarDeclaration.get());

    auto ASTVariableFloat = std::make_unique<ASTVariable>("y", 1);
    ASSERT_EQ(typeChecker.determineType(ASTVariableFloat.get()), ASTValueType::Float);
    env.exitCurrentBlock();
}

TEST(TypeCheckerTest, DetermineTypeStringVariable) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);
    env.enterNewBlock();

    auto ASTValueString = std::make_unique<ASTValue>("Hello", ASTValueType::String, 1);
    auto ASTVarDeclaration = std::make_unique<ASTVariableDeclaration>("greeting", ASTValueType::String, std::move(ASTValueString), 1);
    env.declareVariable("greeting", ASTVarDeclaration.get());

    auto ASTVariableString = std::make_unique<ASTVariable>("greeting", 1);
    ASSERT_EQ(typeChecker.determineType(ASTVariableString.get()), ASTValueType::String);
    env.exitCurrentBlock();
}

TEST(TypeCheckerTest, DetermineTypeUndeclaredVariable) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);
    env.enterNewBlock();

    auto ASTVariableUndeclared = std::make_unique<ASTVariable>("undeclaredVariable", 1);
    ASSERT_THROW(typeChecker.determineType(ASTVariableUndeclared.get()), ZynkError);
    env.exitCurrentBlock();
}

TEST(TypeCheckerTest, DetermineTypeIntegerBinaryOperation) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto ASTLeftValueInt = std::make_unique<ASTValue>("5", ASTValueType::Integer, 1);
    auto ASTRightValueInt = std::make_unique<ASTValue>("3", ASTValueType::Integer, 1);
    auto ASTOperation = std::make_unique<ASTBinaryOperation>(
        std::move(ASTLeftValueInt), "+", std::move(ASTRightValueInt), 1
    );
    ASSERT_EQ(typeChecker.determineType(ASTOperation.get()), ASTValueType::Integer);
}

TEST(TypeCheckerTest, DetermineTypeFloatBinaryOperation) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto ASTLeftValueInt = std::make_unique<ASTValue>("5", ASTValueType::Integer, 1);
    auto ASTRightValueFloat = std::make_unique<ASTValue>("3.14", ASTValueType::Float, 1);
    auto ASTOperation = std::make_unique<ASTBinaryOperation>(
        std::move(ASTLeftValueInt), "*", std::move(ASTRightValueFloat), 1
    );
    ASSERT_EQ(typeChecker.determineType(ASTOperation.get()), ASTValueType::Float);
}

TEST(TypeCheckerTest, DetermineTypeMismatchedBinaryOperation) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto ASTLeftValueString = std::make_unique<ASTValue>("Hello", ASTValueType::String, 1);
    auto ASTRightValueFloat = std::make_unique<ASTValue>("3.14", ASTValueType::Float, 1);
    auto ASTOperation = std::make_unique<ASTBinaryOperation>(
        std::move(ASTLeftValueString), "+", std::move(ASTRightValueFloat), 1
    );
    ASSERT_EQ(typeChecker.determineType(ASTOperation.get()), ASTValueType::String);
}

TEST(TypeCheckerTest, DetermineTypeComparisonOperation) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto ASTLeftValueInt = std::make_unique<ASTValue>("5", ASTValueType::Integer, 1);
    auto ASTRightValueInt = std::make_unique<ASTValue>("3", ASTValueType::Integer, 1);
    auto ComparisonOperation = std::make_unique<ASTComparisonOperation>(
        std::move(ASTLeftValueInt), "==", std::move(ASTRightValueInt), 1
    );
    ASSERT_EQ(typeChecker.determineType(ComparisonOperation.get()), ASTValueType::Bool);

    auto ASTLeftValueFloat = std::make_unique<ASTValue>("2.5", ASTValueType::Float, 1);
    auto ASTRightValueFloat = std::make_unique<ASTValue>("2.5", ASTValueType::Float, 1);
    ComparisonOperation = std::make_unique<ASTComparisonOperation>(
        std::move(ASTLeftValueFloat), "==", std::move(ASTRightValueFloat), 1
    );
    ASSERT_EQ(typeChecker.determineType(ComparisonOperation.get()), ASTValueType::Bool);

    auto ASTLeftValueString = std::make_unique<ASTValue>("hello", ASTValueType::String, 1);
    auto ASTRightValueString = std::make_unique<ASTValue>("world", ASTValueType::String, 1);
    ComparisonOperation = std::make_unique<ASTComparisonOperation>(
        std::move(ASTLeftValueString), "!=", std::move(ASTRightValueString), 1
    );
    ASSERT_EQ(typeChecker.determineType(ComparisonOperation.get()), ASTValueType::Bool);
}

TEST(TypeCheckerTest, DetermineTypeFString) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto fString = std::make_unique<ASTFString>("Street: {name}", 1);
    ASSERT_EQ(typeChecker.determineType(fString.get()), ASTValueType::String);
}

TEST(TypeCheckerTest, DetermineTypeCastFromFloatToInt) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto ASTValueFloat = std::make_unique<ASTValue>("3.14", ASTValueType::Float, 1);
    auto ASTCast = std::make_unique<ASTTypeCast>(std::move(ASTValueFloat), ASTValueType::Integer, 1);
    ASSERT_EQ(typeChecker.determineType(ASTCast.get()), ASTValueType::Integer);
}

TEST(TypeCheckerTest, DetermineTypeCastFromStringToFloat) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto ASTValueString = std::make_unique<ASTValue>("3.14", ASTValueType::String, 1);
    auto ASTCast = std::make_unique<ASTTypeCast>(std::move(ASTValueString), ASTValueType::Float, 1);
    ASSERT_EQ(typeChecker.determineType(ASTCast.get()), ASTValueType::Float);
}

TEST(TypeCheckerTest, DetermineTypeCastFromIntToBool) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto ASTValueInt = std::make_unique<ASTValue>("1", ASTValueType::Integer, 1);
    auto ASTCast = std::make_unique<ASTTypeCast>(std::move(ASTValueInt), ASTValueType::Bool, 1);
    ASSERT_EQ(typeChecker.determineType(ASTCast.get()), ASTValueType::Bool);
}

TEST(TypeCheckerTest, DetermineTypeAndOperation) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);

    auto ASTLeftValueBool = std::make_unique<ASTValue>("true", ASTValueType::Bool, 1);
    auto ASTRightValueBool = std::make_unique<ASTValue>("false", ASTValueType::Bool, 1);
    auto ASTOperation = std::make_unique<ASTAndOperation>(
        std::move(ASTLeftValueBool), std::move(ASTRightValueBool), 1
    );
    ASSERT_EQ(typeChecker.determineType(ASTOperation.get()), ASTValueType::Bool);
}

TEST(TypeCheckerTest, DetermineTypeReturn) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);
    env.enterNewBlock();

    auto func = std::make_unique<ASTFunction>("myFunc", ASTValueType::Integer, 1);
    auto returnValue = std::make_unique<ASTValue>("42", ASTValueType::Integer, 1);
    auto returnStmt = std::make_unique<ASTReturn>(std::move(returnValue), 1);

    env.declareFunction("myFunc", func.get());
    ASSERT_EQ(typeChecker.determineType(returnStmt.get()), ASTValueType::Integer);
    env.exitCurrentBlock();
}

TEST(TypeCheckerTest, DetermineTypeFunctionCall) {
    RuntimeEnvironment env;
    TypeChecker typeChecker(env);
    env.enterNewBlock();

    auto func = std::make_unique<ASTFunction>("myFunc", ASTValueType::Integer, 1);
    env.declareFunction("myFunc", func.get());

    auto funcCall = std::make_unique<ASTFunctionCall>("myFunc", 1);
    ASSERT_EQ(typeChecker.determineType(funcCall.get()), ASTValueType::Integer);
    env.exitCurrentBlock();
}