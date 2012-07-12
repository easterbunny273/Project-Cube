// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_DETAIL_FORMAT_SIGNATURE_HPP
# define LUAPONTE_DETAIL_FORMAT_SIGNATURE_HPP

# include <luaponte/config.hpp>
# include <luaponte/lua_include.hpp>
# include <luaponte/typeid.hpp>

# include <boost/mpl/begin_end.hpp>
# include <boost/mpl/next.hpp>
# include <boost/mpl/size.hpp>

namespace luaponte {
namespace adl {

  class object;
  class argument;
  template <class Base>
  struct table;

} // namespace adl

using adl::object;
using adl::argument;
using adl::table;

namespace detail {

LUAPONTE_API std::string get_class_name(lua_State* L, type_id const& i);

template <class T>
struct type_to_string
{
    static void get(lua_State* L)
    {
        lua_pushstring(L, get_class_name(L, typeid(T)).c_str());
    }
};

template <class T>
struct type_to_string<T*>
{
    static void get(lua_State* L)
    {
        type_to_string<T>::get(L);
        lua_pushstring(L, "*");
        lua_concat(L, 2);
    }
};

template <class T>
struct type_to_string<T&>
{
    static void get(lua_State* L)
    {
        type_to_string<T>::get(L);
        lua_pushstring(L, "&");
        lua_concat(L, 2);
    }
};

template <class T>
struct type_to_string<T const>
{
    static void get(lua_State* L)
    {
        type_to_string<T>::get(L);
        lua_pushstring(L, " const");
        lua_concat(L, 2);
    }
};

# define LUAPONTE_TYPE_TO_STRING(x) \
    template <> \
    struct type_to_string<x> \
    { \
        static void get(lua_State* L) \
        { \
            lua_pushstring(L, #x); \
        } \
    };

# define LUAPONTE_INTEGRAL_TYPE_TO_STRING(x) \
    LUAPONTE_TYPE_TO_STRING(x) \
    LUAPONTE_TYPE_TO_STRING(unsigned x)

LUAPONTE_INTEGRAL_TYPE_TO_STRING(char)
LUAPONTE_INTEGRAL_TYPE_TO_STRING(short)
LUAPONTE_INTEGRAL_TYPE_TO_STRING(int)
LUAPONTE_INTEGRAL_TYPE_TO_STRING(long)

LUAPONTE_TYPE_TO_STRING(void)
LUAPONTE_TYPE_TO_STRING(bool)
LUAPONTE_TYPE_TO_STRING(std::string)
LUAPONTE_TYPE_TO_STRING(lua_State)

LUAPONTE_TYPE_TO_STRING(luaponte::object)
LUAPONTE_TYPE_TO_STRING(luaponte::argument)

# undef LUAPONTE_INTEGRAL_TYPE_TO_STRING
# undef LUAPONTE_TYPE_TO_STRING

template <class Base>
struct type_to_string<table<Base> >
{
    static void get(lua_State* L)
    {
        lua_pushstring(L, "table");
    }
};

template <class End>
void format_signature_aux(lua_State*, bool, End, End)
{}

template <class Iter, class End>
void format_signature_aux(lua_State* L, bool first, Iter, End end)
{
    if (!first)
        lua_pushstring(L, ",");
    type_to_string<typename Iter::type>::get(L);
    format_signature_aux(L, false, typename mpl::next<Iter>::type(), end);
}

template <class Signature>
void format_signature(lua_State* L, char const* function, Signature)
{
    typedef typename mpl::begin<Signature>::type first;

    type_to_string<typename first::type>::get(L);

    lua_pushstring(L, " ");
    lua_pushstring(L, function);

    lua_pushstring(L, "(");
    format_signature_aux(
        L
      , true
      , typename mpl::next<first>::type()
      , typename mpl::end<Signature>::type()
    );
    lua_pushstring(L, ")");

    lua_concat(L, static_cast<int>(mpl::size<Signature>()) * 2 + 2);
}

} // namespace detail
} // namespace luaponte

#endif // LUAPONTE_DETAIL_FORMAT_SIGNATURE_HPP
