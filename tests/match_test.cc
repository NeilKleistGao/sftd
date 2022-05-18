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

/// @file match_test.cc

#include <gtest/gtest.h>

#include "sugars/matcher.hpp"

TEST(MatchTest, ValueMatchTest) {
  auto res1 = Matcher<int, int>::Match(42)
      ->Case(2)([](const int& _) -> int {
        return 0;
      })
      ->Case(12)([](const int& _) -> int {
        return 1;
      })
      ->Case(22)([](const int& _) -> int {
        return 2;
      })
      ->Case(32)([](const int& _) -> int {
        return 3;
      })
      ->Case(42)([](const int& _) -> int {
        return 4;
      })
      ->Case(52)([](const int& _) -> int {
        return 5;
      })->Result();

  EXPECT_EQ(res1.has_value(), true);
  EXPECT_EQ(res1.value(), 4);

  auto res2 = Matcher<std::string, int>::Match("To be")
      ->Case("To be")([](const std::string& _) -> int {
        return 1;
      })
      ->Case("Not to be")([](const std::string& _) -> int {
        return 0;
      })
      ->CaseDefault()([](const std::string& _) -> int {
        return -1;
      })->Result();

  EXPECT_EQ(res2.has_value(), true);
  EXPECT_EQ(res2.value(), 1);

  auto res3 = Matcher<std::string, int>::Match("TODO")
      ->Case("To be")([](const std::string& _) -> int {
        return 1;
      })
      ->Case("Not to be")([](const std::string& _) -> int {
        return 0;
      })->Result();

  EXPECT_EQ(res3.has_value(), false);

  auto res4 = Matcher<std::string, std::string>::Match("TODO")
      ->Case("To be")([](const std::string& _) -> std::string {
        return "Yep";
      })
      ->Case("Not to be")([](const std::string& _) -> std::string {
        return "Nop";
      })
      ->CaseDefault()([](const std::string& _) -> std::string {
        return "WTF?";
      })->Result();

  EXPECT_EQ(res4.has_value(), true);
  EXPECT_EQ(res4.value(), "WTF?");
}

class A {
protected:
  virtual void foo() {};
};

class B : public A{
protected:
  void foo() override {};
};

class C : public A{
protected:
  void foo() override {};
};

TEST(MatchTest, ClassMatchTest) {
  auto* b = new B();
  auto res1 = Matcher<A*, int>::Match(b)
      ->Case<B*>()([](B* _){ return 1; })
      ->Case<C*>()([](C* _){ return 2; })
      ->CaseDefault()([](A* _){ return 0; })->Result();

  EXPECT_EQ(res1.has_value(), true);
  EXPECT_EQ(res1.value(), 1);

  auto* c = new C();
  auto res2 = Matcher<A*, int>::Match(c)
      ->Case<B*>()([](B* _){ return 1; })
      ->Case<C*>()([](C* _){ return 2; })
      ->CaseDefault()([](A* _){ return 0; })->Result();

  EXPECT_EQ(res2.has_value(), true);
  EXPECT_EQ(res2.value(), 2);

  auto* a = new A();
  auto res3 = Matcher<A*, int>::Match(a)
      ->Case<B*>()([](B* _){ return 1; })
      ->Case<C*>()([](C* _){ return 2; })
      ->CaseDefault()([](A* _){ return 0; })->Result();

  EXPECT_EQ(res3.has_value(), true);
  EXPECT_EQ(res3.value(), 0);
}