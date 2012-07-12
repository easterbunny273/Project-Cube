// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_CONTAINER_POLICY_HPP
#define LUAPONTE_CONTAINER_POLICY_HPP

#include <luaponte/config.hpp>
#include <luaponte/detail/policy.hpp>
#include <boost/mpl/apply_wrap.hpp>

namespace luaponte {
namespace detail {

namespace mpl = boost::mpl;

template<class Policies>
struct container_converter_lua_to_cpp
{
    int consumed_args(...) const
    {
        return 1;
    }

    template<class T>
    T apply(lua_State* L, by_const_reference<T>, int index)
    {
        typedef typename T::value_type value_type;

        typedef typename find_conversion_policy<1, Policies>::type converter_policy;
        typename mpl::apply_wrap2<converter_policy,value_type,lua_to_cpp>::type converter;

        T container;

        lua_pushnil(L);
        while (lua_next(L, index))
        {
            container.push_back(converter.apply(L, LUAPONTE_DECORATE_TYPE(value_type), -1));
            lua_pop(L, 1); // pop value
        }

        return container;
    }

    template<class T>
    T apply(lua_State* L, by_value<T>, int index)
    {
        return apply(L, by_const_reference<T>(), index);
    }

    template<class T>
    static int match(lua_State* L, by_const_reference<T>, int index)
    {
        if (lua_istable(L, index)) return 0; else return -1;
    }

    template<class T>
    void converter_postcall(lua_State*, T, int) {}
};

template<class Policies>
struct container_converter_cpp_to_lua
{
    template<class T>
    void apply(lua_State* L, const T& container)
    {
        typedef typename T::value_type value_type;

        typedef typename find_conversion_policy<1, Policies>::type converter_policy;
        typename mpl::apply_wrap2<converter_policy,value_type,lua_to_cpp>::type converter;

        lua_newtable(L);

        int index = 1;

        for (typename T::const_iterator i = container.begin(); i != container.end(); ++i)
        {
            converter.apply(L, *i);
            lua_rawseti(L, -2, index);
            ++index;
        }
    }
};

template<int N, class Policies>
struct container_policy : conversion_policy<N>
{
    static void precall(lua_State*, const index_map&) {}
    static void postcall(lua_State*, const index_map&) {}

    struct only_accepts_nonconst_pointers {};

    template<class T, class Direction>
    struct apply
    {
        typedef typename boost::mpl::if_<boost::is_same<lua_to_cpp, Direction>
            , container_converter_lua_to_cpp<Policies>
            , container_converter_cpp_to_lua<Policies>
        >::type type;
    };
};

} // namespace detail
} // namespace luaponte

namespace luaponte {

template<int N>
detail::policy_cons<detail::container_policy<N, detail::null_type>, detail::null_type>
container(LUAPONTE_PLACEHOLDER_ARG(N))
{
    return detail::policy_cons<detail::container_policy<N, detail::null_type>, detail::null_type>();
}

template<int N, class Policies>
detail::policy_cons<detail::container_policy<N, Policies>, detail::null_type>
container(LUAPONTE_PLACEHOLDER_ARG(N), const Policies&)
{
    return detail::policy_cons<detail::container_policy<N, Policies>, detail::null_type>();
}

} // namespace luaponte

#endif // LUAPONTE_CONTAINER_POLICY_HPP
