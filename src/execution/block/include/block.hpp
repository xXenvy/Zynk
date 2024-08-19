#ifndef BLOCK_H
#define BLOCK_H

#include "object.hpp"
#include <unordered_map>
#include <memory>
#include <string>

class Block {
public:
    std::unordered_map<std::string, std::unique_ptr<GCObject>> variables;
    std::unordered_map<std::string, std::unique_ptr<GCObject>> functions;
    Block* parentBlock;

    Block(Block* parent = nullptr) : parentBlock(parent) {}

    inline void setVariable(const std::string& name, std::unique_ptr<GCObject> value) {
        variables[name] = std::move(value);
    }

    inline void setFunction(const std::string& name, std::unique_ptr<GCObject> func) {
        functions[name] = std::move(func);
    }

    GCObject* getVariable(const std::string& name, bool deepSearch = true) {
        if (variables.find(name) != variables.end()) {
            return variables[name].get();
        }
        if (parentBlock && deepSearch) return parentBlock->getVariable(name);
        return nullptr;
    }

    GCObject* getFunction(const std::string& name, bool deepSearch = true) {
        if (functions.find(name) != functions.end()) {
            return functions[name].get();
        }
        if (parentBlock && deepSearch) return parentBlock->getFunction(name);
        return nullptr;
    }

    void freeBlock() {
        variables.clear();
        functions.clear();
    }
};

#endif // BLOCK_H