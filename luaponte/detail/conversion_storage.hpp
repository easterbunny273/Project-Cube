// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_DETAIL_CONVERSION_STORAGE_HPP
#define LUAPONTE_DETAIL_CONVERSION_STORAGE_HPP

#include <luaponte/config.hpp>
#include <boost/aligned_storage.hpp>

namespace luaponte {
namespace detail {

typedef void(*destruction_function)(void*);

// This is used by the converters in policy.hpp, and
// class_rep::convert_to as temporary storage when constructing
// holders.

struct conversion_storage
{
    conversion_storage()
      : destructor(0)
    {}

    ~conversion_storage()
    {
        if (destructor)
            destructor(&data);
    }

    // Unfortunately the converters currently doesn't have access to
    // the actual type being converted when this is instantiated, so
    // we have to guess a max size.
    boost::aligned_storage<128> data;
    destruction_function destructor;
};

} // namespace detail
} // namespace luaponte

#endif // LUAPONTE_DETAIL_CONVERSION_STORAGE_HPP
