#include "../errors/include/errors.hpp"
#include "include/runtime.hpp"
#include <cassert>

bool RuntimeEnvironment::isRecursionDepthExceeded() const {
    return currentDepth >= MAX_DEPTH;
}

inline Block* RuntimeEnvironment::currentBlock() const {
    if (blockStack.empty()) return nullptr;
    return blockStack.top().get();
}

void RuntimeEnvironment::declareVariable(const std::string& name, ASTValue* value) {
    if (isVariableDeclared(name, false)) {
        throw ZynkError(
            ZynkErrorType::DuplicateDeclarationError,
            "Variable '" + name + "' is already declared.",
            value->line
        );
    }
    Block* block = currentBlock();
    assert(block != nullptr && "Block should not be nullptr");

    block->setVariable(name, std::make_unique<GCObject>(value));
}

ASTValue* RuntimeEnvironment::getVariable(const std::string& name, const size_t line, bool deepSearch) const {
    Block* block = currentBlock();
    assert(block != nullptr && "Block should not be nullptr");
    GCObject* gcObject = block->getVariable(name, deepSearch);

    if (gcObject == nullptr) {
        throw ZynkError(
            ZynkErrorType::NotDefinedError,
            "Variable named '" + name + "' is not defined.",
            line
        );
    }
    return static_cast<ASTValue*>(gcObject->value);
}

bool RuntimeEnvironment::isVariableDeclared(const std::string& name, bool deepSearch) const {
    if (currentBlock() == nullptr) return false;
    try {
        getVariable(name, 1, deepSearch);
    } catch (const ZynkError&) {
        return false;
    }
    return true;
}

void RuntimeEnvironment::declareFunction(const std::string& name, ASTFunction* func) {
    if (isFunctionDeclared(name)) {
        throw ZynkError{
            ZynkErrorType::DuplicateDeclarationError,
            "Function '" + name + "' is already declared.",
            func->line
        };
    }
    Block* block = currentBlock();
    assert(block != nullptr && "Block should not be nullptr");

    block->setFunction(name, std::make_unique<GCObject>(func));
}

ASTFunction* RuntimeEnvironment::getFunction(const std::string& name, const size_t line) const {
    Block* block = currentBlock();
    assert(block != nullptr && "Block should not be nullptr");
    GCObject* gcObject = block->getFunction(name);

    if (gcObject == nullptr) {
        throw ZynkError{
            ZynkErrorType::NotDefinedError,
            "Function named '" + name + "' is not defined.",
            line
        };
    }
    return static_cast<ASTFunction*>(gcObject->value);
}

bool RuntimeEnvironment::isFunctionDeclared(const std::string& name) const {
    if (currentBlock() == nullptr) return false;
    try {
        getFunction(name, 1);
    } catch (const ZynkError&) {
        return false;
    }
    return true;
}

 void RuntimeEnvironment::enterNewBlock(bool increaseDepth) {
    if (increaseDepth) currentDepth++;
    blockStack.push(std::make_unique<Block>(currentBlock()));
}

void RuntimeEnvironment::exitCurrentBlock(bool decreaseDepth) {
    if (blockStack.empty()) return;
    if (decreaseDepth) currentDepth--;
    blockStack.pop();
}
