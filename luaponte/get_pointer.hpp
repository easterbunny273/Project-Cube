// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_GET_POINTER_HPP
#define LUAPONTE_GET_POINTER_HPP

//
// We need these overloads in the luabind namespace.
//

#include <boost/get_pointer.hpp>
#include <memory>

namespace luaponte {

using boost::get_pointer;

template <typename T>
T* get_pointer(std::shared_ptr<T> const& ptr)
{
    return ptr.get();
}

template <typename T, typename Deleter>
T* get_pointer(std::unique_ptr<T, Deleter> const& ptr)
{
    return ptr.get();
}

} // namespace luaponte

#endif // LUAPONTE_GET_POINTER_HPP
