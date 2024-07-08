#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include "token.hpp"

class Lexer {
private:
    std::string source;
    size_t position = 0;

    char peek() const;
    void moveForward();

    Token next();
    Token identifier();
    Token number();
    Token string();
public:
    Lexer(const std::string& fileSource);
    std::vector<Token> tokenize();
};

#endif // LEXER_H