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

/// @file translator.cc

#include "translator.h"

#include "sugars/matcher.hpp"

void Translator::TranslateDialogue(const std::shared_ptr<Dialogue>& p_dialogue) {
  m_id = p_dialogue->name.value;

  if (p_dialogue->condition != nullptr) {
    auto res = TranslateExpress(p_dialogue->condition);
    auto when = ILCommand{CommandType::CONDITIONAL_JUMP};
    when.parameters.push_back(GetVariableType(res.type));
    when.parameters.push_back(res.value);
    when.parameters.push_back(0); // placeholder
    Push(std::move(when));
  }
  else {
    Push(ILCommand{CommandType::EMPTY});
  }

  auto& when = m_pool.back();

  Push(ILCommand{CommandType::START_DIALOGUE});
  TranslateContent(p_dialogue->content);

  if (p_dialogue->condition != nullptr) {
    when.parameters.back() = m_size;
  }

  Push(ILCommand{CommandType::END_DIALOGUE});
}

Token Translator::TranslateExpress(const std::shared_ptr<Expression>& p_exp) {
  if (p_exp->op == OperatorType::EMPTY) {
    return p_exp->value;
  }

  Token lhs{TokenType::TOKEN_EMPTY}, rhs{TokenType::TOKEN_EMPTY};
  if (p_exp->left != nullptr) {
    lhs = TranslateExpress(p_exp->left);
  }

  rhs = TranslateExpress(p_exp->right);
  ++m_temp_variable_count;
  Token res{TokenType::TOKEN_VARIABLE, -m_temp_variable_count};

  auto cmd = ILCommand{};
  switch (p_exp->op) {
  case OperatorType::ADD:
    cmd.type = CommandType::ADD;
    break;
  case OperatorType::SUB:
    cmd.type = CommandType::SUB;
    break;
  case OperatorType::MUL:
    cmd.type = CommandType::MUL;
    break;
  case OperatorType::DIV:
    cmd.type = CommandType::DIV;
    break;
  case OperatorType::MOD:
    cmd.type = CommandType::MOD;
    break;
  case OperatorType::EQUAL:
    cmd.type = CommandType::EQUAL;
    break;
  case OperatorType::AND:
    cmd.type = CommandType::AND;
    break;
  case OperatorType::OR:
    cmd.type = CommandType::OR;
    break;
  case OperatorType::NOT:
    cmd.type = CommandType::NOT;
    break;
  case OperatorType::NEG:
    cmd.type = CommandType::NEG;
    break;
  case OperatorType::LESS:
    cmd.type = CommandType::LESS;
    break;
  case OperatorType::GREATER:
    cmd.type = CommandType::GREATER;
    break;
  case OperatorType::LESS_EQUAL:
    cmd.type = CommandType::LESS_EQUAL;
    break;
  case OperatorType::GREATER_EQUAL:
    cmd.type = CommandType::GREATER_EQUAL;
    break;
  }

  cmd.parameters.push_back(res.value);
  if (lhs.type != TokenType::TOKEN_EMPTY) {
    cmd.parameters.push_back(GetVariableType(lhs.type));
    cmd.parameters.push_back(lhs.value);
  }

  cmd.parameters.push_back(GetVariableType(rhs.type));
  cmd.parameters.push_back(rhs.value);

  Push(std::move(cmd));
  return res;
}

void Translator::TranslateContent(const std::shared_ptr<Content>& p_content) {
  auto cur = p_content;
  while (cur != nullptr) {
    if (cur->command != nullptr) {
      TranslateCommand(cur->command);
    }

    cur = cur->next;
  }
}

