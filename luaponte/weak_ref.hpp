// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_WEAK_REF_HPP
#define LUAPONTE_WEAK_REF_HPP

#include <luaponte/config.hpp>

struct lua_State;

namespace luaponte {

class LUAPONTE_API weak_ref
{
public:
    weak_ref();
    weak_ref(lua_State* main, lua_State* L, int index);
    weak_ref(weak_ref const&);
    ~weak_ref();

    weak_ref& operator=(weak_ref const&);

    void swap(weak_ref&);

    // returns a unique id that no
    // other weak ref will return
    int id() const;

    lua_State* state() const;
    void get(lua_State* L) const;

private:
    struct impl;
    impl* m_impl;
};

} // namespace luaponte

#endif // LUAPONTE_WEAK_REF_HPP
