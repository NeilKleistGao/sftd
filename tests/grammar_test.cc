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
  {
    const char* EXP = "dialogue test { $temp = 12 + 3 * 4 }";
    Parser parser{EXP, std::strlen(EXP)};
    auto ast = parser.GenerateAST();

    EXPECT_EQ(ast->i18n, nullptr);
    EXPECT_EQ(ast->dialogues->dialogue->type.type, TokenType::TOKEN_EMPTY);
    EXPECT_EQ(ast->dialogues->dialogue->condition, nullptr);
    EXPECT_EQ(ast->dialogues->dialogue->name.value, 0);

    auto cmd = ast->dialogues->dialogue->content->command;
    auto assign = std::dynamic_pointer_cast<Assign>(cmd);
    EXPECT_NE(assign, nullptr);
    EXPECT_EQ(assign->target.value, 1);
    EXPECT_EQ(assign->expression->op, OperatorType::ADD);
    EXPECT_EQ(assign->expression->left->value.value, 12);
    EXPECT_EQ(assign->expression->right->op, OperatorType::MUL);
    EXPECT_EQ(assign->expression->right->left->value.value, 3);
    EXPECT_EQ(assign->expression->right->right->value.value, 4);
  }

  {
    const char* EXP = "dialogue test { $temp = 12 + 3 * 4 == 24 }";
    Parser parser{EXP, std::strlen(EXP)};
    auto ast = parser.GenerateAST();
    auto cmd = ast->dialogues->dialogue->content->command;
    auto assign = std::dynamic_pointer_cast<Assign>(cmd);
    EXPECT_EQ(assign->target.value, 1);
    EXPECT_EQ(assign->expression->op, OperatorType::EQUAL);
    EXPECT_EQ(assign->expression->right->value.value, 24);
    EXPECT_EQ(assign->expression->left->op, OperatorType::ADD);
  }

  {
    const char* EXP = "dialogue test { $temp = not $left or $right }";
    Parser parser{EXP, std::strlen(EXP)};
    auto ast = parser.GenerateAST();
    auto cmd = ast->dialogues->dialogue->content->command;
    auto assign = std::dynamic_pointer_cast<Assign>(cmd);
    EXPECT_EQ(assign->expression->op, OperatorType::OR);
    EXPECT_EQ(assign->expression->left->op, OperatorType::NOT);
    EXPECT_EQ(assign->expression->left->right->value.value, 2);
    EXPECT_EQ(assign->expression->right->value.value, 3);
  }

  {
    const char* EXP = "dialogue test { $temp = 3 * (2 + 3) == 7 and 2.5 / 2 <= 4 or false }";
    Parser parser{EXP, std::strlen(EXP)};
    auto ast = parser.GenerateAST();
    auto cmd = ast->dialogues->dialogue->content->command;
    auto assign = std::dynamic_pointer_cast<Assign>(cmd);
    EXPECT_EQ(assign->expression->op, OperatorType::OR);
    auto left = assign->expression->left;
    EXPECT_EQ(left->op, OperatorType::AND);
    EXPECT_EQ(left->left->op, OperatorType::EQUAL);
    EXPECT_EQ(left->right->op, OperatorType::LESS_EQUAL);
    auto first = left->left; auto second = left->right;
    EXPECT_EQ(first->left->op, OperatorType::MUL);
    EXPECT_EQ(second->left->op, OperatorType::DIV);
  }

  {
    const char* EXP = "dialogue test { $temp = -1 + 2 + 3 == 7 % 5 }";
    Parser parser{EXP, std::strlen(EXP)};
    auto ast = parser.GenerateAST();
    auto cmd = ast->dialogues->dialogue->content->command;
    auto assign = std::dynamic_pointer_cast<Assign>(cmd);
    EXPECT_EQ(assign->expression->op, OperatorType::EQUAL);
    EXPECT_EQ(assign->expression->right->op, OperatorType::MOD);

    auto left = assign->expression->left;
    EXPECT_EQ(left->op, OperatorType::ADD);
    EXPECT_EQ(left->left->op, OperatorType::ADD);
    EXPECT_EQ(left->left->left->op, OperatorType::NEG);
  }
}
