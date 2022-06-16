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
#include "tables/symbol_table.h"
#include "exceptions/lex_exceptions.h"

LexParser::LexParser(const char* p_buffer, unsigned long p_length) : m_line(1) {
  m_current = const_cast<char*>(p_buffer);
  m_end = const_cast<char*>(p_buffer + p_length);

  m_keywords.insert(std::make_pair("dialogue", Token{TokenType::TOKEN_DIALOGUE}));
  m_keywords.insert(std::make_pair("when", Token{TokenType::TOKEN_WHEN}));
  m_keywords.insert(std::make_pair("select", Token{TokenType::TOKEN_SELECT}));
  m_keywords.insert(std::make_pair("if", Token{TokenType::TOKEN_IF}));
  m_keywords.insert(std::make_pair("else", Token{TokenType::TOKEN_ELSE}));
  m_keywords.insert(std::make_pair("goto", Token{TokenType::TOKEN_GOTO}));
  m_keywords.insert(std::make_pair("use", Token{TokenType::TOKEN_USE}));
  m_keywords.insert(std::make_pair("in", Token{TokenType::TOKEN_IN}));
  m_keywords.insert(std::make_pair("null", Token{TokenType::TOKEN_NULL}));
  m_keywords.insert(std::make_pair("and", Token{TokenType::TOKEN_AND}));
  m_keywords.insert(std::make_pair("or", Token{TokenType::TOKEN_OR}));
  m_keywords.insert(std::make_pair("not", Token{TokenType::TOKEN_NOT}));
  m_keywords.insert(std::make_pair("true", Token{TokenType::TOKEN_BOOL, 1}));
  m_keywords.insert(std::make_pair("false", Token{TokenType::TOKEN_BOOL, 0}));
  m_keywords.insert(std::make_pair("sound", Token{TokenType::TOKEN_SOUND}));
  m_keywords.insert(std::make_pair("move", Token{TokenType::TOKEN_MOVE}));
  m_keywords.insert(std::make_pair("animate", Token{TokenType::TOKEN_ANIMATE}));
  m_keywords.insert(std::make_pair("delay", Token{TokenType::TOKEN_DELAY}));
  m_keywords.insert(std::make_pair("interact", Token{TokenType::TOKEN_INTERACT}));
  m_keywords.insert(std::make_pair("trigger", Token{TokenType::TOKEN_TRIGGER}));
  m_keywords.insert(std::make_pair("auto", Token{TokenType::TOKEN_AUTO}));
}

Token LexParser::GetNext() {
  Token res;
  if (!m_next.has_value()) {
    LookNext();
  }

  res = m_next.value();
  m_next.reset();
  return res;
}

Token LexParser::ParseString() {
  ++m_current;
  std::string str;
  Token token {TokenType::TOKEN_STRING};
  while (HasNext() && *m_current != '"' && *m_current != '\n') {
    if (*m_current == '\\') {
      ++m_current;
      if (!HasNext()) {
        throw UnexpectedEndOfString{m_line};
      }

      if (*m_current == 'n') {
        str += '\n';
        continue;
      }
      else if (*m_current == 't') {
        str += '\t';
        continue;
      }
    }

    str += *m_current;
    ++m_current;
  }

  if (*m_current != '"') {
    throw UnexpectedEndOfString{m_line};
  }

  ++m_current;
  token.value = ConstantTable::GetInstance()->Insert(str);
  return token;
}

Token LexParser::ParseNumber() {
  int i = 0; float f = 0.0f, scale = 1.0f;
  bool is_float = false;
  while (HasNext() && (std::isdigit(*m_current) || *m_current == '.')) {
    if (*m_current == '.') {
      if (is_float) {
        throw WrongNumber{m_line};
      }
      is_float = true;
      f = static_cast<float>(i);
    }
    else {
      if (is_float) {
        scale *= 0.1f;
        f += static_cast<float>((*m_current) - '0') * scale;
      }
      else {
        i *= 10; i += static_cast<int>((*m_current) - '0');
      }
    }

    ++m_current;
  }

  if (is_float) {
    union {
      int output; float input;
    } converter{};
    converter.input = f;
    return Token{TokenType::TOKEN_FLOAT, converter.output};
  }
  else {
    return Token{TokenType::TOKEN_INT, i};
  }
}

Token LexParser::ParseOperator() {
  Token token{};
  switch (*m_current) {
  case ':':
    token.type = TokenType::TOKEN_COLON;
    break;
  case '+':
    token.type = TokenType::TOKEN_ADD;
    break;
  case '-':
    token.type = TokenType::TOKEN_SUB;
    break;
  case '*':
    token.type = TokenType::TOKEN_MUL;
    break;
  case '/':
    token.type = TokenType::TOKEN_DIV;
    break;
  case '%':
    token.type = TokenType::TOKEN_MOD;
    break;
  case '=':
    token.type = TokenType::TOKEN_EQUAL;
    break;
  case '[':
    token.type = TokenType::TOKEN_LEFT_SQUARE;
    break;
  case ']':
    token.type = TokenType::TOKEN_RIGHT_SQUARE;
    break;
  case '(':
    token.type = TokenType::TOKEN_LEFT_PARENTHESES;
    break;
  case ')':
    token.type = TokenType::TOKEN_RIGHT_PARENTHESES;
    break;
  case '{':
    token.type = TokenType::TOKEN_LEFT_CURLY;
    break;
  case '}':
    token.type = TokenType::TOKEN_RIGHT_CURLY;
    break;
  case '<':
    token.type = TokenType::TOKEN_LESS;
    break;
  case '>':
    token.type = TokenType::TOKEN_GREATER;
    break;
  case '$':
    token.type = TokenType::TOKEN_DOLLAR;
    break;
  case '@':
    token.type = TokenType::TOKEN_AT;
    break;
  case '#':
    token.type = TokenType::TOKEN_SHARP;
    break;
  case ',':
    token.type = TokenType::TOKEN_COMMA;
    break;
  default:
    throw UnknownNotion{m_line, *m_current};
  }

  ++m_current;
  return token;
}

Token LexParser::ParseKeywords() {
  std::string keyword;
  while (HasNext() && (std::isalnum(*m_current) || *m_current == '_')) {
    keyword += *m_current;
    ++m_current;
  }

  if (m_keywords.find(keyword) != m_keywords.end()) {
    return m_keywords[keyword];
  }
  else {
    return Token{TokenType::TOKEN_VARIABLE, SymbolTable::GetInstance()->Insert(keyword)};
  }
}

Token LexParser::LookNext() {
  if (m_next.has_value()) {
    return m_next.value();
  }

  while (HasNext() && std::isspace(*m_current)) {
    if (*m_current == '\n') {
      ++m_line;
    }
    ++m_current;
  }

  while (HasNext() && *m_current == '/' && *(m_current + 1) == '/') {
    while (HasNext() && *m_current != '\n') {
      ++m_current;
    }

    ++m_current; ++m_line;
    while (HasNext() && std::isspace(*m_current)) {
      if (*m_current == '\n') {
        ++m_line;
      }
      ++m_current;
    }
  }

  if (!HasNext()) {
    Token eof{};
    eof.type = TokenType::TOKEN_EOF;
    return eof;
  }

  if (*m_current == '"') {
    m_next = ParseString();
  }
  else if (std::isdigit(*m_current)) {
    m_next = ParseNumber();
  }
  else if (std::isalpha(*m_current)) {
    m_next = ParseKeywords();
  }
  else {
    m_next = ParseOperator();
  }

  m_next->line = m_line;
  return m_next.value();
}

