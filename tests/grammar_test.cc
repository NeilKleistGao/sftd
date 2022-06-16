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

TEST(Grammar, LanguageTest) {
  {
    const char* PROGRAM = R"(#{"en-GB"})";
    Parser parser{PROGRAM, std::strlen(PROGRAM)};
    auto ast = parser.GenerateAST();

    EXPECT_NE(ast->i18n, nullptr);
    EXPECT_EQ(ast->i18n->options.size(), 1);
    EXPECT_EQ(ast->i18n->options[0].value, 0);
  }

  {
    const char* PROGRAM = R"(#{"en-GB", "zh-CN"})";
    Parser parser{PROGRAM, std::strlen(PROGRAM)};
    auto ast = parser.GenerateAST();

    EXPECT_NE(ast->i18n, nullptr);
    EXPECT_EQ(ast->i18n->options.size(), 2);
    EXPECT_EQ(ast->i18n->options[0].value, 0);
    EXPECT_EQ(ast->i18n->options[1].value, 1);
  }
}

TEST(Grammar, DialogueTypeTest) {
  {
    const char* PROGRAM = "dialogue test {}";
    Parser parser{PROGRAM, std::strlen(PROGRAM)};
    auto ast = parser.GenerateAST();

    EXPECT_EQ(ast->dialogues->dialogue->type.type, TokenType::TOKEN_EMPTY);
  }

  {
    const char* PROGRAM = "auto dialogue test {}";
    Parser parser{PROGRAM, std::strlen(PROGRAM)};
    auto ast = parser.GenerateAST();

    EXPECT_EQ(ast->dialogues->dialogue->type.type, TokenType::TOKEN_AUTO);
  }

  {
    const char* PROGRAM = "interact dialogue test {}";
    Parser parser{PROGRAM, std::strlen(PROGRAM)};
    auto ast = parser.GenerateAST();

    EXPECT_EQ(ast->dialogues->dialogue->type.type, TokenType::TOKEN_INTERACT);
  }

  {
    const char* PROGRAM = "trigger dialogue test {}";
    Parser parser{PROGRAM, std::strlen(PROGRAM)};
    auto ast = parser.GenerateAST();

    EXPECT_EQ(ast->dialogues->dialogue->type.type, TokenType::TOKEN_TRIGGER);
  }
}

TEST(Grammar, DialogueConditionTest) {
  {
    const char* PROGRAM = "dialogue test when (true) {}";
    Parser parser{PROGRAM, std::strlen(PROGRAM)};
    auto ast = parser.GenerateAST();

    auto condition = ast->dialogues->dialogue->condition;
    EXPECT_EQ(condition->value.value, 1);
  }

  {
    const char* PROGRAM = "dialogue test when (1 + 2 == 3 or not $rua) {}";
    Parser parser{PROGRAM, std::strlen(PROGRAM)};
    auto ast = parser.GenerateAST();

    auto condition = ast->dialogues->dialogue->condition;
    EXPECT_EQ(condition->op, OperatorType::OR);
    EXPECT_EQ(condition->left->op, OperatorType::EQUAL);
    EXPECT_EQ(condition->right->op, OperatorType::NOT);
  }
}

