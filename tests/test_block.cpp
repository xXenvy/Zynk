#include <gtest/gtest.h>
#include "../src/include/block.hpp"
#include "../src/include/ast.hpp"

std::shared_ptr<GCObject> produceGCObject(ASTType astType);

TEST(BlockTest, SetAndGetVariable) {
    auto block = std::make_shared<Block>();
    auto obj1 = produceGCObject(ASTType::VariableDeclaration);
    auto obj2 = produceGCObject(ASTType::VariableDeclaration);

    block->setVariable("var1", obj1);
    block->setVariable("var2", obj2);

    ASSERT_EQ(block->getVariable("var1"), obj1);
    ASSERT_EQ(block->getVariable("var2"), obj2);
    ASSERT_EQ(block->getVariable("nonExistentVar"), nullptr);
}

TEST(BlockTest, SetAndGetFunction) {
    auto block = std::make_shared<Block>();
    auto func1 = produceGCObject(ASTType::FunctionDeclaration);
    auto func2 = produceGCObject(ASTType::FunctionDeclaration);

    block->setFunction("func1", func1);
    block->setFunction("func2", func2);

    ASSERT_EQ(block->getFunction("func1"), func1);
    ASSERT_EQ(block->getFunction("func2"), func2);
    ASSERT_EQ(block->getFunction("nonExistentFunc"), nullptr);
}

TEST(BlockTest, GetVariableFromParentBlock) {
    auto parentBlock = std::make_shared<Block>();
    auto childBlock = std::make_shared<Block>(parentBlock);

    auto parentVar = produceGCObject(ASTType::VariableDeclaration);
    auto childVar = produceGCObject(ASTType::VariableDeclaration);

    parentBlock->setVariable("sharedVar", parentVar);
    childBlock->setVariable("uniqueVar", childVar);

    ASSERT_EQ(childBlock->getVariable("sharedVar"), parentVar);
    ASSERT_EQ(childBlock->getVariable("uniqueVar"), childVar);
    ASSERT_EQ(childBlock->getVariable("nonExistentVar"), nullptr);
}

TEST(BlockTest, GetFunctionFromParentBlock) {
    auto parentBlock = std::make_shared<Block>();
    auto childBlock = std::make_shared<Block>(parentBlock);

    auto parentFunc = produceGCObject(ASTType::FunctionDeclaration);
    auto childFunc = produceGCObject(ASTType::FunctionDeclaration);

    parentBlock->setFunction("sharedFunc", parentFunc);
    childBlock->setFunction("uniqueFunc", childFunc);

    ASSERT_EQ(childBlock->getFunction("sharedFunc"), parentFunc);
    ASSERT_EQ(childBlock->getFunction("uniqueFunc"), childFunc);
    ASSERT_EQ(childBlock->getFunction("nonExistentFunc"), nullptr);
}

TEST(BlockTest, OverrideVariableInChildBlock) {
    auto parentBlock = std::make_shared<Block>();
    auto childBlock = std::make_shared<Block>(parentBlock);

    auto parentVar = produceGCObject(ASTType::VariableDeclaration);
    auto childVar = produceGCObject(ASTType::VariableDeclaration);

    parentBlock->setVariable("var", parentVar);
    ASSERT_EQ(childBlock->getVariable("var"), parentVar);

    childBlock->setVariable("var", childVar);

    ASSERT_EQ(childBlock->getVariable("var"), childVar);
    ASSERT_EQ(parentBlock->getVariable("var"), parentVar);
}

TEST(BlockTest, OverrideFunctionInChildBlock) {
    auto parentBlock = std::make_shared<Block>();
    auto childBlock = std::make_shared<Block>(parentBlock);

    auto parentFunc = produceGCObject(ASTType::FunctionDeclaration);
    auto childFunc = produceGCObject(ASTType::FunctionDeclaration);

    parentBlock->setFunction("func", parentFunc);

    ASSERT_EQ(childBlock->getFunction("func"), parentFunc);
    childBlock->setFunction("func", childFunc);

    ASSERT_EQ(childBlock->getFunction("func"), childFunc);
    ASSERT_EQ(parentBlock->getFunction("func"), parentFunc);
}