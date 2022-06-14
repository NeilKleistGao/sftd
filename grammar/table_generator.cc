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

/// @file table_generator.cc

#include "table_generator.h"

void TableGenerator::Build(const GrammarRules& p_rules) {
  MapTerms(p_rules);
  BuildDFA(p_rules);
}

std::string TableGenerator::GetNextCode() {
  ++m_current_state;
  return "";
}

void TableGenerator::BuildDFA(const GrammarRules& p_rules) {
    ++m_states_count;
    m_dfa[0] = std::vector<State>{};
    m_dfa[0].emplace_back("S'", std::vector<Either<std::string, TokenType>>{TermRule{"S"}, TermRule{"$"}}, 0);

    BuildDFA(p_rules, 0);
}

void TableGenerator::BuildDFA(const TableGenerator::GrammarRules &p_rules,
                              int p_state) {
  bool flag = false;
  do {
    flag = false;
    std::vector<State> new_states;
    for (const auto& s : m_dfa[p_state]) {
      if (s.pos < s.to.size()) {
        const auto& next = s.to[s.pos].GetLeft();
        if (next.has_value()) {
          auto it = p_rules.find(next.value());
          if (it != p_rules.end()) {
            new_states.emplace_back(next.value(), it->second, 0);
            flag = true;
          }
        }
      }
    }

    for (const auto& s : new_states) {
      m_dfa[p_state].push_back(s);
    }

  } while (flag);

  for (int i = -static_cast<int>(m_map.size()) - 1; i < static_cast<int>(TokenType::TOKENS_SIZE); ++i) {
    if (i == -1) {
      continue;
    }

    std::vector<State> new_states;
    for (const auto& s : m_dfa[p_state]) {
      if (s.pos < s.to.size()) {
        auto term = s.to[s.pos];
        if (i < -1 && term.GetLeft().has_value() && m_map[term.GetLeft().value()] == i) { // term
          new_states.emplace_back(s.from, s.to, s.pos + 1);
        }
        else if (i > -1 && term.GetRight().has_value() && static_cast<int>(term.GetRight().value()) == i) { // token
          new_states.emplace_back(s.from, s.to, s.pos + 1);
        }
      }
    }

    if (!new_states.empty() && !Exists(new_states)) {
      int id = m_states_count++;
      m_dfa[id] = new_states;
      m_transition[std::make_pair(p_state, i)] = id;
      BuildDFA(p_rules, id);
    }
  }
}

void TableGenerator::MapTerms(const GrammarRules& p_rules) {
  int index = -2;
  for (const auto& [f, _]: p_rules) {
    if (m_map.find(f) == m_map.end()) {
      m_map[f] = index--;
    }
  }
}

bool TableGenerator::Exists(const std::vector<State>& p_states) {
  bool exist = false;
  for (const auto& [_, states] : m_dfa) {
    if (states.size() == p_states.size()) {
      bool identical = true;
      for (const auto& s1: p_states) {
        bool found = false;
        for (const auto& s2: states) {
          if (s1 == s2) {
            found = true;
            break;
          }
        }

        if (!found) {
          identical = false;
          break;
        }
      }

      if (identical) {
        exist = true;
        break;
      }
    }
  }

  return exist;
}

bool TableGenerator::State::operator==(const State& p_other) const {
  if (from == p_other.from && pos == p_other.pos && to.size() == p_other.to.size()) {
    bool same = true;
    for (int i = 0; i < to.size(); ++i) {
      if (!(to[i] == p_other.to[i])) {
        same = false;
        break;
      }
    }

    return same;
  }

  return false;
}