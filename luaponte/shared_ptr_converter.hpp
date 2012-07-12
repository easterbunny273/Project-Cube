// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_SHARED_PTR_CONVERTER_HPP
#define LUAPONTE_SHARED_PTR_CONVERTER_HPP

#include <luaponte/get_main_thread.hpp>
#include <luaponte/handle.hpp>
#include <luaponte/detail/policy.hpp>

#include <memory>

namespace luaponte {

namespace detail {

  struct shared_ptr_deleter
  {
      shared_ptr_deleter(lua_State* L, int index)
        : life_support(get_main_thread(L), L, index)
      {}

      void operator()(void const*)
      {
          handle().swap(life_support);
      }

      handle life_support;
  };

} // namespace detail

template <class T>
struct default_converter<std::shared_ptr<T> >
  : default_converter<T*>
{
    typedef boost::mpl::false_ is_native;

    template <class U>
    int match(lua_State* L, U, int index)
    {
        return default_converter<T*>::match(
            L, LUAPONTE_DECORATE_TYPE(T*), index);
    }

    template <class U>
    std::shared_ptr<T> apply(lua_State* L, U, int index)
    {
        T* raw_ptr = default_converter<T*>::apply(
            L, LUAPONTE_DECORATE_TYPE(T*), index);
        if (!raw_ptr)
            return std::shared_ptr<T>();
        return std::shared_ptr<T>(
            raw_ptr, detail::shared_ptr_deleter(L, index));
    }

    void apply(lua_State* L, std::shared_ptr<T> const& p)
    {
        if (detail::shared_ptr_deleter* d =
                std::get_deleter<detail::shared_ptr_deleter>(p))
        {
            d->life_support.push(L);
        }
        else
        {
            detail::value_converter().apply(L, p);
        }
    }

    template <class U>
    void converter_postcall(lua_State*, U const&, int)
    {}
};

template <class T>
struct default_converter<std::shared_ptr<T> const&>
  : default_converter<std::shared_ptr<T> >
{};

#ifdef BOOST_HAS_RVALUE_REFS
template <class T>
struct default_converter<std::shared_ptr<T>&&>
  : default_converter<std::shared_ptr<T> >
{};
#endif

} // namespace luaponte

#endif // LUAPONTE_SHARED_PTR_CONVERTER_HPP
