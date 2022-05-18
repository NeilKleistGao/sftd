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

/// @file matcher.hpp

#ifndef SFTD_MATCHER_HPP
#define SFTD_MATCHER_HPP

#include <functional>
#include <optional>
#include <memory>
#include <regex>

template<typename ParamType, typename ReturnType, bool Reg = false>
class Matcher {
private:
  template <typename MatchType>
  struct MatchResult {
    bool matched = false;
    Matcher* self = nullptr;
    std::optional<MatchType> param;

    using RefFunc = std::function<ReturnType(const MatchType&)>;
    using PointerFunc = std::function<ReturnType(MatchType*)>;
    using IgnoreFunc = std::function<ReturnType()>;

    Matcher* operator()(const RefFunc& p_func) {
      if (matched) {
        self->m_matched = true;
        self->m_result = p_func(param.value());
      }

      return self;
    }

    Matcher* operator()(const PointerFunc& p_func) {
      if (matched) {
        self->m_matched = true;
        self->m_result = p_func(param.value());
      }

      return self;
    }

    Matcher* operator()(const IgnoreFunc& p_func) {
      if (matched) {
        self->m_matched = true;
        self->m_result = p_func();
      }

      return self;
    }
  };

public:
  explicit Matcher(const ParamType& p_input) : m_input(p_input), m_matched(false) {
  }

  static std::unique_ptr<Matcher> Match(const ParamType& p_input) {
    return std::make_unique<Matcher>(p_input);
  }

  ~Matcher() = default;
  Matcher(const Matcher&) = delete;
  Matcher& operator=(const Matcher&) = delete;
  Matcher(Matcher&&) = delete;
  Matcher& operator=(Matcher&&) = delete;

  template <typename MatchType>
  MatchResult<MatchType> Case() {
    MatchResult<MatchType> result{};
    auto temp = dynamic_cast<MatchType>(m_input);
    result.matched = !m_matched && temp != nullptr;
    if (result.matched) {
      result.param = temp;
    }

    result.self = this;
    return result;
  }

  MatchResult<ParamType> Case(const ParamType& p_pattern) {
    MatchResult<ParamType> result{};
    result.matched = !m_matched && m_input == p_pattern;
    if (result.matched) {
      result.param = m_input;
    }

    result.self = this;
    return result;
  }

  MatchResult<ParamType> CaseDefault() {
    MatchResult<ParamType> result{};
    result.matched = !m_matched;
    if (result.matched) {
      result.param = m_input;
    }

    result.self = this;
    return result;
  }

  std::optional<ReturnType> Result() const {
    return m_result;
  }

private:
  const ParamType& m_input;
  bool m_matched;

  std::optional<ReturnType> m_result;
};

template<typename ParamType, bool Reg>
class Matcher<ParamType, void, Reg> {
private:
  template <typename MatchType>
  struct MatchResult {
    bool matched = false;
    Matcher* self = nullptr;
    std::optional<MatchType> param;

    using RefFunc = std::function<void(const MatchType&)>;
    using PointerFunc = std::function<void(MatchType*)>;
    using IgnoreFunc = std::function<void(void)>;

    Matcher* operator()(const RefFunc& p_func) {
      if (matched) {
        self->m_matched = true;
        p_func(param.value());
      }

      return self;
    }

    Matcher* operator()(const PointerFunc& p_func) {
      if (matched) {
        self->m_matched = true;
        p_func(param.value());
      }

      return self;
    }

    Matcher* operator()(const IgnoreFunc& p_func) {
      if (matched) {
        self->m_matched = true;
        p_func();
      }

      return self;
    }
  };

public:
  explicit Matcher(const ParamType& p_input) : m_input(p_input), m_matched(false) {
  }

  static std::unique_ptr<Matcher> Match(const ParamType& p_input) {
    return std::make_unique<Matcher>(p_input);
  }

  ~Matcher() = default;
  Matcher(const Matcher&) = delete;
  Matcher& operator=(const Matcher&) = delete;
  Matcher(Matcher&&) = delete;
  Matcher& operator=(Matcher&&) = delete;

