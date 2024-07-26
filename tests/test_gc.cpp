#include <gtest/gtest.h>
#include <vector>
#include "../src/gc/include/gc.hpp"

std::unique_ptr<GCObject> produceGCObject(ASTType astType) {
    return std::make_unique<GCObject>(std::make_unique<ASTBase>(astType).get());
}

TEST(GarbageCollectorTest, MarkAndSweepDecrementsCorrectly) {
    GarbageCollector gc;
    std::vector<std::unique_ptr<GCObject>> gcObjects;
    gcObjects.push_back(produceGCObject(ASTType::Program));
    gcObjects.push_back(produceGCObject(ASTType::FunctionDeclaration));
    gcObjects.push_back(produceGCObject(ASTType::Print));
    gcObjects.push_back(produceGCObject(ASTType::FunctionCall));

    // Mark all objects.
    for (const auto& obj : gcObjects) {
        gc.mark(obj.get());
    }
    gc.collectGarbage(nullptr); // Initial garbage collection with all objects marked.

    for (int i = 4; i > 0; i--) {
        ASSERT_EQ(gc.size(), i);

        // Unmark the last object in the vector.
        gcObjects.back()->unmark();
        gcObjects.pop_back();

        // Collect garbage after unmarking one object.
        gc.collectGarbage(nullptr);
    }

    gc.collectGarbage(nullptr); // Final garbage collection to ensure the GC is empty.
    ASSERT_EQ(gc.size(), 0);    // Expect the GC to have no objects left.
}

TEST(GarbageCollectorTest, MarkObjectsCorrectly) {
    GarbageCollector gc;
    auto obj1 = produceGCObject(ASTType::Program);
    auto obj2 = produceGCObject(ASTType::FunctionDeclaration);

    gc.mark(obj1.get());
    ASSERT_TRUE(obj1->isMarked());
    ASSERT_EQ(gc.size(), 1);

    gc.mark(obj2.get());
    ASSERT_TRUE(obj2->isMarked());
    ASSERT_EQ(gc.size(), 2);

    // Try to mark obj1 again (should have no effect on size).
    gc.mark(obj1.get());
    ASSERT_TRUE(obj1->isMarked());
    ASSERT_EQ(gc.size(), 2);
}

TEST(GarbageCollectorTest, HandlesNestedBlocksCorrectly) {
    GarbageCollector gc;

    auto block1 = std::make_unique<Block>(); // Parent block
    auto block2 = std::make_unique<Block>(block1.get()); // Child block

    auto obj1 = produceGCObject(ASTType::Program);
    auto obj2 = produceGCObject(ASTType::FunctionDeclaration);

    // Set variables in different blocks.
    block1->setVariable("var1", std::move(obj1));
    block2->setVariable("var2", std::move(obj2));

    // Collect garbage with block2 as root.
    gc.collectGarbage(block2.get());

    // Check if both objects are marked due to inheritance from block1.
    ASSERT_TRUE(obj1->isMarked());
    ASSERT_TRUE(obj2->isMarked());
    ASSERT_EQ(gc.size(), 2);
}

TEST(GarbageCollectorTest, HandlesEmptyBlock) {
    GarbageCollector gc;
    auto emptyBlock = std::make_unique<Block>();

    // Collect garbage on an empty block.
    gc.collectGarbage(emptyBlock.get());

    // Ensure no objects are tracked since block is empty.
    ASSERT_EQ(gc.size(), 0);
}

TEST(GarbageCollectorTest, HandlesNullBlock) {
    GarbageCollector gc;

    // Collect garbage with a null block.
    gc.collectGarbage(nullptr);

    // Ensure GC is still empty.
    ASSERT_EQ(gc.size(), 0);
}

TEST(GarbageCollectorTest, RepeatedMarkAndUnmark) {
    GarbageCollector gc;
    auto obj1 = produceGCObject(ASTType::Program);

    // Mark and unmark the object multiple times.
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
    auto obj1 = produceGCObject(ASTType::Program);
    auto obj2 = produceGCObject(ASTType::FunctionDeclaration);

    // Mark only one object.
    gc.mark(obj2.get());

    // Collect garbage and check.
    gc.collectGarbage(nullptr);
    ASSERT_EQ(gc.size(), 1);
    ASSERT_TRUE(obj2->isMarked());
    ASSERT_FALSE(obj1->isMarked());
}

