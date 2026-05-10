//
// Created by rzdv on 10.05.2026.
//

#ifndef EXTRACTSTRINGLITERALSFROMCCODE_FILE_IO_H
#define EXTRACTSTRINGLITERALSFROMCCODE_FILE_IO_H

#include <vector>
#include <string>
#include "../include/parser.h"

using namespace std;


/**
 * Записывает результаты извлечения в файл.
 *
 * @param literals Найденные строковые литералы.
 * @param outputPath Путь к выходному файлу.
 * @param errors Список, в который будут добавлены ошибки записи.
 */
void writeResultsToFile(const std::vector<Span>& literals, const std::string& outputPath, std::vector<AppError>& errors);

/**
 * Читает файл и возвращает его содержимое построчно.
 *
 * @param path Путь к входному файлу.
 * @param errors Список, в который будут добавлены ошибки чтения.
 * @return Вектор строк, прочитанных из файла.
 */
std::vector<std::string> readFile(const std::string &path, std::vector<AppError> &errors);


#endif //EXTRACTSTRINGLITERALSFROMCCODE_FILE_IO_H
