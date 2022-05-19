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

/// @file fold.hpp

#ifndef SFTD_FOLD_HPP
#define SFTD_FOLD_HPP

#include <functional>
#include <type_traits>

template<typename ContainerType>
typename ContainerType::value_type FoldLeft(const typename ContainerType::value_type& p_initial, const ContainerType& p_arr,
              const std::function<typename ContainerType::value_type(const typename ContainerType::value_type&, const typename ContainerType::value_type&)>& p_func) {
  typename ContainerType::value_type result = p_initial;
  for (const auto& it : p_arr) {
    result = p_func(result, it);
  }

  return result;
}

template<typename ContainerType>
typename ContainerType::value_type ReduceLeft(const ContainerType& p_arr,
                const std::function<typename ContainerType::value_type(const typename ContainerType::value_type&, const typename ContainerType::value_type&)>& p_func) {
  auto it = p_arr.begin();
  typename ContainerType::value_type result = *p_arr.begin();
  ++it;
  for (; it != p_arr.end(); ++it) {
    result = p_func(result, *it);
  }

  return result;
}

#endif // SFTD_FOLD_HPP
