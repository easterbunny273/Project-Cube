// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_RETURN_REFERENCE_TO_POLICY_HPP
#define LUAPONTE_RETURN_REFERENCE_TO_POLICY_HPP

namespace luaponte {
namespace detail {

template<class T>
struct return_reference_to_converter;

template<>
struct return_reference_to_converter<cpp_to_lua>
{
    template<class T>
    void apply(lua_State* L, const T&)
    {
        lua_pushnil(L);
    }
};

template<int N>
struct return_reference_to_policy : conversion_policy<0>
{
    static void precall(lua_State*, const index_map&) {}
    static void postcall(lua_State* L, const index_map& indices)
    {
        int result_index = indices[0];
        int ref_to_index = indices[N];

        lua_pushvalue(L, ref_to_index);
        lua_replace(L, result_index);
    }

    template<class T, class Direction>
    struct apply
    {
        typedef return_reference_to_converter<Direction> type;
    };
};

} // namespace detail

template<int N>
detail::policy_cons<detail::return_reference_to_policy<N>, detail::null_type>
return_reference_to(LUAPONTE_PLACEHOLDER_ARG(N))
{
    return detail::policy_cons<detail::return_reference_to_policy<N>, detail::null_type>();
}

} // namespace luaponte

#endif // LUAPONTE_RETURN_REFERENCE_TO_POLICY_HPP
