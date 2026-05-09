//
// Created by rzdv on 10.05.2026.
//

#ifndef EXTRACTSTRINGLITERALSFROMCCODE_TEST_UTILS_H
#define EXTRACTSTRINGLITERALSFROMCCODE_TEST_UTILS_H

#include <vector>
#include <string>
#include "../include/parser.h"

/**
 * Создаёт текстовый курсор с заданной позицией для тестирования.
 *
 * @param text Вектор строк текста.
 * @param row Номер строки.
 * @param col Номер колонки.
 * @return TextCursor, установленный на указанную позицию.
 */
TextCursor MakeCursor(const std::vector<std::string>& text, int row, int col);

#endif //EXTRACTSTRINGLITERALSFROMCCODE_TEST_UTILS_H
