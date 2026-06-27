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

AppError AppError::unknownError() {
    // Возвращаем "Unknown error" с типом ошибки, который не определён в перечислении ErrorType.
    return AppError(static_cast<ErrorType>(999));
}

std::string AppError::message() const {
    // Формируем сообщение в зависимости от типа ошибки
    switch (type) {

        // Ошибка входного файла (не найден / недоступен / повреждён)
        case ErrorType::INPUT_FILE_ERROR:
            return "Input data file is incorrect. "
                   "File may not exists or be blocked: \"" + path + "\"";

            // Ошибка выходного файла (нет прав / нельзя создать)
        case ErrorType::OUTPUT_FILE_ERROR:
            return "Unable to create or open output file. "
                   "Check the write permissions for directory: \"" + path + "\"";

            // Превышено допустимое количество строк во входном файле
        case ErrorType::TOO_MANY_LINES:
            return "Number of lines exceeds limit (" + to_string(limit) + ").";

            // Строка превышает допустимую длину
        case ErrorType::LINE_TOO_LONG:
            return "Length of line " + to_string(lineNumber) +
                   " exceeds limit of " + to_string(limit) + " symbols.";

            // Неизвестный тип ошибки (fallback)
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
    // Выполняем проверку один раз при создании объекта
    updateValidity();
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
    // Запоминаем позицию начала строки
    TextPos strBegin = cursor.pos;

    // Пропускаем открывающую кавычку "
    cursor.advance();

    std::string value;

    // Читаем строку до закрывающей кавычки
    while (cursor.isValid() && cursor.currentChar() != '"') {

        // Обработка escape-последовательностей (\n, \", \\ и т.д.)
        if (cursor.currentChar() == '\\') {
            value += '\\';
            cursor.advance();

            // Добавляем экранированный символ, если он существует
            if (cursor.isValid()) {
                value += cursor.currentChar();
                cursor.advance();
            }
        }
        else {
            // Обычный символ строки
            value += cursor.currentChar();
            cursor.advance();
        }
    }

    // Сохраняем найденный строковый литерал и его позицию
    literals.push_back({ value, strBegin });

    // Пропускаем закрывающую кавычку, если она есть
    if (cursor.isValid()) {
        cursor.advance();
    }
}

void skipCharLiteral(TextCursor& cursor) {
    // Пропускаем открывающую '
    cursor.advance();

    // Идем до закрывающей '
    while (cursor.isValid() && cursor.currentChar() != '\'') {

        // Обработка экранированных символов внутри символа
        if (cursor.currentChar() == '\\') {
            cursor.advance();

            // пропускаем экранированный символ
            if (cursor.isValid()) {
                cursor.advance();
            }
        }
        else {
            cursor.advance();
        }
    }

    // Пропускаем закрывающую '
    if (cursor.isValid()) {
        cursor.advance();
    }
}

void skipBlockComment(TextCursor& cursor) {
    // Пропускаем "/*"
    cursor.advance(2);

    // Переходим к закрывающему маркеру */
    cursor.skipUntil("*/");

    // Если нашли конец комментария — пропускаем его
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

        // Строковый литерал
        if (cursor.currentChar() == '"') {
            processStringLiteral(cursor, literals);
        }

        // Символьный литерал
        else if (cursor.startsWith("'")) {
            skipCharLiteral(cursor);
        }

        // Однострочный комментарий
        else if (cursor.startsWith("//")) {
            cursor.advanceToNextLine();
        }

        // Многострочный комментарий
        else if (cursor.startsWith("/*")) {
            skipBlockComment(cursor);
        }

        // Обычный символ — просто двигаем курсор дальше
        else {
            cursor.advance();
        }
    }
}