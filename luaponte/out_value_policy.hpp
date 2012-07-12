// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_OUT_VALUE_POLICY_HPP
#define LUAPONTE_OUT_VALUE_POLICY_HPP

#include <luaponte/config.hpp>
#include <luaponte/detail/policy.hpp>
#include <boost/mpl/apply_wrap.hpp>

namespace luaponte {
namespace detail {

template<int N>
struct char_array
{
    char storage[N];
};

#if defined(__GNUC__) && ( __GNUC__ == 3 && __GNUC_MINOR__ == 1 )

template<class U>
char_array<sizeof(U)> indirect_sizeof_test(by_reference<U>);

template<class U>
char_array<sizeof(U)> indirect_sizeof_test(by_const_reference<U>);

template<class U>
char_array<sizeof(U)> indirect_sizeof_test(by_pointer<U>);

template<class U>
char_array<sizeof(U)> indirect_sizeof_test(by_const_pointer<U>);

template<class U>
char_array<sizeof(U)> indirect_sizeof_test(by_value<U>);

#else

template<class U>
char_array<sizeof(typename identity<U>::type)> indirect_sizeof_test(by_reference<U>);

template<class U>
char_array<sizeof(typename identity<U>::type)> indirect_sizeof_test(by_const_reference<U>);

template<class U>
char_array<sizeof(typename identity<U>::type)> indirect_sizeof_test(by_pointer<U>);

template<class U>
char_array<sizeof(typename identity<U>::type)> indirect_sizeof_test(by_const_pointer<U>);

template<class U>
char_array<sizeof(typename identity<U>::type)> indirect_sizeof_test(by_value<U>);

#endif

template<class T>
struct indirect_sizeof
{
    BOOST_STATIC_CONSTANT(int, value = sizeof(indirect_sizeof_test(LUAPONTE_DECORATE_TYPE(T))));
};

namespace mpl = boost::mpl;

template<int Size, class Policies = detail::null_type>
struct out_value_converter
{
    int consumed_args(...) const
    {
        return 1;
    }

    template<class T>
    T& apply(lua_State* L, by_reference<T>, int index)
    {
        typedef typename find_conversion_policy<1, Policies>::type converter_policy;
        typename mpl::apply_wrap2<converter_policy,T,lua_to_cpp>::type converter;
#if defined(__GNUC__) && __GNUC__ >= 4
        T* storage = reinterpret_cast<T*>(m_storage);
        new (storage) T(converter.apply(L, LUAPONTE_DECORATE_TYPE(T), index));
        return *storage;
#else
        new (m_storage) T(converter.apply(L, LUAPONTE_DECORATE_TYPE(T), index));
        return *reinterpret_cast<T*>(m_storage);
#endif
    }

    template<class T>
    static int match(lua_State* L, by_reference<T>, int index)
    {
        typedef typename find_conversion_policy<1, Policies>::type converter_policy;
        typedef typename mpl::apply_wrap2<converter_policy,T,lua_to_cpp>::type converter;
        return converter::match(L, LUAPONTE_DECORATE_TYPE(T), index);
    }

    template<class T>
    void converter_postcall(lua_State* L, by_reference<T>, int)
    {
        typedef typename find_conversion_policy<2, Policies>::type converter_policy;
        typename mpl::apply_wrap2<converter_policy,T,cpp_to_lua>::type converter;
#if defined(__GNUC__) && __GNUC__ >= 4
        T* storage = reinterpret_cast<T*>(m_storage);
        converter.apply(L, *storage);
        storage->~T();
#else
        converter.apply(L, *reinterpret_cast<T*>(m_storage));
        reinterpret_cast<T*>(m_storage)->~T();
#endif
    }

    template<class T>
    T* apply(lua_State* L, by_pointer<T>, int index)
    {
        typedef typename find_conversion_policy<1, Policies>::type converter_policy;
        typename mpl::apply_wrap2<converter_policy,T,lua_to_cpp>::type converter;
#if defined(__GNUC__) && __GNUC__ >= 4
        T* storage = reinterpret_cast<T*>(m_storage);
        new (storage) T(converter.apply(L, LUAPONTE_DECORATE_TYPE(T), index));
        return storage;
#else
        new (m_storage) T(converter.apply(L, LUAPONTE_DECORATE_TYPE(T), index));
        return reinterpret_cast<T*>(m_storage);
#endif
    }

    template<class T>
    static int match(lua_State* L, by_pointer<T>, int index)
    {
        typedef typename find_conversion_policy<1, Policies>::type converter_policy;
        typedef typename mpl::apply_wrap2<converter_policy,T,lua_to_cpp>::type converter;
        return converter::match(L, LUAPONTE_DECORATE_TYPE(T), index);
    }

    template<class T>
    void converter_postcall(lua_State* L, by_pointer<T>, int)
    {
        typedef typename find_conversion_policy<2, Policies>::type converter_policy;
        typename mpl::apply_wrap2<converter_policy,T,cpp_to_lua>::type converter;
#if defined(__GNUC__) && __GNUC__ >= 4
        T* storage = reinterpret_cast<T*>(m_storage);
        converter.apply(L, *storage);
        storage->~T();
#else
        converter.apply(L, *reinterpret_cast<T*>(m_storage));
        reinterpret_cast<T*>(m_storage)->~T();
#endif
    }

