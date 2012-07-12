// Luaponte library

// Copyright (c) 2012 Peter Colberg

// Luaponte is based on Luabind, a library, inspired by and similar to
// Boost.Python, that helps you create bindings between C++ and Lua,
// Copyright (c) 2003-2010 Daniel Wallin and Arvid Norberg.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#if !BOOST_PP_IS_ITERATING

# ifndef LUAPONTE_DETAIL_CALL_HPP
#  define LUAPONTE_DETAIL_CALL_HPP

#  include <boost/mpl/apply_wrap.hpp>
#  include <boost/mpl/begin_end.hpp>
#  include <boost/mpl/deref.hpp>
#  include <boost/mpl/front.hpp>
#  include <boost/mpl/long.hpp>
#  include <boost/mpl/size.hpp>
#  include <boost/preprocessor/control/if.hpp>
#  include <boost/preprocessor/iteration/iterate.hpp>
#  include <boost/preprocessor/iteration/local.hpp>
#  include <boost/preprocessor/repetition/enum.hpp>
#  include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#  include <boost/type_traits/is_void.hpp>

#  include <luaponte/config.hpp>
#  include <luaponte/detail/policy.hpp>
#  include <luaponte/yield_policy.hpp>

namespace luaponte {
namespace detail {

struct invoke_context;

struct LUAPONTE_API function_object
{
    function_object(lua_CFunction entry)
      : entry(entry)
      , next(0)
    {}

    virtual ~function_object()
    {}

    virtual int call(
        lua_State* L, invoke_context& ctx) const = 0;
    virtual void format_signature(lua_State* L, char const* function) const = 0;

    lua_CFunction entry;
    std::string name;
    function_object* next;
    object keepalive;
};

struct LUAPONTE_API invoke_context
{
    invoke_context()
      : best_score((std::numeric_limits<int>::max)())
      , candidate_index(0)
    {}

    operator bool() const
    {
        return candidate_index == 1;
    }

    void format_error(lua_State* L, function_object const* overloads) const;

    int best_score;
    function_object const* candidates[10];
    int candidate_index;
};

template <class F, class Signature, class Policies, class IsVoid>
inline int invoke0(
    lua_State* L, function_object const& self, invoke_context& ctx
  , F const& f, Signature, Policies const& policies, IsVoid, mpl::true_)
{
    return invoke_member(
        L, self, ctx, f, Signature(), policies
      , mpl::long_<mpl::size<Signature>::value - 1>(), IsVoid()
    );
}

template <class F, class Signature, class Policies, class IsVoid>
inline int invoke0(
    lua_State* L, function_object const& self, invoke_context& ctx,
    F const& f, Signature, Policies const& policies, IsVoid, mpl::false_)
{
    return invoke_normal(
        L, self, ctx, f, Signature(), policies
      , mpl::long_<mpl::size<Signature>::value - 1>(), IsVoid()
    );
}

template <class F, class Signature, class Policies>
inline int invoke(
    lua_State* L, function_object const& self, invoke_context& ctx
  , F const& f, Signature, Policies const& policies)
{
    return invoke0(
        L, self, ctx, f, Signature(), policies
      , boost::is_void<typename mpl::front<Signature>::type>()
      , boost::is_member_function_pointer<F>()
   );
}

inline int maybe_yield_aux(lua_State*, int results, mpl::false_)
{
    return results;
}

inline int maybe_yield_aux(lua_State* L, int results, mpl::true_)
{
    return lua_yield(L, results);
}

template <class Policies>
int maybe_yield(lua_State* L, int results, Policies*)
{
    return maybe_yield_aux(
        L, results, has_policy<Policies, yield_policy>());
}

inline int sum_scores(int const* first, int const* last)
{
    int result = 0;

    for (; first != last; ++first)
    {
        if (*first < 0)
            return *first;
        result += *first;
    }

    return result;
}

#  define LUAPONTE_INVOKE_NEXT_ITER(n) \
    typename mpl::next< \
        BOOST_PP_IF( \
            n, BOOST_PP_CAT(iter,BOOST_PP_DEC(n)), first) \
    >::type

#  define LUAPONTE_INVOKE_NEXT_INDEX(n) \
    BOOST_PP_IF( \
        n \
      , BOOST_PP_CAT(index,BOOST_PP_DEC(n)) + \
            BOOST_PP_CAT(c,BOOST_PP_DEC(n)).consumed_args() \
      , 1 \
    )

#  define LUAPONTE_INVOKE_COMPUTE_ARITY(n) + BOOST_PP_CAT(c,n).consumed_args()

#  define LUAPONTE_INVOKE_DECLARE_CONVERTER(n) \
    typedef LUAPONTE_INVOKE_NEXT_ITER(n) BOOST_PP_CAT(iter,n); \
    typedef typename mpl::deref<BOOST_PP_CAT(iter,n)>::type \
        BOOST_PP_CAT(a,n); \
    typedef typename find_conversion_policy<n + 1, Policies>::type \
        BOOST_PP_CAT(p,n); \
    typename mpl::apply_wrap2< \
        BOOST_PP_CAT(p,n), BOOST_PP_CAT(a,n), lua_to_cpp>::type BOOST_PP_CAT(c,n); \
    int const BOOST_PP_CAT(index,n) = LUAPONTE_INVOKE_NEXT_INDEX(n);

#  define LUAPONTE_INVOKE_COMPUTE_SCORE(n)                                   \
    , BOOST_PP_CAT(c,n).match(                                              \
        L, LUAPONTE_DECORATE_TYPE(BOOST_PP_CAT(a,n)), BOOST_PP_CAT(index,n))

#  define LUAPONTE_INVOKE_ARG(z, n, base) \
    BOOST_PP_CAT(c,base(n)).apply( \
        L, LUAPONTE_DECORATE_TYPE(BOOST_PP_CAT(a,base(n))), BOOST_PP_CAT(index,base(n)))

#  define LUAPONTE_INVOKE_CONVERTER_POSTCALL(n) \
    BOOST_PP_CAT(c,n).converter_postcall( \
        L, LUAPONTE_DECORATE_TYPE(BOOST_PP_CAT(a,n)), BOOST_PP_CAT(index,n));

#  define BOOST_PP_ITERATION_PARAMS_1 \
    (3, (0, LUAPONTE_MAX_ARITY, <luaponte/detail/call.hpp>))
#  include BOOST_PP_ITERATE()

#  define LUAPONTE_INVOKE_VOID
#  define BOOST_PP_ITERATION_PARAMS_1 \
    (3, (0, LUAPONTE_MAX_ARITY, <luaponte/detail/call.hpp>))
#  include BOOST_PP_ITERATE()

#  undef LUAPONTE_INVOKE_VOID
#  define LUAPONTE_INVOKE_MEMBER
#  define BOOST_PP_ITERATION_PARAMS_1 \
    (3, (0, LUAPONTE_MAX_ARITY, <luaponte/detail/call.hpp>))
#  include BOOST_PP_ITERATE()

#  define LUAPONTE_INVOKE_VOID
#  define BOOST_PP_ITERATION_PARAMS_1 \
    (3, (0, LUAPONTE_MAX_ARITY, <luaponte/detail/call.hpp>))
#  include BOOST_PP_ITERATE()

} // namespace detail
} // namespace luaponte

