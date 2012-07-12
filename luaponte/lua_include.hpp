// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUA_INCLUDE_HPP
#define LUA_INCLUDE_HPP

#ifndef LUAPONTE_CPLUSPLUS_LUA
extern "C"
{
#endif

#include "lua.h"
#include "lauxlib.h"

#ifndef LUAPONTE_CPLUSPLUS_LUA
}
#endif

#endif // LUA_INCLUDE_HPP
