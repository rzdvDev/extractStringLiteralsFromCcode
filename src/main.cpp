#include "../include/file_io.h"
#include "../include/parser.h"
#include <iostream>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

using namespace std;


int main(const int argc, char* argv[]) {

#ifdef _WIN32
    // Установка кодировки консоли для корректного отображения текста (Windows)
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
#endif

    // Проверка количества аргументов командной строки
    if (argc != 3) {
        cerr << "Usage:" << argv[0]
             << " <path to input file> <path to output file>" << endl;
        return 1; // некорректный вызов программы
    }

    // Извлекаем пути к входному и выходному файлу
    const string inputPath = argv[1];
    const string outputPath = argv[2];

    // Список для накопления ошибок при чтении файла
    vector<AppError> errors;

    // Чтение исходного кода из входного файла
    const vector<string> code = readFile(inputPath, errors);

    // Если при чтении возникли ошибки — выводим их и завершаем работу
    if (!errors.empty()) {
        for (const AppError& e : errors) {
            e.print();
        }
        return static_cast<int>(errors[0].type);
    }

    // Список для хранения найденных строковых литералов
    vector<Span> literals;

    // Извлечение строковых литералов из исходного кода
    extractStringLiteralsFromCcode(code, literals);

    // Запись результата в выходной файл
    writeResultsToFile(literals, outputPath, errors);

    // Проверка ошибок записи
    if (!errors.empty()) {
        for (const AppError& e : errors) {
            e.print();
        }
        return static_cast<int>(errors[0].type);
    }

    // Успешное завершение программы
    return 0;
}