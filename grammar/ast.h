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

/// @file ast.h

#ifndef SFTD_AST_H
#define SFTD_AST_H

#include <memory>
#include <vector>

#include "lex/token.h"

enum class OperatorType {
  EMPTY = 0, ADD, SUB, MUL, DIV, MOD, LESS, GREATER, LESS_EQUAL, GREATER_EQUAL, EQUAL, AND, OR, NOT, NEG
};

struct I18NOptions {
  std::vector<Token> options;
};

struct Expression {
  Token value = Token{TokenType::TOKEN_EMPTY};
  OperatorType op = OperatorType::EMPTY;
  std::shared_ptr<Expression> left;
  std::shared_ptr<Expression> right;
};

struct Command {
protected:
  virtual void foo() {}
};

struct Animate : public Command {
  Token name;
  std::shared_ptr<Expression> anim;
};

struct Sound : public Command {
  std::shared_ptr<Expression> effect;
};

struct Delay : public Command {
  std::shared_ptr<Expression> time;
};

struct Move : public Command{
  Token name;
  std::shared_ptr<Expression> x, y;
  std::shared_ptr<Expression> time;
};

struct Goto : public Command {
  Token name;
};

struct Use : public Command {
  Token name;
};

struct Content;

struct If : public Command {
  std::shared_ptr<Expression> condition;
  std::shared_ptr<Content> true_block;
  std::shared_ptr<Content> false_block;
  std::shared_ptr<If> another;
};

struct Option {
  Token hint;
  std::shared_ptr<Command> command;
  std::shared_ptr<Content> res;
  std::shared_ptr<Option> next;
};

struct Select : Command {
  std::shared_ptr<Option> option;
};

struct Assign : public Command {
  Token target;
  std::shared_ptr<Expression> expression;
};

struct Speaker {
  Token name;
  Token State = Token{TokenType::TOKEN_EMPTY};
};

struct Message : public Command {
  Token str;
  std::shared_ptr<Expression> time;
};

struct Speak : public Command {
  std::shared_ptr<Speaker> speaker;
  std::shared_ptr<Content> content;
  std::shared_ptr<Message> message;
};

struct Publish : public Command {
  Token name;
  std::vector<std::shared_ptr<Expression>> params;
};

struct Content {
  std::shared_ptr<Command> command;
  std::shared_ptr<Content> next;
};

struct Dialogue {
  Token type;
  Token name;
  std::shared_ptr<Expression> condition;
  std::shared_ptr<Content> content;
};

struct Dialogues {
  std::shared_ptr<Dialogue> dialogue;
  std::shared_ptr<Dialogues> next;
};

struct Program {
  std::shared_ptr<I18NOptions> i18n;
  std::shared_ptr<Dialogues> dialogues;
};

#endif // SFTD_AST_H
