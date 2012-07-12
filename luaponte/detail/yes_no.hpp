// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_DETAIL_YES_NO_HPP
#define LUAPONTE_DETAIL_YES_NO_HPP

namespace luaponte {
namespace detail {

typedef char(&yes_t)[1];
typedef char(&no_t)[2];

} // namespace detail
} // namespace luaponte

#endif // LUAPONTE_DETAIL_YES_NO_HPP
