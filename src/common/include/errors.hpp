#ifndef ERRORS_H
#define ERRORS_H

#include <string>
#include <iostream>
#include <optional>

#define RESET "\033[0m"
#define RED "\033[31m"
#define CYAN "\033[36m"

enum class ZynkErrorType {
    SyntaxError,
    RuntimeError,
    UnknownError,
    TypeError,
    NotDefinedError,
    FileOpenError,
    ExpressionError,
    PanicError,
    CLIError,
    DuplicateDeclarationError,
    TypeCastError
};

struct ZynkError : public std::runtime_error {
    const ZynkErrorType base_type;
    std::optional<size_t> line;

    ZynkError(ZynkErrorType type, const std::string& message, std::optional<size_t> line = std::nullopt)
        : std::runtime_error(message), base_type(type), line(line) {}

    void print(std::optional<std::string> filepath = std::nullopt) const {
        std::cout << std::endl;
        std::cerr << "=== " << RED << "Error Report" << RESET << " ===" << std::endl;
        std::cerr << CYAN << "-> Type: " << RESET << errorToString() << std::endl;
        std::cerr << CYAN << "-> Message: " << RESET << what() << std::endl;

        if (filepath.has_value()) {
            std::cerr << CYAN << "-> File: " << RESET << filepath.value() << std::endl;
        }
        if (line.has_value()) {
            std::cerr << CYAN << "-> Line: " << RESET << line.value() << std::endl;
        }
        std::cerr << RESET << "=====================" << std::endl;
    }

private:
    std::string errorToString() const {
        switch (base_type) {
            case ZynkErrorType::SyntaxError: return "SyntaxError";
            case ZynkErrorType::RuntimeError: return "RuntimeError";
            case ZynkErrorType::TypeError: return "TypeError";
            case ZynkErrorType::FileOpenError: return "FileOpenError";
            case ZynkErrorType::ExpressionError: return "ExpressionError";
            case ZynkErrorType::PanicError: return "PanicError";
            case ZynkErrorType::CLIError: return "CLIError";
            case ZynkErrorType::NotDefinedError: return "NotDefinedError";
            case ZynkErrorType::DuplicateDeclarationError: return "DuplicateDeclarationError";
            case ZynkErrorType::TypeCastError: return "TypeCastError";
            default: return "UnknownError";
        }
    }
};

#endif // ERRORS_H
