//  Copyright (c) 2005-2009 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_ENGINE_REGISTER_MEMBERS_HPP
#define SAGA_IMPL_ENGINE_REGISTER_MEMBERS_HPP

#include <boost/version.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/assert.hpp>

#include <saga/saga/adaptors/adaptor.hpp>
#include <saga/impl/engine/cpi_info.hpp>
#include <saga/saga/adaptors/adaptor.hpp>

// Allow to work around the MPL problem in BOOST_MPL_ASSERT_MSG generating
// multiple definition linker errors for certain compilers (VC++)
#if SAGA_DONT_USE_MPL_ASSERT_MSG != 0
#include <boost/static_assert.hpp>
#define SAGA_ASSERT_MSG(Cond, Msg, Types)                                     \
        BOOST_STATIC_ASSERT(Cond)
#else
#include <boost/mpl/assert.hpp>
#define SAGA_ASSERT_MSG(Cond, Msg, Types)                                     \
        BOOST_MPL_ASSERT_MSG(Cond, Msg, Types)
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl {

///////////////////////////////////////////////////////////////////////////////
//
//  The set of function_traits meta template functions below is used to
//  return the type of the class a certain given function pointer type is
//  implemented in.
//
//  The different overloads are required for different numbers of parameters
//  these functions take.
//
//  Note: using the variadics template features (as proposed for C++0x) this
//        could be rewritten as:
//
//          template <typename T>
//          struct function_traits;
//
//          template <typename RT, typename Base, typename... Args>
//          struct function_traits<RT (Base::*)(Args...)>
//          {
//              typedef Base type;
//          };
//
template <typename T>
struct function_traits;

template <typename RT, typename Base>
struct function_traits<RT (Base::*)()>
{
    typedef Base base_type;
    typedef boost::mpl::vector<> parameter_types;
    typedef RT return_type;
};

template <typename RT, typename Base, typename T1>
struct function_traits<RT (Base::*)(T1)>
{
    typedef Base base_type;
    typedef boost::mpl::vector<T1> parameter_types;
    typedef RT return_type;
};

template <typename RT, typename Base, typename T1, typename T2>
struct function_traits<RT (Base::*)(T1, T2)>
{
    typedef Base base_type;
    typedef boost::mpl::vector<T1, T2> parameter_types;
    typedef RT return_type;
};

template <typename RT, typename Base, typename T1, typename T2, typename T3>
struct function_traits<RT (Base::*)(T1, T2, T3)>
{
    typedef Base base_type;
    typedef boost::mpl::vector<T1, T2, T3> parameter_types;
    typedef RT return_type;
};

template <
    typename RT, typename Base, typename T1, typename T2, typename T3,
    typename T4
>
struct function_traits<RT (Base::*)(T1, T2, T3, T4)>
{
    typedef Base base_type;
    typedef boost::mpl::vector<T1, T2, T3, T4> parameter_types;
    typedef RT return_type;
};

template <
    typename RT, typename Base, typename T1, typename T2, typename T3,
    typename T4, typename T5
>
struct function_traits<RT (Base::*)(T1, T2, T3, T4, T5)>
{
    typedef Base base_type;
    typedef boost::mpl::vector<T1, T2, T3, T4, T5> parameter_types;
    typedef RT return_type;
};

template <
    typename RT, typename Base, typename T1, typename T2, typename T3,
    typename T4, typename T5, typename T6
>
struct function_traits<RT (Base::*)(T1, T2, T3, T4, T5, T6)>
{
    typedef Base base_type;
    typedef boost::mpl::vector<T1, T2, T3, T4, T5, T6> parameter_types;
    typedef RT return_type;
};

///////////////////////////////////////////////////////////////////////////////
#if BOOST_VERSION >= 103401
template <typename Func, typename BaseFunc>
struct signatures_match
{
    typedef function_traits<Func> func_traits;
    typedef function_traits<BaseFunc> basefunc_traits;

    typedef typename boost::mpl::or_<
            TR1::is_base_of<
                typename basefunc_traits::return_type,
                typename func_traits::return_type
            >,
            TR1::is_same<
                typename basefunc_traits::return_type,
                typename func_traits::return_type
            >
        >::type return_type_ok;

    typedef typename boost::mpl::and_<
            TR1::is_same<
                typename basefunc_traits::parameter_types,
                typename func_traits::parameter_types
            >,
            return_type_ok,
            TR1::is_base_of<
                typename basefunc_traits::base_type,
                typename func_traits::base_type
            >
        >::type type;
};
#else
// In Boost V1.33.1 is_base_of<> isn't true if supplied types are the same
template <typename Func, typename BaseFunc>
struct signatures_match
{
    typedef function_traits<Func> func_traits;
    typedef function_traits<BaseFunc> basefunc_traits;

    typedef typename boost::mpl::and_<
            TR1::is_same<
                typename basefunc_traits::parameter_types,
                typename func_traits::parameter_types
            >,
            boost::mpl::or_<
                TR1::is_base_of<
                    typename basefunc_traits::return_type,
                    typename func_traits::return_type
                >,
                TR1::is_same<
                    typename basefunc_traits::return_type,
                    typename func_traits::return_type
                >
            >,
            boost::mpl::or_<
                TR1::is_base_of<
                    typename basefunc_traits::base_type,
                    typename func_traits::base_type
                >,
                TR1::is_same<
                    typename basefunc_traits::base_type,
                    typename func_traits::base_type
                >
            >
        >::type type;
};
#endif

///////////////////////////////////////////////////////////////////////////////
//
//  The register_member_helper and dont_register_member_helper types are pure
//  helpers allowing to dispatch the registration of a certain member function
//  either to no-ops or to the real registration.
//
template <typename Cpi, typename Func>
struct register_member_helper
{
    // register synchronous CPI function
    static bool
    eval_sync(saga::impl::v1_0::cpi_info& info, char const* name,
        saga::impl::v1_0::preference_type const& prefs, Func f)
    {
        if (Cpi::register_sync_member(name)) {
            // add this function to the engine's registry
            saga::impl::v1_0::op_info::sync_func sync =
                (saga::impl::v1_0::op_info::sync_func)f;

            info.add_op(saga::impl::v1_0::op_info (name, prefs, sync));

            SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
                << "  function registration: " << std::string(name);
        }
        else {
            SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
                << "  function registration: disabled: " << std::string(name);
        }
        return true;
    }

    // register asynchronous CPI function
    static bool
    eval_async(saga::impl::v1_0::cpi_info& info, char const* name,
        saga::impl::v1_0::preference_type const& prefs, Func f)
    {
        if (Cpi::register_async_member(name)) {
            // add this function to the engine's registry
            saga::impl::v1_0::op_info::async_func async =
                (saga::impl::v1_0::op_info::async_func)f;

            info.add_op(saga::impl::v1_0::op_info (name, prefs, NULL, async));

            SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
                << "  function registration: " << std::string(name);
        }
        else {
            SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
                << "  function registration: disabled: " << std::string(name);
        }
        return true;
    }
};

template <typename Func>
struct dont_register_member_helper
{
    static bool
    eval_sync(saga::impl::v1_0::cpi_info&, char const* name,
        saga::impl::v1_0::preference_type const&, Func)
    {
        SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
            << "  function registration: skipped: " << std::string(name);
        return false;   // don't do anything
    }

    static bool
    eval_async(saga::impl::v1_0::cpi_info&, char const* name,
        saga::impl::v1_0::preference_type const&, Func)
    {
        SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
            << "  function registration: skipped: " << std::string(name);
        return false;   // don't do anything
    }
};

///////////////////////////////////////////////////////////////////////////////
//
//  The functions register_member_sync() and register_member_async() use some
//  meta template programming tricks to figure out, whether the given member
//  function 'Func' is implemented in a derived class of the given 'BaseCpi'
//  type.
//
//  If the function _is_ implemented, then it will be registered with the SAGA
//  engine. If it is implemented in the corresponding base CPI only (see
//  function_traits meta template function), the function will not be registered
//  with the SAGA engine).
//
//  The function returns true if the function is defined in a derived class,
//  otherwise it returns false.
//
template <typename BaseCpi, typename Func, typename BaseFunc>
inline bool
register_member_sync(Func f, BaseFunc bf, saga::impl::v1_0::cpi_info& info,
    char const* name, saga::impl::v1_0::preference_type const& prefs)
{
    typedef typename function_traits<Func>::base_type func_base_type;
    typedef typename boost::mpl::if_<
        typename boost::mpl::and_<
            TR1::is_base_of<BaseCpi, func_base_type>,
            boost::mpl::not_<TR1::is_same<BaseCpi, func_base_type> >
        >::type,
        register_member_helper<func_base_type, Func>,
        dont_register_member_helper<Func>
    >::type register_type;

    SAGA_ASSERT_MSG((signatures_match<Func, BaseFunc>::type::value),
        SIGNATURE_OF_CPI_FUNCTION_DOES_NOT_MATCH, (Func, BaseFunc));

    return register_type::eval_sync(info, name, prefs, f);
}

template <typename BaseCpi, typename Func, typename BaseFunc>
inline bool
register_member_async(Func f, BaseFunc bf, saga::impl::v1_0::cpi_info& info,
    char const* name, saga::impl::v1_0::preference_type const& prefs)
{
    typedef typename function_traits<Func>::base_type func_base_type;
    typedef typename boost::mpl::if_<
        typename boost::mpl::and_<
            TR1::is_base_of<BaseCpi, func_base_type>,
            boost::mpl::not_<TR1::is_same<BaseCpi, func_base_type> >
        >::type,
        register_member_helper<func_base_type, Func>,
        dont_register_member_helper<Func>
    >::type register_type;

    SAGA_ASSERT_MSG((signatures_match<Func, BaseFunc>::type::value),
        SIGNATURE_OF_CPI_FUNCTION_DOES_NOT_MATCH, (Func, BaseFunc));

    return register_type::eval_async(info, name, prefs, f);
}

///////////////////////////////////////////////////////////////////////////////
}}  // saga::impl

