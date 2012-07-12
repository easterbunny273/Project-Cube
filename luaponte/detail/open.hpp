// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_DETAIL_OPEN_HPP
#define LUAPONTE_DETAIL_OPEN_HPP

#include <luaponte/config.hpp>

namespace luaponte {
namespace detail {

LUAPONTE_API void add_operator_to_metatable(lua_State* L, int op_index);
LUAPONTE_API int create_cpp_class_metatable(lua_State* L);
LUAPONTE_API int create_cpp_instance_metatable(lua_State* L);
LUAPONTE_API int create_lua_class_metatable(lua_State* L);
LUAPONTE_API int create_lua_instance_metatable(lua_State* L);
LUAPONTE_API int create_lua_function_metatable(lua_State* L);

} // namespace detail

LUAPONTE_API void open(lua_State* L);

} // namespace luaponte

#endif // LUAPONTE_DETAIL_OPEN_HPP
