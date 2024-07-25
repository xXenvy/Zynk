#ifndef GC_H
#define GC_H

#include "../../parsing/include/ast.hpp"
#include "block.hpp"
#include <memory>
#include <unordered_set>

class GCObject {
private:
    bool marked = false; // Whether the object is alive and should not be deleted.
public:
    GCObject(const std::shared_ptr<ASTBase> value);
    const std::shared_ptr<ASTBase> value; // Protected value by GCObject.

    void mark();
    void unmark();
    bool isMarked() const;
};

class GarbageCollector {
private:
    void sweep();
    void markBlock(const std::shared_ptr<Block> block);

    std::unordered_set<std::shared_ptr<GCObject>> trackingObjects;
public:
    void collectGarbage(const std::shared_ptr<Block> protectedBlock);
    void mark(const std::shared_ptr<GCObject> obj);
    size_t size() const;
};

#endif // GC_H