TEST(GarbageCollectorTest, MarkAndRetainObjectsInNestedBlocks) {
    GarbageCollector gc;
    auto block1 = std::make_unique<Block>(); // Parent block
    auto block2 = std::make_unique<Block>(block1.get()); // Child block

    auto obj1 = produceGCObject(ASTType::VariableDeclaration);
    auto obj2 = produceGCObject(ASTType::FunctionDeclaration);

    block1->setVariable("var", std::move(obj1));
    block2->setFunction("func", std::move(obj2));

    // Mark only obj2.
    gc.mark(obj2.get());

    gc.collectGarbage(block2.get());

    // Check if both objects are marked.
    ASSERT_TRUE(obj1->isMarked());
    ASSERT_TRUE(obj2->isMarked());
    ASSERT_EQ(gc.size(), 2);
}

TEST(GarbageCollectorTest, MarkAllObjectsInHierarchicalBlocks) {
    GarbageCollector gc;
    auto block1 = std::make_unique<Block>(); // Parent block
    auto block2 = std::make_unique<Block>(block1.get()); // Child block

    auto obj1 = produceGCObject(ASTType::VariableDeclaration);
    auto obj2 = produceGCObject(ASTType::VariableDeclaration);
    auto obj3 = produceGCObject(ASTType::VariableDeclaration);

    block1->setVariable("var1", std::move(obj1));
    block1->setVariable("var2", std::move(obj2));
    block2->setVariable("var3", std::move(obj3));

    gc.collectGarbage(block2.get());

    ASSERT_TRUE(obj1->isMarked());
    ASSERT_TRUE(obj2->isMarked());
    ASSERT_TRUE(obj3->isMarked());
    ASSERT_EQ(gc.size(), 3);
}

TEST(GarbageCollectorTest, MarkAllObjectsInComplexStructure) {
    GarbageCollector gc;
    auto block1 = std::make_unique<Block>();
    auto block2 = std::make_unique<Block>(block1.get());
    auto block3 = std::make_unique<Block>(block2.get());

    auto obj1 = produceGCObject(ASTType::VariableDeclaration);
    auto obj2 = produceGCObject(ASTType::VariableDeclaration);
    auto obj3 = produceGCObject(ASTType::VariableDeclaration);
    auto obj4 = produceGCObject(ASTType::FunctionDeclaration);

    block1->setVariable("var1", std::move(obj1));
    block2->setVariable("var2", std::move(obj2));
    block3->setVariable("var3", std::move(obj3));
    block3->setFunction("func1", std::move(obj4));

    gc.collectGarbage(block3.get());

    // Ensure all objects in the hierarchy are marked.
    ASSERT_TRUE(obj1->isMarked());
    ASSERT_TRUE(obj2->isMarked());
    ASSERT_TRUE(obj3->isMarked());
    ASSERT_TRUE(obj4->isMarked());
    ASSERT_EQ(gc.size(), 4);
}

TEST(GarbageCollectorTest, CollectGarbageWithFunctionObjects) {
    GarbageCollector gc;
    auto block1 = std::make_unique<Block>();
    auto block2 = std::make_unique<Block>(block1.get());

    auto func1 = produceGCObject(ASTType::FunctionDeclaration);
    auto func2 = produceGCObject(ASTType::FunctionDeclaration);

    // Add functions to the blocks
    block1->setFunction("func1", std::move(func1));
    block2->setFunction("func2", std::move(func2));

    // Mark all objects.
    gc.collectGarbage(block2.get());

    ASSERT_TRUE(func1->isMarked());
    ASSERT_TRUE(func2->isMarked());
    ASSERT_EQ(gc.size(), 2);

    func1->unmark();
    gc.collectGarbage(nullptr);

    ASSERT_FALSE(func1->isMarked());
    ASSERT_TRUE(func2->isMarked());
    ASSERT_EQ(gc.size(), 1);

    func2->unmark();
    gc.collectGarbage(nullptr);

    // Ensure that all objects are collected
    ASSERT_EQ(gc.size(), 0);
}