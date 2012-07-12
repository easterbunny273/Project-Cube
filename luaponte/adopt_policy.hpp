// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_ADOPT_POLICY_HPP
#define LUAPONTE_ADOPT_POLICY_HPP

#include <luaponte/config.hpp>
#include <luaponte/wrapper_base.hpp>
#include <luaponte/detail/policy.hpp>
#include <luaponte/back_reference_fwd.hpp>
#include <luaponte/wrapper_base.hpp>

#include <boost/type_traits/is_polymorphic.hpp>
#include <memory>

namespace luaponte {
namespace detail {

template <class T>
void adjust_backref_ownership(T* ptr, mpl::true_)
{
    if (wrap_base* p = dynamic_cast<wrap_base*>(ptr))
    {
        wrapped_self_t& wrapper = wrap_access::ref(*p);
        wrapper.get(wrapper.state());
        wrapper.m_strong_ref.set(wrapper.state());
    }
}

inline void adjust_backref_ownership(void*, mpl::false_)
{}

template <class Pointer, class Direction = lua_to_cpp>
struct adopt_pointer : pointer_converter
{
    typedef adopt_pointer type;

    int consumed_args(...) const
    {
        return 1;
    }

    template<class T>
    T* apply(lua_State* L, by_pointer<T>, int index)
    {
        T* ptr = pointer_converter::apply(
            L, LUAPONTE_DECORATE_TYPE(T*), index);

        object_rep* obj = static_cast<object_rep*>(
            lua_touserdata(L, index));
        obj->release();

        adjust_backref_ownership(ptr, boost::is_polymorphic<T>());

        return ptr;
    }

    template<class T>
    int match(lua_State* L, by_pointer<T>, int index)
    {
        return pointer_converter::match(
            L, LUAPONTE_DECORATE_TYPE(T*), index);
    }

    template<class T>
    void converter_postcall(lua_State*, T, int) {}
};

template <class Pointer, class T>
struct pointer_or_default
{
    typedef Pointer type;
};

template <class T>
struct pointer_or_default<void, T>
{
    typedef std::unique_ptr<T> type;
};

template <class Pointer>
struct adopt_pointer<Pointer, cpp_to_lua>
{
    typedef adopt_pointer type;

    template<class T>
    void apply(lua_State* L, T* ptr)
    {
        if (ptr == 0)
        {
            lua_pushnil(L);
            return;
        }

        // if there is a back_reference, then the
        // ownership will be removed from the
        // back reference and put on the lua stack.
        if (luaponte::move_back_reference(L, ptr))
            return;

        typedef typename pointer_or_default<Pointer, T>::type
            pointer_type;

        make_instance(L, pointer_type(ptr));
    }
};

template <int N, class Pointer = void>
struct adopt_policy : conversion_policy<N>
{
    static void precall(lua_State*, const index_map&) {}
    static void postcall(lua_State*, const index_map&) {}

    struct only_accepts_nonconst_pointers {};

    template<class T, class Direction>
    struct apply
    {
        typedef luaponte::detail::is_nonconst_pointer<T> is_nonconst_p;
        typedef typename boost::mpl::if_<
            is_nonconst_p
          , adopt_pointer<Pointer, Direction>
          , only_accepts_nonconst_pointers
        >::type type;
    };
};

} // namespace detail
} // namespace luaponte

namespace luaponte {

template<int N>
detail::policy_cons<detail::adopt_policy<N>, detail::null_type>
adopt(LUAPONTE_PLACEHOLDER_ARG(N))
{
    return detail::policy_cons<detail::adopt_policy<N>, detail::null_type>();
}

template <class Pointer, int N>
detail::policy_cons<detail::adopt_policy<N, Pointer>, detail::null_type>
adopt(LUAPONTE_PLACEHOLDER_ARG(N))
{
    return detail::policy_cons<detail::adopt_policy<N, Pointer>, detail::null_type>();
}

} // namespace luaponte

#endif // LUAPONTE_ADOPT_POLICY_HPP_INCLUDE
