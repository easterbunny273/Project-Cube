// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_CONFIG_HPP
#define LUAPONTE_CONFIG_HPP

#include <boost/config.hpp>

#ifdef BOOST_MSVC
    #define LUAPONTE_ANONYMOUS_FIX static
#else
    #define LUAPONTE_ANONYMOUS_FIX
#endif

#if defined (BOOST_MSVC) && (BOOST_MSVC <= 1200)

#define for if (false) {} else for

#include <cstring>

namespace std {

using ::strlen;
using ::strcmp;
using ::type_info;

}

#endif

#if defined (BOOST_MSVC) && (BOOST_MSVC <= 1300)
    #define LUAPONTE_MSVC_TYPENAME
#else
    #define LUAPONTE_MSVC_TYPENAME typename
#endif

// the maximum number of arguments of functions that's
// registered. Must at least be 2
#ifndef LUAPONTE_MAX_ARITY
    #define LUAPONTE_MAX_ARITY 10
#elif LUAPONTE_MAX_ARITY <= 1
    #undef LUAPONTE_MAX_ARITY
    #define LUAPONTE_MAX_ARITY 2
#endif

// the maximum number of classes one class
// can derive from
// max bases must at least be 1
#ifndef LUAPONTE_MAX_BASES
    #define LUAPONTE_MAX_BASES 4
#elif LUAPONTE_MAX_BASES <= 0
    #undef LUAPONTE_MAX_BASES
    #define LUAPONTE_MAX_BASES 1
#endif

// LUAPONTE_NO_ERROR_CHECKING
// define this to remove all error checks
// this will improve performance and memory
// footprint.
// if it is defined matchers will only be called on
// overloaded functions, functions that's
// not overloaded will be called directly. The
// parameters on the lua stack are assumed
// to match those of the function.
// exceptions will still be catched when there's
// no error checking.

// LUAPONTE_NOT_THREADSAFE
// this define will make luabind non-thread safe. That is,
// it will rely on a static variable. You can still have
// multiple lua states and use coroutines, but only
// one of your real threads may run lua code.

// LUAPONTE_NO_EXCEPTIONS
// this define will disable all usage of try, catch and throw in
// luabind. This will in many cases disable runtime-errors, such
// as invalid casts, when calling lua-functions that fails or
// returns values that cannot be converted by the given policy.
// Luabind requires that no function called directly or indirectly
// by luabind throws an exception (throwing exceptions through
// C code has undefined behavior, lua is written in C).

#ifdef LUAPONTE_DYNAMIC_LINK
# ifdef BOOST_WINDOWS
#  ifdef LUAPONTE_BUILDING
#   define LUAPONTE_API __declspec(dllexport)
#  else
#   define LUAPONTE_API __declspec(dllimport)
#  endif
# else
#  if defined(_GNUC_) && _GNUC_ >=4
#   define LUAPONTE_API __attribute__ ((visibility("default")))
#  endif
# endif
#endif

#ifndef LUAPONTE_API
# define LUAPONTE_API
#endif

namespace luaponte {

LUAPONTE_API void disable_super_deprecation();

} // namespace luaponte

#endif // LUAPONTE_CONFIG_HPP
