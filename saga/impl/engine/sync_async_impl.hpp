//  Copyright (c) 2005-2009 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_PP_IS_ITERATING)

#if !defined(SAGA_IMPL_ENGINE_SYNC_ASYNC_IMPL_HPP)
#define SAGA_IMPL_ENGINE_SYNC_ASYNC_IMPL_HPP

#include <saga/impl/config.hpp>

#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>

#define BOOST_PP_ITERATION_PARAMS_1                                           \
    (3, (1, SAGA_ARGUMENT_LIMIT, "saga/impl/engine/sync_async_impl.hpp"))     \
    /**/

#include BOOST_PP_ITERATE()

#endif // SAGA_IMPL_ENGINE_SYNC_ASYNC_IMPL_HPP

///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////
#else // defined(BOOST_PP_IS_ITERATING)

#define K BOOST_PP_ITERATION()

    ///////////////////////////////////////////////////////////////////////////
    // need sync, have sync
    template <
        typename Cpi, 
        typename Base, 
        typename RetVal,
        BOOST_PP_ENUM_PARAMS (K, typename FuncArg),
        BOOST_PP_ENUM_PARAMS (K, typename Arg)
    >
    inline saga::task
    sync_sync (TR1::shared_ptr <Cpi> cpi,
               void (Base::*sync) (RetVal &, BOOST_PP_ENUM_PARAMS (K, FuncArg)),
               BOOST_PP_ENUM_BINARY_PARAMS (K, Arg, const & arg))
    {
        saga::task t(saga::task::Done);
        (cpi.get()->*sync)(
            t.get_result<RetVal>(), BOOST_PP_ENUM_PARAMS (K, arg));
        return t;
    }


    template <
        typename Cpi, 
        typename Base,
        BOOST_PP_ENUM_PARAMS (K, typename FuncArg),
        BOOST_PP_ENUM_PARAMS (K, typename Arg)
    >
    inline saga::task
    sync_sync (TR1::shared_ptr <Cpi> cpi,
               void (Base::*sync) (saga::impl::void_t &, BOOST_PP_ENUM_PARAMS (K, FuncArg)),
               BOOST_PP_ENUM_BINARY_PARAMS (K, Arg, const& arg))
    {
        saga::task t (saga::task::Done);
        saga::impl::void_t void_result;

        (cpi.get()->*sync)(void_result, BOOST_PP_ENUM_PARAMS (K, arg));

        return t;
    }


    ///////////////////////////////////////////////////////////////////////////
    //  need async, have sync
    template <
        typename Cpi, 
        typename Base, 
        typename RetVal,
        BOOST_PP_ENUM_PARAMS (K, typename FuncArg),
        BOOST_PP_ENUM_PARAMS (K, typename Arg)
    >
    inline saga::task
    async_sync (proxy * prxy, 
                TR1::shared_ptr <Cpi> cpi, 
                TR1::shared_ptr <adaptor_selector_state> state,
                void (Base::*sync) (RetVal&, BOOST_PP_ENUM_PARAMS (K, FuncArg)),
                BOOST_PP_ENUM_BINARY_PARAMS (K, Arg, const& arg),
                bool (Base::*prep) (RetVal&, BOOST_PP_ENUM_PARAMS (K, FuncArg), saga::uuid))
    {
        saga::adaptors::task t(state->get_op_name(), cpi, prxy, sync, 
                               BOOST_PP_ENUM_PARAMS(K, arg), prep);

        detail::set_selector_state(t, state);
        
        return t;
    }


    ///////////////////////////////////////////////////////////////////////////
    // need sync, have async
    template <
         typename Cpi, 
         typename Base, 
         BOOST_PP_ENUM_PARAMS (K, typename FuncArg),
         BOOST_PP_ENUM_PARAMS (K, typename Arg)
    >
    inline saga::task
    sync_async (TR1::shared_ptr <Cpi> cpi,
                saga::task (Base::*async)(BOOST_PP_ENUM_PARAMS (K, FuncArg)),
                BOOST_PP_ENUM_BINARY_PARAMS (K, Arg, const & arg))
    {
        return saga::detail::run_wait(
            (cpi.get()->*async)(BOOST_PP_ENUM_PARAMS(K, arg)));
    }


    ///////////////////////////////////////////////////////////////////////////
    // need async, have async
    template <
        typename Cpi, typename Base,
        BOOST_PP_ENUM_PARAMS(K, typename FuncArg),
        BOOST_PP_ENUM_PARAMS(K, typename Arg)
    >
    inline saga::task
    async_async (TR1::shared_ptr <Cpi> cpi,
                 TR1::shared_ptr <adaptor_selector_state> state,
                 saga::task (Base::*async) (BOOST_PP_ENUM_PARAMS (K, FuncArg)),
                 BOOST_PP_ENUM_BINARY_PARAMS (K, Arg, const & arg))
    {
        saga::task t ((cpi.get()->*async)(BOOST_PP_ENUM_PARAMS(K, arg)));
        detail::set_selector_state(t, state);
        return t;
    }


    ///////////////////////////////////////////////////////////////////////////
    template <
        typename Base, typename RetVal,
        BOOST_PP_ENUM_PARAMS (K, typename FuncArg),
        BOOST_PP_ENUM_PARAMS (K, typename Arg)
    >
    inline saga::task
    dispatch_sync (run_mode     mode, 
                   char const * name,
                   TR1::shared_ptr<v1_0::cpi> cpi_instance,
                   void       (Base::*sync ) (RetVal &, BOOST_PP_ENUM_PARAMS (K, FuncArg)),
                   saga::task (Base::*async) (          BOOST_PP_ENUM_PARAMS (K, FuncArg)),
                   BOOST_PP_ENUM_BINARY_PARAMS (K, Arg, const & arg))
    {
        TR1::shared_ptr<Base> c = TR1::static_pointer_cast<Base>(cpi_instance);

        switch (mode) {
        case Sync_Sync:
            return sync_sync(c, sync, BOOST_PP_ENUM_PARAMS(K, arg));

        case Sync_Async:
            return sync_async(c, async, BOOST_PP_ENUM_PARAMS(K, arg));

        case Async_Sync:
        case Async_Async:
            BOOST_ASSERT(false);
            break;

        default:
            break;
        }

        // no adaptor found  (Invalid mode)!
        SAGA_THROW_VERBATIM(cpi_instance.get(),
            std::string ("No adaptor implements method: ") + name,
            adaptors::NoAdaptor);

        return saga::task(saga::task_base::Done);
    }


    ///////////////////////////////////////////////////////////////////////////
    template <
        typename Base, typename RetVal,
        BOOST_PP_ENUM_PARAMS (K, typename FuncArg),
        BOOST_PP_ENUM_PARAMS (K, typename Arg)
    >
    inline saga::task
    dispatch_async (proxy * prxy, 
                    TR1::shared_ptr <adaptor_selector_state> state,
                    void       (Base::*sync_)  (RetVal &, BOOST_PP_ENUM_PARAMS (K, FuncArg)),
                    saga::task (Base::*async_) (          BOOST_PP_ENUM_PARAMS (K, FuncArg)),
                    BOOST_PP_ENUM_BINARY_PARAMS (K, Arg, const & arg),
                    bool       (Base::*prep_)  (RetVal &, BOOST_PP_ENUM_PARAMS(K, FuncArg), saga::uuid) = NULL)
    {
        typedef void       (Base::*sync_func )(RetVal&, BOOST_PP_ENUM_PARAMS(K, FuncArg));
        typedef saga::task (Base::*async_func)(         BOOST_PP_ENUM_PARAMS(K, FuncArg));
        typedef bool       (Base::*prep_func )(RetVal&, BOOST_PP_ENUM_PARAMS(K, FuncArg), saga::uuid);

        void       (Base::*sync )() = NULL;
        saga::task (Base::*async)() = NULL;
        bool       (Base::*prep )() = NULL;

        run_mode mode = Unknown;
        TR1::shared_ptr<Base> c (
            state->get_next_cpi (mode, &sync, &async, &prep));

//      BOOST_ASSERT(NULL == sync  || (sync_func)  sync  == sync_ );
//      BOOST_ASSERT(NULL == async || (async_func) async == async_);
//      BOOST_ASSERT(NULL == prep  || (prep_func)  prep  == prep_ );

        switch (mode) {
        case Async_Sync:
            BOOST_ASSERT(sync);
            return async_sync(prxy, c, state, (sync_func)sync, 
                BOOST_PP_ENUM_PARAMS(K, arg), (prep_func)prep);

        case Async_Async:
            BOOST_ASSERT(async);
            return async_async(c, state, (async_func)async, 
                BOOST_PP_ENUM_PARAMS(K, arg));

        case Sync_Sync:
        case Sync_Async:
            BOOST_ASSERT(false);
            break;

        default:
            break;
        }

        // no adaptor found  (Invalid mode)!
        SAGA_THROW_VERBATIM(c.get(),
                            std::string ("No adaptor implements method: ") + state->get_op_name(),
                            adaptors::NoAdaptor);

        // this makes some compilers happy, but will never be called in fact
        // (didn't you see the throw above?)
        return saga::task(saga::task_base::Done);
    }


    ///////////////////////////////////////////////////////////////////////////
    template <
        typename Base, typename RetVal,
        BOOST_PP_ENUM_PARAMS (K, typename FuncArg),
        BOOST_PP_ENUM_PARAMS (K, typename Arg)
    >
    inline saga::task
    execute_sync_async (proxy                       * prxy,
                        char const                  * cpi_name, 
                        char const                  * name, 
                        char const                  * op_name,
                        bool                          is_sync,
                        void       (Base::*sync)  (RetVal&, BOOST_PP_ENUM_PARAMS (K, FuncArg)),
                        saga::task (Base::*async) (         BOOST_PP_ENUM_PARAMS (K, FuncArg)),
                        BOOST_PP_ENUM_BINARY_PARAMS (K, Arg, const & arg));

#undef K

#endif // defined(BOOST_PP_IS_ITERATING)

