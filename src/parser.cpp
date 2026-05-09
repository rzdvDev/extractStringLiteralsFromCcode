#include "../include/parser.h"

#include <iostream>
#include <fstream>
#include <map>
#include <windows.h>


using namespace std;

AppError::AppError(ErrorType t) : type(t) {}

AppError AppError::fileNotFound(const std::string &path) {
    AppError e(ErrorType::INPUT_FILE_ERROR);
    e.path = path;
    return e;
}

AppError AppError::outputFailed(const std::string &path) {
    AppError e(ErrorType::OUTPUT_FILE_ERROR);
    e.path = path;
    return e;
}

AppError AppError::tooManyLines(const std::string &path) {
    AppError e(ErrorType::TOO_MANY_LINES);
    e.path = path;
    return e;
}

AppError AppError::lineTooLong(int lineNumber, int limit) {
    AppError e(ErrorType::LINE_TOO_LONG);
    e.limit = limit;
    e.lineNumber = lineNumber;
    return e;
}

string AppError::message() const {
    switch (type) {
        case ErrorType::INPUT_FILE_ERROR:
            return "Неверно указан файл с входными данными. "
            "Возможно файл не существует или заблокирован: \"" + path + "\"";
        case ErrorType::OUTPUT_FILE_ERROR:
            return "Невозможно создать выходной файл. "
            "Проверьте права на запись в директорию: \"" + path + "\"";
        case ErrorType::TOO_MANY_LINES:
            return "Количество строк превышает " + to_string(limit) + ".";
        case ErrorType::LINE_TOO_LONG:
            return "Длина строки " + to_string(lineNumber) +
            " более " + to_string(limit) + " символов.";
        default:
            return "Unknown error.";
    }
}

void AppError::print() const {
    cerr << "Ошибка: " << message() << endl;
}

TextCursor::TextCursor(const vector<string>& text) : text(text), pos({0, 0}) {}

bool TextCursor::isValid() const {
    if (pos.row < 0 || pos.row >= static_cast<int>(text.size()))
        return false;
    return pos.col <= static_cast<int>(text[pos.row].size());
}

char TextCursor::currentChar() const {
    if (!isValid())
        return '\0';
    if (pos.col == static_cast<int>(text[pos.row].size()))
        return '\n';
    return text[pos.row][pos.col];
}


void TextCursor::advance(const unsigned int shift) {
    for (unsigned int i = 0; i < shift; i++) {
        if (!isValid())
            return;
        if (pos.col >= static_cast<int>(text[pos.row].size())) {
            pos.col = 0;
            pos.row++;
        } else {
            pos.col++;
        }
    }
}

void TextCursor::advanceToNextLine() {
    pos.row++;
    pos.col = 0;
}

bool TextCursor::startsWith(const std::string &sub) const {
    if (!isValid())
        return false;

    TextCursor tmp = *this;

    for (int i = 0; i < static_cast<int>(sub.size()); i++) {
        if (!tmp.isValid() || tmp.pos.row >= static_cast<int>(text.size()))
            return false;

        if (tmp.currentChar() != sub[i])
            return false;

        tmp.advance();
    }
    return true;
}
