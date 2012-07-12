// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_COPY_POLICY_HPP
# define LUAPONTE_COPY_POLICY_HPP

# include <luaponte/detail/policy.hpp>

namespace luaponte {

namespace detail {

  struct copy_converter
  {
      template <class T>
      void apply(lua_State* L, T const& x)
      {
          value_converter().apply(L, x);
      }

      template <class T>
      void apply(lua_State* L, T* x)
      {
          if (!x)
              lua_pushnil(L);
          else
              apply(L, *x);
      }
  };

  template <int N>
  struct copy_policy : conversion_policy<N>
  {
      static void precall(lua_State*, index_map const&)
      {}

      static void postcall(lua_State*, index_map const&)
      {}

      template <class T, class Direction>
      struct apply
      {
          typedef copy_converter type;
      };
  };

} // namespace detail

template <int N>
detail::policy_cons<detail::copy_policy<N>, detail::null_type>
copy(LUAPONTE_PLACEHOLDER_ARG(N))
{
    return detail::policy_cons<detail::copy_policy<N>, detail::null_type>();
}

} // namespace luaponte

#endif // LUAPONTE_COPY_POLICY_HPP
