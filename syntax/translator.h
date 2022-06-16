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
  explicit Translator() = default;
  ~Translator() = default;
  Translator(const Translator&) = delete;
  Translator& operator=(const Translator&) = delete;
  Translator(Translator&&) = delete;
  Translator& operator=(Translator&&) = delete;

  std::vector<ILCommand>& TranslateDialogue(const std::shared_ptr<Dialogue>& p_dialogue);
private:
  std::vector<ILCommand> m_pool;
};

#endif // SFTD_TRANSLATOR_H
