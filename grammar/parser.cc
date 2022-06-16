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

/// @file parser.cc

#include "parser.h"

#include "tables/constant_table.h"
#include "tables/symbol_table.h"

Parser::Parser(const char* p_buffer, unsigned long p_length) : m_lex(p_buffer, p_length) {
  ConstantTable::GetInstance()->Clear();
  SymbolTable::GetInstance()->Clear();
}

std::shared_ptr<Program> Parser::GenerateAST() {
  auto ast = std::make_shared<Program>();
  Token head = m_lex.LookNext();
  if (head.type == TokenType::TOKEN_SHARP) {
    m_lex.GetNext();
    ast->i18n = ParseI18NInfo();
  }

  ast->dialogues = ParseDialogues();
  return ast;
}

std::shared_ptr<I18NOptions> Parser::ParseI18NInfo() {
  Token tk = m_lex.GetNext();
  if (tk.type != TokenType::TOKEN_LEFT_CURLY) {
    // TODO: throw
  }

  auto options = std::make_shared<I18NOptions>();
  while (true) {
    tk = m_lex.GetNext();
    if (tk.type == TokenType::TOKEN_STRING) {
      options->options.push_back(tk);
      tk = m_lex.GetNext();
      if (tk.type == TokenType::TOKEN_RIGHT_CURLY) {
        break;
      }
      else if (tk.type != TokenType::TOKEN_COMMA) {
        // TODO: throw
      }
    }
    else {
      // TODO: throw
    }
  }

  return options;
}

std::shared_ptr<Dialogues> Parser::ParseDialogues() {
  if (!m_lex.HasNext()) {
    return nullptr;
  }

  Token head = m_lex.GetNext();
  auto dialogues = std::make_shared<Dialogues>();

  Token type = Token{TokenType::TOKEN_EMPTY};
  if (head.type == TokenType::TOKEN_AUTO || head.type == TokenType::TOKEN_TRIGGER || head.type == TokenType::TOKEN_INTERACT) {
    type = head;
    head = m_lex.GetNext();
  }

  if (head.type == TokenType::TOKEN_DIALOGUE) {
    dialogues->dialogue = ParseDialogue();
  }
  else {
    // TODO: throw
  }

  dialogues->dialogue->type = type;
  if (m_lex.HasNext()) {
    dialogues->next = ParseDialogues();
  }

  return dialogues;
}

std::shared_ptr<Dialogue> Parser::ParseDialogue() {
  auto dialogue = std::make_shared<Dialogue>();
  Token tk = m_lex.GetNext();

  if (tk.type == TokenType::TOKEN_VARIABLE) {
    dialogue->name = tk;
  }
  else {
    //TODO: throw
  }

  tk = m_lex.GetNext();
  if (tk.type == TokenType::TOKEN_WHEN) {
    tk = m_lex.GetNext();
    if (tk.type != TokenType::TOKEN_LEFT_PARENTHESES) {
      // TODO: throw
    }

    dialogue->condition = ParseExpression0();

    tk = m_lex.GetNext();
    if (tk.type != TokenType::TOKEN_RIGHT_PARENTHESES) {
      // TODO: throw
    }

    tk = m_lex.GetNext();
  }

  if (tk.type != TokenType::TOKEN_LEFT_CURLY) {
    // TODO: throw
  }

  dialogue->content = ParseContent();

  tk = m_lex.GetNext();
  if (tk.type != TokenType::TOKEN_RIGHT_CURLY) {
    // TODO: throw
  }

  return dialogue;
}

std::shared_ptr<Expression> Parser::ParseExpression0() {
  auto exp = std::make_shared<Expression>();
  exp->left = ParseExpression1();

  Token tk = m_lex.LookNext();
  if (tk.type == TokenType::TOKEN_OR) {
    tk = m_lex.GetNext();
    exp->op = OperatorType::OR;
    exp->right = ParseExpression1();
  }
  else {
    exp = exp->left;
  }

  tk = m_lex.LookNext();
  if (tk.type == TokenType::TOKEN_OR) {
    auto p = std::make_shared<Expression>();
    p->left = exp;
    tk = m_lex.GetNext();
    p->op = OperatorType::OR;
    p->right = ParseExpression0();
    return p;
  }

  return exp;
}

