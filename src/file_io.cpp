//
// Created by rzdv on 10.05.2026.
//

#include "../include/file_io.h"
#include <fstream>
#include <map>


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

void writeResultsToFile(const std::vector<Span> &literals, const std::string &outputPath, std::vector<AppError> &errors) {
    vector<string> insertionOrder;
    map<string, vector<TextPos>> grouped;

    for (const Span &span : literals) {
        if (!grouped.contains(span.value)) {
            insertionOrder.push_back(span.value);
        }
        grouped[span.value].push_back(span.pos);
    }

    ofstream out(outputPath);

    if (!out.is_open()) {
        errors.push_back(AppError::outputFailed(outputPath));
        return;
    }

    for (const string &value : insertionOrder) {
        const auto& positions = grouped[value];
        out << "\"" << value << "\": [";

        for (int i = 0; i < static_cast<int>(positions.size()); i++) {
            if (i > 0) {
                out << ", ";
            }
            out << "{" << positions[i].row << ", " << positions[i].col + 1 << "}";
        }
        out << "]\n";
    }
}
