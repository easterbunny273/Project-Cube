// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_MAKE_FUNCTION_HPP
# define LUAPONTE_MAKE_FUNCTION_HPP

# include <luaponte/config.hpp>
# include <luaponte/object.hpp>
# include <luaponte/detail/call.hpp>
# include <luaponte/detail/deduce_signature.hpp>
# include <luaponte/detail/format_signature.hpp>

namespace luaponte {
namespace detail {

# ifndef LUAPONTE_NO_EXCEPTIONS
LUAPONTE_API void handle_exception_aux(lua_State* L);
# endif

// MSVC complains about member being sensitive to alignment (C4121)
// when F is a pointer to member of a class with virtual bases.
# ifdef BOOST_MSVC
#  pragma pack(push)
#  pragma pack(16)
# endif

template <class F, class Signature, class Policies>
struct function_object_impl : function_object
{
    function_object_impl(F f, Policies const& policies)
      : function_object(&entry_point)
      , f(f)
      , policies(policies)
    {}

    int call(lua_State* L, invoke_context& ctx) const
    {
        return invoke(L, *this, ctx, f, Signature(), policies);
    }

    void format_signature(lua_State* L, char const* function) const
    {
        detail::format_signature(L, function, Signature());
    }

    static int entry_point(lua_State* L)
    {
        function_object_impl const* impl =
            *(function_object_impl const**)lua_touserdata(L, lua_upvalueindex(1));

        invoke_context ctx;

        int results = 0;

# ifndef LUAPONTE_NO_EXCEPTIONS
        bool exception_caught = false;

        try
        {
            results = invoke(
                L, *impl, ctx, impl->f, Signature(), impl->policies);
        }
        catch (...)
        {
            exception_caught = true;
            handle_exception_aux(L);
        }

        if (exception_caught)
            lua_error(L);
# else
        results = invoke(L, *impl, ctx, impl->f, Signature(), impl->policies);
# endif

        if (!ctx)
        {
            ctx.format_error(L, impl);
            lua_error(L);
        }

        return results;
    }

    F f;
    Policies policies;
};

# ifdef BOOST_MSVC
#  pragma pack(pop)
# endif

LUAPONTE_API object make_function_aux(
    lua_State* L, function_object* impl
);

LUAPONTE_API void add_overload(object const&, char const*, object const&);

} // namespace detail

template <class F, class Signature, class Policies>
object make_function(lua_State* L, F f, Signature, Policies)
{
    return detail::make_function_aux(
        L
      , new detail::function_object_impl<F, Signature, Policies>(
            f, Policies()
        )
    );
}

template <class F>
object make_function(lua_State* L, F f)
{
    return make_function(L, detail::deduce_signature(f), detail::null_type());
}

} // namespace luaponte

#endif // LUAPONTE_MAKE_FUNCTION_HPP
