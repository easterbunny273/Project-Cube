// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_DEPENDENCY_POLICY_HPP
#define LUAPONTE_DEPENDENCY_POLICY_HPP

#include <luaponte/config.hpp>
#include <luaponte/detail/policy.hpp>

namespace luaponte {
namespace detail {

// makes A dependent on B, meaning B will outlive A.
// internally A stores a reference to B
template<int A, int B>
struct dependency_policy
{
    static void postcall(lua_State* L, const index_map& indices)
    {
        int nurse_index = indices[A];
        int patient = indices[B];

        object_rep* nurse = static_cast<object_rep*>(lua_touserdata(L, nurse_index));

        // If the nurse isn't an object_rep, just make this a nop.
        if (nurse == 0)
            return;

        nurse->add_dependency(L, patient);
    }
};

} // namespace detail
} // namespace luaponte

#if defined (BOOST_MSVC) && (BOOST_MSVC <= 1200)

namespace luaponte {

// most absurd workaround of all time?
namespace detail {

template<int N>
struct size_char_array
{
    char storage[N + 2];
};

template<int N>
size_char_array<N> deduce_size(LUAPONTE_PLACEHOLDER_ARG(N));

template<class T>
struct get_index_workaround
{
    static T t;
    BOOST_STATIC_CONSTANT(int, value = sizeof(deduce_size(t)) - 2);
};

} // namespace detail

template<class A, class B>
detail::policy_cons<detail::dependency_policy<detail::get_index_workaround<A>::value
    , detail::get_index_workaround<B>::value>, detail::null_type> dependency(A,B)
{
    return detail::policy_cons<detail::dependency_policy<
        detail::get_index_workaround<A>::value, detail::get_index_workaround<B>::value>
        , detail::null_type>();
}

template<class A>
detail::policy_cons<detail::dependency_policy<0
    , detail::get_index_workaround<A>::value>, detail::null_type>
return_internal_reference(A)
{
    return detail::policy_cons<detail::dependency_policy<0
        , detail::get_index_workaround<A>::value>, detail::null_type>();
}

} // namespace luaponte

#else

namespace luaponte {

template<int A, int B>
detail::policy_cons<detail::dependency_policy<A, B>, detail::null_type>
dependency(LUAPONTE_PLACEHOLDER_ARG(A), LUAPONTE_PLACEHOLDER_ARG(B))
{
    return detail::policy_cons<detail::dependency_policy<A, B>, detail::null_type>();
}

template<int A>
detail::policy_cons<detail::dependency_policy<0, A>, detail::null_type>
return_internal_reference(LUAPONTE_PLACEHOLDER_ARG(A))
{
    return detail::policy_cons<detail::dependency_policy<0, A>, detail::null_type>();
}

} // namespace luaponte

#endif

#endif // LUAPONTE_DEPENDENCY_POLICY_HPP
