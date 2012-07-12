// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#if !BOOST_PP_IS_ITERATING

# ifndef LUAPONTE_DETAIL_CONSTRUCTOR_HPP
#  define LUAPONTE_DETAIL_CONSTRUCTOR_HPP

#  include <luaponte/get_main_thread.hpp>
#  include <luaponte/object.hpp>
#  include <luaponte/wrapper_base.hpp>
#  include <luaponte/detail/inheritance.hpp>

#  include <boost/preprocessor/iteration/iterate.hpp>
#  include <boost/preprocessor/iteration/local.hpp>
#  include <boost/preprocessor/repetition/enum_params.hpp>
#  include <boost/preprocessor/repetition/enum_binary_params.hpp>
#  include <memory>

namespace luaponte {
namespace detail {

inline void inject_backref(lua_State*, void*, void*)
{}

template <class T>
void inject_backref(lua_State* L, T* p, wrap_base*)
{
    weak_ref(get_main_thread(L), L, 1).swap(wrap_access::ref(*p));
}

template <std::size_t Arity, class T, class Pointer, class Signature>
struct construct_aux;

template <class T, class Pointer, class Signature>
struct construct
  : construct_aux<mpl::size<Signature>::value - 2, T, Pointer, Signature>
{};

template <class T, class Pointer, class Signature>
struct construct_aux<0, T, Pointer, Signature>
{
    typedef pointer_holder<Pointer, T> holder_type;

    void operator()(argument const& self_) const
    {
        object_rep* self = touserdata<object_rep>(self_);

        std::unique_ptr<T> instance(new T);
        inject_backref(self_.interpreter(), instance.get(), instance.get());

        void* naked_ptr = instance.get();
        Pointer ptr(instance.release());

        void* storage = self->allocate(sizeof(holder_type));

        self->set_instance(new (storage) holder_type(
            std::move(ptr), registered_class<T>::id, naked_ptr));
    }
};

#  define BOOST_PP_ITERATION_PARAMS_1 \
    (3, (1, LUAPONTE_MAX_ARITY, <luaponte/detail/constructor.hpp>))
#  include BOOST_PP_ITERATE()

} // namespace detail
} // namespace luaponte

# endif // LUAPONTE_DETAIL_CONSTRUCTOR_HPP

#else // !BOOST_PP_IS_ITERATING

# define N BOOST_PP_ITERATION()

template <class T, class Pointer, class Signature>
struct construct_aux<N, T, Pointer, Signature>
{
    typedef typename mpl::begin<Signature>::type first;
    typedef typename mpl::next<first>::type iter0;

# define BOOST_PP_LOCAL_MACRO(n) \
    typedef typename mpl::next< \
        BOOST_PP_CAT(iter,BOOST_PP_DEC(n))>::type BOOST_PP_CAT(iter,n); \
    typedef typename BOOST_PP_CAT(iter,n)::type BOOST_PP_CAT(a,BOOST_PP_DEC(n));

# define BOOST_PP_LOCAL_LIMITS (1,N)
# include BOOST_PP_LOCAL_ITERATE()

    typedef pointer_holder<Pointer, T> holder_type;

    void operator()(argument const& self_, BOOST_PP_ENUM_BINARY_PARAMS(N,a,_)) const
    {
        object_rep* self = touserdata<object_rep>(self_);

        std::unique_ptr<T> instance(new T(BOOST_PP_ENUM_PARAMS(N,_)));
        inject_backref(self_.interpreter(), instance.get(), instance.get());

        void* naked_ptr = instance.get();
        Pointer ptr(instance.release());

        void* storage = self->allocate(sizeof(holder_type));

        self->set_instance(new (storage) holder_type(
            std::move(ptr), registered_class<T>::id, naked_ptr));
    }
};

# undef N

#endif
