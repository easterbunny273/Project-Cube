// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_DETAIL_MOST_DERIVED_HPP
#define LUAPONTE_DETAIL_MOST_DERIVED_HPP

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>

namespace luaponte {
namespace detail {

template<class Class, class WrappedClass>
struct most_derived
{
    typedef typename boost::mpl::if_<
        boost::is_base_and_derived<Class, WrappedClass>
      , WrappedClass
      , Class
    >::type type;
};

} // namespace detail
} // namespace luaponte

#endif // LUAPONTE_DETAIL_MOST_DERIVED_HPP
