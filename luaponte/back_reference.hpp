// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_BACK_REFERENCE_HPP
#define LUAPONTE_BACK_REFERENCE_HPP

#include <luaponte/lua_include.hpp>
#include <luaponte/wrapper_base.hpp>
#include <luaponte/detail/has_get_pointer.hpp>
#include <luaponte/get_pointer.hpp>

#include <boost/type_traits/is_polymorphic.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/mpl/if.hpp>
#include <type_traits>

namespace luaponte {
namespace detail {

  namespace mpl = boost::mpl;

  template<class T>
  wrap_base const* get_back_reference_aux0(T const* p, mpl::true_)
  {
      return dynamic_cast<wrap_base const*>(p);
  }

  template<class T>
  wrap_base const* get_back_reference_aux0(T const*, mpl::false_)
  {
      return 0;
  }

  template<class T>
  wrap_base const* get_back_reference_aux1(T const* p)
  {
      return get_back_reference_aux0(p, boost::is_polymorphic<T>());
  }

  template<class T>
  typename std::enable_if<has_get_pointer<T>::value, wrap_base const*>::type
  get_back_reference(T const& x)
  {
      return get_back_reference_aux1(get_pointer(x));
  }

  template<class T>
  typename std::enable_if<!has_get_pointer<T>::value, wrap_base const*>::type
  get_back_reference(T const& x)
  {
      return get_back_reference_aux1(&x);
  }

} // namespace detail

template<class T>
bool get_back_reference(lua_State* L, T const& x)
{
#ifndef LUAPONTE_NO_RTTI
    if (wrap_base const* w = detail::get_back_reference(x))
    {
        detail::wrap_access::ref(*w).get(L);
        return true;
    }
#endif
    return false;
}

template<class T>
bool move_back_reference(lua_State* L, T const& x)
{
#ifndef LUAPONTE_NO_RTTI
    if (wrap_base* w = const_cast<wrap_base*>(detail::get_back_reference(x)))
    {
        assert(detail::wrap_access::ref(*w).m_strong_ref.is_valid());
        detail::wrap_access::ref(*w).get(L);
        detail::wrap_access::ref(*w).m_strong_ref.reset();
        return true;
    }
#endif
    return false;
}

} // namespace luaponte

#endif // LUAPONTE_BACK_REFERENCE_HPP
