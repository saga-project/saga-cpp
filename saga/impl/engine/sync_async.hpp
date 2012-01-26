//  Copyright (c) 2005-2009 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SAGA_IMPL_ENGINE_SYNC_ASYNC_HPP)
#define SAGA_IMPL_ENGINE_SYNC_ASYNC_HPP

#include <boost/assert.hpp>

#include <saga/impl/engine/cpi_info.hpp>

#include <saga/saga/error.hpp>
#include <saga/saga/util.hpp>

#include <saga/impl/engine/adaptor_selector_state.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace detail 
{
    // store the selector state to be used with this task instance
    SAGA_EXPORT void set_selector_state(saga::task t, 
        TR1::shared_ptr<impl::adaptor_selector_state> state);
}}

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl 
{
    // in general:
    // Adaptors can provide their API functions synchronously or asynchronously
    // or both of them.
    // Towards the application, the engine has to provide all api functions
    // synchronously, asynchronously and as a Task (asynchronously but not yet
    // started).
    // The following code dispatches between those possibilities.

    ///////////////////////////////////////////////////////////////////////////
    //  need sync, have sync
    //  The application wants a synchronous call, the adaptor provides a
    //  synchronous one. So there is nothing to do, we have only to call the
    //  function.
    //  If the sync call fails, the exception is simply passed upwards
    template <typename Cpi, typename Base, typename RetVal>
    inline saga::task
    sync_sync (TR1::shared_ptr<Cpi> cpi, void (Base::*sync)(RetVal&))
    {
        saga::task t(saga::task::Done);
        (cpi.get()->*sync)(t.get_result<RetVal>());
        return t;
    }

    ///////////////////////////////////////////////////////////////////////////
    //  Just as above, but for void methods
    template <typename Cpi, typename Base>
    inline saga::task
    sync_sync (TR1::shared_ptr<Cpi> cpi, void (Base::*sync)(saga::impl::void_t&))
    {
        saga::task t(saga::task::Done);
        saga::impl::void_t void_result;

        (cpi.get()->*sync)(void_result);
        
        return t;
    }

    ///////////////////////////////////////////////////////////////////////////
    //  need async, have sync
    //  The application wants a asynchronous call, but the adaptor provides
    //  only a synchronous one. So the call is executed in a thread, and by
    //  that way made asynchronous.
    template <typename Cpi, typename Base, typename RetVal>
    inline saga::task
    async_sync (proxy *prxy, 
                TR1::shared_ptr<Cpi> cpi, 
                TR1::shared_ptr<adaptor_selector_state> state,
                void (Base::*sync)(RetVal&), 
                bool (Base::*prep)(RetVal&, saga::uuid))
    {
        saga::adaptors::task t(state->get_op_name(), cpi, prxy, sync, prep);
        detail::set_selector_state(t, state);
        return t;
    }

    ///////////////////////////////////////////////////////////////////////////
    //  need sync, have async
    //  The application wants a synchronous call, the adaptor provides an
    //  asynchronous one. So, we wait for the call to finish.
    template <typename Cpi, typename Base>
    inline saga::task
    sync_async (TR1::shared_ptr<Cpi> cpi, saga::task (Base::*async)())
    {
        return saga::detail::run_wait((cpi.get()->*async)());
    }

    ///////////////////////////////////////////////////////////////////////////
    //  need async, have async
    //  The application wants an asynchronous call, the adaptor provides an
    //  asynchronous one. So there is nothing to do, we have only to call the
    //  function.
    template <typename Cpi, typename Base>
    inline saga::task
    async_async (TR1::shared_ptr<Cpi> cpi, 
                 TR1::shared_ptr<adaptor_selector_state> state, 
                 saga::task (Base::*async)())
    {
        saga::task t ((cpi.get()->*async)());
        detail::set_selector_state(t, state);
        return t;
    }

    ///////////////////////////////////////////////////////////////////////////
    //  dispatching to the appropriate functions defined above.
    template <typename Base, typename RetVal>
    inline saga::task
    dispatch_sync (run_mode     mode, 
                   char const * name,
                   TR1::shared_ptr<v1_0::cpi> cpi_instance,
                   void       (Base::*sync)(RetVal&), 
                   saga::task (Base::*async)())
    {
        TR1::shared_ptr<Base> c (TR1::static_pointer_cast<Base>(cpi_instance));

        switch (mode) {
        case Sync_Sync:
            return sync_sync(c, sync);

        case Sync_Async:
            return sync_async(c, async);

        case Async_Sync:
        case Async_Async:
            BOOST_ASSERT(false);
            break;

        default:
            break;
        }

        // no adaptor found (Invalid mode)!
        SAGA_THROW_VERBATIM(cpi_instance.get(),
                            std::string ("No adaptor implements method: ") + name,
                            adaptors::NoAdaptor);

        return saga::task(saga::task_base::Done);
    }

    ///////////////////////////////////////////////////////////////////////////
    //  dispatching to the appropriate functions defined above.
    template <typename Base, typename RetVal>
    inline saga::task
    dispatch_async (proxy *prxy, 
                    TR1::shared_ptr<adaptor_selector_state> state,
                    void (Base::*sync_)(RetVal &), saga::task (Base::*async_)(),
                    bool (Base::*prep_)(RetVal &, saga::uuid) = NULL)
    {
        typedef void       (Base::*sync_func )(RetVal&);
        typedef saga::task (Base::*async_func)();
        typedef bool       (Base::*prep_func )(RetVal&, saga::uuid);

        void       (Base::*sync )() = NULL;
        saga::task (Base::*async)() = NULL;
        bool       (Base::*prep )() = NULL;

        run_mode mode = Unknown;

        TR1::shared_ptr<Base> c (
            state->get_next_cpi(mode, &sync, &async, &prep));

//      BOOST_ASSERT(NULL == sync  || (sync_func)  sync  == sync_);
//      BOOST_ASSERT(NULL == async || (async_func) async == async_);
//      BOOST_ASSERT(NULL == prep  || (prep_func)  prep  == prep_);

        switch (mode) {
        case Async_Sync:
            BOOST_ASSERT(sync);
            return async_sync(prxy, c, state, (sync_func)sync, (prep_func)prep);

        case Async_Async:
            BOOST_ASSERT(async);
            return async_async(c, state, (async_func)async);

        case Sync_Sync:
        case Sync_Async:
            BOOST_ASSERT(false);
            break;

        default:
            break;
        }

        // no adaptor found (Invalid mode)!
        SAGA_THROW_VERBATIM(c.get(),
                            std::string ("No adaptor implements method: ") + state->get_op_name(),
                            adaptors::NoAdaptor);

        // this makes some compilers happy, but will never be called in fact
        // (didn't you see the throw above?)
        return saga::task(saga::task_base::Done); 
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Base, typename RetVal>
    inline saga::task
    execute_sync_async (proxy      * prxy,
                        char const * cpi_name, 
                        char const * name, 
                        char const * op_name,
                        bool         is_sync,
                        void       (Base::*sync) (RetVal &), 
                        saga::task (Base::*async)());

    ///////////////////////////////////////////////////////////////////////////
    // bring in higher order functions, i.e. functions with multiple parameters
    #include <saga/impl/engine/sync_async_impl.hpp>

///////////////////////////////////////////////////////////////////////////////
}}  // namespace saga::impl

#endif // SAGA_IMPL_ENGINE_SYNC_ASYNC_HPP
