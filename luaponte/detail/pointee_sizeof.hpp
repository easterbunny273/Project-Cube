// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_DETAIL_POINTEE_SIZEOF_HPP
#define LUAPONTE_DETAIL_POINTEE_SIZEOF_HPP

#include <boost/mpl/int.hpp>

namespace luaponte {
namespace detail {

template<class T> T& deref_type(T(*)(), int);
template<class T> T& deref_type(T*(*)(), long);

} // namespace detail

// returns the indirect sizeof U, as in
//    sizeof(T*) = sizeof(T)
//    sizeof(T&) = sizeof(T)
//    sizeof(T)  = sizeof(T)
template<class T>
struct pointee_sizeof
{
    BOOST_STATIC_CONSTANT(int, value = (
        sizeof(detail::deref_type((T(*)())0), 0L)
    ));

    typedef boost::mpl::int_<value> type;
};

} // namespace luaponte

#endif // LUAPONTE_DETAIL_POINTEE_SIZEOF_HPP
