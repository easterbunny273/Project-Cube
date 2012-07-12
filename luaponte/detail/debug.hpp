// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_DETAIL_DEBUG_HPP
#define LUAPONTE_DETAIL_DEBUG_HPP

#ifndef NDEBUG

# include <luaponte/lua_include.hpp>
# include <cassert>

namespace luaponte {
namespace detail {

struct stack_checker_type
{
    stack_checker_type(lua_State* L)
        : m_L(L)
        , m_stack(lua_gettop(m_L))
    {}

    ~stack_checker_type()
    {
        assert(m_stack == lua_gettop(m_L));
    }

    lua_State* m_L;
    int m_stack;
};

} // namespace detail
} // namespace luaponte

# define LUAPONTE_CHECK_STACK(L) luaponte::detail::stack_checker_type stack_checker_object(L)

#else

# define LUAPONTE_CHECK_STACK(L) do {} while (0)

#endif

#endif // LUAPONTE_DETAIL_DEBUG_HPP