std::shared_ptr<Expression> Parser::ParseExpression1() {
  auto exp = std::make_shared<Expression>();
  exp->left = ParseExpression2();

  Token tk = m_lex.LookNext();
  if (tk.type == TokenType::TOKEN_AND) {
    tk = m_lex.GetNext();
    exp->op = OperatorType::AND;
    exp->right = ParseExpression2();
  }
  else {
    exp = exp->left;
  }

  tk = m_lex.LookNext();
  if (tk.type == TokenType::TOKEN_AND) {
    auto p = std::make_shared<Expression>();
    p->left = exp;
    tk = m_lex.GetNext();
    p->op = OperatorType::AND;
    p->right = ParseExpression1();
    return p;
  }

  return exp;
}

std::shared_ptr<Expression> Parser::ParseExpression2() {
  auto exp = std::make_shared<Expression>();
  exp->left = ParseExpression3();

  Token tk = m_lex.LookNext();
  if (tk.type == TokenType::TOKEN_EQUAL) {
    tk = m_lex.GetNext();
    exp->op = OperatorType::EQUAL;
    tk = m_lex.GetNext();
    if (tk.type != TokenType::TOKEN_EQUAL) {
      // TODO: throw
    }

    exp->right = ParseExpression3();
  }
  else {
    exp = exp->left;
  }

  tk = m_lex.LookNext();
  if (tk.type == TokenType::TOKEN_EQUAL) {
    tk = m_lex.GetNext();
    if (tk.type != TokenType::TOKEN_EQUAL) {
      // TODO: throw
    }

    auto p = std::make_shared<Expression>();
    p->left = exp;
    tk = m_lex.GetNext();
    p->op = OperatorType::EQUAL;
    p->right = ParseExpression2();
    return p;
  }

  return exp;
}

std::shared_ptr<Expression> Parser::ParseExpression3() {
  auto exp = std::make_shared<Expression>();
  exp->left = ParseExpression4();

  Token tk = m_lex.LookNext();
  if (tk.type == TokenType::TOKEN_LESS || tk.type == TokenType::TOKEN_GREATER) {
    tk = m_lex.GetNext();
    auto eq = m_lex.LookNext();
    if (eq.type == TokenType::TOKEN_EQUAL) {
      m_lex.GetNext();
      if (tk.type == TokenType::TOKEN_LESS) {
        exp->op = OperatorType::LESS_EQUAL;
      }
      else {
        exp->op = OperatorType::GREATER_EQUAL;
      }
    }
    else {
      if (tk.type == TokenType::TOKEN_LESS) {
        exp->op = OperatorType::LESS;
      }
      else {
        exp->op = OperatorType::GREATER;
      }
    }

    exp->right = ParseExpression4();
  }
  else {
    exp = exp->left;
  }

  tk = m_lex.LookNext();
  if (tk.type == TokenType::TOKEN_LESS || tk.type == TokenType::TOKEN_GREATER) {
    auto p = std::make_shared<Expression>();
    p->left = exp;
    tk = m_lex.GetNext();
    auto eq = m_lex.LookNext();
    if (eq.type == TokenType::TOKEN_EQUAL) {
      m_lex.GetNext();
      if (tk.type == TokenType::TOKEN_LESS) {
        p->op = OperatorType::LESS_EQUAL;
      }
      else {
        p->op = OperatorType::GREATER_EQUAL;
      }
    }
    else {
      if (tk.type == TokenType::TOKEN_LESS) {
        p->op = OperatorType::LESS;
      }
      else {
        p->op = OperatorType::GREATER;
      }
    }

    p->right = ParseExpression3();
    return p;
  }

  return exp;
}

std::shared_ptr<Expression> Parser::ParseExpression4() {
  auto exp = std::make_shared<Expression>();
  exp->left = ParseExpression5();

  Token tk = m_lex.LookNext();
  if (tk.type == TokenType::TOKEN_ADD || tk.type == TokenType::TOKEN_SUB) {
    tk = m_lex.GetNext();
    exp->op = (tk.type == TokenType::TOKEN_ADD) ? OperatorType::ADD : OperatorType::SUB;
    exp->right = ParseExpression5();
  }
  else {
    exp = exp->left;
  }

  tk = m_lex.LookNext();
  if (tk.type == TokenType::TOKEN_ADD || tk.type == TokenType::TOKEN_SUB) {
    auto p = std::make_shared<Expression>();
    p->left = exp;
    tk = m_lex.GetNext();
    p->op = (tk.type == TokenType::TOKEN_ADD) ? OperatorType::ADD : OperatorType::SUB;
    p->right = ParseExpression4();
    return p;
  }

  return exp;
}

