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

/// @file compile_test.cc

#include <gtest/gtest.h>
#include <cstring>
#include <cstdio>

#include "syntax/compiler.h"

TEST(CompileTest, I18NTest) {
  {
    const char* PROGRAM = R"(#{"en_GB", "zh_CN"})";
    Compiler compiler{};
    compiler.Compile(const_cast<char*>(PROGRAM), std::strlen(PROGRAM), "i18n_test");

    FILE* fp = fopen("i18n_test.csv", "r");
    EXPECT_NE(fp, nullptr);
    char lang1[8], lang2[8];
    fscanf(fp, "keys\t%s\t%s", lang1, lang2);
    std::string s1 = lang1, s2 = lang2;
    EXPECT_EQ(s1, "en_GB");
    EXPECT_EQ(s2, "zh_CN");
  }

  {
    const char* PROGRAM = R"(#{"en_GB", "zh_CN"} dialogue test {"Hello!"})";
    Compiler compiler{};
    compiler.Compile(const_cast<char*>(PROGRAM), std::strlen(PROGRAM), "i18n_test_2");

    FILE* fp = fopen("i18n_test_2.csv", "r");
    EXPECT_NE(fp, nullptr);
    char lang1[8], lang2[8];
    fscanf(fp, "keys\t%s\t%s", lang1, lang2);
    char key[32], value[32], place[32];
    fscanf(fp, "%s\t%s\t%s", key, value, place);
    std::string s1 = key, s2 = value, s3 = place;
    EXPECT_EQ(s1, "i18n_test_20");
    EXPECT_EQ(s2, "Hello!");
    EXPECT_EQ(s3, "TODO");
  }

  {
    const char* PROGRAM = "";
    Compiler compiler{};
    compiler.Compile(const_cast<char*>(PROGRAM), std::strlen(PROGRAM), "i18n_test_3");

    FILE* fp = fopen("i18n_test_3.csv", "r");
    EXPECT_EQ(fp, nullptr);
  }
}

TEST(CompileTest, SymbolTableTest) {
  {
    const char* PROGRAM = "";
    Compiler compiler{};
    compiler.Compile(const_cast<char*>(PROGRAM), std::strlen(PROGRAM), "");
    auto size = compiler.GetSize();
    char buffer[size];
    compiler.Write(buffer);

    EXPECT_EQ(buffer[0], static_cast<char>(0xAE));
    EXPECT_EQ(buffer[1], static_cast<char>(0x86));
    EXPECT_EQ(buffer[2], 0);
    EXPECT_EQ(buffer[3], 0);

    EXPECT_EQ(buffer[4], 0);
    EXPECT_EQ(buffer[5], 0);
    EXPECT_EQ(buffer[6], 0);
    EXPECT_EQ(buffer[7], 0);
  }

  {
    const char* PROGRAM = "dialogue test1{}"
                          "dialogue test2{}";
    Compiler compiler{};
    compiler.Compile(const_cast<char*>(PROGRAM), std::strlen(PROGRAM), "");
    auto size = compiler.GetSize();
    char buffer[size];
    compiler.Write(buffer);

    EXPECT_EQ(buffer[4], 0);
    EXPECT_EQ(buffer[5], 0);
    EXPECT_EQ(buffer[6], 0);
    EXPECT_EQ(buffer[7], 2);

    EXPECT_EQ(buffer[8], 0);
    EXPECT_EQ(buffer[9], 0);
    EXPECT_EQ(buffer[10], 0);
    EXPECT_EQ(buffer[11], 0);

    EXPECT_EQ(buffer[12], 0);
    EXPECT_EQ(buffer[13], 0);
    EXPECT_EQ(buffer[14], 0);
    EXPECT_EQ(buffer[15], 5);

    EXPECT_EQ(buffer[16], 't');
    EXPECT_EQ(buffer[17], 'e');
    EXPECT_EQ(buffer[18], 's');
    EXPECT_EQ(buffer[19], 't');
    EXPECT_EQ(buffer[20], '1');

    EXPECT_EQ(buffer[21], 0);
    EXPECT_EQ(buffer[22], 0);
    EXPECT_EQ(buffer[23], 0);
    EXPECT_EQ(buffer[24], 1);

    EXPECT_EQ(buffer[25], 0);
    EXPECT_EQ(buffer[26], 0);
    EXPECT_EQ(buffer[27], 0);
    EXPECT_EQ(buffer[28], 5);

    EXPECT_EQ(buffer[29], 't');
    EXPECT_EQ(buffer[30], 'e');
    EXPECT_EQ(buffer[31], 's');
    EXPECT_EQ(buffer[32], 't');
    EXPECT_EQ(buffer[33], '2');
  }
}

