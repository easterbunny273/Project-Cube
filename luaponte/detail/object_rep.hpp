// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUAPONTE_DETAIL_OBJECT_REP_HPP
#define LUAPONTE_DETAIL_OBJECT_REP_HPP

#include <boost/aligned_storage.hpp>
#include <luaponte/config.hpp>
#include <luaponte/detail/class_rep.hpp>
#include <luaponte/detail/instance_holder.hpp>
#include <luaponte/detail/ref.hpp>

namespace luaponte {
namespace detail {

void finalize(lua_State* L, class_rep* crep);

// this class is allocated inside lua for each pointer.
// it contains the actual c++ object-pointer.
// it also tells if it is const or not.
class LUAPONTE_API object_rep
{
public:
    object_rep(instance_holder* instance, class_rep* crep);
    ~object_rep();

    const class_rep* crep() const { return m_classrep; }
    class_rep* crep() { return m_classrep; }

    void set_instance(instance_holder* instance) { m_instance = instance; }

    void add_dependency(lua_State* L, int index);
    void release_dependency_refs(lua_State* L);

    std::pair<void*, int> get_instance(class_id target) const
    {
        if (m_instance == 0)
            return std::pair<void*, int>((void*)0, -1);
        return m_instance->get(m_classrep->casts(), target);
    }

    bool is_const() const
    {
        return m_instance && m_instance->pointee_const();
    }

    void release()
    {
        if (m_instance)
            m_instance->release();
    }

    void* allocate(std::size_t size)
    {
        if (size <= 32)
            return &m_instance_buffer;
        return std::malloc(size);
    }

    void deallocate(void* storage)
    {
        if (storage == &m_instance_buffer)
            return;
        std::free(storage);
    }

private:

object_rep(object_rep const&)
{}

void operator=(object_rep const&)
{}

    instance_holder* m_instance;
    boost::aligned_storage<32> m_instance_buffer;
    class_rep* m_classrep; // the class information about this object's type
    std::size_t m_dependency_cnt; // counts dependencies
};

template<class T>
struct delete_s
{
    static void apply(void* ptr)
    {
        delete static_cast<T*>(ptr);
    }
};

template<class T>
struct destruct_only_s
{
    static void apply(void* ptr)
    {
        // Removes unreferenced formal parameter warning on VC7.
        (void)ptr;
#ifndef NDEBUG
        int completeness_check[sizeof(T)];
        (void)completeness_check;
#endif
        static_cast<T*>(ptr)->~T();
    }
};

LUAPONTE_API object_rep* get_instance(lua_State* L, int index);
LUAPONTE_API void push_instance_metatable(lua_State* L);
LUAPONTE_API object_rep* push_new_instance(lua_State* L, class_rep* cls);

} // namespace detail
} // namespace luaponte

#endif // LUAPONTE_DETAIL_OBJECT_REP_HPP
