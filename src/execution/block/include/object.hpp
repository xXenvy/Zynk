#ifndef GC_OBJECT_H
#define GC_OBJECT_H

#include "../../../parsing/include/ast.hpp"

class GCObject {
public:
    ASTBase* value;
    GCObject(ASTBase* value) : value(value) {}
};

#endif // GC_OBJECT_H