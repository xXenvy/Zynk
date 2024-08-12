#include "../common/include/errors.hpp"
#include "include/runtime.hpp"
#include <cassert>

Block* RuntimeEnvironment::currentBlock() {
    if (blockStack.empty()) return nullptr;
    return blockStack.top().get();
}

void RuntimeEnvironment::declareVariable(const std::string& name, ASTVariableDeclaration* value) {
    if (isVariableDeclared(name)) { // todo: check only for current block.
        throw ZynkError(
            ZynkErrorType::DuplicateDeclarationError,
            "Variable '" + name + "' is already declared.",
            value->line
        );
    }
    Block* block = currentBlock();
    assert(block != nullptr && "Block should not be nullptr");

    auto gcObject = std::make_unique<GCObject>(value);
    gc.mark(gcObject.get());
    block->setVariable(name, std::move(gcObject));
}

ASTVariableDeclaration* RuntimeEnvironment::getVariable(const std::string& name, const size_t line) {
    Block* block = currentBlock();
    assert(block != nullptr && "Block should not be nullptr");
    GCObject* gcObject = block->getVariable(name);

    if (gcObject == nullptr) {
        throw ZynkError(
            ZynkErrorType::NotDefinedError,
            "Variable named '" + name + "' is not defined.",
            line
        );
    }
    return static_cast<ASTVariableDeclaration*>(gcObject->value);
}

bool RuntimeEnvironment::isVariableDeclared(const std::string& name) {
    if (currentBlock() == nullptr) return false;
    try {
        getVariable(name, 1);
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

    auto gcObject = std::make_unique<GCObject>(func);
    gc.mark(gcObject.get());
    block->setFunction(name, std::move(gcObject));
}

ASTFunction* RuntimeEnvironment::getFunction(const std::string& name, const size_t line) {
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

bool RuntimeEnvironment::isFunctionDeclared(const std::string& name) {
    if (currentBlock() == nullptr) return false;
    try {
        getFunction(name, 1);
    } catch (const ZynkError&) {
        return false;
    }
    return true;
}

void RuntimeEnvironment::collectGarbage() {
    gc.collectGarbage(currentBlock());
}

void RuntimeEnvironment::enterNewBlock() {
    blockStack.push(std::make_unique<Block>(currentBlock()));
}

void RuntimeEnvironment::exitCurrentBlock() {
    if (blockStack.empty()) return;
    Block* block = currentBlock();
    assert(block != nullptr && "Block should not be nullptr");
    
    // Unmarking all functions / variables in the current block since we don't need them anymore.
    for (const auto& varPair : block->variables) {
        varPair.second->unmark();
    }
    for (const auto& funcPair : block->functions) {
        funcPair.second->unmark();
    }
    // Keep this block until garbage collection is complete, to ensure that the objects in the block are not destroyed prematurely.
    std::unique_ptr<Block> blockPtr = std::move(blockStack.top());
    blockStack.pop();
    collectGarbage();
}
