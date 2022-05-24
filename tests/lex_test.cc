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

#include "lex/lex_parser.h"

TEST(LexTest, CharacterTest) {
  const char* CHARACTER_FILE = "character kusanagi_motoko {\n"
                               "    default {\n"
                               "        avatar: \"motoko.png\"\n"
                               "        font: default\n"
                               "        sound: null\n"
                               "    }\n"
                               "    \n"
                               "    state happy {\n"
                               "        // avatar: \"motoko_happy.png\"\n"
                               "        // font: default\n"
                               "        // sound: null\n"
                               "    }\n"
                               "    state angry {\n"
                               "        avatar: \"motoko_angry_pic.png\"\n"
                               "        // font: default\n"
                               "        // sound: null\n"
                               "    }\n"
                               "}";

  LexParser parser{CHARACTER_FILE, std::strlen(CHARACTER_FILE)};

  EXPECT_EQ(parser.HasNext(), true);
  auto token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_CHARACTER);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_VARIABLE);
  EXPECT_EQ(token.value, 0);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_LEFT_CURLY);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_DEFAULT);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_LEFT_CURLY);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_AVATAR);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_COLON);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_STRING);
  EXPECT_EQ(token.value, 0);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_FONT);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_COLON);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_DEFAULT);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_SOUND);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_COLON);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_NULL);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_RIGHT_CURLY);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_STATE);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_VARIABLE);
  EXPECT_EQ(token.value, 1);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_LEFT_CURLY);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_RIGHT_CURLY);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_STATE);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_VARIABLE);
  EXPECT_EQ(token.value, 2);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_LEFT_CURLY);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_AVATAR);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_COLON);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_STRING);
  EXPECT_EQ(token.value, 1);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_RIGHT_CURLY);

  EXPECT_EQ(parser.HasNext(), true);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_RIGHT_CURLY);

  EXPECT_EQ(parser.HasNext(), false);
  token = parser.GetNext();
  EXPECT_EQ(token.type, TokenType::TOKEN_EOF);
}

TEST(LexTest, EffectTest) {
  EXPECT_EQ(true, true);
}

TEST(LexTest, DialogueTest) {
  EXPECT_EQ(true, true);
}

TEST(LexTest, ErrorTest) {
  EXPECT_EQ(true, true);
}