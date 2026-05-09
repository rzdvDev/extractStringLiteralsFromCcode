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
 * Читает файл и возвращает его содержимое построчно.
 *
 * @param path Путь к входному файлу.
 * @param errors Список, в который будут добавлены ошибки чтения.
 * @return Вектор строк, прочитанных из файла.
 */
std::vector<std::string> readFile(const std::string &path, std::vector<AppError> &errors);

/**
 * Извлекает строковые литералы из C-кода.
 *
 * @param text Исходный текст программы.
 * @param litrals Контейнер для найденных литералов и их позиций.
 */
void extractStringLiteralsFromCcode(const std::vector<std::string>& text, std::vector<Span>& litrals);

/**
 * Записывает результаты извлечения в файл.
 *
 * @param literals Найденные строковые литералы.
 * @param outputPath Путь к выходному файлу.
 * @param errors Список, в который будут добавлены ошибки записи.
 */
void writeResultsToFile(const std::vector<Span>& literals, const std::string& outputPath, std::vector<AppError>& errors);

#endif //EXTRACTSTRINGLITERALSFROMCCODE_PARSER_H
