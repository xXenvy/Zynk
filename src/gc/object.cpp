#include "include/object.hpp"
#include <memory>

GCObject::GCObject(ASTBase* value) : value(value) {}

void GCObject::mark() {
    marked = true;
}

void GCObject::unmark() {
    marked = false;
}

bool GCObject::isMarked() const {
    return marked;
}