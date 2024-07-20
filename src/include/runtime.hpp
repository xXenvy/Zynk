#ifndef RUNTIME_H
#define RUNTIME_H

#include "ast.hpp"

#include <map>
#include <string>


class RuntimeEnvironment {
public:
	void show();
	void declareVariable(const std::string& name, ASTVariableDeclaration* variable);
	ASTVariableDeclaration* getVariable(const std::string& name);
private:
	std::map<std::string, ASTVariableDeclaration*> variables;
};


#endif // RUNTIME_H