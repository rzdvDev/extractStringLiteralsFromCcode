//
// Created by rzdv on 10.05.2026.
//

#include "gtest/gtest.h"
#include "../include/parser.h"
#include "test_utils.h"

using std::vector;
using std::string;

class TextCursorStartsWithTests : public ::testing::Test {
};

TEST_F(TextCursorStartsWithTests, StartsWith_MatchingPrefix_ReturnsTrue)
{
    const vector<string> text = { "hello" };
    const TextCursor cursor = MakeCursor(text, 0, 0);

    EXPECT_TRUE(cursor.startsWith("he"));
}

TEST_F(TextCursorStartsWithTests, StartsWith_NonMatchingPrefix_ReturnsFalse)
{
    const vector<string> text = { "hello" };
    const TextCursor cursor = MakeCursor(text, 0, 0);

    EXPECT_FALSE(cursor.startsWith("hi"));
}

TEST_F(TextCursorStartsWithTests, StartsWith_PrefixAcrossLineBoundary_ReturnsTrue)
{
    const vector<string> text = { "ab", "cd" };
    const TextCursor cursor = MakeCursor(text, 0, 1);

    EXPECT_TRUE(cursor.startsWith("b\nc"));
}

TEST_F(TextCursorStartsWithTests, StartsWith_EmptySubstring_ReturnsTrue)
{
    const vector<string> text = { "abc" };
    const TextCursor cursor = MakeCursor(text, 0, 1);

    EXPECT_TRUE(cursor.startsWith(""));
}

TEST_F(TextCursorStartsWithTests, StartsWith_InvalidCursor_ReturnsFalse)
{
    const vector<string> text = { "abc" };
    const TextCursor cursor = MakeCursor(text, 1, 0);

    EXPECT_FALSE(cursor.startsWith("a"));
}
