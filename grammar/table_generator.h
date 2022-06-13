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

/// @file table_generator.h

#ifndef SFTD_TABLE_GENERATOR_H
#define SFTD_TABLE_GENERATOR_H

#include <map>
#include <unordered_map>
#include <vector>

#include "action.h"
#include "lex/token.h"

class TableGenerator {
public:
  TableGenerator() = default;
  ~TableGenerator() = default;
  TableGenerator(const TableGenerator&) = delete;
  TableGenerator& operator=(const TableGenerator&) = delete;
  TableGenerator(TableGenerator&&) = delete;
  TableGenerator& operator=(TableGenerator&&) = delete;

  using GrammarRules = std::unordered_map<std::string, std::vector<std::string>>;

  void Build(const GrammarRules& p_rules);
  bool HasNext() const;
  std::string GetNextCode();
private:
  using TableIndex = std::pair<int, int>;

  std::map<TableIndex, Action> m_table;
  std::map<TableIndex, int> m_reduce;

  int m_states_count;
  int m_current_state;
};

#endif // SFTD_TABLE_GENERATOR_H