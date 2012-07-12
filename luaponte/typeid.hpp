// Luaponte library

// Copyright (c) 2011-2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_TYPEID_HPP
# define LUAPONTE_TYPEID_HPP

# include <boost/operators.hpp>
# include <cstdlib>
# include <cstring>
# include <typeinfo>
# include <luaponte/detail/primitives.hpp>

// boost/units/detail/utility.hpp
# if defined(__GLIBCXX__) || defined(__GLIBCPP__)
#  define LUAPONTE_USE_DEMANGLING
#  include <cxxabi.h>
# endif // __GNUC__

namespace luaponte {

# ifdef BOOST_MSVC
#  pragma warning(push)
// std::type_info::before() returns int, rather than bool.
// At least on MSVC7.1, this is true for the comparison
// operators as well.
#  pragma warning(disable:4800)
# endif

class type_id
  : public boost::less_than_comparable<type_id>
{
public:
    type_id()
      : id(&typeid(detail::null_type))
    {}

    type_id(std::type_info const& id)
      : id(&id)
    {}

    bool operator!=(type_id const& other) const
    {
        return std::strcmp(id->name(), other.id->name()) != 0;
    }

    bool operator==(type_id const& other) const
    {
        return std::strcmp(id->name(), other.id->name()) == 0;
    }

    bool operator<(type_id const& other) const
    {
        return std::strcmp(id->name(), other.id->name()) < 0;
    }

    std::string name() const
    {
# ifdef LUAPONTE_USE_DEMANGLING
        int status;
        char* buf = abi::__cxa_demangle(id->name(), 0, 0, &status);
        if (buf != 0) {
            std::string name(buf);
            std::free(buf);
            return name;
        }
# endif
        return id->name();
    }

private:
    std::type_info const* id;
};

# ifdef BOOST_MSVC
#  pragma warning(pop)
# endif

} // namespace luaponte

#endif // LUAPONTE_TYPEID_HPP
