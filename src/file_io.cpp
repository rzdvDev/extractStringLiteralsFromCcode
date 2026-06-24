//
// Created by rzdv on 10.05.2026.
//

#include "../include/file_io.h"
#include <fstream>
#include <map>


std::vector<std::string> readFile(const std::string &path, std::vector<AppError> &errors) {
    // Открываем входной файл для чтения
    ifstream in(path);

    // Проверка успешности открытия файла
    if (!in.is_open()) {
        errors.push_back(AppError::fileNotFound(path));
        return {};
    }

    vector<string> lines;
    string line;
    // Номер текущей строки используется для контроля ограничений.
    int lineNumber = 1;

    // Последовательно читаем файл построчно.
    while (getline(in, line)) {
        // Проверка максимально допустимого количества строк.
        if (lineNumber > MAX_LINES) {
            errors.push_back(AppError::tooManyLines(path));
            return {};
        }
        // Проверка максимально допустимой длины строки.
        if (static_cast<int>(line.size()) > MAX_LINE_LEN) {
            errors.push_back(AppError::lineTooLong(lineNumber, MAX_LINE_LEN));
            return {};
        }
        // Добавляем корректную строку в результирующий контейнер.
        lines.push_back(line);
        lineNumber++;
    }
    return lines;
}

void writeResultsToFile(const std::vector<Span> &literals, const std::string &outputPath, std::vector<AppError> &errors) {

    // Хранит порядок первого появления литералов.
    vector<string> insertionOrder;

    // Отображение: значение литерала -> список его позиций.
    map<string, vector<TextPos>> grouped;

    // Группируем все найденные литералы по их текстовому содержимому.
    for (const Span &span : literals) {

        // Запоминаем первое появление нового литерала.
        if (!grouped.contains(span.value)) {
            insertionOrder.push_back(span.value);
        }
        // Добавляем координаты текущего вхождения.
        grouped[span.value].push_back(span.pos);
    }

    // Открываем выходной файл.
    ofstream out(outputPath);

    // Проверяем успешность открытия файла.
    if (!out.is_open()) {
        errors.push_back(AppError::outputFailed(outputPath));
        return;
    }

    // Записываем данные в порядке первого появления литералов.
    for (const string &value : insertionOrder) {
        const auto& positions = grouped[value];

        // Выводим значение литерала.
        out << "\"" << value << "\": [";

        // Выводим список координат всех его вхождений.
        for (int i = 0; i < static_cast<int>(positions.size()); i++) {
            if (i > 0) {
                out << ", ";
            }
            out << "{" << positions[i].row << ", " << positions[i].col + 1 << "}";
        }
        out << "]\n";
    }
}
