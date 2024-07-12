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
    if (position > source.size()) return;
    position++;
    if (peek() == '\n') line++;
}

Token Lexer::next() {
    while (std::isspace(peek())) moveForward(); // To remove the spaces.
    const char current = peek();
    const size_t current_line = line;

    switch (peek()) {
        case '\0': return Token(TokenType::END_OF_FILE, "EOF", current_line);
        case '"': return string();
        default: {
            if (std::isalpha(current) || current == '_') return identifier();
            if (std::isdigit(current)) return number();
        }
    }

    moveForward();
    switch (current) {
        case ':': return Token(TokenType::COLON, ":", current_line);
        case '=': return Token(TokenType::EQUAL, "=", current_line);
        case '+': return Token(TokenType::ADD, "+", current_line);
        case '-': return Token(TokenType::SUBTRACT, "-", current_line);
        case '*': return Token(TokenType::MULTIPLY, "*", current_line);
        case '/': return Token(TokenType::DIVIDE, "/", current_line);
        case '{': return Token(TokenType::LBRACE, "{", current_line);
        case '}': return Token(TokenType::RBRACE, "}", current_line);
        case ';': return Token(TokenType::SEMICOLON, ";", current_line);
        case '(': return Token(TokenType::LBRACKET, "(", current_line);
        case ')': return Token(TokenType::RBRACKET, ")", current_line);
        case '!': {
            if (peek() != '=') return Token(TokenType::UNKNOWN, std::string(1, current), current_line);
            moveForward();
            return Token(TokenType::NOT_EQUAL, "!=", current_line);
        }
        default: return Token(TokenType::UNKNOWN, std::string(1, current), current_line);
    }
}

Token Lexer::identifier() {
    const size_t start = position;
    while (std::isalnum(peek()) || peek() == '_') moveForward();
    const std::string value = source.substr(start, position - start);

    if (value == "def") return Token(TokenType::DEF, value, line);
    if (value == "println") return Token(TokenType::PRINTLN, value, line);
    if (value == "print") return Token(TokenType::PRINT, value, line);
    if (value == "true" || value == "false") return Token(TokenType::BOOL, value, line);
    if (value == "int") return Token(TokenType::INT, "int", line);
    if (value == "float") return Token(TokenType::FLOAT, "float", line);
    if (value == "String") return Token(TokenType::STRING, "String", line);
    if (value == "bool") return Token(TokenType::BOOL, "bool", line);
    return Token(TokenType::IDENTIFIER, value, line);
}

Token Lexer::number() {
    const size_t start = position;
    char current = peek();

    while (isdigit(current) || current == '.') {
        moveForward();
        current = peek();
    }
    const std::string value = source.substr(start, position - start);
    if (value.find('.') != std::string::npos) return Token(TokenType::FLOAT, value, line);
    return Token(TokenType::INT, value, line);
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
        return Token(TokenType::UNKNOWN, "Unterminated string", line);
    }
    const std::string value = source.substr(start, position - start);
    moveForward();
    return Token(TokenType::STRING, value, line);
}