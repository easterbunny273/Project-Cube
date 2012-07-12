// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_DETAIL_OPERATOR_ID_HPP
#define LUAPONTE_DETAIL_OPERATOR_ID_HPP

#include <luaponte/config.hpp>

namespace luaponte {
namespace detail {

enum operator_id
{
    op_add = 0,
    op_sub,
    op_mul,
    op_div,
    op_pow,
    op_lt,
    op_le,
    op_eq,
    op_call,
    op_unm,
    op_tostring,
    op_concat,
    op_len,

    number_of_operators
};

inline const char* get_operator_name(int i)
{
    static const char* a[number_of_operators] = {
        "__add", "__sub", "__mul", "__div", "__pow",
        "__lt", "__le", "__eq", "__call", "__unm",
        "__tostring", "__concat", "__len" };
    return a[i];
}

inline const char* get_operator_symbol(int i)
{
    static const char* a[number_of_operators] = {
        "+", "-", "*", "/", "^", "<",
        "<=", "==", "()", "- (unary)",
        "tostring", "..", "#" };
    return a[i];
}

inline bool is_unary(int i)
{
    // the reason why unary minus is not considered a unary operator here is
    // that it always is given two parameters, where the second parameter always
    // is nil.
    return i == op_tostring;
}

} // namespace detail
} // namespace luaponte

#endif // LUAPONTE_DETAIL_OPERATOR_ID_HPP
