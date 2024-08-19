#ifndef GC_OBJECT_H
#define GC_OBJECT_H

#include "../../../parsing/include/ast.hpp"

class GCObject {
private:
    bool marked = false; // Whether the object is alive and should not be deleted.
public:
    GCObject(ASTBase* value);
    ASTBase* value; // Protected value by GCObject.

    void mark();
    void unmark();
    bool isMarked() const;
};

#endif // GC_OBJECT_H