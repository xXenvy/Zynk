#ifndef RUNTIME_H
#define RUNTIME_H

#include "../../gc/include/block.hpp"
#include "../../gc/include/gc.hpp"

#include <memory>
#include <stack>

class RuntimeEnvironment {
public:
    GarbageCollector gc;

    void declareVariable(const std::string& name, ASTVariableDeclaration* value);
    ASTVariableDeclaration* getVariable(const std::string& name);
    bool isVariableDeclared(const std::string& name);

    void declareFunction(const std::string& name, ASTFunction* func);
    ASTFunction* getFunction(const std::string& name);
    bool isFunctionDeclared(const std::string& name);

    Block* currentBlock();
    void collectGarbage();
    void enterNewBlock();
    void exitCurrentBlock();
private:
    std::stack<std::unique_ptr<Block>> blockStack;
};

#endif // RUNTIME_H