void Translator::TranslateCommand(const std::shared_ptr<Command>& p_cmd) {
  Matcher<Command, void, std::shared_ptr<Command>>::Match(p_cmd)
      ->Case<Animate>()([this](const std::shared_ptr<Animate>& p_p) { TranslateAnimate(p_p); })
      ->Case<Sound>()([this](const std::shared_ptr<Sound>& p_p) { TranslateSound(p_p); })
      ->Case<Delay>()([this](const std::shared_ptr<Delay>& p_p) { TranslateDelay(p_p); })
      ->Case<Move>()([this](const std::shared_ptr<Move>& p_p) { TranslateMove(p_p); })
      ->Case<Goto>()([this](const std::shared_ptr<Goto>& p_p) { TranslateGoto(p_p); })
      ->Case<Use>()([this](const std::shared_ptr<Use>& p_p) { TranslateUse(p_p); })
      ->Case<If>()([this](const std::shared_ptr<If>& p_p) { TranslateIf(p_p); })
      ->Case<Select>()([this](const std::shared_ptr<Select>& p_p) { TranslateSelect(p_p->option); })
      ->Case<Assign>()([this](const std::shared_ptr<Assign>& p_p) { TranslateAssign(p_p); })
      ->Case<Message>()([this](const std::shared_ptr<Message>& p_p) { TranslateMessage(p_p); })
      ->Case<Publish>()([this](const std::shared_ptr<Publish>& p_p) { TranslatePublish(p_p); })
      ->Case<Speak>()([this](const std::shared_ptr<Speak>& p_p) { TranslateSpeak(p_p); })
      ->CaseDefault()([](const std::shared_ptr<Command>& p_cmd){ /*TODO: throw*/ });
}

void Translator::TranslateAnimate(const std::shared_ptr<Animate>& p_cmd) {
  auto res = ILCommand{CommandType::PLAY_ANIMATION};
  res.parameters.push_back(p_cmd->name.value);

  auto exp = TranslateExpress(p_cmd->anim);
  res.parameters.push_back(GetVariableType(exp.type));
  res.parameters.push_back(exp.value);
  Push(std::move(res));
}

void Translator::TranslateSound(const std::shared_ptr<Sound>& p_cmd) {
  auto res = ILCommand{CommandType::PLAY_SOUND};

  auto exp = TranslateExpress(p_cmd->effect);
  res.parameters.push_back(GetVariableType(exp.type));
  res.parameters.push_back(exp.value);
  Push(std::move(res));
}

void Translator::TranslateDelay(const std::shared_ptr<Delay>& p_cmd) {
  auto res = ILCommand{CommandType::DELAY};

  auto exp = TranslateExpress(p_cmd->time);
  res.parameters.push_back(GetVariableType(exp.type));
  res.parameters.push_back(exp.value);
  Push(std::move(res));
}

void Translator::TranslateMove(const std::shared_ptr<Move>& p_cmd) {
  auto res = ILCommand{CommandType::MOVE};
  res.parameters.push_back(p_cmd->name.value);

  auto exp = TranslateExpress(p_cmd->x);
  res.parameters.push_back(GetVariableType(exp.type));
  res.parameters.push_back(exp.value);

  exp = TranslateExpress(p_cmd->y);
  res.parameters.push_back(GetVariableType(exp.type));
  res.parameters.push_back(exp.value);

  exp = TranslateExpress(p_cmd->time);
  res.parameters.push_back(GetVariableType(exp.type));
  res.parameters.push_back(exp.value);

  Push(std::move(res));
}

void Translator::TranslateGoto(const std::shared_ptr<Goto>& p_cmd) {
  auto res = ILCommand{CommandType::GOTO};
  res.parameters.push_back(p_cmd->name.value);
  Push(std::move(res));
}

void Translator::TranslateUse(const std::shared_ptr<Use>& p_cmd) {
  auto res = ILCommand{CommandType::USE};
  res.parameters.push_back(p_cmd->name.value);
  Push(std::move(res));
}

