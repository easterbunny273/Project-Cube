// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_BACK_REFERENCE_FWD_HPP
#define LUAPONTE_BACK_REFERENCE_FWD_HPP

namespace luaponte {

template<class T>
bool get_back_reference(lua_State* L, T const& x);

template<class T>
bool move_back_reference(lua_State* L, T const& x);

} // namespace luaponte

#endif // LUAPONTE_BACK_REFERENCE_FWD_HPP
