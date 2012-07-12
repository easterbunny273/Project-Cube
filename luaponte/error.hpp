// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_ERROR_HPP
#define LUAPONTE_ERROR_HPP

#include <luaponte/prefix.hpp>
#include <exception>
#include <luaponte/config.hpp>
#include <luaponte/typeid.hpp>

struct lua_State;

namespace luaponte {

#ifndef LUAPONTE_NO_EXCEPTIONS

// this exception usually means that the lua function you called
// from C++ failed with an error code. You will have to
// read the error code from the top of the lua stack
// the reason why this exception class doesn't contain
// the message itself is that std::string's copy constructor
// may throw, if the copy constructor of an exception that is
// being thrown throws another exception, terminate will be called
// and the entire application is killed.
class LUAPONTE_API error : public std::exception
{
public:
    explicit error(lua_State* L): m_L(L) {}
    lua_State* state() const throw() { return m_L; }
    virtual const char* what() const throw()
    {
        return "lua runtime error";
    }
private:
    lua_State* m_L;
};

// if an object_cast<>() fails, this is thrown
// it is also thrown if the return value of
// a lua function cannot be converted
class LUAPONTE_API cast_failed : public std::exception
{
public:
    cast_failed(lua_State* L, type_id const& i): m_L(L), m_info(i) {}
    lua_State* state() const throw() { return m_L; }
    type_id info() const throw() { return m_info; }
    virtual const char* what() const throw() { return "unable to make cast"; }
private:
    lua_State* m_L;
    type_id m_info;
};

#else

typedef void(*error_callback_fun)(lua_State*);
typedef void(*cast_failed_callback_fun)(lua_State*, type_id const&);

LUAPONTE_API void set_error_callback(error_callback_fun e);
LUAPONTE_API void set_cast_failed_callback(cast_failed_callback_fun c);
LUAPONTE_API error_callback_fun get_error_callback();
LUAPONTE_API cast_failed_callback_fun get_cast_failed_callback();

#endif

typedef int(*pcall_callback_fun)(lua_State*);
LUAPONTE_API void set_pcall_callback(pcall_callback_fun e);
LUAPONTE_API pcall_callback_fun get_pcall_callback();

} // namespace luaponte

#endif // LUAPONTE_ERROR_HPP
