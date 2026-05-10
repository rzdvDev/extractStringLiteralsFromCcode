//
// Created by rzdv on 10.05.2026.
//

#include "gtest/gtest.h"
#include "../include/parser.h"
#include "test_utils.h"

using std::vector;
using std::string;

TEST(TextCursorSkipUntilTests, SkipUntil_TokenOnSameLine_StopsAtTokenStart)
{
	const vector<string> text = { "abc*/def" };
	TextCursor cursor = MakeCursor(text, 0, 0);

	cursor.skipUntil("*/");

	AssertTextPos(cursor.pos, 0, 3);
}

TEST(TextCursorSkipUntilTests, SkipUntil_TokenOnNextLine_StopsAtNextRow)
{
	const vector<string> text = { "abc", "*/def" };
	TextCursor cursor = MakeCursor(text, 0, 0);

	cursor.skipUntil("*/");

	AssertTextPos(cursor.pos, 1, 0);
}

TEST(TextCursorSkipUntilTests, SkipUntil_MissingToken_LeavesCursorInvalidAfterFullScan)
{
	const vector<string> text = { "abc", "def" };
	TextCursor cursor = MakeCursor(text, 0, 0);

	cursor.skipUntil("*/");

	ASSERT_FALSE(cursor.isValid());
	AssertTextPos(cursor.pos, 2, 0);
}

TEST(TextCursorSkipUntilTests, SkipUntil_TokenAlreadyUnderCursor_DoesNotMove)
{
	const vector<string> text = { "*/abc" };
	TextCursor cursor = MakeCursor(text, 0, 0);

	cursor.skipUntil("*/");

	AssertTextPos(cursor.pos, 0, 0);
}

TEST(TextCursorSkipUntilTests, SkipUntil_EmptyToken_DoesNotMove)
{
	const vector<string> text = { "abc" };
	TextCursor cursor = MakeCursor(text, 0, 1);

	cursor.skipUntil("");

	AssertTextPos(cursor.pos, 0, 1);
}

