// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_FUNCTION_HPP
#define LUAPONTE_FUNCTION_HPP

#include <luaponte/make_function.hpp>
#include <luaponte/scope.hpp>
#include <luaponte/detail/call_function.hpp>

#include <memory>

namespace luaponte {
namespace detail {

template <class F, class Policies>
struct function_registration : registration
{
    function_registration(char const* name, F f, Policies const& policies)
      : name(name)
      , f(f)
      , policies(policies)
    {}

    void register_(lua_State* L) const
    {
        object fn = make_function(L, f, deduce_signature(f), policies);

        add_overload(
            object(from_stack(L, -1))
          , name
          , fn
        );
    }

    char const* name;
    F f;
    Policies policies;
};

LUAPONTE_API bool is_luabind_function(lua_State* L, int index);

} // namespace detail

template <class F, class Policies>
scope def(char const* name, F f, Policies const& policies)
{
    return scope(std::unique_ptr<detail::registration>(
        new detail::function_registration<F, Policies>(name, f, policies)));
}

template <class F>
scope def(char const* name, F f)
{
    return def(name, f, detail::null_type());
}

} // namespace luaponte

#endif // LUAPONTE_FUNCTION_HPP
