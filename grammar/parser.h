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

/// @file parser.h

#ifndef SFTD_PARSER_H
#define SFTD_PARSER_H

#include <memory>

#include "lex/lex_parser.h"
#include "ast.h"

class Parser {
public:
  Parser(const char* p_buffer, unsigned long p_length);
  ~Parser() = default;

  std::shared_ptr<Program> GenerateAST();
private:
  LexParser m_lex;

  std::shared_ptr<I18NOptions> ParseI18NInfo();
  std::shared_ptr<Dialogues> ParseDialogues();
  std::shared_ptr<Dialogue> ParseDialogue();
  std::shared_ptr<Expression> ParseExpression0(); // or
  std::shared_ptr<Expression> ParseExpression1(); // and
  std::shared_ptr<Expression> ParseExpression2(); // ==
  std::shared_ptr<Expression> ParseExpression3(); // > < >= <=
  std::shared_ptr<Expression> ParseExpression4(); // + -
  std::shared_ptr<Expression> ParseExpression5(); // * / %
  std::shared_ptr<Expression> ParseExpression6(); // not
  std::shared_ptr<Expression> ParseExpression7(); // -
  std::shared_ptr<Expression> ParseExpression8(); // ()
  std::shared_ptr<Content> ParseContent();
  std::shared_ptr<Command> ParseCommand();
  std::shared_ptr<Animate> ParseAnimate();
  std::shared_ptr<Sound> ParseSound();
  std::shared_ptr<Move> ParseMove();
  std::shared_ptr<Delay> ParseDelay();
  std::shared_ptr<If> ParseIf();
  std::shared_ptr<Select> ParseSelect();
  std::shared_ptr<Goto> ParseGoto();
  std::shared_ptr<Use> ParseUse();
  std::shared_ptr<Message> ParseMessage();
  std::shared_ptr<Speak> ParseSpeak();
  std::shared_ptr<Assign> ParseAssign();
  std::shared_ptr<Publish> ParsePublish();
  std::shared_ptr<Option> ParseOption();
  std::shared_ptr<Speaker> ParseSpeaker();
};

#endif // SFTD_PARSER_H
