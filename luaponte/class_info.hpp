// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_CLASS_INFO_HPP
#define LUAPONTE_CLASS_INFO_HPP

#include <luaponte/prefix.hpp>
#include <luaponte/lua_include.hpp>
#include <luaponte/luaponte.hpp>
#include <luaponte/object.hpp>

namespace luaponte {

struct LUAPONTE_API class_info
{
    std::string name;
    object methods;
    object attributes;
};

LUAPONTE_API class_info get_class_info(argument const&);

// returns a table of bound class names
LUAPONTE_API object get_class_names(lua_State* L);

LUAPONTE_API void bind_class_info(lua_State*);

} // namespace luaponte

#endif // LUAPONTE_CLASS_INFO_HPP
