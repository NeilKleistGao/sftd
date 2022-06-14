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
#include <unordered_set>
#include <utility>
#include <vector>

#include "action.h"
#include "lex/token.h"
#include "sugars/either.hpp"

class TableGenerator {
public:
  TableGenerator(): m_current_state(0), m_states_count(0) {};
  ~TableGenerator() = default;
  TableGenerator(const TableGenerator&) = delete;
  TableGenerator& operator=(const TableGenerator&) = delete;
  TableGenerator(TableGenerator&&) = delete;
  TableGenerator& operator=(TableGenerator&&) = delete;

  using GrammarRules = std::unordered_map<std::string, std::vector<Either<std::string, TokenType>>>;

  void Build(const GrammarRules& p_rules);

  inline bool HasNext() const {
    return m_current_state < m_states_count;
  }

  std::string GetNextCode();
private:
  using TableIndex = std::pair<int, int>; // rows \times cols
  using TermRule = Either<std::string, TokenType>;

  struct State {
    std::string from;
    std::vector<TermRule> to;
    int pos{};

    State() = default;
    State(std::string p_from, std::vector<TermRule> p_to, int p_pos)
        : from(std::move(p_from)), to(std::move(p_to)), pos(p_pos) {}

    bool operator==(const State& p_other) const;
  };

  std::map<TableIndex, Action> m_table;
  std::map<TableIndex, int> m_transition;
  std::unordered_map<std::string, int> m_map;
  std::unordered_map<int, std::vector<State>> m_dfa;
  std::unordered_map<int, std::unordered_set<TokenType>> m_follow;

  int m_states_count;
  int m_current_state;

  void MapTerms(const GrammarRules& p_rules);
  void GetFollow(const GrammarRules& p_rules);

  void BuildDFA(const GrammarRules& p_rules);
  void BuildDFA(const GrammarRules& p_rules, int p_state);

  bool Exists(const std::vector<State>& p_states);

  void CreateLRTable();
};

#endif // SFTD_TABLE_GENERATOR_H
