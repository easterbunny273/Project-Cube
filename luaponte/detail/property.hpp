// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_DETAIL_PROPERTY_HPP
#define LUAPONTE_DETAIL_PROPERTY_HPP

namespace luaponte {
namespace detail {

template <class Class, class T, class Result = T>
struct access_member_ptr
{
    access_member_ptr(T Class::* mem_ptr)
      : mem_ptr(mem_ptr)
    {}

    Result operator()(Class const& x) const
    {
        return const_cast<Class&>(x).*mem_ptr;
    }

    void operator()(Class& x, T const& value) const
    {
        x.*mem_ptr = value;
    }

    T Class::* mem_ptr;
};

} // namespace detail
} // namespace luaponte

#endif // LUAPONTE_DETAIL_PROPERTY_HPP
