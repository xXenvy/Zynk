#ifndef RUNTIME_H
#define RUNTIME_H

#include "block.hpp"
#include "gc.hpp"

#include <memory>
#include <stack>

class RuntimeEnvironment {
public:
    RuntimeEnvironment();
    GarbageCollector gc;

    void declareVariable(const std::string& name, const std::shared_ptr<ASTVariableDeclaration> value);
    std::shared_ptr<ASTVariableDeclaration> getVariable(const std::string& name);
    bool isVariableDeclared(const std::string& name);

    void declareFunction(const std::string& name, const std::shared_ptr<ASTFunction> func);
    std::shared_ptr<ASTFunction> getFunction(const std::string& name);
    bool isFunctionDeclared(const std::string& name);

    std::shared_ptr<Block> currentBlock();
    void collectGarbage();
    void enterNewBlock();
    void exitCurrentBlock();
private:
    std::shared_ptr<Block> globalBlock;
    std::stack<std::shared_ptr<Block>> blockStack;
};

#endif // RUNTIME_H