// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_DETAIL_HAS_GET_POINTER_HPP
#define LUAPONTE_DETAIL_HAS_GET_POINTER_HPP

#include <luaponte/get_pointer.hpp>

#include <type_traits>
#include <utility>

namespace luaponte {
namespace detail {

template <typename T, typename Enable = void>
struct has_get_pointer
  : std::false_type {};


template <typename T>
struct has_get_pointer<T
  , typename std::enable_if<
        std::is_pointer<
            decltype(get_pointer(std::declval<T>()))
        >::value
    >::type>
  : std::true_type {};

} // namespace detail
} // namespace luaponte

#endif // LUAPONTE_DETAIL_HAS_GET_POINTER_HPP
