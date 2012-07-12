// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_DETAIL_INSTANCE_HOLDER_HPP
#define LUAPONTE_DETAIL_INSTANCE_HOLDER_HPP

#include <luaponte/detail/inheritance.hpp>
#include <luaponte/get_pointer.hpp>
#include <luaponte/typeid.hpp>

#include <boost/type_traits/is_polymorphic.hpp>
#include <memory>
#include <stdexcept>

namespace luaponte {
namespace detail {

class instance_holder
{
public:
    instance_holder(bool pointee_const)
      : m_pointee_const(pointee_const)
    {}

    virtual ~instance_holder()
    {}

    virtual std::pair<void*, int> get(
        cast_graph const& casts, class_id target) const = 0;

    virtual void release() = 0;

    bool pointee_const() const
    {
        return m_pointee_const;
    }

private:
    bool m_pointee_const;
};

namespace mpl = boost::mpl;

inline mpl::false_ check_const_pointer(void*)
{
    return mpl::false_();
}

inline mpl::true_ check_const_pointer(void const*)
{
    return mpl::true_();
}

template <class T>
void release_ownership(std::unique_ptr<T>& p)
{
    p.release();
}

template <class P>
void release_ownership(P const&)
{
    throw std::runtime_error(
        "luabind: smart pointer does not allow ownership transfer");
}

template <class T>
class_id static_class_id(T*)
{
    return registered_class<T>::id;
}

template <class P, class Pointee = void const>
class pointer_holder : public instance_holder
{
public:
    pointer_holder(
        P p, class_id dynamic_id, void* dynamic_ptr
    )
      : instance_holder(check_const_pointer(false ? get_pointer(p) : 0))
      , p(std::move(p))
      , weak(0)
      , dynamic_id(dynamic_id)
      , dynamic_ptr(dynamic_ptr)
    {}

    std::pair<void*, int> get(cast_graph const& casts, class_id target) const
    {
        if (target == registered_class<P>::id)
            return std::pair<void*, int>(&this->p, 0);

        void* naked_ptr = const_cast<void*>(static_cast<void const*>(
            weak ? weak : get_pointer(p)));

        if (!naked_ptr)
            return std::pair<void*, int>((void*)0, 0);

        return casts.cast(
            naked_ptr
          , static_class_id(false ? get_pointer(p) : 0)
          , target
          , dynamic_id
          , dynamic_ptr
        );
    }

    void release()
    {
        weak = const_cast<void*>(static_cast<void const*>(
            get_pointer(p)));
        release_ownership(p);
    }

private:
    mutable P p;
    // weak will hold a possibly stale pointer to the object owned
    // by p once p has released it's owership. This is a workaround
    // to make adopt() work with virtual function wrapper classes.
    void* weak;
    class_id dynamic_id;
    void* dynamic_ptr;
};

} // namespace detail
} // namespace luaponte

#endif // LUAPONTE_DETAIL_INSTANCE_HOLDER_HPP
