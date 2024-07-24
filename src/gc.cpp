#include "include/gc.hpp"
#include "include/block.hpp"
#include <iostream>

GCObject::GCObject(const std::shared_ptr<ASTBase> value) : value(value) {}

void GCObject::mark() {
    marked = true;
}

void GCObject::unmark() {
    marked = false;
}

bool GCObject::isMarked() const {
    return marked;
}

void GarbageCollector::mark(const std::shared_ptr<GCObject> obj) {
    if (!obj || obj->isMarked()) return;
    obj->mark();
    trackingObjects.insert(obj);
}

void GarbageCollector::sweep() {
    for (auto it = trackingObjects.begin(); it != trackingObjects.end();) {
        if (!(*it)->isMarked()) {
            it = trackingObjects.erase(it);
        } else {
            ++it;
        }
    }
}

void GarbageCollector::collectGarbage(const std::shared_ptr<Block> block) {
    markBlock(block); // Mark the current block to avoid deleting the necessary objects.
    sweep(); // Delete objects which are not marked.
}

void GarbageCollector::markBlock(const std::shared_ptr<Block> block) {
    if (!block) return;

    for (const auto& varPair : block->variables) {
        mark(varPair.second);
    }
    for (const auto& funcPair : block->functions) {
        mark(funcPair.second);
    }
    if (block->parentBlock) markBlock(block->parentBlock);
}

size_t GarbageCollector::size() const {
    return trackingObjects.size();
}