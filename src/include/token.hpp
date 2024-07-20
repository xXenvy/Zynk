#ifndef TOKEN_H
#define TOKEN_H

#include "string"

enum class TokenType {
    DEF, PRINTLN, PRINT, VARIABLE, // Keywords.

    INT, FLOAT, STRING, BOOL, // Types.

    ADD, SUBTRACT, MULTIPLY, // Operators.
    DIVIDE, EQUAL, NOT_EQUAL,

    IDENTIFIER, COLON, LBRACE, // Syntax.
    RBRACE, SEMICOLON, LBRACKET,
    RBRACKET,

    END_OF_FILE, UNKNOWN, // Extra.
};

struct Token {
    TokenType type;
    std::string value;
    size_t line;

    Token(TokenType type, const std::string& value, size_t line) : type(type), value(value), line(line) {}
};

#endif // TOKEN_H