void Translator::TranslateSelect(const std::shared_ptr<Option>& p_cmd) {
  if (p_cmd != nullptr) {
    auto res = ILCommand{CommandType::OPTION};
    res.parameters.push_back(p_cmd->hint.value);
    res.parameters.push_back(0); // placeholder
    Push(std::move(res));

    auto& op = m_pool.back();
    if (p_cmd->command != nullptr) {
      TranslateCommand(p_cmd->command);
    }
    else {
      TranslateContent(p_cmd->res);
    }

    op.parameters.back() = m_size + 4;
    TranslateSelect(p_cmd->next);
  }
}

void Translator::TranslateIf(const std::shared_ptr<If>& p_cmd) {
  auto res = ILCommand{CommandType::CONDITIONAL_JUMP};
  auto exp = TranslateExpress(p_cmd->condition);
  res.parameters.push_back(GetVariableType(exp.type));
  res.parameters.push_back(exp.value);
  res.parameters.push_back(0); // placeholder
  Push(std::move(res));

  auto& address = m_pool.back();
  TranslateContent(p_cmd->true_block);
  address.parameters.back() = m_size;
  TranslateContent(p_cmd->false_block);
}

void Translator::TranslateAssign(const std::shared_ptr<Assign>& p_cmd) {
  auto exp = TranslateExpress(p_cmd->expression);
  if (exp.type == TokenType::TOKEN_VARIABLE && exp.value < 0) {
    --m_temp_variable_count;
    m_pool.back().parameters.front() = p_cmd->target.value;
  }
  else {
    auto res = ILCommand{CommandType::SET_DATA};
    res.parameters.push_back(GetVariableType(exp.type));
    res.parameters.push_back(exp.value);
    Push(std::move(res));
  }
}

void Translator::TranslateMessage(const std::shared_ptr<Message>& p_cmd) {
  if (p_cmd->time == nullptr) {
    auto res = ILCommand{CommandType::TALK};
    res.parameters.push_back(p_cmd->str.value);
    Push(std::move(res));
  }
  else {
    auto res = ILCommand{CommandType::TALK_IN_TIME};
    res.parameters.push_back(p_cmd->str.value);
    auto exp = TranslateExpress(p_cmd->time);
    res.parameters.push_back(GetVariableType(exp.type));
    res.parameters.push_back(exp.value);
    Push(std::move(res));
  }
}

void Translator::TranslateSpeak(const std::shared_ptr<Speak>& p_cmd) {
  auto speaker = p_cmd->speaker;
  if (speaker->State.type == TokenType::TOKEN_EMPTY) {
    auto res = ILCommand{CommandType::SET_SPEAKER};
    res.parameters.push_back(speaker->name.value);
    Push(std::move(res));
  }
  else {
    auto res = ILCommand{CommandType::SET_SPEAKER_WITH_STATE};
    res.parameters.push_back(speaker->name.value);
    res.parameters.push_back(speaker->State.value);
    Push(std::move(res));
  }

  if (p_cmd->message != nullptr) {
    TranslateMessage(p_cmd->message);
  }
  else {
    TranslateContent(p_cmd->content);
  }
}

void Translator::TranslatePublish(const std::shared_ptr<Publish>& p_cmd) {
  auto res = ILCommand{CommandType::PUBLISH};
  res.parameters.push_back(p_cmd->name.value);
  res.parameters.push_back(p_cmd->params.size());
  for (const auto& p : p_cmd->params) {
    auto exp = TranslateExpress(p);
    res.parameters.push_back(GetVariableType(exp.type));
    res.parameters.push_back(exp.value);
  }

  Push(std::move(res));
}

int Translator::GetVariableType(TokenType p_type) {
  int res;
  switch (p_type) {
  case TokenType::TOKEN_VARIABLE:
    res = 0;
    break;
  case TokenType::TOKEN_STRING:
    res = 1;
    break;
  case TokenType::TOKEN_INT:
    res = 2;
    break;
  case TokenType::TOKEN_FLOAT:
    res = 3;
    break;
  case TokenType::TOKEN_BOOL:
    res = 4;
    break;
  default:
    // TODO: throw
    break;
  }

  return res;
}
