// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_DETAIL_CLASS_REGISTRY_HPP
#define LUAPONTE_DETAIL_CLASS_REGISTRY_HPP

#include <map>

#include <luaponte/config.hpp>
#include <luaponte/open.hpp>
#include <luaponte/typeid.hpp>

namespace luaponte {
namespace detail {

    class class_rep;

    struct LUAPONTE_API class_registry
    {
        class_registry(lua_State* L);

        static class_registry* get_registry(lua_State* L);

        int cpp_instance() const { return m_instance_metatable; }
        int cpp_class() const { return m_cpp_class_metatable; }

        int lua_instance() const { return m_instance_metatable; }
        int lua_class() const { return m_lua_class_metatable; }
        int lua_function() const { return m_lua_function_metatable; }

        void add_class(type_id const& info, class_rep* crep);

        class_rep* find_class(type_id const& info) const;

        std::map<type_id, class_rep*> const& get_classes() const
        {
            return m_classes;
        }

    private:

        std::map<type_id, class_rep*> m_classes;

        // this is a lua reference that points to the lua table
        // that is to be used as meta table for all C++ class
        // instances. It is a kind of v-table.
        int m_instance_metatable;

        // this is a lua reference to the metatable to be used
        // for all classes defined in C++.
        int m_cpp_class_metatable;

        // this is a lua reference to the metatable to be used
        // for all classes defined in lua
        int m_lua_class_metatable;

        // this metatable only contains a destructor
        // for luaponte::Detail::free_functions::function_rep
        int m_lua_function_metatable;

    };

} // namespace detail
} // namespace luaponte

#endif // LUAPONTE_DETAIL_CLASS_REGISTRY_HPP
