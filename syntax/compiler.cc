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
#include <cstdio>

#include "grammar/parser.h"
#include "tables/symbol_table.h"
#include "tables/constant_table.h"
#include "tables/i18n_table.h"
#include "exceptions/lex_exceptions.h"
#include "exceptions/grammar_exceptions.h"

const char* Compiler::Compile(char* p_content, unsigned long p_length, const char* p_i18n_prefix) {
  try {
    Parser parser{p_content, p_length};
    auto program = parser.GenerateAST();

    GenerateI18NFiles(program, p_i18n_prefix);
    GenerateHeader();

    auto cur = program->dialogues;
    while (cur != nullptr) {
      auto d = cur->dialogue;
      auto translator = Translator{};
      translator.TranslateDialogue(d);
      m_size += translator.GetSize();

      switch (d->type.type) {
      case TokenType::TOKEN_EMPTY:
        m_other.push_back(translator);
        break;
      case TokenType::TOKEN_INTERACT:
        m_interact.push_back(translator);
        break;
      case TokenType::TOKEN_TRIGGER:
        m_trigger.push_back(translator);
        break;
      case TokenType::TOKEN_AUTO:
        m_auto.push_back(translator);
        break;
      default:
        // TODO: throw
        break;
      }

      cur = cur->next;
    }

    RecordGlobalAddress();
  }
  catch (...) {
    // TODO: throw
  }
}

void Compiler::Write(char* p_buffer) {
  for (char c : m_header) {
    *p_buffer = c;
    ++p_buffer;
  }

  WriteTranslators(m_auto, &p_buffer);
  WriteTranslators(m_trigger, &p_buffer);
  WriteTranslators(m_interact, &p_buffer);
  WriteTranslators(m_other, &p_buffer);
}

void Compiler::GenerateI18NFiles(const std::shared_ptr<Program>& p_program, const std::string& p_prefix) {
  auto i18n = p_program->i18n;
  if (i18n == nullptr) {
    return;
  }

  m_i18n = true;
  std::vector<std::string> languages;
  auto ct = ConstantTable::GetInstance();
  auto it = I18NTable::GetInstance();

  for (const auto& op : i18n->options) {
    languages.push_back(ct->Find(op.value));
  }

  std::sort(languages.begin(), languages.end());
  languages.erase(std::unique(languages.begin(), languages.end()), languages.end());

  FILE* fp = fopen((p_prefix + ".csv").c_str(), "w");
  if (fp == nullptr) {
    return;
  }

  fprintf(fp, "keys");
  for (const auto& lang: languages) {
    fprintf(fp, "\t%s", lang.c_str());
  }

  fputc('\n', fp);
  auto size = it->GetSize();
  for (int i = 0; i < size; ++i) {
    fprintf(fp, "%s%d", p_prefix.c_str(), i);
    for (int j = 0; j < languages.size(); ++j) {
      if (j == 0) {
        auto id = it->GetStringID(i);
        auto str = ct->Find(id);
        fprintf(fp, "\t%s", str.c_str());
        ct->Replace(id, p_prefix + std::to_string(i));
      }
      else {
        fprintf(fp, "\tTODO");
      }
    }

    fputc('\n', fp);
  }

  fclose(fp);
}

void Compiler::GenerateHeader() {
  // Magic Number
  m_header.push_back(static_cast<char>(0xAE));
  m_header.push_back(static_cast<char>(0x86));

  // Enable I18N
  if (m_i18n) {
    m_header.push_back(static_cast<char>(0xFF));
    m_header.push_back(static_cast<char>(0xFF));
  }
  else {
    m_header.push_back(static_cast<char>(0));
    m_header.push_back(static_cast<char>(0));
  }

  // Symbol Table
  {
    auto table = SymbolTable::GetInstance();
    auto size = table->GetSize();
    PushIntInHeader(size);

    for (int i = 0; i < size; ++i) {
      std::string entry = table->GetEntry(i);
      PushIntInHeader(i);
      PushStringInHead(entry);
    }
  }

  // String Table
  {
    auto table = ConstantTable::GetInstance();
    auto size = table->GetSize();
    PushIntInHeader(size);

    for (int i = 0; i < size; ++i) {
      std::string str = table->Find(i);
      PushIntInHeader(i);
      PushStringInHead(str);
    }
  }

  m_size += m_header.size();
}

void Compiler::PushStringInHead(const std::string& p_str) {
  PushIntInHeader(p_str.size());

  for (char c : p_str) {
    m_header.push_back(c);
  }
}

void Compiler::RecordGlobalAddress() {
  int now = m_header.size();
  for (const auto& t : m_auto) {
    int id = t.GetID();
    m_address[id] = now;
    now += t.GetSize();
  }

  for (const auto& t : m_trigger) {
    int id = t.GetID();
    m_address[id] = now;
    now += t.GetSize();
  }

  for (const auto& t : m_interact) {
    int id = t.GetID();
    m_address[id] = now;
    now += t.GetSize();
  }

  for (const auto& t : m_other) {
    int id = t.GetID();
    m_address[id] = now;
    now += t.GetSize();
  }
}

void Compiler::WriteTranslators(std::vector<Translator>& p_list, char** p_buffer) {
  WriteInt(p_list.size(), p_buffer);

  for (auto& trans : p_list) {
    auto cmd_list = trans.Access();
    WriteInt(cmd_list.size(), p_buffer);
    for (auto& cmd : cmd_list) {
      WriteInt(static_cast<int>(cmd.type), p_buffer);
      if (cmd.type == CommandType::USE || cmd.type == CommandType::GOTO) {
        cmd.parameters.front() = m_address[cmd.parameters.front()];
      }

      for (int p : cmd.parameters) {
        WriteInt(p, p_buffer);
      }
    }
  }
}