std::shared_ptr<Expression> Parser::ParseExpression5() {
  auto exp = std::make_shared<Expression>();
  exp->left = ParseExpression6();

  Token tk = m_lex.LookNext();
  if (tk.type == TokenType::TOKEN_MUL || tk.type == TokenType::TOKEN_DIV || tk.type == TokenType::TOKEN_MOD) {
    tk = m_lex.GetNext();
    exp->op = (tk.type == TokenType::TOKEN_MUL) ? OperatorType::MUL : (tk.type == TokenType::TOKEN_DIV) ? OperatorType::DIV : OperatorType::MOD;
    exp->right = ParseExpression6();
  }
  else {
    exp = exp->left;
  }

  tk = m_lex.LookNext();
  if (tk.type == TokenType::TOKEN_MUL || tk.type == TokenType::TOKEN_DIV || tk.type == TokenType::TOKEN_MOD) {
    auto p = std::make_shared<Expression>();
    p->left = exp;
    tk = m_lex.GetNext();
    p->op = (tk.type == TokenType::TOKEN_MUL) ? OperatorType::MUL : (tk.type == TokenType::TOKEN_DIV) ? OperatorType::DIV : OperatorType::MOD;
    p->right = ParseExpression4();
    return p;
  }

  return exp;
}

std::shared_ptr<Expression> Parser::ParseExpression6() {
  auto exp = std::make_shared<Expression>();

  Token tk = m_lex.LookNext();
  if (tk.type == TokenType::TOKEN_NOT) {
    tk = m_lex.GetNext();
    exp->op = OperatorType::NOT;
    exp->right = ParseExpression7();
  }
  else {
    exp = ParseExpression7();
  }

  return exp;
}

std::shared_ptr<Expression> Parser::ParseExpression7() {
  auto exp = std::make_shared<Expression>();

  Token tk = m_lex.LookNext();
  if (tk.type == TokenType::TOKEN_SUB) {
    tk = m_lex.GetNext();
    exp->op = OperatorType::NEG;
    exp->right = ParseExpression8();
  }
  else {
    exp = ParseExpression8();
  }

  return exp;
}

std::shared_ptr<Expression> Parser::ParseExpression8() {
  auto res = std::make_shared<Expression>();
  Token tk = m_lex.GetNext();

  switch (tk.type) {
  case TokenType::TOKEN_INT:
  case TokenType::TOKEN_FLOAT:
  case TokenType::TOKEN_STRING:
  case TokenType::TOKEN_BOOL:
    res->value = tk;
    break;
  case TokenType::TOKEN_DOLLAR: {
    tk = m_lex.GetNext();
    if (tk.type != TokenType::TOKEN_VARIABLE) {
      // TODO: throw
    }

    res->value = tk;
    break;
  }
  case TokenType::TOKEN_LEFT_PARENTHESES: {
    res = ParseExpression0();
    tk = m_lex.GetNext();
    if (tk.type != TokenType::TOKEN_RIGHT_PARENTHESES) {
      // TODO: throw
    }
    break;
  }
  default:
    // TODO: throw
    break;
  }

  return res;
}

std::shared_ptr<Content> Parser::ParseContent() {
  auto content = std::make_shared<Content>();
  content->command = ParseCommand();

  Token tk = m_lex.LookNext();
  if (tk.type != TokenType::TOKEN_RIGHT_CURLY) {
    content->next = ParseContent();
  }

  return content;
}

std::shared_ptr<Command> Parser::ParseCommand() {
  Token tk = m_lex.LookNext();

  switch (tk.type) {
  case TokenType::TOKEN_ANIMATE:
    return ParseAnimate();
    break;
  case TokenType::TOKEN_SOUND:
    return ParseSound();
    break;
  case TokenType::TOKEN_MOVE:
    return ParseMove();
    break;
  case TokenType::TOKEN_DELAY:
    return ParseDelay();
    break;
  case TokenType::TOKEN_IF:
    return ParseIf();
    break;
  case TokenType::TOKEN_SELECT:
    return ParseSelect();
    break;
  case TokenType::TOKEN_GOTO:
    return ParseGoto();
    break;
  case TokenType::TOKEN_USE:
    return ParseUse();
    break;
  case TokenType::TOKEN_STRING:
    return ParseMessage();
    break;
  case TokenType::TOKEN_LEFT_SQUARE:
    return ParseSpeak();
    break;
  case TokenType::TOKEN_DOLLAR:
    return ParseAssign();
    break;
  case TokenType::TOKEN_LESS:
    return ParsePublish();
  default:
    // TODO: throw
    break;
  }
}

