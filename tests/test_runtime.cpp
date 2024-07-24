#include <gtest/gtest.h>

#include "../src/include/runtime.hpp"
#include "../src/include/ast.hpp"
#include "../src/include/errors.hpp"

TEST(RuntimeEnvironmentTest, VariableDeclaration) {
    RuntimeEnvironment env;
    env.enterNewBlock();

    auto varValue = std::make_shared<ASTValue>("10");
    auto varDeclaration = std::make_shared<ASTVariableDeclaration>("x", "int", varValue);
    ASSERT_NO_THROW(env.declareVariable("x", varDeclaration));

    auto retrievedVar = env.getVariable("x");
    ASSERT_EQ(retrievedVar->name, "x");

    ASSERT_THROW(env.declareVariable("x", varDeclaration), ZynkError);
    ASSERT_EQ(env.isVariableDeclared("x"), true);
    env.exitCurrentBlock();
}

TEST(RuntimeEnvironmentTest, FunctionDeclaration) {
    RuntimeEnvironment env;
    env.enterNewBlock();

    auto funcDeclaration = std::make_shared<ASTFunction>("myFunction");
    ASSERT_NO_THROW(env.declareFunction("myFunction", funcDeclaration));

    auto retrievedFunc = env.getFunction("myFunction");
    ASSERT_EQ(retrievedFunc->name, "myFunction");
    ASSERT_EQ(retrievedFunc->body.size(), 0);

    ASSERT_THROW(env.declareFunction("myFunction", funcDeclaration), ZynkError);
    ASSERT_EQ(env.isFunctionDeclared("myFunction"), true);
    env.exitCurrentBlock();
}

TEST(RuntimeEnvironmentTest, VariableNotDefinedError) {
    RuntimeEnvironment env;
    env.enterNewBlock();

    ASSERT_THROW(env.getVariable("undefinedVar"), ZynkError);
    ASSERT_EQ(env.isVariableDeclared("undefinedVar"), false);
    env.exitCurrentBlock();
}

TEST(RuntimeEnvironmentTest, FunctionNotDefinedError) {
    RuntimeEnvironment env;
    env.enterNewBlock();

    ASSERT_THROW(env.getFunction("undefinedFunc"), ZynkError);
    ASSERT_EQ(env.isFunctionDeclared("undefinedFunc"), false);
    env.exitCurrentBlock();
}

TEST(RuntimeEnvironmentTest, EnterAndExitBlock) {
    RuntimeEnvironment env;

    ASSERT_EQ(env.currentBlock(), nullptr);
    env.enterNewBlock();
    ASSERT_NE(env.currentBlock(), nullptr);

    auto varValue = std::make_shared<ASTValue>("1.5");
    auto varDeclaration = std::make_shared<ASTVariableDeclaration>("x", "float", varValue);
    env.declareVariable("x", varDeclaration);

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

    auto varValue1 = std::make_shared<ASTValue>("1.5");
    auto varDeclaration1 = std::make_shared<ASTVariableDeclaration>("x", "float", varValue1);

    auto varValue2 = std::make_shared<ASTValue>("5");
    auto varDeclaration2 = std::make_shared<ASTVariableDeclaration>("z", "int", varValue2);

    env.declareVariable("var1", varDeclaration1);
    env.declareVariable("var2", varDeclaration2);

    ASSERT_EQ(env.gc.size(), 2);
    env.exitCurrentBlock();
    ASSERT_EQ(env.gc.size(), 0);
}

TEST(RuntimeEnvironmentTest, GarbageCollectionWithNestedBlocks) {
    RuntimeEnvironment env;
    env.enterNewBlock();

    auto varValue = std::make_shared<ASTValue>("Abc");
    auto globalVar = std::make_shared<ASTVariableDeclaration>("globalVar", "String", varValue);
    auto globalFunc = std::make_shared<ASTFunction>("globalFunc");

    ASSERT_NO_THROW(env.declareVariable("globalVar", globalVar));
    ASSERT_NO_THROW(env.declareFunction("globalFunc", globalFunc));

    ASSERT_EQ(env.gc.size(), 2);
    ASSERT_TRUE(env.isVariableDeclared("globalVar"));
    ASSERT_TRUE(env.isFunctionDeclared("globalFunc"));

    env.enterNewBlock();
    auto innerVarValue = std::make_shared<ASTValue>("Cba");
    auto innerVar = std::make_shared<ASTVariableDeclaration>("innerVar", "String", varValue);
    auto innerFunc = std::make_shared<ASTFunction>("innerFunc");

    env.declareVariable("innerVar", innerVar);
    env.declareFunction("innerFunc", innerFunc);

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