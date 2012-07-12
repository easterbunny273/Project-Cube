// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_DETAIL_GARBAGE_COLLECTOR_HPP
#define LUAPONTE_DETAIL_GARBAGE_COLLECTOR_HPP

#include <luaponte/config.hpp>

namespace luaponte {
namespace detail {

// function that is used as __gc metafunction on several objects
template<class T>
inline int garbage_collector(lua_State* L)
{
    T* obj = static_cast<T*>(lua_touserdata(L, -1));
    obj->~T();
    return 0;
}

template<class T>
struct garbage_collector_s
{
    static int apply(lua_State* L)
    {
        T* obj = static_cast<T*>(lua_touserdata(L, -1));
        obj->~T();
        return 0;
    }
};

} // namespace detail
} // namespace luaponte

#endif // LUAPONTE_DETAIL_GARBAGE_COLLECTOR_HPP