std::shared_ptr<Animate> Parser::ParseAnimate() {
  m_lex.GetNext();
  auto anim = std::make_shared<Animate>();

  Token tk = m_lex.GetNext();
  if (tk.type != TokenType::TOKEN_AT) {
    // TODO: throw
  }

  tk = m_lex.GetNext();
  if (tk.type != TokenType::TOKEN_VARIABLE) {
    // TODO: throw
  }

  anim->name = tk;
  anim->anim = ParseExpression0();
  return anim;
}

std::shared_ptr<Sound> Parser::ParseSound() {
  m_lex.GetNext();
  auto sound = std::make_shared<Sound>();

  sound->effect = ParseExpression0();
  return sound;
}

std::shared_ptr<Move> Parser::ParseMove() {
  m_lex.GetNext();
  auto move = std::make_shared<Move>();

  Token tk = m_lex.GetNext();
  if (tk.type != TokenType::TOKEN_AT) {
    // TODO: throw
  }

  tk = m_lex.GetNext();
  if (tk.type != TokenType::TOKEN_VARIABLE) {
    // TODO: throw
  }

  move->name = tk;

  tk = m_lex.GetNext();
  if (tk.type != TokenType::TOKEN_LEFT_PARENTHESES) {
    // TODO: throw
  }

  move->x = ParseExpression0();

  tk = m_lex.GetNext();
  if (tk.type != TokenType::TOKEN_COMMA) {
    // TODO: throw
  }+

  move->y = ParseExpression0();

  tk = m_lex.GetNext();
  if (tk.type != TokenType::TOKEN_RIGHT_PARENTHESES) {
    // TODO: throw
  }

  tk = m_lex.GetNext();
  if (tk.type != TokenType::TOKEN_IN) {
    // TODO: throw
  }

  move->time = ParseExpression0();

  return move;
}

std::shared_ptr<Delay> Parser::ParseDelay() {
  m_lex.GetNext();
  auto delay = std::make_shared<Delay>();
  delay->time = ParseExpression0();
  return delay;
}

std::shared_ptr<If> Parser::ParseIf() {
  m_lex.GetNext();
  auto res = std::make_shared<If>();

  Token tk = m_lex.GetNext();
  if (tk.type != TokenType::TOKEN_LEFT_PARENTHESES) {
    // TODO: throw
  }

  res->condition = ParseExpression0();
  tk = m_lex.GetNext();
  if (tk.type != TokenType::TOKEN_RIGHT_PARENTHESES) {
    // TODO: throw
  }

  tk = m_lex.GetNext();
  if (tk.type != TokenType::TOKEN_LEFT_CURLY) {
    // TODO: throw
  }

  res->true_block = ParseContent();

  tk = m_lex.GetNext();
  if (tk.type != TokenType::TOKEN_RIGHT_CURLY) {
    // TODO: throw
  }

  tk = m_lex.LookNext();
  if (tk.type == TokenType::TOKEN_ELSE) {
    m_lex.GetNext();
    tk = m_lex.LookNext();
    if (tk.type == TokenType::TOKEN_IF) {
      res->another = ParseIf();
    }
    else if (tk.type == TokenType::TOKEN_LEFT_CURLY) {
      m_lex.GetNext();
      res->false_block = ParseContent();
      tk = m_lex.GetNext();
      if (tk.type != TokenType::TOKEN_RIGHT_CURLY) {
        // TODO: throw
      }
    }
    else {
      // TODO: throw
    }
  }

  return res;
}

std::shared_ptr<Select> Parser::ParseSelect() {
  m_lex.GetNext();
  auto res = std::make_shared<Select>();

  Token tk = m_lex.GetNext();
  if (tk.type != TokenType::TOKEN_LEFT_PARENTHESES) {
    // TODO: throw
  }

  res->option = ParseOption();

  tk = m_lex.LookNext();
  if (tk.type != TokenType::TOKEN_RIGHT_PARENTHESES) {
    res->next = ParseSelect();
  }

  m_lex.GetNext();
  return res;
}

