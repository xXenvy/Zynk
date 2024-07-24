#include "include/runtime.hpp"
#include "include/errors.hpp"
#include <cassert>

RuntimeEnvironment::RuntimeEnvironment() : globalBlock(std::make_shared<Block>()) {}

std::shared_ptr<Block> RuntimeEnvironment::currentBlock() {
    if (blockStack.empty()) return nullptr;
    return blockStack.top();
}

void RuntimeEnvironment::declareVariable(const std::string& name, const std::shared_ptr<ASTVariableDeclaration> value) {
    if (isVariableDeclared(name)) {
        throw ZynkError{
            ZynkErrorType::DuplicateDeclarationError,
            "Variable '" + name + "' is already declared."
        };
    }
    std::shared_ptr<Block> block = currentBlock();
    assert(block != nullptr && "Block should not be nullptr");

    std::shared_ptr<GCObject> gcObject = std::make_shared<GCObject>(value);
    block->setVariable(name, gcObject);
    gc.mark(gcObject);
}

std::shared_ptr<ASTVariableDeclaration> RuntimeEnvironment::getVariable(const std::string& name) {
    std::shared_ptr<Block> block = currentBlock();
    assert(block != nullptr && "Block should not be nullptr");
    std::shared_ptr<GCObject> gcObject = block->getVariable(name);

    if (gcObject == nullptr) {
        throw ZynkError{
            ZynkErrorType::NotDefinedError,
            "Variable named '" + name + "' is not defined."
        };
    }
    return std::static_pointer_cast<ASTVariableDeclaration>(gcObject->value);
}

bool RuntimeEnvironment::isVariableDeclared(const std::string& name) {
    if (currentBlock() == nullptr) return false;
    try {
        getVariable(name);
    } catch (const ZynkError) {
        return false;
    }
    return true;
}

void RuntimeEnvironment::declareFunction(const std::string& name, const std::shared_ptr<ASTFunction> func) {
    if (isFunctionDeclared(name)) {
        throw ZynkError{
            ZynkErrorType::DuplicateDeclarationError,
            "Function '" + name + "' is already declared."
        };
    }
    std::shared_ptr<Block> block = currentBlock();
    assert(block != nullptr && "Block should not be nullptr");

    std::shared_ptr<GCObject> gcObject = std::make_shared<GCObject>(func);
    block->setFunction(name, gcObject);
    gc.mark(gcObject);
}

std::shared_ptr<ASTFunction> RuntimeEnvironment::getFunction(const std::string& name) {
    std::shared_ptr<Block> block = currentBlock();
    assert(block != nullptr && "Block should not be nullptr");

    std::shared_ptr<GCObject> gcObject = block->getFunction(name);
    if (gcObject == nullptr) {
        throw ZynkError{
            ZynkErrorType::NotDefinedError,
            "Function named '" + name + "' is not defined."
        };
    }
    return std::static_pointer_cast<ASTFunction>(gcObject->value);
}

bool RuntimeEnvironment::isFunctionDeclared(const std::string& name) {
    if (currentBlock() == nullptr) return false;
    try {
        getFunction(name);
    } catch (const ZynkError) {
        return false;
    }
    return true;
}

void RuntimeEnvironment::collectGarbage() {
    gc.collectGarbage(currentBlock());
}

void RuntimeEnvironment::enterNewBlock() {
    std::shared_ptr<Block> newBlock = std::make_shared<Block>(currentBlock());
    blockStack.push(newBlock);
}

void RuntimeEnvironment::exitCurrentBlock() {
    if (blockStack.empty()) return;
    std::shared_ptr<Block> block = currentBlock();
    assert(block != nullptr && "Block should not be nullptr");
    
    // Unmarking all functions / variables in the current block since we don't need them anymore.
    for (const auto& varPair : block->variables) {
        varPair.second->unmark();
    }
    for (const auto& funcPair : block->functions) {
        funcPair.second->unmark();
    }
    blockStack.pop();
    collectGarbage();
}
