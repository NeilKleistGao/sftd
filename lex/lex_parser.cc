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

/// @file lex_parser.cc

#include "lex_parser.h"

#include <cctype>
#include <string>

#include "tables/constant_table.h"

LexParser::LexParser(const char* p_buffer, unsigned int p_length) : m_begin(p_buffer) {
  m_current1 = m_current2 = const_cast<char*>(p_buffer);
  m_end = const_cast<char*>(p_buffer + p_length);
}

Token LexParser::GetNext() {
  while (HasNext() && std::isblank(*m_current2)) {
    ++m_current2; ++m_current1;
  }

  if (HasNext() && *m_current2 == '/' && *(m_current2 + 1) == '/') {
    while (HasNext() && *m_current2 != '\n') {
      ++m_current2; ++m_current1;
    }
  }

  if (!HasNext()) {
    Token eof{};
    eof.type = TokenType::TOKEN_EOF;
    return eof;
  }

  if (*m_current2 == '*') {
    return ParseString();
  }
  else if (std::isdigit(*m_current2)) {
    return ParseNumber();
  }
  else {
    return ParseOperator();
  }
}

Token LexParser::ParseString() {
  ++m_current2;
  std::string str;
  while (*m_current2 != '"') {
    if (*m_current2 == '\\') {
      ++m_current2;
    }

    str += *m_current2;
    ++m_current2;
  }

  m_current1 = m_current2++;



  Token token{};
  token.type = TokenType::TOKEN_STRING;
  token.value = ConstantTable::GetInstance()->Insert(str);
  return token;
}

Token LexParser::ParseNumber() {

}

Token LexParser::ParseOperator() {

}
