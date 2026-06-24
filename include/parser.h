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

    /**
     * Создаёт ошибку с заданным типом.
     *
     * @param path Путь к входному или выходному файлу.
     * @return Объект ошибки типа INPUT_FILE_ERROR.
     */
    static AppError fileNotFound(const std::string &path);

    /**
     * Создаёт ошибку при невозможности записать выходной файл.
     *
     * @param path Путь к выходному файлу.
     * @return Объект ошибки типа OUTPUT_FILE_ERROR.
     */
    static AppError outputFailed(const std::string &path);

    /**
     * Создаёт ошибку, если количество строк превышает допустимый предел.
     *
     * @param path Путь к файлу с входными данными.
     * @return Объект ошибки типа TOO_MANY_LINES.
     */
    static AppError tooManyLines(const std::string &path);

    /**
     * Создаёт ошибку, если длина строки превышает допустимый предел.
     *
     * @param lineNumber Номер строки.
     * @param limit Максимально допустимая длина строки.
     * @return Объект ошибки типа LINE_TOO_LONG.
     */
    static AppError lineTooLong(int lineNumber, int limit);

    /**
     * Возвращает человекочитаемое сообщение об ошибке.
     *
     * @return Текст сообщения об ошибке.
     */
    [[nodiscard]] std::string message() const;

    /**
     * Выводит сообщение об ошибке в стандартный поток ошибок.
     */
    void print() const;

private:
    AppError(ErrorType t);
};

struct TextPos {
    int row;
    int col;
};

struct Span {
    std::string value;
    TextPos pos;
};

struct TextCursor {
    const std::vector<std::string> &text;
    TextPos pos;

    /**
     * Создаёт курсор для обхода текста.
     *
     * @param text Исходный текст в виде массива строк.
     */
    TextCursor(const std::vector<std::string> &text);

    /**
     * Проверяет, находится ли курсор в допустимой позиции.
     *
     * @return true, если текущая позиция валидна; иначе false.
     */
    [[nodiscard]] bool isValid() const;

    /**
     * Возвращает символ в текущей позиции курсора.
     *
     * @return Текущий символ.
     */
    [[nodiscard]] char currentChar() const;

    /**
     * Сдвигает курсор вперёд на указанное количество символов.
     *
     * @param shift Количество символов для сдвига.
     */
    void advance(unsigned int shift = 1);

    /**
     * Переходит на начало следующей строки.
     */
    void advanceToNextLine();

    /**
     * Проверяет, начинается ли текущая позиция с заданной подстроки.
     *
     * @param sub Подстрока для проверки.
     * @return true, если текущий фрагмент начинается с sub; иначе false.
     */
    [[nodiscard]] bool startsWith(const std::string& sub) const;

    /**
     * Пропускает символы до встречи с закрывающим токеном.
     *
     * @param closeToken Токен, до которого выполняется пропуск.
     */
    void skipUntil(const std::string& closeToken);
};


/**
 * @brief Обрабатывает строковый литерал C/C++.
 *
 * Извлекает строковый литерал, начиная с текущей позиции курсора.
 * Поддерживает escape-последовательности (например, \n, \", \\).
 *
 * @param cursor Курсор по исходному тексту.
 * @param literals Вектор, в который сохраняются найденные строковые литералы.
 */
void processStringLiteral(TextCursor& cursor, std::vector<Span>& literals);

/**
 * @brief Пропускает символьный литерал C/C++.
 *
 * Обрабатывает символы вида 'a', '\n', '\0' и корректно пропускает escape-последовательности.
 *
 * @param cursor Курсор по исходному тексту.
 */
void skipCharLiteral(TextCursor& cursor);

/**
 * @brief Пропускает многострочный комментарий.
 *
 * Пропускает блок комментария вида / * ... * / до закрывающего маркера.
 *
 * @param cursor Курсор по исходному тексту.
 */
void skipBlockComment(TextCursor& cursor);

/**
 * @brief Извлекает строковые литералы из C/C++ кода.
 *
 * Проходит по исходному тексту и извлекает строковые литералы,
 * игнорируя символьные литералы и комментарии (// и / * * /).
 *
 * @param text Входной исходный текст построчно.
 * @param literals Выходной список найденных строковых литералов с их позициями.
 */
void extractStringLiteralsFromCcode(
    const std::vector<std::string>& text,
    std::vector<Span>& literals);


#endif //EXTRACTSTRINGLITERALSFROMCCODE_PARSER_H