# endif // LUAPONTE_DETAIL_CALL_HPP

#else // BOOST_PP_IS_ITERATING

# ifdef LUAPONTE_INVOKE_MEMBER
#  define N BOOST_PP_INC(BOOST_PP_ITERATION())
# else
#  define N BOOST_PP_ITERATION()
# endif

template <class F, class Signature, class Policies>
inline int
# ifdef LUAPONTE_INVOKE_MEMBER
invoke_member
# else
invoke_normal
# endif
(
    lua_State* L, function_object const& self, invoke_context& ctx
  , F const& f, Signature, Policies const&, mpl::long_<N>
# ifdef LUAPONTE_INVOKE_VOID
  , mpl::true_
# else
  , mpl::false_
# endif
)
{
    typedef typename mpl::begin<Signature>::type first;
# ifndef LUAPONTE_INVOKE_VOID
    typedef typename mpl::deref<first>::type result_type;
    typedef typename find_conversion_policy<0, Policies>::type result_policy;
    typename mpl::apply_wrap2<
        result_policy, result_type, cpp_to_lua>::type result_converter;
# endif

# if N > 0
#  define BOOST_PP_LOCAL_MACRO(n) LUAPONTE_INVOKE_DECLARE_CONVERTER(n)
#  define BOOST_PP_LOCAL_LIMITS (0,N-1)
#  include BOOST_PP_LOCAL_ITERATE()
# endif

    int const arity = 0
# if N > 0
#  define BOOST_PP_LOCAL_MACRO(n) LUAPONTE_INVOKE_COMPUTE_ARITY(n)
#  define BOOST_PP_LOCAL_LIMITS (0,N-1)
#  include BOOST_PP_LOCAL_ITERATE()
# endif
    ;

    int const arguments = lua_gettop(L);

    int score = -1;

    if (arity == arguments)
    {
        int const scores[] = {
            0
# if N > 0
#  define BOOST_PP_LOCAL_MACRO(n) LUAPONTE_INVOKE_COMPUTE_SCORE(n)
#  define BOOST_PP_LOCAL_LIMITS (0,N-1)
#  include BOOST_PP_LOCAL_ITERATE()
# endif
        };

        score = sum_scores(scores + 1, scores + 1 + N);
    }

    if (score >= 0 && score < ctx.best_score)
    {
        ctx.best_score = score;
        ctx.candidates[0] = &self;
        ctx.candidate_index = 1;
    }
    else if (score == ctx.best_score)
    {
        ctx.candidates[ctx.candidate_index++] = &self;
    }

    int results = 0;

    if (self.next)
    {
        results = self.next->call(L, ctx);
    }

    if (score == ctx.best_score && ctx.candidate_index == 1)
    {
# ifndef LUAPONTE_INVOKE_VOID
        result_converter.apply(
            L,
# endif
# ifdef LUAPONTE_INVOKE_MEMBER
            (c0.apply(L, LUAPONTE_DECORATE_TYPE(a0), index0).*f)(
                BOOST_PP_ENUM(BOOST_PP_DEC(N), LUAPONTE_INVOKE_ARG, BOOST_PP_INC)
            )
# else
#  define LUAPONTE_INVOKE_IDENTITY(x) x
            f(
                BOOST_PP_ENUM(N, LUAPONTE_INVOKE_ARG, LUAPONTE_INVOKE_IDENTITY)
            )
#  undef LUAPONTE_INVOKE_IDENTITY
# endif
# ifndef LUAPONTE_INVOKE_VOID
        )
# endif
        ;

# if N > 0
#  define BOOST_PP_LOCAL_MACRO(n) LUAPONTE_INVOKE_CONVERTER_POSTCALL(n)
#  define BOOST_PP_LOCAL_LIMITS (0,N-1)
#  include BOOST_PP_LOCAL_ITERATE()
# endif

        results = maybe_yield(L, lua_gettop(L) - arguments, (Policies*)0);

        int const indices[] = {
            arguments + results BOOST_PP_ENUM_TRAILING_PARAMS(N, index)
        };

        policy_list_postcall<Policies>::apply(L, indices);
    }

    return results;
}

# undef N

#endif
