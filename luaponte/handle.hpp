// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_HANDLE_HPP
#define LUAPONTE_HANDLE_HPP

#include <luaponte/lua_include.hpp>
#include <luaponte/value_wrapper.hpp>

namespace luaponte {

// A reference to a Lua value. Represents an entry in the
// registry table.
class handle
{
public:
    handle();
    handle(lua_State* interpreter, int stack_index);
    handle(lua_State* main, lua_State* interpreter, int stack_index);
    handle(handle const& other);
    ~handle() noexcept;

    handle& operator=(handle const& other);
    void swap(handle& other);

    void push(lua_State* interpreter) const;

    lua_State* interpreter() const;

    void replace(lua_State* interpreter, int stack_index);

private:
    lua_State* m_interpreter;
    int m_index;
};

inline handle::handle()
  : m_interpreter(0)
  , m_index(LUA_NOREF)
{}

inline handle::handle(handle const& other)
  : m_interpreter(other.m_interpreter)
  , m_index(LUA_NOREF)
{
    if (m_interpreter == 0) return;
    lua_rawgeti(m_interpreter, LUA_REGISTRYINDEX, other.m_index);
    m_index = luaL_ref(m_interpreter, LUA_REGISTRYINDEX);
}

inline handle::handle(lua_State* interpreter, int stack_index)
  : m_interpreter(interpreter)
  , m_index(LUA_NOREF)
{
    lua_pushvalue(interpreter, stack_index);
    m_index = luaL_ref(interpreter, LUA_REGISTRYINDEX);
}

inline handle::handle(lua_State* main, lua_State* interpreter, int stack_index)
  : m_interpreter(main)
  , m_index(LUA_NOREF)
{
    lua_pushvalue(interpreter, stack_index);
    m_index = luaL_ref(interpreter, LUA_REGISTRYINDEX);
}

inline handle::~handle() noexcept
{
    if (m_interpreter && m_index != LUA_NOREF)
        luaL_unref(m_interpreter, LUA_REGISTRYINDEX, m_index);
}

inline handle& handle::operator=(handle const& other)
{
    handle(other).swap(*this);
    return *this;
}

inline void handle::swap(handle& other)
{
    std::swap(m_interpreter, other.m_interpreter);
    std::swap(m_index, other.m_index);
}

inline void handle::push(lua_State* interpreter) const
{
    lua_rawgeti(interpreter, LUA_REGISTRYINDEX, m_index);
}

inline lua_State* handle::interpreter() const
{
    return m_interpreter;
}

inline void handle::replace(lua_State* interpreter, int stack_index)
{
    lua_pushvalue(interpreter, stack_index);
    lua_rawseti(interpreter, LUA_REGISTRYINDEX, m_index);
}

template<>
struct value_wrapper_traits<handle>
{
    typedef boost::mpl::true_ is_specialized;

    static lua_State* interpreter(handle const& value)
    {
        return value.interpreter();
    }

    static void unwrap(lua_State* interpreter, handle const& value)
    {
        value.push(interpreter);
    }

    static bool check(...)
    {
        return true;
    }
};

} // namespace luaponte

#endif // LUAPONTE_HANDLE_HPP
