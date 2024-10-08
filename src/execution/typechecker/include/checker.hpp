#ifndef CHECKER_HPP
#define CHECKER_HPP

#include "../../../parsing/include/ast.hpp"
#include "../../include/runtime.hpp"

class TypeChecker {
public:
    TypeChecker(RuntimeEnvironment& env);
    void checkType(const ASTValueType& declared, ASTBase* value);
    void checkType(ASTFunction* func, ASTBase* value);
    ASTValueType determineType(ASTBase* expression);
    std::string typeToString(const ASTValueType& type);
private:
    RuntimeEnvironment& env;
    inline bool isNumber(const ASTValueType& type);
};

#endif // CHECKER_HPP