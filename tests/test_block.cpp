#include <gtest/gtest.h>
#include "../src/execution/block/include/block.hpp"
#include "../src/parsing/include/ast.hpp"

TEST(BlockTest, SetAndGetVariable) {
    auto block = std::make_unique<Block>();

    auto ast1 = std::make_unique<ASTValue>("1", ASTValueType::Integer, 1);
    auto ast2 = std::make_unique<ASTValue>("2", ASTValueType::Integer, 2);

    block->setVariable("var1", std::move(ast1));
    block->setVariable("var2", std::move(ast2));

    ASSERT_EQ(block->getVariable("var1")->value, "1");
    ASSERT_EQ(block->getVariable("var2")->value, "2");
    ASSERT_EQ(block->getVariable("nonExistentVar"), nullptr);
}

TEST(BlockTest, SetAndGetFunction) {
    auto block = std::make_unique<Block>();

    auto funcAst1 = std::make_unique<ASTFunction>("test", ASTValueType::None, 1);
    auto funcAst2 = std::make_unique<ASTFunction>("test2", ASTValueType::None, 2);

    block->setFunction(std::move(funcAst1));
    block->setFunction(std::move(funcAst2));
    
    ASSERT_EQ(block->getFunction("test")->type, ASTType::FunctionDeclaration);
    ASSERT_EQ(block->getFunction("test2")->type, ASTType::FunctionDeclaration);
    ASSERT_EQ(block->getFunction("nonExistentFunc"), nullptr);
}

TEST(BlockTest, OverrideVariableInChildBlock) {
    auto parentBlock = std::make_unique<Block>();
    auto childBlock = std::make_unique<Block>(parentBlock.get());

    auto parentAstVar = std::make_unique<ASTValue>("parentVar", ASTValueType::Integer, 1);
    auto childAstVar = std::make_unique<ASTValue>("childVar", ASTValueType::Integer, 2);

    parentBlock->setVariable("var", std::move(parentAstVar));

    ASSERT_EQ(childBlock->getVariable("var")->value, "parentVar");

    childBlock->setVariable("var", std::move(childAstVar));

    ASSERT_EQ(childBlock->getVariable("var")->value, "childVar");
    ASSERT_EQ(parentBlock->getVariable("var")->value, "parentVar");
}

TEST(BlockTest, OverrideFunctionInChildBlock) {
    auto parentBlock = std::make_unique<Block>();
    auto childBlock = std::make_unique<Block>(parentBlock.get());

    auto parentFuncAst = std::make_unique<ASTFunction>("parent", ASTValueType::None, 1);
    auto childFuncAst = std::make_unique<ASTFunction>("child", ASTValueType::None, 2);

    parentBlock->setFunction(std::move(parentFuncAst));
    ASSERT_EQ(childBlock->getFunction("parent")->type, ASTType::FunctionDeclaration);

    childBlock->setFunction(std::move(childFuncAst));
    ASSERT_EQ(childBlock->getFunction("child")->type, ASTType::FunctionDeclaration);
    ASSERT_EQ(parentBlock->getFunction("parent")->type, ASTType::FunctionDeclaration);
}
