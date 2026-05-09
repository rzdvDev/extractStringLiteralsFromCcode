//
// Created by rzdv on 10.05.2026.
//

#include "test_utils.h"

TextCursor MakeCursor(const std::vector<std::string>& text, int row, int col) {
    TextCursor cursor(text);
    cursor.pos.row = row;
    cursor.pos.col = col;
    return cursor;
}