TEST(Grammar, CommandTest) {
  {
    const char* PROGRAM = "dialogue test { delay 2 }";
    Parser parser{PROGRAM, std::strlen(PROGRAM)};
    auto ast = parser.GenerateAST();

    auto cmd = ast->dialogues->dialogue->content->command;
    auto delay = std::dynamic_pointer_cast<Delay>(cmd);
    EXPECT_NE(delay, nullptr);
    EXPECT_EQ(delay->time->value.value, 2);
  }

  {
    const char* PROGRAM = R"(dialogue test { sound "rua" })";
    Parser parser{PROGRAM, std::strlen(PROGRAM)};
    auto ast = parser.GenerateAST();

    auto cmd = ast->dialogues->dialogue->content->command;
    auto sound = std::dynamic_pointer_cast<Sound>(cmd);
    EXPECT_NE(sound, nullptr);
    EXPECT_EQ(sound->effect->value.value, 0);
  }

  {
    const char* PROGRAM = R"(dialogue test { animate @alice "thank" })";
    Parser parser{PROGRAM, std::strlen(PROGRAM)};
    auto ast = parser.GenerateAST();

    auto cmd = ast->dialogues->dialogue->content->command;
    auto anime = std::dynamic_pointer_cast<Animate>(cmd);
    EXPECT_NE(anime, nullptr);
    EXPECT_EQ(anime->name.value, 1);
    EXPECT_EQ(anime->anim->value.value, 0);
  }

  {
    const char* PROGRAM = "dialogue test { move @bob (2, 0) in 4 }";
    Parser parser{PROGRAM, std::strlen(PROGRAM)};
    auto ast = parser.GenerateAST();

    auto cmd = ast->dialogues->dialogue->content->command;
    auto move = std::dynamic_pointer_cast<Move>(cmd);
    EXPECT_NE(move, nullptr);
    EXPECT_EQ(move->name.value, 1);
    EXPECT_EQ(move->time->value.value, 4);
    EXPECT_EQ(move->x->value.value, 2);
    EXPECT_EQ(move->y->value.value, 0);
  }

  {
    const char* PROGRAM = R"(dialogue test { (NewItem, "cake") })";
    Parser parser{PROGRAM, std::strlen(PROGRAM)};
    auto ast = parser.GenerateAST();

    auto cmd = ast->dialogues->dialogue->content->command;
    auto publish = std::dynamic_pointer_cast<Publish>(cmd);
    EXPECT_NE(publish, nullptr);
    EXPECT_EQ(publish->name.value, 1);
    EXPECT_EQ(publish->params.size(), 1);
    EXPECT_EQ(publish->params[0]->value.value, 0);
  }
}

TEST(Grammar, IfElseTest) {
  {
    const char* PROGRAM = "dialogue test { if ($try) {} }";
    Parser parser{PROGRAM, std::strlen(PROGRAM)};
    auto ast = parser.GenerateAST();

    auto cmd = ast->dialogues->dialogue->content->command;
    auto if_cmd = std::dynamic_pointer_cast<If>(cmd);
    EXPECT_NE(if_cmd, nullptr);
    EXPECT_EQ(if_cmd->condition->value.value, 1);
    EXPECT_NE(if_cmd->true_block, nullptr);
    EXPECT_EQ(if_cmd->false_block, nullptr);
    EXPECT_EQ(if_cmd->another, nullptr);
  }

  {
    const char* PROGRAM = "dialogue test { if ($try) {} else {} }";
    Parser parser{PROGRAM, std::strlen(PROGRAM)};
    auto ast = parser.GenerateAST();

    auto cmd = ast->dialogues->dialogue->content->command;
    auto if_cmd = std::dynamic_pointer_cast<If>(cmd);
    EXPECT_NE(if_cmd, nullptr);
    EXPECT_NE(if_cmd->true_block, nullptr);
    EXPECT_NE(if_cmd->false_block, nullptr);
    EXPECT_EQ(if_cmd->another, nullptr);
  }

  {
    const char* PROGRAM = "dialogue test { if ($try) {} else if ($try2) {} }";
    Parser parser{PROGRAM, std::strlen(PROGRAM)};
    auto ast = parser.GenerateAST();

    auto cmd = ast->dialogues->dialogue->content->command;
    auto if_cmd = std::dynamic_pointer_cast<If>(cmd);
    EXPECT_NE(if_cmd, nullptr);
    EXPECT_NE(if_cmd->true_block, nullptr);
    EXPECT_EQ(if_cmd->false_block, nullptr);
    EXPECT_NE(if_cmd->another, nullptr);

    auto another = if_cmd->another;
    EXPECT_EQ(another->condition->value.value, 2);
    EXPECT_NE(another->true_block, nullptr);
    EXPECT_EQ(another->false_block, nullptr);
    EXPECT_EQ(another->another, nullptr);
  }

  {
    const char* PROGRAM = "dialogue test { if ($try) {} else if ($try2) {} else {} }";
    Parser parser{PROGRAM, std::strlen(PROGRAM)};
    auto ast = parser.GenerateAST();

    auto cmd = ast->dialogues->dialogue->content->command;
    auto if_cmd = std::dynamic_pointer_cast<If>(cmd);
    EXPECT_NE(if_cmd, nullptr);
    EXPECT_NE(if_cmd->true_block, nullptr);
    EXPECT_EQ(if_cmd->false_block, nullptr);
    EXPECT_NE(if_cmd->another, nullptr);

    auto another = if_cmd->another;
    EXPECT_NE(another->true_block, nullptr);
    EXPECT_NE(another->false_block, nullptr);
    EXPECT_EQ(another->another, nullptr);
  }
}

