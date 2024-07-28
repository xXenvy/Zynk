#ifndef GC_H
#define GC_H

#include "block.hpp"
#include "object.hpp"
#include <memory>
#include <unordered_set>

class GarbageCollector {
private:
    void sweep();
    void markBlock(Block* block);

    std::unordered_set<GCObject*> trackingObjects;
public:
    void collectGarbage(Block* protectedBlock);
    void mark(GCObject* obj);
    size_t size() const;
};

#endif // GC_H