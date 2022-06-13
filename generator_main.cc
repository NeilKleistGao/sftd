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

/// @file generator_main.cc

#include <iostream>
#include <fstream>

#include "grammar.hpp"
#include "grammar/table_generator.h"

void GenerateTable(std::ofstream& p_fp) {
  p_fp << "static std::map<std::pair<int, int>, Action> s_slr_table = {" << std::endl;

  TableGenerator generator{};
  generator.Build(s_grammar_rules);

  while (generator.HasNext()) {
    p_fp << generator.GetNextCode() << std::endl;
  }

  p_fp << "};" << std::endl;
}

int main(int argc, char* argv[]) {
  std::ofstream fp{"../../grammar/lr_table_gen.hpp"};
  fp << "#ifndef SFTD_LR_TABLE_GEN_HPP" << std::endl;
  fp << "#define SFTD_LR_TABLE_GEN_HPP" << std::endl;
  fp << std::endl;
  fp << "#include <unordered_map>" << std::endl;
  fp << "#include \"action.h\"" << std::endl;

  fp << std::endl;
  GenerateTable(fp);
  fp << std::endl;

  fp << "#endif // SFTD_LR_TABLE_GEN_HPP" << std::endl;
  fp << std::endl;

  fp.close();

  return 0;
}