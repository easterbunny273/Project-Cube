// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define N BOOST_PP_ITERATION()

#define LUAPONTE_UNWRAP_PARAMETER(z, n, _) \
    typename detail::unwrap_parameter_type<T, BOOST_PP_CAT(A, n)>::type \
        BOOST_PP_CAT(_, n)

template<class Self BOOST_PP_ENUM_TRAILING_PARAMS(N, class A)>
struct BOOST_PP_CAT(call_operator, N)
    : detail::operator_<
          BOOST_PP_CAT(call_operator, N)<
              Self BOOST_PP_ENUM_TRAILING_PARAMS(N, A)
          >
      >
{
    BOOST_PP_CAT(call_operator, N)(int) {}

    template<class T, class Policies>
    struct apply
    {
        static void execute(
            lua_State* L
          , typename detail::unwrap_parameter_type<T, Self>::type self
            BOOST_PP_ENUM_TRAILING(N, LUAPONTE_UNWRAP_PARAMETER, _)
        )
        {
            using namespace detail;
            operator_result(
                L
#if BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
              , self(BOOST_PP_ENUM_PARAMS(N, _))
#else
              , (self(BOOST_PP_ENUM_PARAMS(N, _)), detail::operator_void_return())
#endif
              , (Policies*)0
            );
        }
    };

    static char const* name() { return "__call"; }
};

#undef LUAPONTE_UNWRAP_PARAMETER
#undef N