  template <typename MatchType>
  MatchResult<MatchType> Case() {
    MatchResult<MatchType> result{};
    auto temp = dynamic_cast<MatchType>(m_input);
    result.matched = !m_matched && temp != nullptr;
    if (result.matched) {
      result.param = temp;
    }

    result.self = this;
    return result;
  }

  MatchResult<ParamType> Case(const ParamType& p_pattern) {
    MatchResult<ParamType> result{};
    result.matched = !m_matched && m_input == p_pattern;
    if (result.matched) {
      result.param = m_input;
    }

    result.self = this;
    return result;
  }

  MatchResult<ParamType> CaseDefault() {
    MatchResult<ParamType> result{};
    result.matched = !m_matched;
    if (result.matched) {
      result.param = m_input;
    }

    result.self = this;
    return result;
  }

private:
  const ParamType& m_input;
  bool m_matched;
};

template<typename ReturnType>
class Matcher<std::string, ReturnType, true> {
private:
  struct MatchResult {
    bool matched = false;
    Matcher* self = nullptr;
    std::optional<std::string> param;

    using RefFunc = std::function<ReturnType(const std::string&)>;
    using IgnoreFunc = std::function<ReturnType()>;

    Matcher* operator()(const RefFunc& p_func) {
      if (matched) {
        self->m_matched = true;
        self->m_result = p_func(param.value());
      }

      return self;
    }

    Matcher* operator()(const IgnoreFunc& p_func) {
      if (matched) {
        self->m_matched = true;
        self->m_result = p_func();
      }

      return self;
    }
  };

public:
  explicit Matcher(const std::string& p_input) : m_input(p_input), m_matched(false) {
  }

  static std::unique_ptr<Matcher> Match(const std::string& p_input) {
    return std::make_unique<Matcher>(p_input);
  }

  ~Matcher() = default;
  Matcher(const Matcher&) = delete;
  Matcher& operator=(const Matcher&) = delete;
  Matcher(Matcher&&) = delete;
  Matcher& operator=(Matcher&&) = delete;

  MatchResult Case(const std::string& p_pattern) {
    MatchResult result{};
    std::regex reg{p_pattern};
    result.matched = !m_matched && std::regex_match(m_input, reg);
    if (result.matched) {
      result.param = m_input;
    }

    result.self = this;
    return result;
  }

  MatchResult CaseDefault() {
    MatchResult result{};
    result.matched = !m_matched;
    if (result.matched) {
      result.param = m_input;
    }

    result.self = this;
    return result;
  }

  std::optional<ReturnType> Result() const {
    return m_result;
  }

private:
  const std::string& m_input;
  bool m_matched;

  std::optional<ReturnType> m_result;
};

template<>
class Matcher<std::string, void, true> {
private:
  struct MatchResult {
    bool matched = false;
    Matcher* self = nullptr;
    std::optional<std::string> param;

    using RefFunc = std::function<void(const std::string&)>;
    using IgnoreFunc = std::function<void(void)>;

    Matcher* operator()(const RefFunc& p_func) {
      if (matched) {
        self->m_matched = true;
        p_func(param.value());
      }

      return self;
    }

    Matcher* operator()(const IgnoreFunc& p_func) {
      if (matched) {
        self->m_matched = true;
        p_func();
      }

      return self;
    }
  };

public:
  explicit Matcher(const std::string& p_input) : m_input(p_input), m_matched(false) {
  }

  static std::unique_ptr<Matcher> Match(const std::string& p_input) {
    return std::make_unique<Matcher>(p_input);
  }

  ~Matcher() = default;
  Matcher(const Matcher&) = delete;
  Matcher& operator=(const Matcher&) = delete;
  Matcher(Matcher&&) = delete;
  Matcher& operator=(Matcher&&) = delete;

  MatchResult Case(const std::string& p_pattern) {
    MatchResult result{};
    std::regex reg{p_pattern};
    result.matched = !m_matched && std::regex_match(m_input, reg);
    if (result.matched) {
      result.param = m_input;
    }

    result.self = this;
    return result;
  }

  MatchResult CaseDefault() {
    MatchResult result{};
    result.matched = !m_matched;
    if (result.matched) {
      result.param = m_input;
    }

    result.self = this;
    return result;
  }

private:
  const std::string& m_input;
  bool m_matched;
};

#endif // SFTD_MATCHER_HPP
