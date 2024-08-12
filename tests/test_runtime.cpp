#include <gtest/gtest.h>

#include "../src/execution/include/runtime.hpp"
#include "../src/parsing/include/ast.hpp"
#include "../src/common/include/errors.hpp"

TEST(RuntimeEnvironmentTest, VariableDeclaration) {
    RuntimeEnvironment env;
    env.enterNewBlock();

    auto varValue = std::make_unique<ASTValue>("10", ASTValueType::Integer, 10);
    auto varDeclaration = std::make_unique<ASTVariableDeclaration>("x", ASTValueType::Integer, std::move(varValue), 10);
    ASSERT_NO_THROW(env.declareVariable("x", varDeclaration.get()));

    auto retrievedVar = env.getVariable("x", 10);
    ASSERT_EQ(retrievedVar->name, "x");

    ASSERT_THROW(env.declareVariable("x", varDeclaration.get()), ZynkError);
    ASSERT_EQ(env.isVariableDeclared("x"), true);
    env.exitCurrentBlock();
}

TEST(RuntimeEnvironmentTest, FunctionDeclaration) {
    RuntimeEnvironment env;
    env.enterNewBlock();

    auto funcDeclaration = std::make_unique<ASTFunction>("myFunction", 20);
    ASSERT_NO_THROW(env.declareFunction("myFunction", funcDeclaration.get()));

    auto retrievedFunc = env.getFunction("myFunction", 20);
    ASSERT_EQ(retrievedFunc->name, "myFunction");
    ASSERT_EQ(retrievedFunc->body.size(), 0);

    ASSERT_THROW(env.declareFunction("myFunction", funcDeclaration.get()), ZynkError);
    ASSERT_EQ(env.isFunctionDeclared("myFunction"), true);
    env.exitCurrentBlock();
}

TEST(RuntimeEnvironmentTest, VariableNotDefinedError) {
    RuntimeEnvironment env;
    env.enterNewBlock();

    ASSERT_THROW(env.getVariable("undefinedVar", 1), ZynkError);
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
    auto varDeclaration = std::make_unique<ASTVariableDeclaration>("x", ASTValueType::Float, std::move(varValue), 30);
    env.declareVariable("x", varDeclaration.get());

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
    auto varDeclaration1 = std::make_unique<ASTVariableDeclaration>("x", ASTValueType::Float, std::move(varValue1), 40);

    auto varValue2 = std::make_unique<ASTValue>("5", ASTValueType::Integer, 41);
    auto varDeclaration2 = std::make_unique<ASTVariableDeclaration>("z", ASTValueType::Integer, std::move(varValue2), 41);

    env.declareVariable("var1", varDeclaration1.get());
    env.declareVariable("var2", varDeclaration2.get());

    ASSERT_EQ(env.gc.size(), 2);
    env.exitCurrentBlock();
    ASSERT_EQ(env.gc.size(), 0);
}

TEST(RuntimeEnvironmentTest, GarbageCollectionWithNestedBlocks) {
    RuntimeEnvironment env;
    env.enterNewBlock();

    auto varValue = std::make_unique<ASTValue>("Abc", ASTValueType::String, 50);
    auto globalVar = std::make_unique<ASTVariableDeclaration>("globalVar", ASTValueType::String, std::move(varValue), 50);
    auto globalFunc = std::make_unique<ASTFunction>("globalFunc", 51);

    ASSERT_NO_THROW(env.declareVariable("globalVar", globalVar.get()));
    ASSERT_NO_THROW(env.declareFunction("globalFunc", globalFunc.get()));

    ASSERT_EQ(env.gc.size(), 2);
    ASSERT_TRUE(env.isVariableDeclared("globalVar"));
    ASSERT_TRUE(env.isFunctionDeclared("globalFunc"));

    env.enterNewBlock();
    auto innerVarValue = std::make_unique<ASTValue>("Cba", ASTValueType::String, 60);
    auto innerVar = std::make_unique<ASTVariableDeclaration>("innerVar", ASTValueType::String, std::move(innerVarValue), 60);
    auto innerFunc = std::make_unique<ASTFunction>("innerFunc", 61);

    env.declareVariable("innerVar", innerVar.get());
    env.declareFunction("innerFunc", innerFunc.get());

    ASSERT_EQ(env.gc.size(), 4);
    ASSERT_TRUE(env.isVariableDeclared("innerVar"));
    ASSERT_TRUE(env.isFunctionDeclared("innerFunc"));

    env.exitCurrentBlock();
    ASSERT_EQ(env.gc.size(), 2);
    ASSERT_FALSE(env.isVariableDeclared("innerVar"));
    ASSERT_FALSE(env.isFunctionDeclared("innerFunc"));
    ASSERT_TRUE(env.isVariableDeclared("globalVar"));
    ASSERT_TRUE(env.isFunctionDeclared("globalFunc"));

    env.exitCurrentBlock();
    ASSERT_EQ(env.gc.size(), 0);
    ASSERT_FALSE(env.isVariableDeclared("globalVar"));
    ASSERT_FALSE(env.isFunctionDeclared("globalFunc"));
}
