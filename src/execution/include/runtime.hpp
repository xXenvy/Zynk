#ifndef RUNTIME_H
#define RUNTIME_H

#include "../../gc/include/block.hpp"
#include "../../gc/include/gc.hpp"

#include <memory>
#include <stack>


class RuntimeEnvironment {
public:
    const size_t MAX_DEPTH = 1000;
    GarbageCollector gc;

    bool isRecursionDepthExceeded() const;

    void declareVariable(const std::string& name, ASTVariableDeclaration* value);
    ASTVariableDeclaration* getVariable(const std::string& name, const size_t line, bool deepSearch = true);
    bool isVariableDeclared(const std::string& name, bool deepSearch = true);

    void declareFunction(const std::string& name, ASTFunction* func);
    ASTFunction* getFunction(const std::string& name, const size_t line);
    bool isFunctionDeclared(const std::string& name);

    Block* currentBlock();
    void collectGarbage();
    void enterNewBlock(const bool increaseDepth = false);
    void exitCurrentBlock(const bool decreaseDepth = false);
private:
    std::stack<std::unique_ptr<Block>> blockStack;
    size_t currentDepth = 0;
};

#endif // RUNTIME_H