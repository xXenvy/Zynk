#include <gtest/gtest.h>

#include "../src/execution/include/runtime.hpp"
#include "../src/parsing/include/ast.hpp"
#include "../src/errors/include/errors.hpp"

TEST(RuntimeEnvironmentTest, VariableDeclaration) {
    RuntimeEnvironment env;
    env.enterNewBlock();

    auto varValue = std::make_unique<ASTValue>("10", ASTValueType::Integer, 10);
    env.declareVariable("x", std::move(varValue));

    auto retrievedVar = env.getVariable("x", 10, true);
    ASSERT_EQ(retrievedVar->value, "10");

    auto varValue2 = std::make_unique<ASTValue>("11", ASTValueType::Integer, 11);
    ASSERT_THROW(env.declareVariable("x", std::move(varValue2)), ZynkError);
    ASSERT_EQ(env.isVariableDeclared("x"), true);
    env.exitCurrentBlock();
}

TEST(RuntimeEnvironmentTest, VariableDeclarationWithoutDeepSearch) {
    RuntimeEnvironment env;
    env.enterNewBlock();

    auto varValue = std::make_unique<ASTValue>("10", ASTValueType::Integer, 10);
    ASSERT_NO_THROW(env.declareVariable("x", std::move(varValue)));

    env.enterNewBlock();
    auto retrievedVar = env.getVariable("x", 10, true);
    ASSERT_EQ(retrievedVar->line, 10);

    auto varValue2 = std::make_unique<ASTValue>("50", ASTValueType::Integer, 11);
    ASSERT_NO_THROW(env.declareVariable("x", std::move(varValue2)));

    auto retrievedVar2 = env.getVariable("x", 50, false);
    ASSERT_EQ(retrievedVar2->line, 11);
    env.exitCurrentBlock();

    auto varValue3 = std::make_unique<ASTValue>("12", ASTValueType::Integer, 12);
    ASSERT_THROW(env.declareVariable("x", std::move(varValue3)), ZynkError);
    ASSERT_EQ(env.isVariableDeclared("x"), true);
    env.exitCurrentBlock();
}

TEST(RuntimeEnvironmentTest, FunctionDeclaration) {
    RuntimeEnvironment env;
    env.enterNewBlock();

    auto funcDeclaration = std::make_unique<ASTFunction>("myFunction", ASTValueType::None, 20);
    ASSERT_NO_THROW(env.declareFunction(std::move(funcDeclaration)));

    auto retrievedFunc = env.getFunction("myFunction", 20);
    ASSERT_EQ(retrievedFunc->name, "myFunction");
    ASSERT_EQ(retrievedFunc->body.size(), 0);

    auto funcDeclaration2 = std::make_unique<ASTFunction>("myFunction", ASTValueType::None, 5);
    ASSERT_THROW(env.declareFunction(std::move(funcDeclaration2)), ZynkError);
    ASSERT_EQ(env.isFunctionDeclared("myFunction"), true);
    env.exitCurrentBlock();
}

TEST(RuntimeEnvironmentTest, VariableNotDefinedError) {
    RuntimeEnvironment env;
    env.enterNewBlock();

    ASSERT_THROW(env.getVariable("undefinedVar", 1, true), ZynkError);
    ASSERT_EQ(env.isVariableDeclared("undefinedVar"), false);

    env.exitCurrentBlock();
}

TEST(RuntimeEnvironmentTest, FunctionNotDefinedError) {
    RuntimeEnvironment env;
    env.enterNewBlock();

    ASSERT_THROW(env.getFunction("undefinedFunc", 10), ZynkError);
    ASSERT_EQ(env.isFunctionDeclared("undefinedFunc"), false);

    env.exitCurrentBlock();
}

TEST(RuntimeEnvironmentTest, EnterAndExitBlock) {
    RuntimeEnvironment env;

    ASSERT_EQ(env.currentBlock(), nullptr);
    env.enterNewBlock();
    ASSERT_NE(env.currentBlock(), nullptr);

    auto varValue = std::make_unique<ASTValue>("1.5", ASTValueType::Float, 30);
    env.declareVariable("x", std::move(varValue));

    ASSERT_EQ(env.isVariableDeclared("x"), true);
    env.enterNewBlock();
    ASSERT_EQ(env.isVariableDeclared("x"), true);
    env.exitCurrentBlock();

    ASSERT_EQ(env.isVariableDeclared("x"), true);
    env.exitCurrentBlock();

    ASSERT_EQ(env.currentBlock(), nullptr);
    ASSERT_EQ(env.isVariableDeclared("x"), false);
}

TEST(RuntimeEnvironmentTest, GarbageCollectionAfterBlockExit) {
    RuntimeEnvironment env;
    env.enterNewBlock();

    auto varValue1 = std::make_unique<ASTValue>("1.5", ASTValueType::Float, 40);
    auto varValue2 = std::make_unique<ASTValue>("5", ASTValueType::Integer, 41);

    env.declareVariable("var1", std::move(varValue1));
    env.declareVariable("var2", std::move(varValue2));

    ASSERT_EQ(env.currentBlock()->variables.size(), 2);
    env.exitCurrentBlock();
    ASSERT_EQ(env.currentBlock(), nullptr);
}

TEST(RuntimeEnvironmentTest, GarbageCollectionWithNestedBlocks) {
    RuntimeEnvironment env;
    env.enterNewBlock();

    auto varValue = std::make_unique<ASTValue>("Abc", ASTValueType::String, 50);
    auto globalVar = std::make_unique<ASTValue>("Abc", ASTValueType::String, 50);

    auto globalFunc = std::make_unique<ASTFunction>("globalFunc", ASTValueType::None, 51);

    ASSERT_NO_THROW(env.declareVariable("globalVar", std::move(globalVar)));
    ASSERT_NO_THROW(env.declareFunction(std::move(globalFunc)));

    ASSERT_EQ(env.currentBlock()->variables.size(), 1);
    ASSERT_EQ(env.currentBlock()->functions.size(), 1);
    ASSERT_TRUE(env.isVariableDeclared("globalVar"));
    ASSERT_TRUE(env.isFunctionDeclared("globalFunc"));

    env.enterNewBlock();
    auto innerVarValue = std::make_unique<ASTValue>("Cba", ASTValueType::String, 60);
    env.declareVariable("innerVar", std::move(innerVarValue));

    auto innerFunc = std::make_unique<ASTFunction>("innerFunc", ASTValueType::None, 61);
    env.declareFunction(std::move(innerFunc));

    ASSERT_EQ(env.currentBlock()->variables.size(), 1);
    ASSERT_TRUE(env.isVariableDeclared("innerVar"));
    ASSERT_TRUE(env.isFunctionDeclared("innerFunc"));

    env.exitCurrentBlock();
    ASSERT_EQ(env.currentBlock()->variables.size(), 1);
    ASSERT_FALSE(env.isVariableDeclared("innerVar"));
    ASSERT_FALSE(env.isFunctionDeclared("innerFunc"));
    ASSERT_TRUE(env.isVariableDeclared("globalVar"));
    ASSERT_TRUE(env.isFunctionDeclared("globalFunc"));

    env.exitCurrentBlock();
    ASSERT_EQ(env.currentBlock(), nullptr);
    ASSERT_FALSE(env.isVariableDeclared("globalVar"));
    ASSERT_FALSE(env.isFunctionDeclared("globalFunc"));
}
