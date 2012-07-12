// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_YIELD_POLICY_HPP
#define LUAPONTE_YIELD_POLICY_HPP

#include <luaponte/config.hpp>
#include <luaponte/detail/policy.hpp>

namespace luaponte {
namespace detail {

struct yield_policy
{
    static void precall(lua_State*, const index_map&) {}
    static void postcall(lua_State*, const index_map&) {}
};

} // namespace detail
} // namespace luaponte

namespace luaponte {

detail::policy_cons<detail::yield_policy, detail::null_type> const yield = {};

namespace detail {

inline void ignore_unused_yield()
{
    (void)yield;
}

} // namespace detail
} // luaponte

#endif // LUAPONTE_YIELD_POLICY_HPP

