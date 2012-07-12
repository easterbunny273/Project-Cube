// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_DETAIL_LINK_COMPATIBILITY_HPP
#define LUAPONTE_DETAIL_LINK_COMPATIBILITY_HPP

#include <luaponte/config.hpp>

namespace luaponte {
namespace detail {

#ifdef LUAPONTE_NOT_THREADSAFE
LUAPONTE_API void not_threadsafe_defined_conflict();
#else
LUAPONTE_API void not_threadsafe_not_defined_conflict();
#endif

#ifdef LUAPONTE_NO_ERROR_CHECKING
LUAPONTE_API void no_error_checking_defined_conflict();
#else
LUAPONTE_API void no_error_checking_not_defined_conflict();
#endif

inline void check_link_compatibility()
{
#ifdef LUAPONTE_NOT_THREADSAFE
    not_threadsafe_defined_conflict();
#else
    not_threadsafe_not_defined_conflict();
#endif

#ifdef LUAPONTE_NO_ERROR_CHECKING
    no_error_checking_defined_conflict();
#else
    no_error_checking_not_defined_conflict();
#endif
}

} // namespace detail
} // namespace luaponte

#endif
