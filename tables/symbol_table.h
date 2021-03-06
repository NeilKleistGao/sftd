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

/// @file symbol_table.h

#ifndef SFTD_SYMBOL_TABLE_H
#define SFTD_SYMBOL_TABLE_H

#include <unordered_map>

#include "sugars/singleton.hpp"

class SymbolTable : public Singleton<SymbolTable> {
public:
  int Insert(const std::string& p_str);

  inline void Clear() {
    m_table.clear(); m_visit.clear();
  }

  inline int GetSize() const {
    return m_visit.size();
  }

  std::string GetEntry(int p_id);
private:
  std::unordered_map<std::string, int> m_visit;
  using SymbolData = std::unordered_map<std::string, int>::const_iterator;
  std::unordered_map<int, SymbolData> m_table;
};

#endif // SFTD_SYMBOL_TABLE_H
