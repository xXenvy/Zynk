#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include "token.hpp"

class Lexer {
private:
    const std::string source;

    size_t position = 0;
    size_t line = 1;

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