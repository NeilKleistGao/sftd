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
  return "";
}

void TableGenerator::BuildDFA(const GrammarRules& p_rules) {
    ++m_states_count;
    m_dfa[0] = std::vector<State>{};
    m_dfa[0].emplace_back("S'", std::vector<std::string>{"S$"}, 0);

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
        const std::string& next = s.to[s.pos];
        auto it = p_rules.find(next);
        if (it != p_rules.end()) {
          new_states.emplace_back(next, it->second, 0);
          flag = true;
        }
      }
    }

    for (const auto& s : new_states) {
      m_dfa[p_state].push_back(s);
    }

  } while (flag);

  //TODO:
}

void TableGenerator::MapTerms(const GrammarRules& p_rules) {
  int index = -2;
  for (const auto& [f, _]: p_rules) {
    if (m_map.find(f) == m_map.end()) {
      m_map[f] = index--;
    }
  }
}
