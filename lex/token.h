/**
* @copybrief
* MIT License
* Copyright (c) 2022 NeilKleistGao
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
 */

/// @file token.h

#ifndef SFTD_TOKEN_H
#define SFTD_TOKEN_H

enum class TokenType {
  TOKEN_EOF = -1,
  TOKEN_DIALOGUE = 0,
  TOKEN_WHEN,
  TOKEN_SELECT,
  TOKEN_IF,
  TOKEN_ELSE,
  TOKEN_GOTO,
  TOKEN_USE,
  TOKEN_IN,
  TOKEN_NULL,
  TOKEN_AND,
  TOKEN_OR,
  TOKEN_NOT,
  TOKEN_ADD,
  TOKEN_SUB,
  TOKEN_MUL,
  TOKEN_DIV,
  TOKEN_MOD,
  TOKEN_EQUAL,
  TOKEN_COLON,
  TOKEN_LEFT_SQUARE,
  TOKEN_RIGHT_SQUARE,
  TOKEN_LEFT_PARENTHESES,
  TOKEN_RIGHT_PARENTHESES,
  TOKEN_LEFT_CURLY,
  TOKEN_RIGHT_CURLY,
  TOKEN_STRING,
  TOKEN_FLOAT,
  TOKEN_INT,
  TOKEN_BOOL,
  TOKEN_DOLLAR,
  TOKEN_SHARP,
  TOKEN_AT,
  TOKEN_LESS,
  TOKEN_GREATER,
  TOKEN_VARIABLE,
  TOKEN_SOUND,
  TOKEN_COMMA,
  TOKEN_MOVE,
  TOKEN_ANIME,
  TOKEN_DELAY,
  TOKEN_INTERACT,
  TOKEN_TRIGGER,
  TOKEN_AUTO,
  TOKENS_SIZE,
  TOKEN_EMPTY
};

struct Token {
  TokenType type{};
  int value{};

  Token() = default;
  explicit Token(TokenType p_type, int p_value = 0) : type(p_type), value(p_value) {}
};

#endif // SFTD_TOKEN_H
