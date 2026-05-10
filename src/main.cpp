#include "../include/file_io.h"
#include "../include/parser.h"
#include <windows.h>
#include <iostream>

using namespace std;


int main(const int argc, char* argv[]) {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    if (argc != 3) {
        cerr << "Использование" << argv[0]
        << " <путь к входному файлу> <путь к выходному файлу>" << endl;
        return 1;
    }
    const string inputPath = argv[1];
    const string outputPath = argv[2];

    vector<AppError> errors;

    const vector<string> code = readFile(inputPath, errors);

    if (!errors.empty()) {
        for (const AppError& e : errors) {
            e.print();
        }
        return static_cast<int>(errors[0].type);
    }
    vector<Span> literals;
    extractStringLiteralsFromCcode(code, literals);

    writeResultsToFile(literals, outputPath, errors);
    if (!errors.empty()) {
        for (const AppError& e : errors) {
            e.print();
        }
        return static_cast<int>(errors[0].type);
    }
    return 0;
}