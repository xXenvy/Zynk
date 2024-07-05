#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

enum class TokenType {
    DEF, PRINTLN, PRINT, // Keywords.

    INT, FLOAT, STRING, // Types. TODO: add bool

    ADD, SUBSTRACT, MULTIPLY, // Operators.
    DIVIDE, EQUAL, NOT_EQUAL,

    IDENTIFIER, COLON, LBRACE, // Syntax.
    RBRACE, SEMICOLON, LBRACKET, 
    RBRACKET,

    END_OF_FILE, UNKNOWN, // Extra.
};

struct Token {
    TokenType type;
    std::string value;

    Token(TokenType type, const std::string& value);
};

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