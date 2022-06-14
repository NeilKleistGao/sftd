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

/// @file either.hpp

#ifndef SFTD_EITHER_HPP
#define SFTD_EITHER_HPP

#include <optional>

template<typename LeftT, typename RightT>
class Either {
public:
  explicit Either(const LeftT& p_v) : m_is_left(true), m_left(p_v) {}
  explicit Either(const RightT& p_v) : m_is_left(false), m_right(p_v) {}

  bool operator==(const LeftT& p_v) const {
    return m_is_left && m_left == p_v;
  }

  bool operator==(const RightT& p_v) const {
    return !m_is_left && m_right == p_v;
  }

  bool operator==(const Either<LeftT, RightT>& p_other) const {
    if (m_is_left && p_other.m_is_left) {
      return m_left.value() == p_other.m_left.value();
    }
    else if (!m_is_left && !p_other.m_is_left) {
      return m_right.value() == p_other.m_right.value();
    }

    return false;
  }

  inline std::optional<LeftT> GetLeft() const {
    return m_left;
  }

  inline std::optional<RightT> GetRight() const {
    return m_right;
  }
private:
  std::optional<LeftT> m_left;
  std::optional<RightT> m_right;
  bool m_is_left{};
};

#endif // SFTD_EITHER_HPP
