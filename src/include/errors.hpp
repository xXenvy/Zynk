#ifndef ERRORS_H
#define ERRORS_H

#include <string>
#include <vector>
#include <iostream>

enum class ZynkErrorType {
    SyntaxError,
    RuntimeError,
    UnknownError,
    InvalidTypeError,
    FileOpenError,
};

struct ZynkError : public std::runtime_error {
    ZynkErrorType type;
    const size_t* line;

    ZynkError(ZynkErrorType type, const std::string& message, const size_t* line = nullptr)
        : std::runtime_error(message), type(type), line(line) {}

    void print() const {
        std::cerr << "Error[" << toString(type) << "]: "
            << (line ? "Error at line: " + std::to_string(*line) + ". " : "")
            << what() << std::endl;
    }
private:
    std::string toString(ZynkErrorType type) const {
        switch (type) {
            case ZynkErrorType::SyntaxError: return "SyntaxError";
            case ZynkErrorType::RuntimeError: return "RuntimeError";
            case ZynkErrorType::InvalidTypeError: return "InvalidTypeError";
            case ZynkErrorType::FileOpenError: return "FileOpenError";
            default: return "UnknownError";
        }
    }
};

#endif