#ifndef RUNTIME_H
#define RUNTIME_H

#include "../block/include/block.hpp"
#include <memory>
#include <stack>

class RuntimeEnvironment {
public:
    const size_t MAX_DEPTH = 1000;

    bool isRecursionDepthExceeded() const;

    void declareVariable(const std::string& name, ASTValue* value);
    ASTValue* getVariable(const std::string& name, const size_t line, bool deepSearch = true) const;
    bool isVariableDeclared(const std::string& name, bool deepSearch = true) const;

    void declareFunction(const std::string& name, ASTFunction* func);
    ASTFunction* getFunction(const std::string& name, const size_t line) const;
    bool isFunctionDeclared(const std::string& name) const;

    inline Block* currentBlock() const;
    void enterNewBlock(bool increaseDepth = false);
    void exitCurrentBlock(bool decreaseDepth = false);

private:
    std::stack<std::unique_ptr<Block>> blockStack;
    size_t currentDepth = 0;
};

#endif // RUNTIME_H
