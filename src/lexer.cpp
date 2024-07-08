#include "include/lexer.hpp"
#include <iostream>

Lexer::Lexer(const std::string& fileSource) : source(fileSource) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (true) {
        Token token = next();
        tokens.push_back(token);
        if (token.type == TokenType::END_OF_FILE) return tokens;
    }
}

char Lexer::peek() const {
    if (position > source.size()) return '\0';
    return source[position];
}

void Lexer::moveForward() {
    if (position <= source.size()) position++;
}

Token Lexer::next() {
    while (std::isspace(peek())) moveForward(); // To remove the spaces.
    const char current = peek();

    switch (peek()) {
        case '\0': return Token(TokenType::END_OF_FILE, "EOF");
        case '"': return string();
        default: {
            if (std::isalpha(current) || current == '_') return identifier();
            if (std::isdigit(current)) return number();
        }
    }

    moveForward();
    switch (current) {
        case ':': return Token(TokenType::COLON, ":");
        case '=': return Token(TokenType::EQUAL, "=");
        case '+': return Token(TokenType::ADD, "+");
        case '-': return Token(TokenType::SUBTRACT, "-");
        case '*': return Token(TokenType::MULTIPLY, "*");
        case '/': return Token(TokenType::DIVIDE, "/");
        case '{': return Token(TokenType::LBRACE, "{");
        case '}': return Token(TokenType::RBRACE, "}");
        case ';': return Token(TokenType::SEMICOLON, ";");
        case '(': return Token(TokenType::LBRACKET, "(");
        case ')': return Token(TokenType::RBRACKET, ")");
        case '!': {
            if (peek() != '=') return Token(TokenType::UNKNOWN, std::string(1, current));
            moveForward();
            return Token(TokenType::NOT_EQUAL, "!=");
        }
        default: return Token(TokenType::UNKNOWN, std::string(1, current));
    }
}

Token Lexer::identifier() {
    const size_t start = position;
    while (std::isalnum(peek()) || peek() == '_') moveForward();
    const std::string value = source.substr(start, position - start);

    if (value == "def") return Token(TokenType::DEF, value);
    if (value == "println") return Token(TokenType::PRINTLN, value);
    if (value == "print") return Token(TokenType::PRINT, value);
    if (value == "true" || value == "false") return Token(TokenType::BOOL, value);
    if (value == "int") return Token(TokenType::INT, "TYPE");
    if (value == "float") return Token(TokenType::FLOAT, "TYPE");
    if (value == "String") return Token(TokenType::STRING, "TYPE");
    if (value == "bool") return Token(TokenType::BOOL, "TYPE");
    return Token(TokenType::IDENTIFIER, value);
}

Token Lexer::number() {
    const size_t start = position;
    char current = peek();

    while (isdigit(current) || current == '.') {
        moveForward();
        current = peek();
    }
    const std::string value = source.substr(start, position - start);
    if (value.find('.') != std::string::npos) return Token(TokenType::FLOAT, value);
    return Token(TokenType::INT, value);
}

Token Lexer::string() {
    moveForward(); // Skip the opening quote.

    const size_t start = position;
    char current = peek();

    while (current != '\0') {
        if (current == '"') break;
        moveForward();
        current = peek();
    }
    if (current == '\0') {
        return Token(TokenType::UNKNOWN, "Unterminated string");
    }
    const std::string value = source.substr(start, position - start);
    moveForward();
    return Token(TokenType::STRING, value);
}