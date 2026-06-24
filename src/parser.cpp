#include "../include/parser.h"

#include <iostream>
#include <fstream>

using namespace std;

AppError::AppError(ErrorType t) : type(t) {
    // Конструктор объекта ошибки.
}

AppError AppError::fileNotFound(const std::string &path) {
    // Выставляем тип ошибки связанный с входным файлом
    AppError e(ErrorType::INPUT_FILE_ERROR);
    e.path = path;
    return e;
}

AppError AppError::outputFailed(const std::string &path) {
    // Выставляем тип ошибки связанный с выходным файлом
    AppError e(ErrorType::OUTPUT_FILE_ERROR);
    e.path = path;
    return e;
}

AppError AppError::tooManyLines(const std::string &path) {
    // Выставляем тип ошибки связанный с слишком большим количеством строк
    AppError e(ErrorType::TOO_MANY_LINES);
    e.path = path;
    return e;
}

AppError AppError::lineTooLong(int lineNumber, int limit) {
    // Выставляем тип ошибки связанный с слишком длинной строкой
    AppError e(ErrorType::LINE_TOO_LONG);
    e.limit = limit;
    e.lineNumber = lineNumber;
    return e;
}

string AppError::message() const {
    // Выводим относительно типа ошибки - текст ошибки для пользователя
    switch (type) {
        case ErrorType::INPUT_FILE_ERROR:
            return "Input data file is incorrect. "
            "File may not exists or be blocked: \"" + path + "\"";
        case ErrorType::OUTPUT_FILE_ERROR:
            return "Unable to create or open output file. "
            "Check the write permissions for directory: \"" + path + "\"";
        case ErrorType::TOO_MANY_LINES:
            return "Number of linex exceeds " + to_string(limit) + ".";
        case ErrorType::LINE_TOO_LONG:
            return "Length string " + to_string(lineNumber) +
            " more " + to_string(limit) + " symbols.";
        default:
            return "Unknown error.";
    }
}

void AppError::print() const {
    // Вывод отформатированного сообщения об ошибке.
    // Формирование текста выполняется функцией message().
    // Перед сообщением выводится префикс Error.
    // После сообщения добавляется перевод строки.
    cerr << "Error: " << message() << endl;
}

TextCursor::TextCursor(const vector<string>& text) : text(text), pos({0, 0}) {
    // Инициализация курсора на начале текста
}

bool TextCursor::isValid() const {
    // Условия валидности:
    // Номер строки не может быть отрицательным
    // и не должен выходить за пределы массива строк.
    if (pos.row < 0 || pos.row >= static_cast<int>(text.size()))
        return false;

    // Проверка корректности номера столбца.
    // Допускается позиция, равная длине строки,
    // так как она соответствует концу строки.
    return pos.col <= static_cast<int>(text[pos.row].size());
}

char TextCursor::currentChar() const {
    // Если текущая позиция не корректна, доступа к символу нет.
    if (!isValid())
        return '\0';
    // Если курсор находится в конце строки, возвращаем символ новой строки.
    if (pos.col == static_cast<int>(text[pos.row].size()))
        return '\n';
    // Возвращаем символ расположенный по текущим координатам курсора
    return text[pos.row][pos.col];
}


void TextCursor::advance(const unsigned int shift) {

    // Цикл выполняется shift раз, каждая итерация равна одному шагу курсора внутри текста.
    for (unsigned int i = 0; i < shift; i++) {
        // Проверка корректности текущей позиции.
        if (!isValid())
            return;

        // Проверка достижения конца строки, переходим на следующую, если текущая кончилась.
        if (pos.col >= static_cast<int>(text[pos.row].size())) {
            // Переход на начало следующей строки.
            pos.col = 0;
            pos.row++;
        } else {
            // Переход к следующему символу в текущей строке.
            pos.col++;
        }
    }
}

void TextCursor::advanceToNextLine() {
    // Переход к началу следующей строки.
    pos.row++;
    pos.col = 0;
}

bool TextCursor::startsWith(const std::string &sub) const {
    // Проверка корректности текущей позиции курсора в тексте.
    if (!isValid())
        return false;

    // Временная копия курсора для проверки последовательности символов.
    TextCursor tmp = *this;

    // Проходим по каждому символу подстроки sub и сравниваем с символами текста.
    for (int i = 0; i < static_cast<int>(sub.size()); i++) {

        // Если временный курсор вышел за пределы текста, значит подстрока не может быть найдена.
        if (!tmp.isValid() || tmp.pos.row >= static_cast<int>(text.size()))
            return false;
        // Сравнение текущего символа с соответствующим символом подстроки
        if (tmp.currentChar() != sub[i])
            return false;
        // Выполняем переход к следующему символу текста, только при успешном совпадении.
        tmp.advance();
    }

    // Если цикл завершился без ошибок, значит подстрока sub полностью совпала с текстом начиная с текущей позиции курсора.
    return true;
}


void TextCursor::skipUntil(const std::string &closeToken) {
    // Цикл работает пока позиция курсора корректна, а также отсутствует искомый токен в текущей позиции курсора.
    while (isValid() && !startsWith(closeToken)) {
        // Выполняем переход на следующий токен.
        advance();
    }
}

void processStringLiteral(TextCursor& cursor, std::vector<Span>& literals) {
    TextPos strBegin = cursor.pos;
    cursor.advance();

    std::string value;

    while (cursor.isValid() && cursor.currentChar() != '"') {
        if (cursor.currentChar() == '\\') {
            value += '\\';
            cursor.advance();

            if (cursor.isValid()) {
                value += cursor.currentChar();
                cursor.advance();
            }
        }
        else {
            value += cursor.currentChar();
            cursor.advance();
        }
    }

    literals.push_back({value, strBegin});

    if (cursor.isValid()) {
        cursor.advance();
    }
}

void skipCharLiteral(TextCursor& cursor) {
    cursor.advance();

    while (cursor.isValid() && cursor.currentChar() != '\'') {
        if (cursor.currentChar() == '\\') {
            cursor.advance();

            if (cursor.isValid()) {
                cursor.advance();
            }
        }
        else {
            cursor.advance();
        }
    }

    if (cursor.isValid()) {
        cursor.advance();
    }
}

void skipBlockComment(TextCursor& cursor) {
    cursor.advance(2);
    cursor.skipUntil("*/");

    if (cursor.isValid()) {
        cursor.advance(2);
    }
}

void extractStringLiteralsFromCcode(
    const std::vector<std::string>& text,
    std::vector<Span>& literals)
{
    TextCursor cursor(text);

    while (cursor.isValid()) {
        if (cursor.currentChar() == '"') {
            processStringLiteral(cursor, literals);
        }
        else if (cursor.startsWith("'")) {
            skipCharLiteral(cursor);
        }
        else if (cursor.startsWith("//")) {
            cursor.advanceToNextLine();
        }
        else if (cursor.startsWith("/*")) {
            skipBlockComment(cursor);
        }
        else {
            cursor.advance();
        }
    }
}
