#ifndef EXTRACTSTRINGLITERALSFROMCCODE_PARSER_H
#define EXTRACTSTRINGLITERALSFROMCCODE_PARSER_H

#include <string>
#include <vector>


const int MAX_LINES = 1000;
const int MAX_LINE_LEN = 1000;

enum class ErrorType {
    INPUT_FILE_ERROR,
    OUTPUT_FILE_ERROR,
    TOO_MANY_LINES,
    LINE_TOO_LONG
};

struct AppError {
    ErrorType type;

    std::string path;
    int lineNumber = 0;
    int limit = 0;

    static AppError fileNotFound(const std::string &path);
    static AppError outputFailed(const std::string &path);
    static AppError tooManyLines(const std::string &path);
    static AppError lineTooLong(int lineNumber, const std::string &path);

    std::string message() const;
    void print() const;

private:
    AppError(ErrorType t);
};
#endif //EXTRACTSTRINGLITERALSFROMCCODE_PARSER_H
