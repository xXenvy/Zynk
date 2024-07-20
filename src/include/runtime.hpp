#ifndef RUNTIME_H
#define RUNTIME_H

#include "ast.hpp"

#include <map>
#include <string>


class RuntimeEnvironment {
public:
	size_t size() const;

	void declareFunction(const std::string& name, ASTFunction* function);
	ASTFunction* getFunction(const std::string& name);

	void declareVariable(const std::string& name, ASTVariableDeclaration* variable);
	ASTVariableDeclaration* getVariable(const std::string& name);
private:
	std::map<std::string, ASTVariableDeclaration*> variables;
	std::map<std::string, ASTFunction*> functions;
};


#endif // RUNTIME_H