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

/// @file grammar_test.cc

#include <gtest/gtest.h>
#include <cstring>

#include "grammar/parser.h"

TEST(Grammar, ExpressionTest) {
  const char* EXP1 = "dialogue test { $temp = 12 + 3 * 4 }";
  Parser parser1{EXP1, std::strlen(EXP1)};
  auto ast1 = parser1.GenerateAST();

  EXPECT_EQ(ast1->i18n, nullptr);
  EXPECT_EQ(ast1->dialogues->dialogue->type.type, TokenType::TOKEN_EMPTY);
  EXPECT_EQ(ast1->dialogues->dialogue->condition, nullptr);
  EXPECT_EQ(ast1->dialogues->dialogue->name.value, 0);

  auto cmd1 = ast1->dialogues->dialogue->content->command;
  auto assign1 = std::dynamic_pointer_cast<Assign>(cmd1);
  EXPECT_NE(assign1, nullptr);
  EXPECT_EQ(assign1->target.value, 1);
  EXPECT_EQ(assign1->expression->op, OperatorType::ADD);
  EXPECT_EQ(assign1->expression->left->value.value, 12);
  EXPECT_EQ(assign1->expression->right->op, OperatorType::MUL);
  EXPECT_EQ(assign1->expression->right->left->value.value, 3);
  EXPECT_EQ(assign1->expression->right->right->value.value, 4);

  const char* EXP2 = "dialogue test { $temp = 12 + 3 * 4 == 24 }";
  Parser parser2{EXP2, std::strlen(EXP2)};
  auto ast2 = parser2.GenerateAST();
  auto cmd2 = ast2->dialogues->dialogue->content->command;
  auto assign2 = std::dynamic_pointer_cast<Assign>(cmd2);
  EXPECT_EQ(assign2->target.value, 1);
  EXPECT_EQ(assign2->expression->op, OperatorType::EQUAL);
  EXPECT_EQ(assign2->expression->right->value.value, 24);
  EXPECT_EQ(assign2->expression->left->op, OperatorType::ADD);

  
}