///////////////////////////////////////////////////////////////////////////////
//  Register the given CPI function for the given CPI implementation.
#define SAGA_REGISTER_MEMBER(retval, info, base_cpi, cpi, func, prefs)        \
        retval = saga::impl::register_member_sync<base_cpi>(                  \
            &cpi::sync_ ## func, &base_cpi::sync_ ## func, info,              \
            "sync_" #func, prefs) || retval;                                  \
        retval = saga::impl::register_member_async<base_cpi>(                 \
            &cpi::async_ ## func, &base_cpi::async_ ## func, info,            \
            "async_" #func, prefs) || retval;                                 \
    /**/

#define SAGA_REGISTER_MEMBER_EX(retval, info, base_cpi, cpi, func, prefs,     \
        basesyncfunc, baseasyncfunc, syncfunc, asyncfunc)                     \
    {                                                                         \
        basesyncfunc bsf = (basesyncfunc)&base_cpi::sync_ ## func;            \
        syncfunc sf = (syncfunc)&cpi::sync_ ## func;                          \
        retval = saga::impl::register_member_sync<base_cpi>(                  \
            sf, bsf, info, "sync_" #func, prefs) || retval;                   \
        baseasyncfunc basf = (baseasyncfunc)&base_cpi::async_ ## func;        \
        asyncfunc asf = (asyncfunc)&cpi::async_ ## func;                      \
        retval = saga::impl::register_member_async<base_cpi>(                 \
            asf, basf, info, "async_" #func, prefs) || retval;                \
    }                                                                         \
    /**/

/* FIXME: add bulk functions to all cpi's                                  \ */
/*     if (cpi::register_bulk_members())                                   \ */
/*         saga::impl::register_member<cpi>(&cpi::bulk_ ## func, info,     \ */
/*             "bulk_" #func, prefs);                                      \ */

#endif // !SAGA_IMPL_ENGINE_REGISTER_MEMBERS_HPP

