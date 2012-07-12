// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_DETAIL_PRIMITIVES_HPP
#define LUAPONTE_DETAIL_PRIMITIVES_HPP

#include <algorithm>
#include <cstring>

#include <luaponte/config.hpp>
#include <luaponte/detail/yes_no.hpp>

namespace luaponte {
namespace detail {

template<class T>
struct identity
{
    typedef T type;
};

template<class T>
struct type_ {};

struct null_type {};

struct lua_to_cpp {};
struct cpp_to_lua {};

template<class T> struct by_value {};
template<class T> struct by_reference {};
template<class T> struct by_const_reference {};
template<class T> struct by_pointer {};
template<class T> struct by_const_pointer {};

struct converter_policy_tag {};

struct ltstr
{
    bool operator()(const char* s1, const char* s2) const { return std::strcmp(s1, s2) < 0; }
};

template<int N>
struct aligned
{
    char storage[N];
};

// returns the offset added to a Derived* when cast to a Base*
// TODO: return ptrdiff
template<class Derived, class Base>
int ptr_offset(type_<Derived>, type_<Base>)
{
    aligned<sizeof(Derived)> obj;
    Derived* ptr = reinterpret_cast<Derived*>(&obj);

    return int(static_cast<char*>(static_cast<void*>(static_cast<Base*>(ptr)))
    - static_cast<char*>(static_cast<void*>(ptr)));
}

} // namespace detail
} // namespace luaponte

#endif // LUAPONTE_DETAIL_PRIMITIVES_HPP
