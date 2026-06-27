//
// Created by rzdv on 10.05.2026.
//

#include "gtest/gtest.h"
#include "../include/parser.h"
#include "test_utils.h"

using std::vector;
using std::string;

TEST(TextCursorAdvanceTests, Advance_InsideLine_MovesColumn)
{
	const vector<string> text = { "abc" };
	TextCursor cursor = MakeCursor(text, 0, 0);

	cursor.advance();

	AssertTextPos(cursor.pos, 0, 1);
}

TEST(TextCursorAdvanceTests, Advance_ZeroShift_DoesNothing)
{
	const vector<string> text = { "abc" };
	TextCursor cursor = MakeCursor(text, 0, 1);

	cursor.advance(0);

	AssertTextPos(cursor.pos, 0, 1);
}

TEST(TextCursorAdvanceTests, Advance_FromEndOfLine_MovesToNextRowStart)
{
	const vector<string> text = { "abc", "de" };
	TextCursor cursor = MakeCursor(text, 0, 3);

	cursor.advance();

	AssertTextPos(cursor.pos, 1, 0);
}

TEST(TextCursorTest, AdvanceToNextLine_OnInvalidCursor_ReturnsImmediately) {
	const std::vector<std::string> text = { "line1" };

	TextCursor cursor(text, 99, 0);

	ASSERT_FALSE(cursor.isValid());

	cursor.advanceToNextLine();

	ASSERT_EQ(99, cursor.pos.row);
	ASSERT_EQ(0, cursor.pos.col);
}

TEST(TextCursorAdvanceTests, Advance_AcrossSeveralRows_AccumulatesTransitions)
{
	const vector<string> text = { "ab", "cd", "e" };
	TextCursor cursor = MakeCursor(text, 0, 2);

	cursor.advance(3);

	AssertTextPos(cursor.pos, 1, 2);
}

TEST(TextCursorAdvanceTests, Advance_InvalidCursor_DoesNotChangePosition)
{
	const vector<string> text = { "abc" };
	TextCursor cursor = MakeCursor(text, 1, 0);

	cursor.advance(2);

	AssertTextPos(cursor.pos, 1, 0);
}
