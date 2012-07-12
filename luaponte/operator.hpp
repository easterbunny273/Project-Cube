// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_OPERATOR_HPP
#define LUAPONTE_OPERATOR_HPP

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/apply_wrap.hpp>
#include <boost/preprocessor/repetition/enum_trailing.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/type_traits/is_same.hpp>
#include <luaponte/detail/other.hpp>
#include <luaponte/raw_policy.hpp>

#if defined(__GNUC__) && __GNUC__ < 3
# define LUAPONTE_NO_STRINGSTREAM
#else
# if defined(BOOST_NO_STRINGSTREAM)
#  define LUAPONTE_NO_STRINGSTREAM
# endif
#endif

#ifdef LUAPONTE_NO_STRINGSTREAM
#include <strstream>
#else
#include <sstream>
#endif

namespace luaponte {
namespace detail {

    template<class W, class T> struct unwrap_parameter_type;
    template<class Derived> struct operator_ {};

    struct operator_void_return {};

#if !BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
    template<class T>
    inline T const& operator,(T const& x, operator_void_return)
    {
        return x;
    }
#endif

}} // namespace luaponte

namespace luaponte { namespace operators {

   #define BOOST_PP_ITERATION_PARAMS_1 (3, \
       (0, LUAPONTE_MAX_ARITY, <luaponte/detail/call_operator_iterate.hpp>))
   #include BOOST_PP_ITERATE()

}} // namespace luaponte::operators

#include <boost/preprocessor/iteration/local.hpp>

namespace luaponte {

    template<class Derived>
    struct self_base
    {
        operators::call_operator0<Derived> operator()() const
        {
            return 0;
        }

#define BOOST_PP_LOCAL_MACRO(n) \
        template<BOOST_PP_ENUM_PARAMS(n, class A)> \
        BOOST_PP_CAT(operators::call_operator, n)< \
            Derived \
            BOOST_PP_ENUM_TRAILING_PARAMS(n, A) \
        >\
        operator()( \
            BOOST_PP_ENUM_BINARY_PARAMS(n, A, const& BOOST_PP_INTERCEPT) \
        ) const \
        { \
            return 0; \
        }

#define BOOST_PP_LOCAL_LIMITS (1, LUAPONTE_MAX_ARITY)
#include BOOST_PP_LOCAL_ITERATE()

    };

    struct self_type : self_base<self_type>
    {
    };

    struct const_self_type : self_base<const_self_type>
    {
    };

namespace detail {

    template<class W, class T>
    struct unwrap_parameter_type
    {
        typedef typename boost::mpl::eval_if<
            boost::is_same<T, self_type>
          , boost::mpl::identity<W&>
          , boost::mpl::eval_if<
                boost::is_same<T, const_self_type>
              , boost::mpl::identity<W const&>
              , unwrap_other<T>
            >
        >::type type;
    };

    template<class Derived, class A, class B>
    struct binary_operator
        : operator_<binary_operator<Derived, A, B> >
    {
        binary_operator(int) {}

        template<class T, class Policies>
        struct apply
        {
            typedef typename unwrap_parameter_type<T, A>::type arg0;
            typedef typename unwrap_parameter_type<T, B>::type arg1;

            static void execute(lua_State* L, arg0 _0, arg1 _1)
            {
                Derived::template apply<arg0, arg1, Policies>::execute(
                    L, _0, _1);
            }
        };

        static char const* name()
        {
            return Derived::name();
        }
    };

    template<class Derived, class A>
    struct unary_operator
        : operator_<unary_operator<Derived, A> >
    {
        unary_operator(int) {}

        template<class T, class Policies>
        struct apply
        {
            typedef typename unwrap_parameter_type<T, A>::type arg0;

            static void execute(lua_State* L, arg0 _0)
            {
                Derived::template apply<arg0, Policies>::execute(L, _0);
            }
        };

        static char const* name()
        {
            return Derived::name();
        }
    };

    template<class Policies>
    inline void operator_result(lua_State* L, operator_void_return, Policies*)
    {
    }

    namespace mpl = boost::mpl;

