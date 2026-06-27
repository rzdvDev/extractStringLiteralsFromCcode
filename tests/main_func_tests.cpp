//
// Created by rzdv on 10.05.2026.
//

#include "gtest/gtest.h"
#include "../include/parser.h"
#include "test_utils.h"

using std::string;
using std::vector;

TEST(ExtractStringLiteralsFromCcodeTests, InputType_CompactCode_ExtractsAllLiteralsInOrder)
{
	const vector<string> text = MakeCompactSearchText();

	const vector<Span> literals = ExtractLiterals(text);

	ASSERT_EQ(2u, literals.size());
	AssertSpan(literals[0], "alpha", 1, FindLiteralStart(text, 1, R"("alpha")"));
	AssertSpan(literals[1], "beta", 2, FindLiteralStart(text, 2, R"("beta")"));
}

TEST(ExtractStringLiteralsFromCcodeTests, InputType_RealisticCode_MixesBusinessStringsCommentsAndEscaping)
{
	const vector<string> text = MakeRealisticSearchText();

	const vector<Span> literals = ExtractLiterals(text);

	ASSERT_EQ(5u, literals.size());
	AssertSpan(literals[0], "INFO", 3, FindLiteralStart(text, 3, R"("INFO")"));
	AssertSpan(literals[1], "ready", 7, FindLiteralStart(text, 7, R"("ready")"));
	AssertSpan(literals[2], "error: invalid code", 11, FindLiteralStart(text, 11, R"("error: invalid code")"));
	AssertSpan(literals[3], R"(C:\\temp\\file.txt)", 13, FindLiteralStart(text, 13, R"("C:\\temp\\file.txt")"));
	AssertSpan(literals[4], R"(a\"b)", 14, FindLiteralStart(text, 14, "\"a\\\"b\""));
}

TEST(ExtractStringLiteralsFromCcodeTests, InputType_LargeRealCode_ExtractsOnlyVisibleLiteralsFromBigSource)
{
	const vector<string> text = MakeLargeRealCodeSearchText();

	const vector<Span> literals = ExtractLiterals(text);

	ASSERT_EQ(9u, literals.size());
	AssertSpan(literals[0], "TRACE", 9, FindLiteralStart(text, 9, R"("TRACE")"));
	AssertSpan(literals[1], "::END", 10, FindLiteralStart(text, 10, R"("::END")"));
	AssertSpan(literals[2], "empty", 15, FindLiteralStart(text, 15, R"("empty")"));
	AssertSpan(literals[3], "warn", 17, FindLiteralStart(text, 17, R"("warn")"));
	AssertSpan(literals[4], "warning", 18, FindLiteralStart(text, 18, R"("warning")"));
	AssertSpan(literals[5], "[A-Z]+", 24, FindLiteralStart(text, 24, R"("[A-Z]+")"));
	AssertSpan(literals[6], R"(C:\\logs\\app.log)", 25, FindLiteralStart(text, 25, R"("C:\\logs\\app.log")"));
	AssertSpan(literals[7], R"({\"ok\":true})", 26, FindLiteralStart(text, 26, "\"{\\\"ok\\\":true}\""));
	AssertSpan(literals[8], "usage: tool <input>", 30, FindLiteralStart(text, 30, R"("usage: tool <input>")"));
}

TEST(ExtractStringLiteralsFromCcodeTests, Basic_SingleLiteral_IsExtracted)
{
	const vector<string> text = { R"(printf("Hello");)" };

	const vector<Span> literals = ExtractLiterals(text);

	ASSERT_EQ(1u, literals.size());
	AssertSpan(literals[0], "Hello", 0, 7);
}

TEST(ExtractStringLiteralsFromCcodeTests, Basic_TwoLiteralsInOneLine_AreExtractedInOrder)
{
	const vector<string> text = { R"(a = "x"; b = "y";)" };

	const vector<Span> literals = ExtractLiterals(text);

	ASSERT_EQ(2u, literals.size());
	AssertSpan(literals[0], "x", 0, 4);
	AssertSpan(literals[1], "y", 0, 13);
}

TEST(ExtractStringLiteralsFromCcodeTests, Boundary_EmptyLiteral_IsExtracted)
{
	const vector<string> text = { R"(const char* s = "";)" };

	const vector<Span> literals = ExtractLiterals(text);

	ASSERT_EQ(1u, literals.size());
	AssertSpan(literals[0], "", 0, 16);
}

TEST(ExtractStringLiteralsFromCcodeTests, Basic_AdjacentLiterals_AreReportedSeparately)
{
	const vector<string> text = { R"("a""b")" };

	const vector<Span> literals = ExtractLiterals(text);

	ASSERT_EQ(2u, literals.size());
	AssertSpan(literals[0], "a", 0, 0);
	AssertSpan(literals[1], "b", 0, 3);
}

TEST(AppErrorTest, Message_UnknownErrorType_ReturnsFallbackMessage) {
	AppError error = AppError::unknownError();

	std::string expectedMessage = "Unknown error.";
	ASSERT_EQ(expectedMessage, error.message());
}

TEST(ExtractStringLiteralsFromCcodeTests, Content_DuplicateLiteralValues_AreNotCollapsedByExtractor)
{
	const vector<string> text = { R"(puts("dup"); puts("dup");)" };

	const vector<Span> literals = ExtractLiterals(text);

	ASSERT_EQ(2u, literals.size());
	AssertSpan(literals[0], "dup", 0, 5);
	AssertSpan(literals[1], "dup", 0, 18);
}

