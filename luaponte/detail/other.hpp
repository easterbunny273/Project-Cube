// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_DETAIL_OTHER_HPP
#define LUAPONTE_DETAIL_OTHER_HPP

// header derived from source code found in Boost.Python

// Copyright David Abrahams 2002. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#include <luaponte/config.hpp>
#include <boost/config.hpp>

namespace luaponte {

    template<class T>
    struct other
    {
        typedef T type;
    };

} // namespace luaponte

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
namespace luaponte {
namespace detail {

    template<typename T>
    class unwrap_other
    {
    public:
            typedef T type;
    };

    template<typename T>
    class unwrap_other<other<T> >
    {
    public:
            typedef T type;
    };

} // namespace detail
} // namespace luaponte

# else // no partial specialization

#include <boost/type.hpp>

namespace luaponte {
namespace detail {

    typedef char (&yes_other_t)[1];
    typedef char (&no_other_t)[2];

    no_other_t is_other_test(...);

    template<typename T>
    yes_other_t is_other_test(type_< other<T> >);

    template<bool wrapped>
    struct other_unwrapper
    {
            template <class T>
            struct apply
            {
                    typedef T type;
            };
    };

    template<>
    struct other_unwrapper<true>
    {
            template <class T>
            struct apply
            {
                    typedef typename T::type type;
            };
    };

    template<typename T>
    class is_other
    {
     public:
            BOOST_STATIC_CONSTANT(
                    bool, value = (
                            sizeof(detail::is_other_test(type_<T>()))
                            == sizeof(detail::yes_other_t)));
    };

    template <typename T>
    class unwrap_other
            : public detail::other_unwrapper<
            is_other<T>::value
    >::template apply<T>
    {};

} // namespace detail
} // namespace luaponte

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#endif // LUAPONTE_DETAIL_OTHER_HPP
