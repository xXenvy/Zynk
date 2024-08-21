#ifndef BLOCK_H
#define BLOCK_H

#include "../../../parsing/include/ast.hpp"
#include <unordered_map>
#include <iostream>
#include <memory>
#include <string>

class Block {
public:
    std::unordered_map<std::string, std::unique_ptr<ASTValue>> variables;
    std::unordered_map<std::string, std::unique_ptr<ASTFunction>> functions;
    Block* parentBlock;

    Block(Block* parent = nullptr) : parentBlock(parent) {}

    inline void setVariable(const std::string& name, std::unique_ptr<ASTValue> value) {
        variables[name] = std::move(value);
    }

    inline void setFunction(std::unique_ptr<ASTFunction> func) {
        functions[func->name] = std::move(func);
    }

    ASTValue* getVariable(const std::string& name, bool deepSearch = true) {
        if (variables.find(name) != variables.end()) {
            return variables[name].get();
        }
        if (parentBlock && deepSearch) return parentBlock->getVariable(name);
        return nullptr;
    }

    ASTFunction* getFunction(const std::string& name, bool deepSearch = true) {
        if (functions.find(name) != functions.end()) {
            return functions[name].get();
        }
        if (parentBlock && deepSearch) return parentBlock->getFunction(name);
        return nullptr;
    }
};

#endif // BLOCK_H