//
// Created by rzdv on 10.05.2026.
//

#include "../include/file_io.h"
#include <fstream>


std::vector<std::string> readFile(const std::string &path, std::vector<AppError> &errors) {
    ifstream in(path);

    if (!in.is_open()) {
        errors.push_back(AppError::fileNotFound(path));
        return {};
    }

    vector<string> lines;
    string line;
    int lineNumber = 1;

    while (getline(in, line)) {
        if (lineNumber > MAX_LINES) {
            errors.push_back(AppError::tooManyLines(path));
            return {};
        }
        if (static_cast<int>(line.size()) > MAX_LINE_LEN) {
            errors.push_back(AppError::lineTooLong(lineNumber, MAX_LINE_LEN));
            return {};
        }

        lines.push_back(line);
        lineNumber++;
    }
    return lines;
}
