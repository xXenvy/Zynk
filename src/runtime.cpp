#include "include/runtime.hpp"
#include "include/errors.hpp"

RuntimeEnvironment::RuntimeEnvironment() : globalBlock(std::make_shared<Block>()) {}

std::shared_ptr<Block> RuntimeEnvironment::currentBlock() {
    if (blockStack.empty()) return nullptr;
    return blockStack.top();
}

void RuntimeEnvironment::declareVariable(const std::string& name, const std::shared_ptr<ASTVariableDeclaration> value) {
    std::shared_ptr<GCObject> gcObject = std::make_shared<GCObject>(value);
    std::shared_ptr<Block> block = currentBlock();

    block->setVariable(name, gcObject);
    gc.mark(gcObject);
}

std::shared_ptr<ASTVariableDeclaration> RuntimeEnvironment::getVariable(const std::string& name) {
    std::shared_ptr<Block> block = currentBlock();
    std::shared_ptr<ASTBase> base = block->getVariable(name)->value;
    return std::static_pointer_cast<ASTVariableDeclaration>(base);
}

void RuntimeEnvironment::declareFunction(const std::string& name, const std::shared_ptr<ASTFunction> func) {
    std::shared_ptr<GCObject> gcObject = std::make_shared<GCObject>(func);
    std::shared_ptr<Block> block = currentBlock();

    block->setFunction(name, gcObject);
    gc.mark(gcObject);
}

std::shared_ptr<ASTFunction> RuntimeEnvironment::getFunction(const std::string& name) {
    std::shared_ptr<Block> block = currentBlock();
    std::shared_ptr<ASTBase> base = block->getFunction(name)->value;
    return std::static_pointer_cast<ASTFunction>(base);
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
    
    // Unmarking all functions / variables in the block since we don't need them anymore.
    for (const auto& varPair : block->variables) {
        varPair.second->unmark();
    }
    for (const auto& funcPair : block->functions) {
        funcPair.second->unmark();
    }
    blockStack.pop();
    collectGarbage();
}
