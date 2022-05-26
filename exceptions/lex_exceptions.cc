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

/// @file lex_exceptions.cc

#include "lex_exceptions.h"

#include <string>

UnexpectedEndOfString::UnexpectedEndOfString(int p_line)
    : BasicException(p_line) {
  m_msg = "Line " + std::to_string(m_line) + ": unexpected EOF.";
}

WrongNumber::WrongNumber(int p_line) : BasicException(p_line) {
  m_msg = "Line " + std::to_string(m_line) + ": wrong number expression.";
}

UnknownNotion::UnknownNotion(int p_line, char p_notion)
    : BasicException(p_line), m_notion(p_notion) {
  m_msg = "Line " + std::to_string(m_line) + ": unknown notion " + m_notion;
}
