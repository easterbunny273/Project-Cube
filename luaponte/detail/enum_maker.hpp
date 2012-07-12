// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_DETAIL_ENUM_MAKER_HPP
#define LUAPONTE_DETAIL_ENUM_MAKER_HPP

#include <vector>
#include <string>

#include <luaponte/config.hpp>
#include <luaponte/detail/class_rep.hpp>

namespace luaponte {

struct value;

struct value_vector : public std::vector<value>
{
    // a bug in intel's compiler forces us to declare these constructors explicitly.
    value_vector();
    virtual ~value_vector();
    value_vector(const value_vector& v);
    value_vector& operator,(const value& rhs);
};

struct value
{
friend class std::vector<value>;
    template<class T>
    value(const char* name, T v)
        : name_(name)
        , val_(v)
    {}

    const char* name_;
    int val_;

    value_vector operator,(const value& rhs) const
    {
        value_vector v;

        v.push_back(*this);
        v.push_back(rhs);

        return v;
    }

private:

    value() {}
};

inline value_vector::value_vector()
    : std::vector<value>()
{
}

inline value_vector::~value_vector() {}

inline value_vector::value_vector(const value_vector& rhs)
    : std::vector<value>(rhs)
{
}

inline value_vector& value_vector::operator,(const value& rhs)
{
    push_back(rhs);
    return *this;
}

namespace detail {

template<class From>
struct enum_maker
{
    explicit enum_maker(From& from): from_(from) {}

    From& operator[](const value& val)
    {
        from_.add_static_constant(val.name_, val.val_);
        return from_;
    }

    From& operator[](const value_vector& values)
    {
        for (value_vector::const_iterator i = values.begin(); i != values.end(); ++i)
        {
            from_.add_static_constant(i->name_, i->val_);
        }

        return from_;
    }

    From& from_;

private:
    void operator=(enum_maker const&); // C4512, assignment operator could not be generated
    template<class T> void operator,(T const&) const;
};

} // namespace detail
} // namespace luaponte

#endif // LUAPONTE_DETAIL_ENUM_MAKER_HPP
