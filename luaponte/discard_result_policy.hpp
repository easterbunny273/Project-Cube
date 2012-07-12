// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_DISCARD_RESULT_POLICY_HPP
#define LUAPONTE_DISCARD_RESULT_POLICY_HPP

#include <luaponte/config.hpp>
#include <luaponte/detail/policy.hpp>

namespace luaponte {
namespace detail {

struct discard_converter
{
    template<class T>
    void apply(lua_State*, T) {}
};

struct discard_result_policy : conversion_policy<0>
{
    static void precall(lua_State*, const index_map&) {}
    static void postcall(lua_State*, const index_map&) {}

    struct can_only_convert_from_cpp_to_lua {};

    template<class T, class Direction>
    struct apply
    {
        typedef typename boost::mpl::if_<boost::is_same<Direction, cpp_to_lua>
            , discard_converter
            , can_only_convert_from_cpp_to_lua
        >::type type;
    };
};

} // namespace detail
} // namespace luaponte

namespace luaponte {

detail::policy_cons<
  detail::discard_result_policy, detail::null_type> const discard_result = {};

namespace detail {

inline void ignore_unused_discard_result()
{
    (void)discard_result;
}

} // namespace detail
} // namespace luaponte

#endif // LUAPONTE_DISCARD_RESULT_POLICY_HPP
