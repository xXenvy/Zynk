#include <gtest/gtest.h>
#include <vector>
#include "../src/gc/include/gc.hpp"
#include "../src/gc/include/block.hpp"
#include "../src/parsing/include/ast.hpp"

TEST(GarbageCollectorTest, MarkAndSweepDecrementsCorrectly) {
    GarbageCollector gc;
    std::vector<GCObject*> gcObjects;

    auto ast1 = std::make_unique<ASTBase>(ASTType::Program);
    auto ast2 = std::make_unique<ASTBase>(ASTType::FunctionDeclaration);
    auto ast3 = std::make_unique<ASTBase>(ASTType::Print);
    auto ast4 = std::make_unique<ASTBase>(ASTType::FunctionCall);

    auto obj1 = std::make_unique<GCObject>(ast1.get());
    auto obj2 = std::make_unique<GCObject>(ast2.get());
    auto obj3 = std::make_unique<GCObject>(ast3.get());
    auto obj4 = std::make_unique<GCObject>(ast4.get());

    gcObjects.push_back(obj1.get());
    gcObjects.push_back(obj2.get());
    gcObjects.push_back(obj3.get());
    gcObjects.push_back(obj4.get());

    for (const auto& objptr : gcObjects) {
        gc.mark(objptr);
    }
    gc.collectGarbage(nullptr);
    ASSERT_EQ(gc.size(), 4);

    for (int i = 4; i > 0; i--) {
        ASSERT_EQ(gc.size(), i);

        gcObjects.back()->unmark();
        gcObjects.pop_back();
        gc.collectGarbage(nullptr);
    }
    gc.collectGarbage(nullptr);
    ASSERT_EQ(gc.size(), 0);
}

TEST(GarbageCollectorTest, MarkObjectsCorrectly) {
    GarbageCollector gc;

    auto ast1 = std::make_unique<ASTBase>(ASTType::Program);
    auto ast2 = std::make_unique<ASTBase>(ASTType::FunctionDeclaration);

    auto obj1 = std::make_unique<GCObject>(ast1.get());
    auto obj2 = std::make_unique<GCObject>(ast2.get());

    gc.mark(obj1.get());
    ASSERT_TRUE(obj1->isMarked());
    ASSERT_EQ(gc.size(), 1);

    gc.mark(obj2.get());
    ASSERT_TRUE(obj2->isMarked());
    ASSERT_EQ(gc.size(), 2);

    gc.mark(obj1.get());
    ASSERT_TRUE(obj1->isMarked());
    ASSERT_EQ(gc.size(), 2);
}

TEST(GarbageCollectorTest, HandlesNestedBlocksCorrectly) {
    GarbageCollector gc;

    auto block1 = std::make_unique<Block>(); // Blok rodzic
    auto block2 = std::make_unique<Block>(block1.get()); // Blok dziecko

    auto ast1 = std::make_unique<ASTBase>(ASTType::Program);
    auto ast2 = std::make_unique<ASTBase>(ASTType::FunctionDeclaration);
    auto obj1 = std::make_unique<GCObject>(ast1.get());
    auto obj2 = std::make_unique<GCObject>(ast2.get());

    block1->setVariable("var1", std::move(obj1));
    block2->setVariable("var2", std::move(obj2));
    gc.collectGarbage(block2.get());

    ASSERT_TRUE(block1->getVariable("var1")->isMarked());
    ASSERT_TRUE(block2->getVariable("var2")->isMarked());
    ASSERT_EQ(gc.size(), 2);
}

TEST(GarbageCollectorTest, HandlesEmptyBlock) {
    GarbageCollector gc;
    auto emptyBlock = std::make_unique<Block>();

    gc.collectGarbage(emptyBlock.get());
    ASSERT_EQ(gc.size(), 0);
}

TEST(GarbageCollectorTest, HandlesNullBlock) {
    GarbageCollector gc;

    gc.collectGarbage(nullptr);
    ASSERT_EQ(gc.size(), 0);
}

TEST(GarbageCollectorTest, RepeatedMarkAndUnmark) {
    GarbageCollector gc;
    auto ast1 = std::make_unique<ASTBase>(ASTType::Program);
    auto obj1 = std::make_unique<GCObject>(ast1.get());

    for (int i = 0; i < 5; ++i) {
        gc.mark(obj1.get());
        ASSERT_TRUE(obj1->isMarked());
        ASSERT_EQ(gc.size(), 1);

        obj1->unmark();
        gc.collectGarbage(nullptr);
        ASSERT_EQ(gc.size(), 0);
    }
}

TEST(GarbageCollectorTest, CollectGarbageWithSomeMarkedObjects) {
    GarbageCollector gc;
    auto ast1 = std::make_unique<ASTBase>(ASTType::Program);
    auto ast2 = std::make_unique<ASTBase>(ASTType::FunctionDeclaration);

    auto obj1 = std::make_unique<GCObject>(ast1.get());
    auto obj2 = std::make_unique<GCObject>(ast2.get());

    gc.mark(obj2.get());
    gc.collectGarbage(nullptr);
    ASSERT_EQ(gc.size(), 1);
    ASSERT_TRUE(obj2->isMarked());
    ASSERT_FALSE(obj1->isMarked());
}

