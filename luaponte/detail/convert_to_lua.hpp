// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_DETAIL_CONVERT_TO_LUA_HPP
#define LUAPONTE_DETAIL_CONVERT_TO_LUA_HPP

#include <luaponte/config.hpp>
#include <luaponte/detail/policy.hpp>
#include <boost/ref.hpp>

#include <boost/mpl/apply_wrap.hpp>

namespace luaponte {
namespace detail {

template<bool IsReferenceWrapper = false>
struct unwrap_ref
{
    template<class T>
    static const T& get(const T& r) { return r; }

    template<class T>
    struct apply
    {
        typedef T type;
    };
};

template<>
struct unwrap_ref<true>
{
    template<class T>
    static T& get(const boost::reference_wrapper<T>& r) { return r.get(); }

    template<class T>
    struct apply
    {
        typedef typename T::type& type;
    };
};

namespace mpl = boost::mpl;

template<class T>
void convert_to_lua(lua_State* L, const T& v)
{
    typedef typename mpl::apply_wrap1<
        unwrap_ref<boost::is_reference_wrapper<T>::value>
      , T
    >::type value_type;

    typename mpl::apply_wrap2<default_policy,value_type,cpp_to_lua>::type converter;

    converter.apply(L, unwrap_ref<boost::is_reference_wrapper<T>::value>::get(v));
}

template<int Index, class T, class Policies>
void convert_to_lua_p(lua_State* L, const T& v, const Policies&)
{
    typedef typename mpl::apply_wrap1<
        unwrap_ref<boost::is_reference_wrapper<T>::value>
      , T
    >::type value_type;

    typedef typename find_conversion_policy<Index, Policies>::type converter_policy;
    typename mpl::apply_wrap2<converter_policy,value_type,cpp_to_lua>::type converter;

    converter.apply(L, unwrap_ref<boost::is_reference_wrapper<T>::value>::get(v));
}

} // namespace detail
} // namespace luaponte

#endif // LUAPONTE_DETAIL_CONVERT_TO_LUA_HPP
