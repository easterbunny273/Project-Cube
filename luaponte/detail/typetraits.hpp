// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_DETAIL_TYPETRAITS_HPP
#define LUAPONTE_DETAIL_TYPETRAITS_HPP

#include <luaponte/config.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/is_const.hpp>
#include <luaponte/detail/primitives.hpp>

namespace luaponte {
namespace detail {

#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

    template<class T>
    struct is_const_type
    {
        typedef typename boost::mpl::if_<boost::is_const<T>
            , yes_t
            , no_t
        >::type type;
    };

    template<bool is_Reference = false>
    struct is_const_reference_helper
    {
        template<class>
        struct apply
        {
            enum
            {
                value = false
            };
        };
    };

    template<class T>
    typename is_const_type<T>::type is_const_reference_tester(T&);
    no_t is_const_reference_tester(...);

    template<>
    struct is_const_reference_helper<true>
    {
        template<class T>
        struct apply
        {
            static T getT();

            enum
            {
                value = sizeof(is_const_reference_tester(getT())) == sizeof(yes_t)
            };
        };
    };

    template<class T>
    struct is_const_reference
        : is_const_reference_helper<boost::is_reference<T>::value>::template apply<T>
    {
        typedef boost::mpl::bool_<value> type;
    };

    template<class T>
    struct is_nonconst_reference
    {
        enum
        {
            value = boost::is_reference<T>::value && !is_const_reference<T>::value
        };
        typedef boost::mpl::bool_<value> type;
    };

#else

    template<class T>
    struct is_const_reference
    {
        enum { value = false };
        typedef boost::mpl::bool_<value> type;
    };

    template<class T>
    struct is_const_reference<const T&>
    {
        enum { value = true };
        typedef boost::mpl::bool_<value> type;
    };

    template<class T>
    struct is_nonconst_reference
    {
        enum { value = false };
        typedef boost::mpl::bool_<value> type;
    };

    template<class T>
    struct is_nonconst_reference<T&>
    {
        enum { value = !is_const_reference<T&>::value };
        typedef boost::mpl::bool_<value> type;
    };

#endif

    template<class A>
    yes_t is_const_pointer_helper(void(*)(const A*));
    no_t is_const_pointer_helper(...);

    template<class T>
    struct is_const_pointer
    {
        enum { value = sizeof(is_const_pointer_helper((void(*)(T))0)) == sizeof(yes_t) };
        typedef boost::mpl::bool_<value> type;
    };

    template<class A>
    yes_t is_nonconst_pointer_helper(void(*)(A*));
    no_t is_nonconst_pointer_helper(...);

    template<class T>
    struct is_nonconst_pointer
    {
        enum { value = sizeof(is_nonconst_pointer_helper((void(*)(T))0)) == sizeof(yes_t) && !is_const_pointer<T>::value };
        typedef boost::mpl::bool_<value> type;
    };
/*
    template<class T>
    struct is_constructable_from_helper
    {
        static yes_t check(const T&);
        static no_t check(...);
    };

    template<class T, class From>
    struct is_constructable_from
    {
        static From getFrom();

        enum
        {
            value = sizeof(is_constructable_from_helper<T>::check(getFrom())) == sizeof(yes_t)
        };
    };

    template<class T>
    struct is_const_member_function_helper
    {
        static no_t test(...);
        template<class R>
        static yes_t test(R(T::*)() const);
        template<class R, class A1>
        static yes_t test(R(T::*)(A1) const);
        template<class R, class A1, class A2>
        static yes_t test(R(T::*)(A1,A2) const);
        template<class R, class A1, class A2, class A3>
        static yes_t test(R(T::*)(A1,A2,A3) const);
    };

    template<class T, class U>
    struct is_const_member_function
    {
        static U getU();

        enum
        {
            value = sizeof(is_const_member_function_helper<T>::test(getU())) == sizeof(yes_t)
        };
    };
*/

    template<int v1, int v2>
    struct max_c
    {
        enum { value = (v1>v2)?v1:v2 };
    };

} // namespace detail
} // namespace luaponte

#endif // LUAPONTE_DETAIL_TYPETRAITS_HPP
