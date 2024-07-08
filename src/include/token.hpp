#ifndef TOKEN_H
#define TOKEN_H

enum class TokenType {
    DEF, PRINTLN, PRINT, // Keywords.

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

    Token(TokenType type, const std::string& value) : type(type), value(value) {}
};

#endif // TOKEN_H