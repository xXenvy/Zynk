#ifndef TOKEN_H
#define TOKEN_H

#include "string"

enum class TokenType {
    DEF, PRINTLN, PRINT,
    VARIABLE, CONDITION, ELSE, 
    READLINE, // Keywords.

    INT, FLOAT, STRING, BOOL, NONE, // Types.

    ADD, SUBTRACT, MULTIPLY, // Operators.
    DIVIDE, ASSIGN, NOT_EQUAL, EQUAL,
    GREATER_THAN, LESS_THAN, GREATER_OR_EQUAL,
    LESS_OR_EQUAL,

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