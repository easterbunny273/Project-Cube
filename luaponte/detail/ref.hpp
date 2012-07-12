// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_DETAIL_REF_HPP
#define LUAPONTE_DETAIL_REF_HPP

#include <cassert>
#include <algorithm>

#include <luaponte/config.hpp>
#include <luaponte/lua_include.hpp>

namespace luaponte {
namespace detail {

struct lua_reference
{
    lua_reference(lua_State* L_ = 0)
        : L(L_)
        , m_ref(LUA_NOREF)
    {}
    lua_reference(lua_reference const& r)
        : L(r.L)
        , m_ref(LUA_NOREF)
    {
        if (!r.is_valid()) return;
        r.get(L);
        set(L);
    }
    ~lua_reference() { reset(); }

    lua_State* state() const { return L; }

    void operator=(lua_reference const& r)
    {
        // TODO: self assignment problems
        reset();
        if (!r.is_valid()) return;
        r.get(r.state());
        set(r.state());
    }

    bool is_valid() const
    { return m_ref != LUA_NOREF; }

    void set(lua_State* L_)
    {
        reset();
        L = L_;
        m_ref = luaL_ref(L, LUA_REGISTRYINDEX);
    }

    void replace(lua_State* L_)
    {
        lua_rawseti(L_, LUA_REGISTRYINDEX, m_ref);
    }

    // L may not be the same pointer as
    // was used when creating this reference
    // since it may be a thread that shares
    // the same globals table.
    void get(lua_State* L_) const
    {
        assert(m_ref != LUA_NOREF);
        assert(L_);
        lua_rawgeti(L_, LUA_REGISTRYINDEX, m_ref);
    }

    void reset()
    {
        if (L && m_ref != LUA_NOREF) luaL_unref(L, LUA_REGISTRYINDEX, m_ref);
        m_ref = LUA_NOREF;
    }

    void swap(lua_reference& r)
    {
        assert(r.L == L);
        std::swap(r.m_ref, m_ref);
    }

private:
    lua_State* L;
    int m_ref;
};

} // namespace detail
} // namespace luaponte

#endif // LUAPONTE_DETAIL_REF_HPP
