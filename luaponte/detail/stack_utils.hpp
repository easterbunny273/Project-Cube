// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_DETAIL_STACK_UTILS_HPP
#define LUAPONTE_DETAIL_STACK_UTILS_HPP

#include <cassert>

namespace luaponte {
namespace detail {

struct stack_pop
{
    stack_pop(lua_State* L, int n)
        : m_state(L)
        , m_n(n)
        {
        }

    ~stack_pop()
    {
        lua_pop(m_state, m_n);
    }

private:

    lua_State* m_state;
    int m_n;
};

} // namespace detail
} // namespace luaponte

#endif // LUAPONTE_DETAIL_STACK_UTILS_HPP
