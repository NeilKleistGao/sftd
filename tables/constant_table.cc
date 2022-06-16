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

/// @file constant_table.cc

#include "constant_table.h"

template<> ConstantTable* Singleton<ConstantTable>::m_instance = nullptr;

int ConstantTable::Insert(const std::string& p_str) {
  if (m_visit.find(p_str) != m_visit.end()) {
    return m_visit[p_str];
  }

  int id = static_cast<int>(m_visit.size());
  m_visit[p_str] = id;
  m_table[id] = m_visit.find(p_str);

  return id;
}

std::string ConstantTable::Find(int p_id) {
  if (m_table.find(p_id) != m_table.end()) {
    return m_table[p_id]->first;
  }

  return "";
}

void ConstantTable::Replace(int p_id, const std::string& p_str) {
  if (m_table.find(p_id) == m_table.end()) {
    return;
  }

  auto origin = m_table[p_id];
  m_visit.erase(origin);
  m_visit[p_str] = p_id;
  m_table[p_id] = m_visit.find(p_str);
}