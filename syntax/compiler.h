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

/// @file compiler.h

#ifndef SFTD_COMPILER_H
#define SFTD_COMPILER_H

#include <vector>
#include <memory>

#include "command.h"
#include "grammar/ast.h"
#include "translator.h"

class Compiler {
public:
  Compiler() : m_size(0), m_i18n(false) {};

  const char* Compile(char* p_content, unsigned long p_length, const char* p_i18n_prefix);
  void Write(char* p_buffer);

  inline long GetSize() const {
    return m_size;
  }
private:
  long m_size;
  std::vector<char> m_header;

  std::vector<Translator> m_auto, m_trigger, m_interact, m_other;

  bool m_i18n;

  void GenerateI18NFiles(const std::shared_ptr<Program>& p_program, const std::string& p_prefix);
  void GenerateHeader();

  inline void PushIntInHeader(int p_i) {
    m_header.push_back(static_cast<char>((p_i >> 24) & 0xFF));
    m_header.push_back(static_cast<char>((p_i >> 16) & 0xFF));
    m_header.push_back(static_cast<char>((p_i >> 8) & 0xFF));
    m_header.push_back(static_cast<char>(p_i & 0xFF));
  }

  void PushStringInHead(const std::string& p_str);
};

#endif // SFTD_COMPILER_H
