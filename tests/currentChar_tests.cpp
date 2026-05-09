//
// Created by rzdv on 10.05.2026.
//

#include "gtest/gtest.h"
#include "../include/parser.h"
#include "test_utils.h"

using std::vector;
using std::string;

TEST(CurrentCharTest, InsideLine_ReturnsSymbol)
{
    const vector<string> text = { "abc" };
    const TextCursor cursor = MakeCursor(text, 0, 1);

    ASSERT_EQ('b', cursor.currentChar());
}

TEST(CurrentCharTest, AtLineEnd_ReturnsNewLineMarker)
{
    const vector<string> text = { "abc" };
    const TextCursor cursor = MakeCursor(text, 0, 3);

    ASSERT_EQ('\n', cursor.currentChar());
}

TEST(CurrentCharTest, OnEmptyLine_ReturnsNewLineMarker)
{
    const vector<string> text = { "" };
    const TextCursor cursor = MakeCursor(text, 0, 0);

    ASSERT_EQ('\n', cursor.currentChar());
}

TEST(CurrentCharTest, InvalidPosition_ReturnsZero)
{
    const vector<string> text = { "abc" };
    const TextCursor cursor = MakeCursor(text, 1, 0);

    ASSERT_EQ('\0', cursor.currentChar());
}