TEST(CompileTest, StringTableTest) {
  {
    const char* PROGRAM = R"(dialogue test{"test"})";
    Compiler compiler{};
    compiler.Compile(const_cast<char*>(PROGRAM), std::strlen(PROGRAM), "");
    auto size = compiler.GetSize();
    char buffer[size];
    compiler.Write(buffer);

    EXPECT_EQ(buffer[20], 0);
    EXPECT_EQ(buffer[21], 0);
    EXPECT_EQ(buffer[22], 0);
    EXPECT_EQ(buffer[23], 1);

    EXPECT_EQ(buffer[24], 0);
    EXPECT_EQ(buffer[25], 0);
    EXPECT_EQ(buffer[26], 0);
    EXPECT_EQ(buffer[27], 0);

    EXPECT_EQ(buffer[28], 0);
    EXPECT_EQ(buffer[29], 0);
    EXPECT_EQ(buffer[30], 0);
    EXPECT_EQ(buffer[31], 4);

    EXPECT_EQ(buffer[32], 't');
    EXPECT_EQ(buffer[33], 'e');
    EXPECT_EQ(buffer[34], 's');
    EXPECT_EQ(buffer[35], 't');
  }
}

TEST(CompileTest, ILTest) {
  {
    const char* PROGRAM = R"(dialogue test1{} auto dialogue test2{} interact dialogue test3{} trigger dialogue test4{})";
    Compiler compiler{};
    compiler.Compile(const_cast<char*>(PROGRAM), std::strlen(PROGRAM), "");
    auto size = compiler.GetSize();
    char buffer[size];
    compiler.Write(buffer);

    EXPECT_EQ(size, 144);
    EXPECT_EQ(buffer[64], 0);
    EXPECT_EQ(buffer[65], 0);
    EXPECT_EQ(buffer[66], 0);
    EXPECT_EQ(buffer[67], 1);

    EXPECT_EQ(buffer[84], 0);
    EXPECT_EQ(buffer[85], 0);
    EXPECT_EQ(buffer[86], 0);
    EXPECT_EQ(buffer[87], 1);

    EXPECT_EQ(buffer[104], 0);
    EXPECT_EQ(buffer[105], 0);
    EXPECT_EQ(buffer[106], 0);
    EXPECT_EQ(buffer[107], 1);

    EXPECT_EQ(buffer[124], 0);
    EXPECT_EQ(buffer[125], 0);
    EXPECT_EQ(buffer[126], 0);
    EXPECT_EQ(buffer[127], 1);
  }

  {
    const char* PROGRAM = R"(dialogue test{"hi!"})";
    Compiler compiler{};
    compiler.Compile(const_cast<char*>(PROGRAM), std::strlen(PROGRAM), "");
    auto size = compiler.GetSize();
    char buffer[size];
    compiler.Write(buffer);

    EXPECT_EQ(buffer[size - 4], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 3], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 2], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 1], static_cast<char>(255));

    EXPECT_EQ(buffer[size - 8], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 7], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 6], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 5], static_cast<char>(0));

    EXPECT_EQ(buffer[size - 12], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 11], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 10], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 9], static_cast<char>(4));

    EXPECT_EQ(buffer[size - 16], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 15], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 14], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 13], static_cast<char>(1));

    EXPECT_EQ(buffer[size - 20], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 19], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 18], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 17], static_cast<char>(0));

    EXPECT_EQ(buffer[size - 24], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 23], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 22], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 21], static_cast<char>(4));
  }

  {
    const char* PROGRAM = R"(dialogue test{"hi!" in 1})";
    Compiler compiler{};
    compiler.Compile(const_cast<char*>(PROGRAM), std::strlen(PROGRAM), "");
    auto size = compiler.GetSize();
    char buffer[size];
    compiler.Write(buffer);

    EXPECT_EQ(buffer[size - 8], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 7], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 6], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 5], static_cast<char>(1));

    EXPECT_EQ(buffer[size - 12], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 11], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 10], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 9], static_cast<char>(2));

    EXPECT_EQ(buffer[size - 16], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 15], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 14], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 13], static_cast<char>(0));

    EXPECT_EQ(buffer[size - 20], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 19], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 18], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 17], static_cast<char>(5));
  }

  {
    const char* PROGRAM = R"(dialogue test{[Alice]: "hi!"})";
    Compiler compiler{};
    compiler.Compile(const_cast<char*>(PROGRAM), std::strlen(PROGRAM), "");
    auto size = compiler.GetSize();
    char buffer[size];
    compiler.Write(buffer);

    EXPECT_EQ(buffer[size - 16], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 15], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 14], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 13], static_cast<char>(1));

    EXPECT_EQ(buffer[size - 20], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 19], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 18], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 17], static_cast<char>(2));
  }

  {
    const char* PROGRAM = R"(dialogue test{[Alice(happy)]: "hi!"})";
    Compiler compiler{};
    compiler.Compile(const_cast<char*>(PROGRAM), std::strlen(PROGRAM), "");
    auto size = compiler.GetSize();
    char buffer[size];
    compiler.Write(buffer);

    EXPECT_EQ(buffer[size - 16], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 15], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 14], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 13], static_cast<char>(2));

    EXPECT_EQ(buffer[size - 20], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 19], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 18], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 17], static_cast<char>(1));

    EXPECT_EQ(buffer[size - 24], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 23], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 22], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 21], static_cast<char>(3));
  }

  {
    const char* PROGRAM = R"(dialogue test1{} dialogue test2{ use test1 })";
    Compiler compiler{};
    compiler.Compile(const_cast<char*>(PROGRAM), std::strlen(PROGRAM), "");
    auto size = compiler.GetSize();
    char buffer[size];
    compiler.Write(buffer);

    EXPECT_EQ(buffer[size - 8], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 7], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 6], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 5], static_cast<char>(size - 40));

    EXPECT_EQ(buffer[size - 12], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 11], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 10], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 9], static_cast<char>(6));
  }

  {
    const char* PROGRAM = R"(dialogue test{ select { "test1": "1" "test2": "2" } })";
    Compiler compiler{};
    compiler.Compile(const_cast<char*>(PROGRAM), std::strlen(PROGRAM), "");
    auto size = compiler.GetSize();
    char buffer[size];
    compiler.Write(buffer);

    EXPECT_EQ(buffer[size - 16], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 15], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 14], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 13], static_cast<char>(52));

    EXPECT_EQ(buffer[size - 20], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 19], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 18], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 17], static_cast<char>(2));

    EXPECT_EQ(buffer[size - 24], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 23], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 22], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 21], static_cast<char>(8));

    EXPECT_EQ(buffer[size - 36], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 35], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 34], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 33], static_cast<char>(32));

    EXPECT_EQ(buffer[size - 40], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 39], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 38], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 37], static_cast<char>(0));

    EXPECT_EQ(buffer[size - 44], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 43], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 42], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 41], static_cast<char>(8));
  }

  {
    const char* PROGRAM = R"(dialogue test{ animate @Bob "w" })";
    Compiler compiler{};
    compiler.Compile(const_cast<char*>(PROGRAM), std::strlen(PROGRAM), "");
    auto size = compiler.GetSize();
    char buffer[size];
    compiler.Write(buffer);

    EXPECT_EQ(buffer[size - 8], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 7], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 6], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 5], static_cast<char>(0));

    EXPECT_EQ(buffer[size - 12], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 11], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 10], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 9], static_cast<char>(1));

    EXPECT_EQ(buffer[size - 16], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 15], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 14], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 13], static_cast<char>(1));

    EXPECT_EQ(buffer[size - 20], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 19], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 18], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 17], static_cast<char>(9));
  }

  {
    const char* PROGRAM = R"(dialogue test{ sound "test" })";
    Compiler compiler{};
    compiler.Compile(const_cast<char*>(PROGRAM), std::strlen(PROGRAM), "");
    auto size = compiler.GetSize();
    char buffer[size];
    compiler.Write(buffer);

    EXPECT_EQ(buffer[size - 8], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 7], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 6], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 5], static_cast<char>(0));

    EXPECT_EQ(buffer[size - 12], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 11], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 10], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 9], static_cast<char>(1));

    EXPECT_EQ(buffer[size - 16], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 15], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 14], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 13], static_cast<char>(10));
  }

  {
    const char* PROGRAM = R"(dialogue test{ move @Chalice (1, 2) in 1 })";
    Compiler compiler{};
    compiler.Compile(const_cast<char*>(PROGRAM), std::strlen(PROGRAM), "");
    auto size = compiler.GetSize();
    char buffer[size];
    compiler.Write(buffer);

    EXPECT_EQ(buffer[size - 8], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 7], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 6], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 5], static_cast<char>(1));

    EXPECT_EQ(buffer[size - 12], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 11], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 10], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 9], static_cast<char>(2));

    EXPECT_EQ(buffer[size - 16], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 15], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 14], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 13], static_cast<char>(2));

    EXPECT_EQ(buffer[size - 20], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 19], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 18], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 17], static_cast<char>(2));

    EXPECT_EQ(buffer[size - 24], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 23], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 22], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 21], static_cast<char>(1));

    EXPECT_EQ(buffer[size - 28], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 27], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 26], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 25], static_cast<char>(2));

    EXPECT_EQ(buffer[size - 32], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 31], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 30], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 29], static_cast<char>(1));

    EXPECT_EQ(buffer[size - 36], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 35], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 34], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 33], static_cast<char>(11));
  }

  {
    const char* PROGRAM = R"(dialogue test when ($first or $second) {})";
    Compiler compiler{};
    compiler.Compile(const_cast<char*>(PROGRAM), std::strlen(PROGRAM), "");
    auto size = compiler.GetSize();
    char buffer[size];
    compiler.Write(buffer);

    EXPECT_EQ(buffer[size - 12], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 11], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 10], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 9], static_cast<char>(size - 74));

    EXPECT_EQ(buffer[size - 16], static_cast<char>(0xFF));
    EXPECT_EQ(buffer[size - 15], static_cast<char>(0xFF));
    EXPECT_EQ(buffer[size - 14], static_cast<char>(0xFF));
    EXPECT_EQ(buffer[size - 13], static_cast<char>(0xFF));

    EXPECT_EQ(buffer[size - 20], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 19], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 18], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 17], static_cast<char>(0));

    EXPECT_EQ(buffer[size - 24], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 23], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 22], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 21], static_cast<char>(12));
  }

  {
    const char* PROGRAM = R"(dialogue test {if (not $a) { "1" } else if (false) { "2" } else { "3" }})";
    Compiler compiler{};
    compiler.Compile(const_cast<char*>(PROGRAM), std::strlen(PROGRAM), "");
    auto size = compiler.GetSize();
    char buffer[size];
    compiler.Write(buffer);

    EXPECT_EQ(buffer[size - 24], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 23], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 22], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 21], static_cast<char>(76));

    EXPECT_EQ(buffer[size - 28], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 27], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 26], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 25], static_cast<char>(0));

    EXPECT_EQ(buffer[size - 32], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 31], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 30], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 29], static_cast<char>(4));

    EXPECT_EQ(buffer[size - 36], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 35], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 34], static_cast<char>(0));
    EXPECT_EQ(buffer[size - 33], static_cast<char>(12));
  }
}
