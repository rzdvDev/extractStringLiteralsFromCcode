//
// Created by rzdv on 10.05.2026.
//

#ifndef EXTRACTSTRINGLITERALSFROMCCODE_TEST_UTILS_H
#define EXTRACTSTRINGLITERALSFROMCCODE_TEST_UTILS_H

#include <vector>
#include <string>
#include "gtest/gtest.h"
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

std::vector<Span> ExtractLiterals(const std::vector<std::string>& text);

void AssertSpan(const Span& span, const std::string& expectedValue, int expectedRow, int expectedCol);

int FindLiteralStart(const std::vector<std::string>& text, size_t line, const std::string& needle);

std::vector<std::string> MakeCompactSearchText();

std::vector<std::string> MakeRealisticSearchText();

std::vector<std::string> MakeLargeRealCodeSearchText();

/**
 * Ассерты для сравнения позиции курсора в тестах.
 * Использует GoogleTest макросы EXPECT_EQ.
 */
inline void AssertTextPos(const TextPos& pos, int expectedRow, int expectedCol) {
	EXPECT_EQ(expectedRow, pos.row);
	EXPECT_EQ(expectedCol, pos.col);
}

#endif //EXTRACTSTRINGLITERALSFROMCCODE_TEST_UTILS_H
