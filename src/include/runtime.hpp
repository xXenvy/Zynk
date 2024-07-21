#ifndef RUNTIME_H
#define RUNTIME_H

#include "ast.hpp"

#include <map>
#include <string>


class RuntimeEnvironment {
public:
	size_t size() const;

	void declareFunction(const std::string& name, std::shared_ptr<ASTFunction> function);
	std::shared_ptr<ASTFunction> getFunction(const std::string& name);

	void declareVariable(const std::string& name, std::shared_ptr<ASTVariableDeclaration> variable);
	std::shared_ptr<ASTVariableDeclaration> getVariable(const std::string& name);
private:
	std::map<std::string, std::shared_ptr<ASTVariableDeclaration>> variables;
	std::map<std::string, std::shared_ptr<ASTFunction>> functions;
};


#endif // RUNTIME_H