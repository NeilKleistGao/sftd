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

/// @file lex_parser.h

#ifndef SFTD_LEX_PARSER_H
#define SFTD_LEX_PARSER_H

#include <unordered_map>
#include <optional>

#include "token.h"

class LexParser {
public:
  LexParser(const char* p_buffer, unsigned long p_length);
  ~LexParser() = default;

  Token GetNext();
  Token LookNext();

  inline bool HasNext() const {
    return m_current != m_end;
  }

private:
  char* m_current;
  char* m_end;

  int m_line;

  std::unordered_map<std::string, Token> m_keywords;
  std::optional<Token> m_next;

  Token ParseString();
  Token ParseNumber();
  Token ParseOperator();
  Token ParseKeywords();
};

#endif // SFTD_LEX_PARSER_H