std::shared_ptr<Goto> Parser::ParseGoto() {
  m_lex.GetNext();
  auto res = std::make_shared<Goto>();

  Token tk = m_lex.GetNext();
  if (tk.type != TokenType::TOKEN_VARIABLE) {
    // TODO: throw
  }

  res->name = tk;
  return res;
}

std::shared_ptr<Use> Parser::ParseUse() {
  m_lex.GetNext();
  auto res = std::make_shared<Use>();

  Token tk = m_lex.GetNext();
  if (tk.type != TokenType::TOKEN_VARIABLE) {
    // TODO: throw
  }

  res->name = tk;
  return res;
}

std::shared_ptr<Message> Parser::ParseMessage() {
  m_lex.GetNext();
  auto res = std::make_shared<Message>();

  Token tk = m_lex.GetNext();
  if (tk.type != TokenType::TOKEN_STRING) {
    // TODO: throw
  }

  res->str = tk;
  tk = m_lex.LookNext();
  if (tk.type == TokenType::TOKEN_IN) {
    m_lex.GetNext();
    res->time = ParseExpression0();
  }

  return res;
}

std::shared_ptr<Speak> Parser::ParseSpeak() {
  auto res = std::make_shared<Speak>();
  res->speaker = ParseSpeaker();

  Token tk = m_lex.LookNext();
  if (tk.type == TokenType::TOKEN_LEFT_CURLY) {
    m_lex.GetNext();
    res->content = ParseContent();

    tk = m_lex.GetNext();
    if (tk.type != TokenType::TOKEN_RIGHT_CURLY) {
      // TODO: throw
    }
  }
  else {
    res->message = ParseMessage();
  }

  return res;
}

std::shared_ptr<Assign> Parser::ParseAssign() {
  m_lex.GetNext();
  auto res = std::make_shared<Assign>();
  res->target = m_lex.GetNext();

  Token tk = m_lex.GetNext();
  if (tk.type != TokenType::TOKEN_EQUAL) {
    // TODO: throw
  }

  res->expression = ParseExpression0();
  return res;
}

std::shared_ptr<Publish> Parser::ParsePublish() {
  m_lex.GetNext();
  auto res = std::make_shared<Publish>();

  Token tk = m_lex.GetNext();
  if (tk.type != TokenType::TOKEN_VARIABLE) {
    // TODO: throw
  }

  res->name = tk;
  tk = m_lex.LookNext();
  if (tk.type == TokenType::TOKEN_COMMA) {
    m_lex.GetNext();
    res->p1 = ParseExpression0();

    tk = m_lex.LookNext();
    if (tk.type == TokenType::TOKEN_COMMA) {
      m_lex.GetNext();
      res->p2 = ParseExpression0();
    }
  }

  tk = m_lex.GetNext();
  if (tk.type != TokenType::TOKEN_GREATER) {
    // TODO: throw
  }

  return res;
}

std::shared_ptr<Option> Parser::ParseOption() {
  auto res = std::make_shared<Option>();
  Token tk = m_lex.GetNext();

  if (tk.type != TokenType::TOKEN_STRING) {
    // TODO: throw
  }

  res->hint = tk;

  tk = m_lex.LookNext();
  if (tk.type == TokenType::TOKEN_LEFT_CURLY) {
    m_lex.GetNext();
    res->res = ParseContent();
    tk = m_lex.GetNext();
    if (tk.type != TokenType::TOKEN_RIGHT_CURLY) {
      // TODO: throw
    }
  }
  else {
    res->command = ParseCommand();
  }
}

std::shared_ptr<Speaker> Parser::ParseSpeaker() {
  m_lex.GetNext();
  auto res = std::make_shared<Speaker>();

  Token tk = m_lex.GetNext();
  if (tk.type != TokenType::TOKEN_VARIABLE) {
    // TODO: throw
  }

  res->name = tk;
  tk = m_lex.LookNext();
  if (tk.type == TokenType::TOKEN_LEFT_PARENTHESES) {
    m_lex.GetNext();
    tk = m_lex.GetNext();
    if (tk.type != TokenType::TOKEN_VARIABLE) {
      // TODO: throw
    }

    res->State = tk;
  }

  tk = m_lex.GetNext();
  if (tk.type != TokenType::TOKEN_RIGHT_SQUARE) {
    // TODO: throw
  }

  return res;
}