    template<class T, class Policies>
    inline void operator_result(lua_State* L, T const& x, Policies*)
    {
        typedef typename find_conversion_policy<
            0
          , Policies
        >::type cv_policy;

        typename mpl::apply_wrap2<cv_policy,T,cpp_to_lua>::type cv;

        cv.apply(L, x);
    }

}} // namespace detail::luabind

namespace luaponte {

#define LUAPONTE_BINARY_OPERATOR(name_, op) \
    namespace operators { \
\
        struct name_ \
        { \
            template<class T0, class T1, class Policies> \
            struct apply \
            { \
                static void execute(lua_State* L, T0 _0, T1 _1) \
                { \
                    detail::operator_result(L, _0 op _1, (Policies*)0); \
                } \
            }; \
\
            static char const* name() \
            { \
                return "__" # name_; \
            } \
        }; \
\
    } \
    \
    template<class T, class U> \
    detail::binary_operator< \
        operators::name_ \
      , U \
      , T \
    > \
    inline operator op(self_base<U>, T const&) \
    { \
        return 0; \
    } \
    \
    template<class T, class U> \
    detail::binary_operator< \
        operators::name_ \
      , T \
      , U \
    > \
    inline operator op(T const&, self_base<U>) \
    { \
        return 0; \
    } \
    \
    detail::binary_operator< \
        operators::name_ \
      , self_type \
      , self_type \
    > \
    inline operator op(self_type, self_type) \
    { \
        return 0; \
    } \
    \
    detail::binary_operator< \
        operators::name_ \
      , self_type \
      , const_self_type \
    > \
    inline operator op(self_type, const_self_type) \
    { \
        return 0; \
    } \
    \
    detail::binary_operator< \
        operators::name_ \
      , const_self_type \
      , self_type \
    > \
    inline operator op(const_self_type, self_type) \
    { \
        return 0; \
    } \
    \
    detail::binary_operator< \
        operators::name_ \
      , const_self_type \
      , const_self_type \
    > \
    inline operator op(const_self_type, const_self_type) \
    { \
        return 0; \
    }

    LUAPONTE_BINARY_OPERATOR(add, +)
    LUAPONTE_BINARY_OPERATOR(sub, -)
    LUAPONTE_BINARY_OPERATOR(mul, *)
    LUAPONTE_BINARY_OPERATOR(div, /)
    LUAPONTE_BINARY_OPERATOR(pow, ^)
    LUAPONTE_BINARY_OPERATOR(lt, <)
    LUAPONTE_BINARY_OPERATOR(le, <=)
    LUAPONTE_BINARY_OPERATOR(eq, ==)

#undef LUAPONTE_UNARY_OPERATOR

#define LUAPONTE_UNARY_OPERATOR(name_, op, fn) \
    namespace operators { \
\
        struct name_ \
        { \
            template<class T, class Policies> \
            struct apply \
            { \
                static void execute(lua_State* L, T x) \
                { \
                    detail::operator_result(L, op(x), (Policies*)0); \
                } \
            }; \
\
            static char const* name() \
            { \
                return "__" # name_; \
            } \
        }; \
\
    } \
    \
    template<class T> \
    detail::unary_operator< \
        operators::name_ \
      , T \
    > \
    inline fn(self_base<T>) \
    { \
        return 0; \
    }

    template<class T>
    std::string tostring_operator(T const& x)
    {
#ifdef LUAPONTE_NO_STRINGSTREAM
        std::strstream s;
        s << x << std::ends;
#else
        std::stringstream s;
        s << x;
#endif
        return s.str();
    }

    LUAPONTE_UNARY_OPERATOR(tostring, tostring_operator, tostring)
    LUAPONTE_UNARY_OPERATOR(unm, -, operator-)

#undef LUAPONTE_BINARY_OPERATOR

    namespace {

        LUAPONTE_ANONYMOUS_FIX self_type const self = self_type();
        LUAPONTE_ANONYMOUS_FIX const_self_type const const_self = const_self_type();

    } // namespace unnamed

} // namespace luaponte

#endif // LUAPONTE_OPERATOR_HPP
