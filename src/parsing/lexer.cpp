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

inline char Lexer::peek() const {
    if (position > source.size()) return '\0';
    return source[position];
}

inline void Lexer::moveForward() {
    if (position > source.size()) return;
    position++;
    if (peek() == '\n') line++;
}

Token Lexer::next() {
    while (std::isspace(peek())) moveForward(); // To remove the spaces.
    const char current = peek();
    const size_t currentLine = line;

    switch (peek()) {
        case '\0': return Token(TokenType::END_OF_FILE, "EOF", currentLine);
        case '"': return string();
        default: {
            if (std::isalpha(current) || current == '_') return identifier();
            if (std::isdigit(current)) return number();
        }
    }

    moveForward();
    switch (current) {
        case ',': return Token(TokenType::COMMA, ",", currentLine);
        case ':': return Token(TokenType::COLON, ":", currentLine);
        case '+': return Token(TokenType::ADD, "+", currentLine);
        case '-': return Token(TokenType::SUBTRACT, "-", currentLine);
        case '*': return Token(TokenType::MULTIPLY, "*", currentLine);
        case '{': return Token(TokenType::LBRACE, "{", currentLine);
        case '}': return Token(TokenType::RBRACE, "}", currentLine);
        case ';': return Token(TokenType::SEMICOLON, ";", currentLine);
        case '(': return Token(TokenType::LBRACKET, "(", currentLine);
        case ')': return Token(TokenType::RBRACKET, ")", currentLine);
        case '/': {
            if (peek() != '/') return Token(TokenType::DIVIDE, "/", currentLine);

            // So basically if this is comment, we want to ignore rest of the current line.
            // We are moving to the next line, and then continue parsing.
            while (currentLine == line && peek() != '\0') {
                moveForward();
            }
            return next();
        }
        case '<': {
            if (peek() != '=') return Token(TokenType::LESS_THAN, "<", currentLine);
            moveForward();
            return Token(TokenType::LESS_OR_EQUAL, "<=", currentLine);
        }
        case '>': {
            if (peek() != '=') return Token(TokenType::GREATER_THAN, ">", currentLine);
            moveForward();
            return Token(TokenType::GREATER_OR_EQUAL, ">=", currentLine);
        }
        case '=': {
            if (peek() != '=') return Token(TokenType::ASSIGN, "=", currentLine);
            moveForward();
            return Token(TokenType::EQUAL, "==", currentLine);
        }
        case '!': {
            if (peek() != '=') return Token(TokenType::UNKNOWN, std::string(1, current), currentLine);
            moveForward();
            return Token(TokenType::NOT_EQUAL, "!=", currentLine);
        }
        case '|': {
            if(peek() != '|') return Token(TokenType::UNKNOWN, std::string(1, current), currentLine);
            moveForward();
            return Token(TokenType::OR, "or", currentLine);
        }
        case '&': {
            if (peek() != '&') return Token(TokenType::UNKNOWN, std::string(1, current), currentLine);
            moveForward();
            return Token(TokenType::AND, "and", currentLine);
        }
        default: return Token(TokenType::UNKNOWN, std::string(1, current), currentLine);
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
    if (value == "int") return Token(TokenType::INT, value, line);
    if (value == "float") return Token(TokenType::FLOAT, value, line);
    if (value == "string") return Token(TokenType::STRING, value, line);
    if (value == "bool") return Token(TokenType::BOOL, value, line);
    if (value == "null") return Token(TokenType::NONE, value, line);
    if (value == "var") return Token(TokenType::VARIABLE, value, line);
    if (value == "if") return Token(TokenType::CONDITION, value, line);
    if (value == "else") return Token(TokenType::ELSE, value, line);
    if (value == "readInput") return Token(TokenType::READINPUT, value, line);
    if (value == "or") return Token(TokenType::OR, value, line);
    if (value == "and") return Token(TokenType::AND, value, line);
    if (value == "return") return Token(TokenType::RETURN, value, line);
    if (value == "while") return Token(TokenType::WHILE, value, line);
    if (value == "break") return Token(TokenType::BREAK, value, line);
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