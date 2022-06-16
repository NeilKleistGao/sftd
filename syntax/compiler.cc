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

/// @file compiler.cc

#include "compiler.h"

#include <algorithm>
#include <unordered_map>

#include "grammar/parser.h"
#include "tables/symbol_table.h"
#include "tables/constant_table.h"

const char* Compiler::Compile(char* p_content, unsigned long p_length, const char* p_i18n_prefix) {
  Parser parser{p_content, p_length};
  auto program = parser.GenerateAST();

  GenerateI18NFiles(program, p_i18n_prefix);
  GenerateHeader();
}

void Compiler::Write(char* p_buffer, unsigned long p_length) {

}

void Compiler::GenerateI18NFiles(const std::shared_ptr<Program>& p_program, const std::string& p_prefix) {
  auto i18n = p_program->i18n;
  if (i18n == nullptr) {
    return;
  }

  m_i18n = true;
  std::vector<std::string> languages;
  auto ct = ConstantTable::GetInstance();

  for (const auto& op : i18n->options) {
    languages.push_back(ct->Find(op.value));
  }

  std::sort(languages.begin(), languages.end());
  languages.erase(std::unique(languages.begin(), languages.end()), languages.end());
}

void Compiler::GenerateHeader() {

}