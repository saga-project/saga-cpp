//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_ENGINE_ADAPTOR_SELECTOR_STATE_HPP
#define SAGA_IMPL_ENGINE_ADAPTOR_SELECTOR_STATE_HPP

#include <saga/impl/engine/adaptor_selector.hpp>
#include <saga/impl/engine/cpi_info.hpp>
#include <saga/impl/engine/cpi_list.hpp>
#include <saga/impl/exception_list.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl {

    ///////////////////////////////////////////////////////////////////////////
    class adaptor_selector_state
    {
    public:
        adaptor_selector_state(proxy* prxy, char const *cpi_name, char const *name, 
                char const *op_name, v1_0::preference_type const& prefs)
          : proxy_(prxy), cpi_name_(cpi_name), name_(name), op_name_(op_name),
            prefs_(prefs), found_one_(false)
        {}

        template <typename Base>
        TR1::shared_ptr<Base> get_next_cpi(run_mode& current_mode,
            void (Base::**s)() = NULL, saga::task (Base::**a)() = NULL, 
            bool (Base::**p)() = NULL)
        {
            while (true) {
                try {
                    TR1::shared_ptr<v1_0::cpi> next_cpi;

                    // try to create a cpi_instance implementing the required
                    // function
                    {
                        boost::recursive_mutex::scoped_lock lock (proxy_->mtx_);
                        v1_0::op_info oi (name_);
                        run_mode mode = proxy_->select_run_mode(cpi_name_, name_, 
                            prefs_, false, no_no_list_, oi);
                        BOOST_ASSERT(!proxy_->cpis_.empty());

                    // retrieve info to be able to store it in no_no_list later
                        next_cpi = proxy_->cpis_.get_current();
                        info_ = next_cpi->get_adaptor_info();

                        current_mode = mode;
                        if (s) *s = oi.get_sync_func();
                        if (a) *a = oi.get_async_func();
                        if (p) *p = oi.get_prep_func();

                        found_one_ = true;
                    }

                    return TR1::static_pointer_cast<Base>(next_cpi);
                }
                catch (saga::exception const& e) {
                    // save this exception, avoid NoAdaptor errors if we actually
                    // found at least one
                    saga::error err = e.get_error();
                    if (!found_one_ || 
                        err < (saga::error)saga::adaptors::FirstAdaptorSpecificException)
                    {
                        impl::runtime::get_impl(e)->set_failed_cpi(info_.get_cpi_id());
                        exceptions_.add(e);
                    }

                    // throw, if we don't find anything
                    if (err == (saga::error)saga::adaptors::NoAdaptor ||
                        err == (saga::error)saga::adaptors::NoAdaptorInfo)
                    {
                        err = exceptions_.get_error();
                        if (err >= (saga::error)saga::adaptors::FirstAdaptorSpecificException)
                        {
                            err = NoSuccess;
                        }
                        SAGA_THROW_PLAIN_LIST_EX(proxy_, exceptions_, err);
                    }

                    // just try the next adaptor
                    no_no_list_.push_back(info_);
                }
            }
        }

        char const* get_op_name() const { return op_name_.c_str(); }

        void add_exception(saga::exception const& e)
        {
            impl::runtime::get_impl(e)->set_failed_cpi(info_.get_cpi_id());
            exceptions_.add(e);
        }

        void restart()
        {
            no_no_list_.push_back(info_);
        }

        void rethrow() const
        {
            if (exceptions_.get_error_count())
            {
                SAGA_THROW_PLAIN(proxy_, exceptions_.get_message(), 
                    exceptions_.get_error());
            }
        }

        void set_last_known_good()
        {
            proxy_->set_last_known_good(info_.get_cpi_id());
        }

    private:
        typedef boost::recursive_mutex mutex_type;

        proxy* proxy_;
        std::string cpi_name_;
        std::string name_;
        std::string op_name_;
        v1_0::preference_type prefs_;
        bool found_one_;
        v1_0::cpi_info info_;

        mutex_type instance_data_mtx_;
        adaptor_selector::adaptor_info_list_type no_no_list_;
        exception_list exceptions_;
    };

///////////////////////////////////////////////////////////////////////////////
}} // namespace saga::impl

#endif