TEST(Grammar, SelectTest) {
  {
    const char* PROGRAM = "dialogue test { select {\n"
                          "            \"cookie\": use GetCookie\n"
                          "            \"cake\": {goto GetCake}\n"
                          "        } }";
    Parser parser{PROGRAM, std::strlen(PROGRAM)};
    auto ast = parser.GenerateAST();

    auto cmd = ast->dialogues->dialogue->content->command;
    auto select = std::dynamic_pointer_cast<Select>(cmd);
    EXPECT_NE(select, nullptr);
    EXPECT_NE(select->option, nullptr);

    auto opt1 = select->option;
    auto opt2 = opt1->next;
    EXPECT_NE(opt2, nullptr);
    EXPECT_EQ(opt1->hint.value, 0);

    auto use = std::dynamic_pointer_cast<Use>(opt1->command);
    EXPECT_NE(use, nullptr);
    EXPECT_EQ(use->name.value, 1);

    auto gt = std::dynamic_pointer_cast<Goto>(opt2->res->command);
    EXPECT_NE(gt, nullptr);
    EXPECT_EQ(gt->name.value, 2);
  }
}

TEST(Grammar, TextTest) {
  {
    const char* PROGRAM = R"(dialogue test { "Hello!" })";
    Parser parser{PROGRAM, std::strlen(PROGRAM)};
    auto ast = parser.GenerateAST();

    auto cmd = ast->dialogues->dialogue->content->command;
    auto msg = std::dynamic_pointer_cast<Message>(cmd);
    EXPECT_NE(msg, nullptr);
    EXPECT_EQ(msg->str.value, 0);
  }

  {
    const char* PROGRAM = R"(dialogue test { [Alice]: "Hello!" })";
    Parser parser{PROGRAM, std::strlen(PROGRAM)};
    auto ast = parser.GenerateAST();

    auto cmd = ast->dialogues->dialogue->content->command;
    auto spk = std::dynamic_pointer_cast<Speak>(cmd);
    EXPECT_NE(spk, nullptr);
    EXPECT_EQ(spk->speaker->name.value, 1);
    EXPECT_EQ(spk->speaker->State.type, TokenType::TOKEN_EMPTY);
    EXPECT_EQ(spk->message->str.value, 0);
  }

  {
    const char* PROGRAM = R"(dialogue test { [Alice(happy)]: "Hello!" })";
    Parser parser{PROGRAM, std::strlen(PROGRAM)};
    auto ast = parser.GenerateAST();

    auto cmd = ast->dialogues->dialogue->content->command;
    auto spk = std::dynamic_pointer_cast<Speak>(cmd);
    EXPECT_NE(spk, nullptr);
    EXPECT_EQ(spk->speaker->name.value, 1);
    EXPECT_EQ(spk->speaker->State.value, 2);
  }

  {
    const char* PROGRAM = R"(dialogue test { [Alice]: {"Hello!"} })";
    Parser parser{PROGRAM, std::strlen(PROGRAM)};
    auto ast = parser.GenerateAST();

    auto cmd = ast->dialogues->dialogue->content->command;
    auto spk = std::dynamic_pointer_cast<Speak>(cmd);
    EXPECT_NE(spk, nullptr);
    auto msg = std::dynamic_pointer_cast<Message>(spk->content->command);
    EXPECT_NE(msg, nullptr);
  }
}
