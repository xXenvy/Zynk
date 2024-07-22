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
    std::cout << "GC: Running garbage collection..." << std::endl;

    for (auto it = trackingObjects.begin(); it != trackingObjects.end();) {
        if (!(*it)->isMarked()) {
            const std::shared_ptr<ASTBase> obj = it->get()->value;

            switch (obj->type) {
                case ASTType::FunctionDeclaration:
                    std::cout << "GC: Func declaration found: " << std::static_pointer_cast<ASTFunction>(obj)->name << std::endl;
                    break;
                case ASTType::VariableDeclaration:
                    std::cout << "GC: Var declaration found: '" << std::static_pointer_cast<ASTVariableDeclaration>(obj)->name << "' deleting..." << std::endl;
                    break;
                default:
                    std::cout << "GC: Element found! " << static_cast<int>(it->get()->value->type) << std::endl;

            }

            it = trackingObjects.erase(it);
        }
        else {
            (*it)->unmark();
            ++it;
        }
    }
}

void GarbageCollector::collectGarbage(const std::shared_ptr<Block> rootBlock) {
    markBlock(rootBlock);
    sweep();
}

void GarbageCollector::markBlock(const std::shared_ptr<Block> block) {
    if (!block) return;

    for (const auto& varPair : block->variables) {
        mark(varPair.second);
    }
    for (const auto& funcPair : block->functions) {
        mark(funcPair.second);
    }

    if (block->parentBlock) {
        markBlock(block->parentBlock);
    }
}