    char m_storage[Size];
};

template<int N, class Policies = detail::null_type>
struct out_value_policy : conversion_policy<N>
{
    static void precall(lua_State*, const index_map&) {}
    static void postcall(lua_State*, const index_map&) {}

    struct only_accepts_nonconst_references_or_pointers {};
    struct can_only_convert_from_lua_to_cpp {};

    template<class T, class Direction>
    struct apply
    {
        typedef typename boost::mpl::if_<boost::is_same<lua_to_cpp, Direction>
            , typename boost::mpl::if_<boost::mpl::or_<is_nonconst_reference<T>, is_nonconst_pointer<T> >
                , out_value_converter<indirect_sizeof<T>::value, Policies>
                , only_accepts_nonconst_references_or_pointers
                >::type
            , can_only_convert_from_lua_to_cpp
        >::type type;
    };
};

template<int Size, class Policies = detail::null_type>
struct pure_out_value_converter
{
    int consumed_args(...) const
    {
        return 0;
    }

    template<class T>
    T& apply(lua_State*, by_reference<T>, int)
    {
#if defined(__GNUC__) && __GNUC__ >= 4
        T* storage = reinterpret_cast<T*>(m_storage);
        new (storage) T();
        return *storage;
#else
        new (m_storage) T();
        return *reinterpret_cast<T*>(m_storage);
#endif
    }

    template<class T>
    static int match(lua_State*, by_reference<T>, int)
    {
        return 0;
    }

    template<class T>
    void converter_postcall(lua_State* L, by_reference<T>, int)
    {
        typedef typename find_conversion_policy<1, Policies>::type converter_policy;
        typename mpl::apply_wrap2<converter_policy,T,cpp_to_lua>::type converter;
#if defined(__GNUC__) && __GNUC__ >= 4
        T* storage = reinterpret_cast<T*>(m_storage);
        converter.apply(L, *storage);
        storage->~T();
#else
        converter.apply(L, *reinterpret_cast<T*>(m_storage));
        reinterpret_cast<T*>(m_storage)->~T();
#endif
    }

    template<class T>
    T* apply(lua_State*, by_pointer<T>, int)
    {
#if defined(__GNUC__) && __GNUC__ >= 4
        T* storage = reinterpret_cast<T*>(m_storage);
        new (storage) T();
        return storage;
#else
        new (m_storage) T();
        return reinterpret_cast<T*>(m_storage);
#endif
    }

    template<class T>
    static int match(lua_State*, by_pointer<T>, int)
    {
        return 0;
    }

    template<class T>
    void converter_postcall(lua_State* L, by_pointer<T>, int)
    {
        typedef typename find_conversion_policy<1, Policies>::type converter_policy;
        typename mpl::apply_wrap2<converter_policy,T,cpp_to_lua>::type converter;
#if defined(__GNUC__) && __GNUC__ >= 4
        T* storage = reinterpret_cast<T*>(m_storage);
        converter.apply(L, *storage);
        storage->~T();
#else
        converter.apply(L, *reinterpret_cast<T*>(m_storage));
        reinterpret_cast<T*>(m_storage)->~T();
#endif
    }


    char m_storage[Size];
};

template<int N, class Policies = detail::null_type>
struct pure_out_value_policy : conversion_policy<N, false>
{
    static void precall(lua_State*, const index_map&) {}
    static void postcall(lua_State*, const index_map&) {}

    struct only_accepts_nonconst_references_or_pointers {};
    struct can_only_convert_from_lua_to_cpp {};

    template<class T, class Direction>
    struct apply
    {
        typedef typename boost::mpl::if_<boost::is_same<lua_to_cpp, Direction>
            , typename boost::mpl::if_<boost::mpl::or_<is_nonconst_reference<T>, is_nonconst_pointer<T> >
                , pure_out_value_converter<indirect_sizeof<T>::value, Policies>
                , only_accepts_nonconst_references_or_pointers
                >::type
            , can_only_convert_from_lua_to_cpp
        >::type type;
    };
};

} // namespace detail
} // namespace luaponte

namespace luaponte {

template<int N>
detail::policy_cons<detail::out_value_policy<N>, detail::null_type>
out_value(LUAPONTE_PLACEHOLDER_ARG(N))
{
    return detail::policy_cons<detail::out_value_policy<N>, detail::null_type>();
}

template<int N, class Policies>
detail::policy_cons<detail::out_value_policy<N, Policies>, detail::null_type>
out_value(LUAPONTE_PLACEHOLDER_ARG(N), const Policies&)
{
    return detail::policy_cons<detail::out_value_policy<N, Policies>, detail::null_type>();
}

template<int N>
detail::policy_cons<detail::pure_out_value_policy<N>, detail::null_type>
pure_out_value(LUAPONTE_PLACEHOLDER_ARG(N))
{
    return detail::policy_cons<detail::pure_out_value_policy<N>, detail::null_type>();
}

template<int N, class Policies>
detail::policy_cons<detail::pure_out_value_policy<N, Policies>, detail::null_type>
pure_out_value(LUAPONTE_PLACEHOLDER_ARG(N), const Policies&)
{
    return detail::policy_cons<detail::pure_out_value_policy<N, Policies>, detail::null_type>();
}

} // namespace luaponte

#endif // LUAPONTE_OUT_VALUE_POLICY_HPP
