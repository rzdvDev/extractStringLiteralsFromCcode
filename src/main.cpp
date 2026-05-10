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
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
#endif

    if (argc != 3) {
        cerr << "Usage:" << argv[0]
        << " <path to input file> <path to output file>" << endl;
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