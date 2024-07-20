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
    VariableNotDefined,
    FileOpenError,
    ExpressionError,
    PanicError,
    CLIError,
};

struct ZynkError : public std::runtime_error {
    const ZynkErrorType base_type;
    const size_t* line;

    ZynkError(ZynkErrorType type, const std::string& message, const size_t* line = nullptr)
        : std::runtime_error(message), base_type(type), line(line) {}

    void print() const {
        std::cerr << "Error[" << toString() << "]: "
            << (line ? "At line: " + std::to_string(*line) + ". " : "")
            << what() << std::endl;
    }
private:
    std::string toString() const {
        switch (base_type) {
            case ZynkErrorType::SyntaxError: return "SyntaxError";
            case ZynkErrorType::RuntimeError: return "RuntimeError";
            case ZynkErrorType::InvalidTypeError: return "InvalidTypeError";
            case ZynkErrorType::FileOpenError: return "FileOpenError";
            case ZynkErrorType::ExpressionError: return "ExpressionError";
            case ZynkErrorType::PanicError: return "PanicError";
            case ZynkErrorType::CLIError: return "CLIError";
            case ZynkErrorType::VariableNotDefined: return "VariableNotDefined";
            default: return "UnknownError";
        }
    }
};

#endif