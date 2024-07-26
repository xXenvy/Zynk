#include <gtest/gtest.h>
#include "../src/gc/include/block.hpp"
#include "../src/gc/include/object.hpp"
#include "../src/parsing/include/ast.hpp"

TEST(BlockTest, SetAndGetVariable) {
    auto block = std::make_unique<Block>();

    auto ast1 = std::make_unique<ASTBase>(ASTType::VariableDeclaration);
    auto ast2 = std::make_unique<ASTBase>(ASTType::VariableDeclaration);

    auto obj1 = std::make_unique<GCObject>(ast1.get());
    auto obj2 = std::make_unique<GCObject>(ast2.get());

    block->setVariable("var1", std::move(obj1));
    block->setVariable("var2", std::move(obj2));

    ASSERT_EQ(block->getVariable("var1"), obj1.get());
    ASSERT_EQ(block->getVariable("var2"), obj2.get());
    ASSERT_EQ(block->getVariable("nonExistentVar"), nullptr);
}

TEST(BlockTest, SetAndGetFunction) {
    auto block = std::make_unique<Block>();

    auto funcAst1 = std::make_unique<ASTBase>(ASTType::FunctionDeclaration);
    auto funcAst2 = std::make_unique<ASTBase>(ASTType::FunctionDeclaration);

    auto func1 = std::make_unique<GCObject>(funcAst1.get());
    auto func2 = std::make_unique<GCObject>(funcAst2.get());

    block->setFunction("func1", std::move(func1));
    block->setFunction("func2", std::move(func2));

    ASSERT_EQ(block->getFunction("func1"), func1.get());
    ASSERT_EQ(block->getFunction("func2"), func2.get());
    ASSERT_EQ(block->getFunction("nonExistentFunc"), nullptr);
}

TEST(BlockTest, GetVariableFromParentBlock) {
    auto parentBlock = std::make_unique<Block>();
    auto childBlock = std::make_unique<Block>(parentBlock.get());

    auto parentAstVar = std::make_unique<ASTBase>(ASTType::VariableDeclaration);
    auto childAstVar = std::make_unique<ASTBase>(ASTType::VariableDeclaration);

    auto parentVar = std::make_unique<GCObject>(parentAstVar.get());
    auto childVar = std::make_unique<GCObject>(childAstVar.get());

    parentBlock->setVariable("sharedVar", std::move(parentVar));
    childBlock->setVariable("uniqueVar", std::move(childVar));

    ASSERT_EQ(childBlock->getVariable("sharedVar"), parentVar.get());
    ASSERT_EQ(childBlock->getVariable("uniqueVar"), childVar.get());
    ASSERT_EQ(childBlock->getVariable("nonExistentVar"), nullptr);
}

TEST(BlockTest, GetFunctionFromParentBlock) {
    auto parentBlock = std::make_unique<Block>();
    auto childBlock = std::make_unique<Block>(parentBlock.get());

    auto parentFuncAst = std::make_unique<ASTBase>(ASTType::FunctionDeclaration);
    auto childFuncAst = std::make_unique<ASTBase>(ASTType::FunctionDeclaration);

    auto parentFunc = std::make_unique<GCObject>(parentFuncAst.get());
    auto childFunc = std::make_unique<GCObject>(childFuncAst.get());

    parentBlock->setFunction("sharedFunc", std::move(parentFunc));
    childBlock->setFunction("uniqueFunc", std::move(childFunc));

    ASSERT_EQ(childBlock->getFunction("sharedFunc"), parentFunc.get());
    ASSERT_EQ(childBlock->getFunction("uniqueFunc"), childFunc.get());
    ASSERT_EQ(childBlock->getFunction("nonExistentFunc"), nullptr);
}

TEST(BlockTest, OverrideVariableInChildBlock) {
    auto parentBlock = std::make_unique<Block>();
    auto childBlock = std::make_unique<Block>(parentBlock.get());

    auto parentAstVar = std::make_unique<ASTBase>(ASTType::VariableDeclaration);
    auto childAstVar = std::make_unique<ASTBase>(ASTType::VariableDeclaration);

    auto parentVar = std::make_unique<GCObject>(parentAstVar.get());
    auto childVar = std::make_unique<GCObject>(childAstVar.get());

    parentBlock->setVariable("var", std::move(parentVar));
    ASSERT_EQ(childBlock->getVariable("var"), parentVar.get());

    childBlock->setVariable("var", std::move(childVar));

    ASSERT_EQ(childBlock->getVariable("var"), childVar.get());
    ASSERT_EQ(parentBlock->getVariable("var"), parentVar.get());
}

TEST(BlockTest, OverrideFunctionInChildBlock) {
    auto parentBlock = std::make_unique<Block>();
    auto childBlock = std::make_unique<Block>(parentBlock.get());

    auto parentFuncAst = std::make_unique<ASTBase>(ASTType::FunctionDeclaration);
    auto childFuncAst = std::make_unique<ASTBase>(ASTType::FunctionDeclaration);

    auto parentFunc = std::make_unique<GCObject>(parentFuncAst.get());
    auto childFunc = std::make_unique<GCObject>(childFuncAst.get());

    parentBlock->setFunction("func", std::move(parentFunc));
    ASSERT_EQ(childBlock->getFunction("func"), parentFunc.get());

    childBlock->setFunction("func", std::move(childFunc));

    ASSERT_EQ(childBlock->getFunction("func"), childFunc.get());
    ASSERT_EQ(parentBlock->getFunction("func"), parentFunc.get());
}
