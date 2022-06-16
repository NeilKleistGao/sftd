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

/// @file lex_test.cc

#include <gtest/gtest.h>
#include <cstring>
#include <iostream>

#include "lex/lex_parser.h"
#include "tables/constant_table.h"
#include "tables/symbol_table.h"
#include "exceptions/lex_exceptions.h"

TEST(LexTest, KeywordTest) {
  const char* KEYWORDS_FILE = "when select null and or not sound animate move delay interact trigger auto";
  ConstantTable::GetInstance()->Clear();
  SymbolTable::GetInstance()->Clear();
  LexParser parser{KEYWORDS_FILE, std::strlen(KEYWORDS_FILE)};

  EXPECT_EQ(parser.HasNext(), true);
  auto token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_WHEN);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_SELECT);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_NULL);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_AND);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_OR);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_NOT);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_SOUND);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_ANIME);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_MOVE);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_DELAY);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_INTERACT);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_TRIGGER);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_AUTO);
}

TEST(LexTest, DialogueTest) {
  const char* DIALOGUE_FILE = "#{\"en-GB\", \"zh-CN\"}\n"
                              "\n"
                              "dialogue GetCookie {\n"
                              "    [Alice]: {\n"
                              "        \"Give me some cookies please.\"\n"
                              "        \"I'm not hungry now.\"\n"
                              "    }\n"
                              "    \n"
                              "    <NewItem, \"cookie\">\n"
                              "}\n"
                              "\n"
                              "dialogue GetCake {\n"
                              "    [Alice]: {\n"
                              "        \"Give me some cake please.\"\n"
                              "        \"It looks great.\" in 2\n"
                              "    }\n"
                              "    \n"
                              "    <NewItem, \"cake\">\n"
                              "}\n";

  ConstantTable::GetInstance()->Clear();
  SymbolTable::GetInstance()->Clear();
  LexParser parser{DIALOGUE_FILE, std::strlen(DIALOGUE_FILE)};

  // I18N

  EXPECT_EQ(parser.HasNext(), true);
  auto token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_SHARP);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_LEFT_CURLY);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_STRING);
  EXPECT_EQ(token.value, 0);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_COMMA);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_STRING);
  EXPECT_EQ(token.value, 1);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_RIGHT_CURLY);

  // GetCookie

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_DIALOGUE);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_VARIABLE);
  EXPECT_EQ(token.value, 0);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_LEFT_CURLY);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_LEFT_SQUARE);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_VARIABLE);
  EXPECT_EQ(token.value, 1);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_RIGHT_SQUARE);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_COLON);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_LEFT_CURLY);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_STRING);
  EXPECT_EQ(token.value, 2);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_STRING);
  EXPECT_EQ(token.value, 3);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_RIGHT_CURLY);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_LESS);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_VARIABLE);
  EXPECT_EQ(token.value, 2);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_COMMA);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_STRING);
  EXPECT_EQ(token.value, 4);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_GREATER);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_RIGHT_CURLY);

  // GetCake

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_DIALOGUE);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_VARIABLE);
  EXPECT_EQ(token.value, 3);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_LEFT_CURLY);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_LEFT_SQUARE);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_VARIABLE);
  EXPECT_EQ(token.value, 1);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_RIGHT_SQUARE);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_COLON);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_LEFT_CURLY);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_STRING);
  EXPECT_EQ(token.value, 5);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_STRING);
  EXPECT_EQ(token.value, 6);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_IN);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_INT);
  EXPECT_EQ(token.value, 2);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_RIGHT_CURLY);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_LESS);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_VARIABLE);
  EXPECT_EQ(token.value, 2);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_COMMA);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_STRING);
  EXPECT_EQ(token.value, 7);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_GREATER);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_RIGHT_CURLY);
}

TEST(LexTest, ErrorTest) {
  const char* MUL_LINES_STRING = "\n\n\"abcd\nefg\"";

  ConstantTable::GetInstance()->Clear();
  SymbolTable::GetInstance()->Clear();
  try {
    LexParser parser{MUL_LINES_STRING, std::strlen(MUL_LINES_STRING)};
    auto token = parser.GetNext();
    EXPECT_EQ(true, false);
  }
  catch (const UnexpectedEndOfString& e) {
    std::cout << e.what() << std::endl;
    EXPECT_EQ(std::strcmp("Line 3: unexpected EOF.", e.what()), 0);
  }

  const char* NO_ENDING = "\"abcdefg";

  ConstantTable::GetInstance()->Clear();
  SymbolTable::GetInstance()->Clear();
  try {
    LexParser parser{NO_ENDING, std::strlen(NO_ENDING)};
    auto token = parser.GetNext();
    EXPECT_EQ(true, false);
  }
  catch (const UnexpectedEndOfString& e) {
    std::cout << e.what() << std::endl;
    EXPECT_EQ(std::strcmp("Line 1: unexpected EOF.", e.what()), 0);
  }

  const char* MANY_DOTS = "\n114.514.1919.810";

  ConstantTable::GetInstance()->Clear();
  SymbolTable::GetInstance()->Clear();
  try {
    LexParser parser{MANY_DOTS, std::strlen(MANY_DOTS)};
    auto token = parser.GetNext();
    EXPECT_EQ(true, false);
  }
  catch (const WrongNumber& e) {
    std::cout << e.what() << std::endl;
    EXPECT_EQ(std::strcmp("Line 2: wrong number expression.", e.what()), 0);
  }

  const char* WOW = "!";

  ConstantTable::GetInstance()->Clear();
  SymbolTable::GetInstance()->Clear();
  try {
    LexParser parser{WOW, std::strlen(WOW)};
    auto token = parser.GetNext();
    EXPECT_EQ(true, false);
  }
  catch (const UnknownNotion& e) {
    std::cout << e.what() << std::endl;
    EXPECT_EQ(std::strcmp("Line 1: unknown notion !", e.what()), 0);
  }
}