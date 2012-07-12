// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_NEW_SCOPE_HPP
#define LUAPONTE_NEW_SCOPE_HPP

#include <luaponte/prefix.hpp>
#include <luaponte/config.hpp>
#include <luaponte/object.hpp>
#include <luaponte/lua_include.hpp>
#include <memory>

namespace luaponte {

struct scope;

namespace detail {

struct LUAPONTE_API registration
{
    registration();
    virtual ~registration();

protected:
    virtual void register_(lua_State*) const = 0;

private:
    friend struct ::luaponte::scope;
    registration* m_next;
};

} // namespace detail

struct LUAPONTE_API scope
{
    scope();
    explicit scope(std::unique_ptr<detail::registration> reg);
    scope(scope const& other_);
    ~scope();

    scope& operator=(scope const& other_);

    scope& operator,(scope s);

    void register_(lua_State* L) const;

private:
    detail::registration* m_chain;
};

class LUAPONTE_API namespace_ : public scope
{
public:
    explicit namespace_(char const* name);
    namespace_& operator[](scope s);

private:
    struct registration_;
    registration_* m_registration;
};

class LUAPONTE_API module_
{
public:
    module_(object const& table);
    module_(lua_State* L, char const* name);
    void operator[](scope s);

private:
    object m_table;
};

inline module_ module(object const& table)
{
    return module_(table);
}

inline module_ module(lua_State* L, char const* name = 0)
{
    return module_(L, name);
}

} // namespace luaponte

#endif // LUAPONTE_NEW_SCOPE_HPP
