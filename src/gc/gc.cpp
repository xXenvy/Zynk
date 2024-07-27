#include "include/gc.hpp"
#include "include/block.hpp"
#include <iostream>

void GarbageCollector::mark(GCObject* obj) {
    if (!obj) return;
    obj->mark();
    trackingObjects.insert(obj);
}

void GarbageCollector::sweep() {
    for (auto it = trackingObjects.begin(); it != trackingObjects.end();) {
        if (!(*it)->isMarked()) {
            delete* it;
            it = trackingObjects.erase(it);
        } else {
            ++it;
        }
    }
}

void GarbageCollector::collectGarbage(Block* block) {
    markBlock(block); // Mark the current block to avoid deleting the necessary objects.
    sweep(); // Delete objects which are not marked.
}

void GarbageCollector::markBlock(Block* block) {
    if (!block) return;

    for (const auto& varPair : block->variables) {
        mark(varPair.second.get());
    }
    for (const auto& funcPair : block->functions) {
        mark(funcPair.second.get());
    }
    if (block->parentBlock) markBlock(block->parentBlock);
}

size_t GarbageCollector::size() const {
    return trackingObjects.size();
}