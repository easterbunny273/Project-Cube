// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_NO_DEPENDENCY_HPP
#define LUAPONTE_NO_DEPENDENCY_HPP

#include <luaponte/detail/policy.hpp>

namespace luaponte {
namespace detail {

struct no_dependency_policy
{
    static void precall(lua_State*, index_map const&)
    {}

    static void postcall(lua_State*, index_map const&)
    {}
};

typedef policy_cons<no_dependency_policy, null_type>
    no_dependency_node;

} // namespace detail

detail::no_dependency_node const no_dependency = {};

namespace detail {

inline void ignore_unused_no_dependency()
{
    (void)no_dependency;
}

} // namespace detail
} // namespace luaponte

#endif // LUAPONTE_NO_DEPENDENCY_HPP
