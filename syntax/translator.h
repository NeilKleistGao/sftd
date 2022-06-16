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

/// @file translator.h

#ifndef SFTD_TRANSLATOR_H
#define SFTD_TRANSLATOR_H

#include <memory>
#include <unordered_set>

#include "grammar/ast.h"
#include "command.h"

class Translator {
public:
  Translator() : m_temp_variable_count(0) { };
  ~Translator() = default;

  void TranslateDialogue(const std::shared_ptr<Dialogue>& p_dialogue);

  inline std::vector<ILCommand>& Access() {
    return m_pool;
  }

  inline int GetSize() const {
    return m_size;
  }

private:
  std::vector<ILCommand> m_pool;
  int m_temp_variable_count;
  int m_size;

  Token TranslateExpress(const std::shared_ptr<Expression>& p_exp);
  void TranslateContent(const std::shared_ptr<Content>& p_content);
  void TranslateCommand(const std::shared_ptr<Command>& p_cmd);
  void TranslateAnimate(const std::shared_ptr<Animate>& p_cmd);
  void TranslateSound(const std::shared_ptr<Sound>& p_cmd);
  void TranslateDelay(const std::shared_ptr<Delay>& p_cmd);
  void TranslateMove(const std::shared_ptr<Move>& p_cmd);
  void TranslateGoto(const std::shared_ptr<Goto>& p_cmd);
  void TranslateUse(const std::shared_ptr<Use>& p_cmd);
  void TranslateSelect(const std::shared_ptr<Select>& p_cmd);
  void TranslateIf(const std::shared_ptr<If>& p_cmd);
  void TranslateAssign(const std::shared_ptr<Assign>& p_cmd);
  void TranslateMessage(const std::shared_ptr<Message>& p_cmd);
  void TranslateSpeak(const std::shared_ptr<Speak>& p_cmd);
  void TranslatePublish(const std::shared_ptr<Publish>& p_cmd);

  static int GetVariableType(TokenType p_type);

  inline void Push(ILCommand&& p_cmd) {
    m_size += 4 * static_cast<int>(p_cmd.parameters.size());
    m_pool.push_back(p_cmd);
  }
};

#endif // SFTD_TRANSLATOR_H
