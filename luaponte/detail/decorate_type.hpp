// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_DETAIL_DECORATE_TYPE_HPP
#define LUAPONTE_DETAIL_DECORATE_TYPE_HPP

#include <luaponte/config.hpp>
#include <luaponte/detail/primitives.hpp>

namespace luaponte {
namespace detail {

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

    template<class T>
    struct decorated_type
    {
        static by_value<T> t;
        static inline by_value<T>& get() { return /*by_value<T>()*/t; }
    };

    template<class T>
    by_value<T> decorated_type<T>::t;

    template<class T>
    struct decorated_type<T*>
    {
        static by_pointer<T> t;
        static inline by_pointer<T>& get() { return /*by_pointer<T>()*/t; }
    };

    template<class T>
    by_pointer<T> decorated_type<T*>::t;

    template<class T>
    struct decorated_type<const T*>
    {
        static by_const_pointer<T> t;
        static inline by_const_pointer<T> get() { return /*by_const_pointer<T>()*/t; }
    };

    template<class T>
    by_const_pointer<T> decorated_type<const T*>::t;

    template<class T>
    struct decorated_type<const T* const>
    {
        static by_const_pointer<T> t;
        static inline by_const_pointer<T>& get() { return /*by_const_pointer<T>()*/t; }
    };

    template<class T>
    by_const_pointer<T> decorated_type<const T* const>::t;

    template<class T>
    struct decorated_type<T&>
    {
        static by_reference<T> t;
        static inline by_reference<T>& get() { return /*by_reference<T>()*/t; }
    };

    template<class T>
    by_reference<T> decorated_type<T&>::t;

    template<class T>
    struct decorated_type<const T&>
    {
        static by_const_reference<T> t;
        static inline by_const_reference<T>& get() { return /*by_const_reference<T>()*/t; }
    };

    template<class T>
    by_const_reference<T> decorated_type<const T&>::t;

#ifdef BOOST_HAS_RVALUE_REFS
    template<class T>
    struct decorated_type<T&&>
    {
        static by_value<T> t;
        static inline by_value<T>& get() { return /*by_value<T>()*/t; }
    };

    template<class T>
    by_value<T> decorated_type<T&&>::t;
#endif

    #define LUAPONTE_DECORATE_TYPE(t) luaponte::detail::decorated_type<t>::get()

#else

#include <boost/type_traits/is_array.hpp>

    namespace
    {
        LUAPONTE_ANONYMOUS_FIX char decorated_type_array[64];
    }

    template<class T>
    struct decorated_type_cref_impl
    {
#if defined(BOOST_MSVC) && BOOST_MSVC == 1200
        template<class U>
        static by_const_reference<U> get(const U&)
        {
            return by_const_reference<U>();
        }
        static T data() { return reinterpret_cast<T>(decorated_type_array); }
#else

        static void(*data())(T)
        { return (void(*)(T))0; }

        template<class U>
        static by_const_reference<U> get(void(*f)(const U&))
        { return by_const_reference<U>(); }
#endif
    };

    template<class T>
    struct decorated_type_ref_impl
    {
#if defined(BOOST_MSVC) && BOOST_MSVC == 1200
        template<class U>
        static by_reference<U> get(U&)
        {
            return by_reference<U>();
        }
        static T data() { return reinterpret_cast<T>(decorated_type_array); }
#else
        static void(*data())(T)
        { return (void(*)(T))0; }

        template<class U>
        static by_reference<U> get(void(*)(U&))
        { return by_reference<U>(); }
#endif
    };

    template<class T>
    struct decorated_type_cptr_impl
    {
#if defined(BOOST_MSVC) && BOOST_MSVC == 1200
        template<class U>
        static by_const_pointer<U> get(const U*)
        {
            return by_const_pointer<U>();
        }
        static T& data() { return reinterpret_cast<T&>(decorated_type_array); }
#else
        static void(*data())(T)
        { return (void(*)(T))0; }

        template<class U>
        static by_const_pointer<U> get(void(*)(const U*))
        { return by_const_pointer<U>(); }
#endif
    };

    template<class T>
    struct decorated_type_ptr_impl
    {
#if defined(BOOST_MSVC) && BOOST_MSVC == 1200
        template<class U>
        static by_pointer<U> get(U*)
        {
            return by_pointer<U>();
        }
        static T& data() { return reinterpret_cast<T&>(decorated_type_array); }
#else
        static void(*data())(T)
        { return (void(*)(T))0; }

        template<class U>
        static by_pointer<U> get(void(*)(U*))
        { return by_pointer<U>(); }
#endif
    };

    template<class T>
    struct decorated_type_value_impl
    {
#if defined(BOOST_MSVC) && BOOST_MSVC == 1200
        template<class U>
        static by_value<U> get(U&)
        {
            return by_value<U>();
        }
        static T& data() { return reinterpret_cast<T&>(decorated_type_array); }
#else
        static void(*data())(T&)
        { return (void(*)(T&))0; }

        template<class U>
        static by_value<U> get(void(*)(U&))
        { return by_value<U>(); }
#endif
    };

    template<>
    struct decorated_type_value_impl<void>
    {
        static by_value<void> get(int)
        {
            return by_value<void>();
        }
        static int data() { return 0; }
    };

    template<class T>
    struct decorated_type_array_impl
    {
        template<class U>
        static by_pointer<U> get(U*)
        {
            return by_pointer<U>();
        }

        template<class U>
        static by_pointer<U> get(void(*)(U))
        { return by_pointer<U>(); }

        static T& data() { return reinterpret_cast<T&>(decorated_type_array); }
    };

    template<class T>
    struct decorated_type
        :       boost::mpl::if_<luaponte::detail::is_const_reference<T>
                    , decorated_type_cref_impl<T>
                    , typename boost::mpl::if_<luaponte::detail::is_nonconst_reference<T>
                        , decorated_type_ref_impl<T>
                        , typename boost::mpl::if_<luaponte::detail::is_nonconst_pointer<T>
                            , decorated_type_ptr_impl<T>
                            , typename boost::mpl::if_<luaponte::detail::is_const_pointer<T>
                                , decorated_type_cptr_impl<T>
                                , decorated_type_value_impl<T>
                    >::type
                >::type
            >::type
        >::type
    {
    };

#if defined(BOOST_MSVC) && BOOST_MSVC == 1200
    #define LUAPONTE_DECORATE_TYPE(t) luaponte::detail::decorated_type<t>::get(luaponte::detail::decorated_type<t>::data())
#else
    #define LUAPONTE_DECORATE_TYPE(t) luaponte::detail::decorated_type<t>::get(luaponte::detail::decorated_type<t>::data())
#endif

#endif

} // namespace detail
} // namespace luaponte

#endif // LUAPONTE_DETAIL_DECORATE_TYPE_HPP
