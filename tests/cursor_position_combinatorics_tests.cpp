//
// Created by rzdv on 07.06.2026.
//

#include "gtest/gtest.h"
#include "../include/parser.h"
#include "test_utils.h"

#include <ostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace {

struct CursorPositionCase {
    const char* rowName;
    int row;
    const char* colName;
    int col;
    bool expectedValid;
    char expectedChar;
    TextPos expectedAfterAdvance;
};

void PrintTo(const CursorPositionCase& param, std::ostream* os)
{
    *os << param.rowName << "_" << param.colName;
}

const vector<string>& CursorMatrixText()
{
    static const vector<string> text = { "abc", "def", "ghi" };
    return text;
}

vector<CursorPositionCase> MakeCursorPositionCases()
{
    const vector<string>& text = CursorMatrixText();
    const int lineLength = static_cast<int>(text.front().size());

    struct RowCase {
        const char* name;
        int row;
    };

    struct ColCase {
        const char* name;
        int col;
    };

    const vector<RowCase> rowCases = {
        { "FirstLine", 0 },
        { "MiddleLine", 1 },
        { "LastLine", 2 },
        { "OutsideText", static_cast<int>(text.size()) },
    };

    const vector<ColCase> colCases = {
        { "LineStart", 0 },
        { "LineMiddle", 1 },
        { "LineEnd", lineLength },
        { "OutsideLine", lineLength + 1 },
    };

    vector<CursorPositionCase> cases;
    for (const RowCase& rowCase : rowCases) {
        for (const ColCase& colCase : colCases) {
            const bool rowIsInsideText = rowCase.row >= 0 && rowCase.row < static_cast<int>(text.size());
            const bool expectedValid = rowIsInsideText && colCase.col <= lineLength;

            char expectedChar = '\0';
            if (expectedValid) {
                expectedChar = colCase.col == lineLength ? '\n' : text[rowCase.row][colCase.col];
            }

            TextPos expectedAfterAdvance = { rowCase.row, colCase.col };
            if (expectedValid) {
                if (colCase.col == lineLength) {
                    expectedAfterAdvance = { rowCase.row + 1, 0 };
                }
                else {
                    expectedAfterAdvance = { rowCase.row, colCase.col + 1 };
                }
            }

            cases.push_back({
                rowCase.name,
                rowCase.row,
                colCase.name,
                colCase.col,
                expectedValid,
                expectedChar,
                expectedAfterAdvance,
            });
        }
    }

    return cases;
}

string CursorPositionCaseName(const ::testing::TestParamInfo<CursorPositionCase>& info)
{
    return string(info.param.rowName) + "_" + info.param.colName;
}

class CursorPositionCombinatoricsTests
    : public ::testing::TestWithParam<CursorPositionCase> {
};

} // namespace

TEST_P(CursorPositionCombinatoricsTests, IsValid_ReturnsExpectedValueForCursorPositionMatrix)
{
    const CursorPositionCase& param = GetParam();
    const TextCursor cursor = MakeCursor(CursorMatrixText(), param.row, param.col);

    EXPECT_EQ(param.expectedValid, cursor.isValid());
}

TEST_P(CursorPositionCombinatoricsTests, CurrentChar_ReturnsExpectedSymbolForCursorPositionMatrix)
{
    const CursorPositionCase& param = GetParam();
    const TextCursor cursor = MakeCursor(CursorMatrixText(), param.row, param.col);

    EXPECT_EQ(param.expectedChar, cursor.currentChar());
}

TEST_P(CursorPositionCombinatoricsTests, Advance_MovesExpectedlyForCursorPositionMatrix)
{
    const CursorPositionCase& param = GetParam();
    TextCursor cursor = MakeCursor(CursorMatrixText(), param.row, param.col);

    cursor.advance();

    AssertTextPos(cursor.pos, param.expectedAfterAdvance.row, param.expectedAfterAdvance.col);
}

INSTANTIATE_TEST_SUITE_P(
    CursorPositionMatrix,
    CursorPositionCombinatoricsTests,
    ::testing::ValuesIn(MakeCursorPositionCases()),
    CursorPositionCaseName);
