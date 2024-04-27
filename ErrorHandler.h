#pragma once

#include <iostream>

enum ErrorCodes { NO_SUCH_FILE, INCORRECT_DELIMITER, NO_SUCH_ARGUMENT,
    ARGUMENT_NOT_SPECIFIED, FILE_NOT_SPECIFIED, NOT_UNSIGNED_INT,
    INT_OUT_OF_RANGE };

static void RaiseError(const ErrorCodes code, const char* note = nullptr) {
    switch (code) {
        case ErrorCodes::NO_SUCH_FILE:
            std::cerr << "There is no such file: " << note << '\n';
            break;
        case ErrorCodes::INCORRECT_DELIMITER:
            std::cerr << "Incorrect delimiter: " << note << '\n';
            break;
        case ErrorCodes::NO_SUCH_ARGUMENT:
            std::cerr << "There is no argument: " << note << '\n';
            break;
        case ErrorCodes::ARGUMENT_NOT_SPECIFIED:
            std::cerr << "You must specify argument instead of " << note << '\n';
            break;
        case ErrorCodes::FILE_NOT_SPECIFIED:
            std::cerr << "File is not specified\n";
            break;
        case ErrorCodes::NOT_UNSIGNED_INT:
            std::cerr << "Entered string must be unsigned integer: " << note << '\n';
            break;
        case ErrorCodes::INT_OUT_OF_RANGE:
            std::cerr << "Entered integer is too big: " << note << '\n';
            break;
        default:
            std::cerr << "Something going wrong\n";
            break;
    }
}
