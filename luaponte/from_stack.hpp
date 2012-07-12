// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_FROM_STACK_HPP
#define LUAPONTE_FROM_STACK_HPP

namespace luaponte {

struct from_stack
{
    from_stack(lua_State* interpreter, int index)
      : interpreter(interpreter)
      , index(index)
    {}

    lua_State* interpreter;
    int index;
};

} // namespace luaponte

#endif // LUAPONTE_FROM_STACK_HPP
