#ifndef BLOCK_H
#define BLOCK_H

#include <unordered_map>
#include <memory>
#include <string>

class GCObject;

class Block {
public:
    std::unordered_map<std::string, std::shared_ptr<GCObject>> variables;
    std::unordered_map<std::string, std::shared_ptr<GCObject>> functions;
    const std::shared_ptr<Block> parentBlock;

    Block(const std::shared_ptr<Block> parent = nullptr) : parentBlock(parent) {}

    void setVariable(const std::string& name, const std::shared_ptr<GCObject> value) {
        variables[name] = value;
    }

    void setFunction(const std::string& name, const std::shared_ptr<GCObject> func) {
        functions[name] = func;
    }

    std::shared_ptr<GCObject> getVariable(const std::string& name) {
        if (variables.find(name) != variables.end()) {
            return variables[name];
        }
        if (parentBlock) return parentBlock->getVariable(name);
        return nullptr;
    }

    std::shared_ptr<GCObject> getFunction(const std::string& name) {
        if (functions.find(name) != functions.end()) {
            return functions[name];
        }
        if (parentBlock) return parentBlock->getFunction(name);
        return nullptr;
    }

};

#endif // BLOCK_H