TEST(GarbageCollectorTest, MarkAndRetainObjectsInNestedBlocks) {
    GarbageCollector gc;

    auto block1 = std::make_unique<Block>(); // Blok rodzic
    auto block2 = std::make_unique<Block>(block1.get()); // Blok dziecko

    auto ast1 = std::make_unique<ASTBase>(ASTType::VariableDeclaration);
    auto ast2 = std::make_unique<ASTBase>(ASTType::FunctionDeclaration);

    auto obj1 = std::make_unique<GCObject>(ast1.get());
    auto obj2 = std::make_unique<GCObject>(ast2.get());

    block1->setVariable("var", std::move(obj1));
    block2->setFunction("func", std::move(obj2));

    gc.mark(block2->getFunction("func"));
    gc.collectGarbage(block2.get());

    ASSERT_TRUE(block1->getVariable("var")->isMarked());
    ASSERT_TRUE(block2->getFunction("func")->isMarked());
    ASSERT_EQ(gc.size(), 2);
}

TEST(GarbageCollectorTest, MarkAllObjectsInHierarchicalBlocks) {
    GarbageCollector gc;

    auto block1 = std::make_unique<Block>(); // Blok rodzic
    auto block2 = std::make_unique<Block>(block1.get()); // Blok dziecko

    auto ast1 = std::make_unique<ASTBase>(ASTType::VariableDeclaration);
    auto ast2 = std::make_unique<ASTBase>(ASTType::VariableDeclaration);
    auto ast3 = std::make_unique<ASTBase>(ASTType::VariableDeclaration);

    auto obj1 = std::make_unique<GCObject>(ast1.get());
    auto obj2 = std::make_unique<GCObject>(ast2.get());
    auto obj3 = std::make_unique<GCObject>(ast3.get());

    block1->setVariable("var1", std::move(obj1));
    block1->setVariable("var2", std::move(obj2));
    block2->setVariable("var3", std::move(obj3));

    gc.collectGarbage(block2.get());
    ASSERT_TRUE(block1->getVariable("var1")->isMarked());
    ASSERT_TRUE(block1->getVariable("var2")->isMarked());
    ASSERT_TRUE(block2->getVariable("var3")->isMarked());
    ASSERT_EQ(gc.size(), 3);
}

TEST(GarbageCollectorTest, MarkAllObjectsInComplexStructure) {
    GarbageCollector gc;
    auto block1 = std::make_unique<Block>();
    auto block2 = std::make_unique<Block>(block1.get());
    auto block3 = std::make_unique<Block>(block2.get());

    auto ast1 = std::make_unique<ASTBase>(ASTType::VariableDeclaration);
    auto ast2 = std::make_unique<ASTBase>(ASTType::VariableDeclaration);
    auto ast3 = std::make_unique<ASTBase>(ASTType::VariableDeclaration);
    auto ast4 = std::make_unique<ASTBase>(ASTType::FunctionDeclaration);

    auto obj1 = std::make_unique<GCObject>(ast1.get());
    auto obj2 = std::make_unique<GCObject>(ast2.get());
    auto obj3 = std::make_unique<GCObject>(ast3.get());
    auto obj4 = std::make_unique<GCObject>(ast4.get());

    block1->setVariable("var1", std::move(obj1));
    block2->setVariable("var2", std::move(obj2));
    block3->setVariable("var3", std::move(obj3));
    block3->setFunction("func1", std::move(obj4));

    gc.collectGarbage(block3.get());
    ASSERT_TRUE(block1->getVariable("var1")->isMarked());
    ASSERT_TRUE(block2->getVariable("var2")->isMarked());
    ASSERT_TRUE(block3->getVariable("var3")->isMarked());
    ASSERT_TRUE(block3->getFunction("func1")->isMarked());
    ASSERT_EQ(gc.size(), 4);
}

TEST(GarbageCollectorTest, CollectGarbageWithFunctionObjects) {
    GarbageCollector gc;

    auto block1 = std::make_unique<Block>();
    auto block2 = std::make_unique<Block>(block1.get());

    auto ast1 = std::make_unique<ASTBase>(ASTType::FunctionDeclaration);
    auto ast2 = std::make_unique<ASTBase>(ASTType::FunctionDeclaration);

    auto func1 = std::make_unique<GCObject>(ast1.get());
    auto func2 = std::make_unique<GCObject>(ast2.get());

    block1->setFunction("func1", std::move(func1));
    block2->setFunction("func2", std::move(func2));

    gc.collectGarbage(block2.get());
    ASSERT_TRUE(block1->getFunction("func1")->isMarked());
    ASSERT_TRUE(block2->getFunction("func2")->isMarked());
    ASSERT_EQ(gc.size(), 2);

    block1->getFunction("func1")->unmark();
    gc.collectGarbage(nullptr);

    ASSERT_FALSE(block1->getFunction("func1")->isMarked());
    ASSERT_TRUE(block2->getFunction("func2")->isMarked());
    ASSERT_EQ(gc.size(), 1);

    block2->getFunction("func2")->unmark();
    gc.collectGarbage(nullptr);
    ASSERT_EQ(gc.size(), 0);
}
