//  Copyright (c) 2005-2009 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_PP_IS_ITERATING)

#if !defined(SAGA_IMPL_ENGINE_RUN_MODE_WRAPPER_IMPL_HPP)
#define SAGA_IMPL_ENGINE_RUN_MODE_WRAPPER_IMPL_HPP

#include <saga/impl/config.hpp>

#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>

#define BOOST_PP_ITERATION_PARAMS_1                                           \
    (3, (1, SAGA_ARGUMENT_LIMIT,                                              \
    "saga/impl/engine/run_mode_wrapper_impl.hpp"))                                 \
    /**/

#include BOOST_PP_ITERATE()

#endif // !SAGA_IMPL_ENGINE_RUN_MODE_WRAPPER_IMPL_HPP

///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////
#else // defined(BOOST_PP_IS_ITERATING)

#define K BOOST_PP_ITERATION()

    ///////////////////////////////////////////////////////////////////////////
    template <
        typename Base, typename RetVal,
        BOOST_PP_ENUM_PARAMS(K, typename FuncArg),
        BOOST_PP_ENUM_PARAMS(K, typename Arg)
    >
    inline saga::task
    execute_sync(proxy *prxy,
        char const *cpi_name, char const *name, char const *op_name,
        v1_0::preference_type const& prefs,
        void (Base::*sync)(RetVal&, BOOST_PP_ENUM_PARAMS(K, FuncArg)),
        saga::task (Base::*async)(BOOST_PP_ENUM_PARAMS(K, FuncArg)),
        BOOST_PP_ENUM_BINARY_PARAMS(K, Arg, const& arg))
    {
        adaptor_selector::adaptor_info_list_type no_no_list;
        exception_list exceptions;
        bool found_one = false;
        v1_0::op_info oi (name);
        while (true) {
            v1_0::cpi_info info;
            try {
                run_mode current_mode = Unknown;
                cpi_list::cpi_type current_cpi;

                {
                // try to create a cpi_instance implementing the required
                // function
                    proxy::mutex_type::scoped_lock lock (prxy->mtx_);
                    current_mode = prxy->select_run_mode(cpi_name, name, prefs,
                        true, no_no_list, oi);
                    BOOST_ASSERT(!prxy->cpis_.empty());

                // retrieve info to be able to store it in no_no_list later
                    current_cpi = prxy->cpis_.get_current();
                    info = current_cpi->get_adaptor_info();
                }

                saga::task t = dispatch_sync(current_mode, op_name, current_cpi,
                    sync, async, BOOST_PP_ENUM_PARAMS(K, arg));

                found_one = true;

                // store last known good adaptor in saga object
                prxy->set_last_known_good(info.get_cpi_id());
                return t;
            }
            catch (saga::exception const& e) {
                // save this exception, avoid NoAdaptor errors if we actually
                // found at least one
                saga::error err = e.get_error();
                if (!found_one || 
                    err < (saga::error)saga::adaptors::FirstAdaptorSpecificException)
                {
                    if (err >= (saga::error)saga::adaptors::FirstAdaptorSpecificException)
                    {
                        impl::runtime::get_impl(e)->set_error(NoSuccess);
                    }

                    impl::runtime::get_impl(e)->set_failed_cpi(info.get_cpi_id());
                    exceptions.add(e);
                }

                // throw, if we don't find anything
                if (err == (saga::error)saga::adaptors::NoAdaptor ||
                    err == (saga::error)saga::adaptors::NoAdaptorInfo)
                {
                    // The synchronous functions do not throw, any error gets
                    // reported during get_result and/or rethrow
                    err = exceptions.get_error();
                    if (err >= (saga::error)saga::adaptors::FirstAdaptorSpecificException)
                    {
                        err = NoSuccess;
                    }
                    saga::task t(saga::task::Failed);
                    return detail::set_task_exception(t, prxy, exceptions, err);
                }

                // just try the next adaptor
                no_no_list.push_back(info);
            }
            catch (std::exception const& e) {
                // save this exception
                TR1::shared_ptr<saga::impl::object> impl(
                    TR1::static_pointer_cast<saga::impl::object>(
                        TR1::const_pointer_cast<saga::impl::proxy>(
                            prxy->shared_from_this())));

                saga::exception se(runtime::get_object(impl),
                    std::string("std::exception caught: ") + e.what(), 
                    saga::NoSuccess);
                impl::runtime::get_impl(se)->set_failed_cpi(info.get_cpi_id());
                exceptions.add(se);

                // just try the next adaptor
                no_no_list.push_back(info);
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    template <
        typename Base, typename RetVal,
        BOOST_PP_ENUM_PARAMS(K, typename FuncArg),
        BOOST_PP_ENUM_PARAMS(K, typename Arg)
    >
    inline saga::task
    execute_async(proxy *prxy,
        char const *cpi_name, char const *name, char const *op_name,
        v1_0::preference_type const& prefs,
        void (Base::*sync)(RetVal&, BOOST_PP_ENUM_PARAMS(K, FuncArg)),
        saga::task (Base::*async)(BOOST_PP_ENUM_PARAMS(K, FuncArg)),
        BOOST_PP_ENUM_BINARY_PARAMS(K, Arg, const& arg))
    {
        TR1::shared_ptr<adaptor_selector_state> state(
            new adaptor_selector_state(prxy, cpi_name, name, op_name, prefs));

        return dispatch_async(prxy, state, sync, async, BOOST_PP_ENUM_PARAMS(K, arg));
    }

    ///////////////////////////////////////////////////////////////////////////
    template <
        typename Base, typename RetVal,
        BOOST_PP_ENUM_PARAMS(K, typename FuncArg),
        BOOST_PP_ENUM_PARAMS(K, typename Arg)
    >
    inline saga::task
    execute_sync_async(proxy *prxy,
        char const *cpi_name, char const *name, char const *op_name, bool is_sync,
        void (Base::*sync)(RetVal&, BOOST_PP_ENUM_PARAMS(K, FuncArg)),
        saga::task (Base::*async)(BOOST_PP_ENUM_PARAMS(K, FuncArg)),
        BOOST_PP_ENUM_BINARY_PARAMS(K, Arg, const& arg))
    {
        v1_0::preference_type const& prefs(detail::get_preferences(prxy->get_session()));
        return is_sync ?
            execute_sync(prxy, cpi_name, name, op_name, prefs, sync, async,
                BOOST_PP_ENUM_PARAMS(K, arg)) :
            execute_async(prxy, cpi_name, name, op_name, prefs, sync, async,
                BOOST_PP_ENUM_PARAMS(K, arg));
    }

#undef K

#endif // defined(BOOST_PP_IS_ITERATING)