TEST(ExtractStringLiteralsFromCcodeTests, Escaping_EscapedQuote_DoesNotCloseLiteral)
{
	const vector<string> text = { R"(printf("a\"b");)" };

	const vector<Span> literals = ExtractLiterals(text);

	ASSERT_EQ(1u, literals.size());
	AssertSpan(literals[0], "a\\\"b", 0, 7);
}

TEST(ExtractStringLiteralsFromCcodeTests, Escaping_DoubleBackslash_IsPreservedByCurrentImplementation)
{
	const vector<string> text = { R"(printf("a\\b");)" };

	const vector<Span> literals = ExtractLiterals(text);

	ASSERT_EQ(1u, literals.size());
	AssertSpan(literals[0], "a\\\\b", 0, 7);
}

TEST(ExtractStringLiteralsFromCcodeTests, Comments_LineCommentAfterLiteral_IsIgnored)
{
	const vector<string> text = { R"(printf("abc"); // "fake")" };

	const vector<Span> literals = ExtractLiterals(text);

	ASSERT_EQ(1u, literals.size());
	AssertSpan(literals[0], "abc", 0, 7);
}

TEST(ExtractStringLiteralsFromCcodeTests, Comments_LiteralInsideLineComment_IsIgnored)
{
	const vector<string> text = { R"(// printf("hidden"))" };

	const vector<Span> literals = ExtractLiterals(text);

	ASSERT_EQ(0u, literals.size());
}

TEST(ExtractStringLiteralsFromCcodeTests, Comments_LiteralInsideSingleLineBlockComment_IsIgnored)
{
	const vector<string> text = { R"(/* "hidden" */)", R"(printf("real");)" };

	const vector<Span> literals = ExtractLiterals(text);

	ASSERT_EQ(1u, literals.size());
	AssertSpan(literals[0], "real", 1, 7);
}

TEST(ExtractStringLiteralsFromCcodeTests, Comments_MultilineBlockComment_IsSkippedFully)
{
	const vector<string> text = { R"(/* start)", R"("hidden")", R"(end */)", R"(x = "ok";)" };

	const vector<Span> literals = ExtractLiterals(text);

	ASSERT_EQ(1u, literals.size());
	AssertSpan(literals[0], "ok", 3, 4);
}

TEST(ExtractStringLiteralsFromCcodeTests, Comments_ParserContinuesAfterClosingBlockCommentOnSameLine)
{
		const vector<string> text = { R"(/* skip */ printf("ok");)" };

	const vector<Span> literals = ExtractLiterals(text);

	ASSERT_EQ(1u, literals.size());
	AssertSpan(literals[0], "ok", 0, 18);
}

TEST(ExtractStringLiteralsFromCcodeTests, CharLiteral_DoubleQuoteInsideChar_DoesNotStartStringLiteral)
{
	const vector<string> text = { R"(char c = '"'; printf("ok");)" };

	const vector<Span> literals = ExtractLiterals(text);

	ASSERT_EQ(1u, literals.size());
	AssertSpan(literals[0], "ok", 0, 21);
}

TEST(ExtractStringLiteralsFromCcodeTests, CharLiteral_EscapedApostrophe_IsSkippedCorrectly)
{
	const vector<string> text = { R"(char c = '\''; printf("ok");)" };

	const vector<Span> literals = ExtractLiterals(text);

	ASSERT_EQ(1u, literals.size());
	AssertSpan(literals[0], "ok", 0, 22);
}

TEST(ExtractStringLiteralsFromCcodeTests, Boundary_EmptyInput_ReturnsEmptyCollection)
{
	const vector<string> text = {};

	const vector<Span> literals = ExtractLiterals(text);

	ASSERT_EQ(0u, literals.size());
}

TEST(ExtractStringLiteralsFromCcodeTests, Negative_TextWithoutStringLiterals_ReturnsEmptyCollection)
{
	const vector<string> text = { R"(int x = 42;)", R"(return x;)" };

	const vector<Span> literals = ExtractLiterals(text);

	ASSERT_EQ(0u, literals.size());
}

TEST(ExtractStringLiteralsFromCcodeTests, Robustness_UnterminatedLiteralAtEndOfFile_ConsumesToEnd)
{
	const vector<string> text = { R"(printf("abc)" };

	const vector<Span> literals = ExtractLiterals(text);

	ASSERT_EQ(1u, literals.size());
	AssertSpan(literals[0], "abc\n", 0, 7);
}

TEST(ExtractStringLiteralsFromCcodeTests, Robustness_LiteralCrossingLineBoundary_IsCollectedWithNewline)
{
	const vector<string> text = { R"(printf("abc)", R"(def");)" };

	const vector<Span> literals = ExtractLiterals(text);

	ASSERT_EQ(1u, literals.size());
	AssertSpan(literals[0], "abc\ndef", 0, 7);
}

TEST(ExtractStringLiteralsFromCcodeTests, Content_CommentMarkersInsideLiteral_AreTreatedAsPlainText)
{
	const vector<string> text = { R"(printf("/*not comment*/ // still string");)" };

	const vector<Span> literals = ExtractLiterals(text);

	ASSERT_EQ(1u, literals.size());
	AssertSpan(literals[0], "/*not comment*/ // still string", 0, 7);
}

