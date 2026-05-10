//
// Created by rzdv on 10.05.2026.
//

#include "test_utils.h"

using std::string;
using std::vector;

TextCursor MakeCursor(const std::vector<std::string>& text, int row, int col) {
    TextCursor cursor(text);
    cursor.pos.row = row;
    cursor.pos.col = col;
    return cursor;
}

vector<Span> ExtractLiterals(const vector<string>& text)
{
  vector<Span> literals;
  extractStringLiteralsFromCcode(text, literals);
  return literals;
}

void AssertSpan(const Span& span, const string& expectedValue, int expectedRow, int expectedCol)
{
  EXPECT_EQ(expectedValue, span.value);
  EXPECT_EQ(expectedRow, span.pos.row);
  EXPECT_EQ(expectedCol, span.pos.col);
}

int FindLiteralStart(const vector<string>& text, size_t line, const string& needle)
{
  const auto pos = text.at(line).find(needle);
  EXPECT_NE(string::npos, pos);
  return pos == string::npos ? -1 : static_cast<int>(pos);
}

vector<string> MakeCompactSearchText()
{
  return {
    R"(int main() {)",
    R"(    puts("alpha");)",
    R"(    puts("beta");)",
    R"(})"
  };
}

vector<string> MakeRealisticSearchText()
{
  return {
    R"(#include <stdio.h>)",
    R"(void log_message(const char* text) {)",
    R"(    // setup)",
    R"(    log_info("INFO");)",
    R"(})",
    R"(/* skipped */)",
    R"(int state = 0;)",
    R"(    status = "ready";)",
    R"(// comment)",
    R"(more filler)",
    R"(yet more filler)",
    R"(    report("error: invalid code");)",
    R"(placeholder)",
    R"(    const char* path = "C:\\temp\\file.txt";)",
    "    const char* weird = \"a\\\"b\";",
  };
}

vector<string> MakeLargeRealCodeSearchText()
{
  vector<string> text(31, R"(// filler)");
  text[9] = R"(    trace("TRACE");)";
  text[10] = R"(    end_marker("::END");)";
  text[15] = R"(    const char* empty = "empty";)";
  text[17] = R"(    log("warn");)";
  text[18] = R"(    log("warning");)";
  text[24] = R"(    const char* pattern = "[A-Z]+";)";
  text[25] = R"(    const char* path = "C:\\logs\\app.log";)";
  text[26] = "    const char* json = \"{\\\"ok\\\":true}\";";
  text[30] = R"(    puts("usage: tool <input>");)";
  return text;
}

