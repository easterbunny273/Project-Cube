// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_DETAIL_SIGNATURE_MATCH_HPP
#define LUAPONTE_DETAIL_SIGNATURE_MATCH_HPP

#include <luaponte/config.hpp>

#include <boost/config.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/remove.hpp>

namespace luaponte {

namespace adl {

class argument;

} // namespace adl

template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(LUAPONTE_MAX_ARITY, class A, detail::null_type)>
struct constructor
{
    typedef BOOST_PP_CAT(
        boost::mpl::vector, BOOST_PP_INC(BOOST_PP_INC(LUAPONTE_MAX_ARITY)))<
            void, argument const&, BOOST_PP_ENUM_PARAMS(LUAPONTE_MAX_ARITY, A)
    > signature0;

    typedef typename boost::mpl::remove<
        signature0, detail::null_type>::type signature;
};

} // namespace luaponte

#endif // LUAPONTE_DETAIL_SIGNATURE_MATCH_HPP
