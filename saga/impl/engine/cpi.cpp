//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2006      Stephan Hirmer (shirmer@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <cctype>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>

#include <saga/saga/util.hpp>
#include <saga/saga/adaptors/task_declaration.hpp>
#include <saga/impl/runtime.hpp>

#include <saga/impl/engine/cpi.hpp>
#include <saga/impl/engine/session.hpp>
#include <saga/impl/engine/proxy.hpp>

#include <saga/saga/adaptors/adaptor.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl { namespace v1_0 { 

    cpi::cpi (saga::impl::proxy * p, cpi_info const& info,
            TR1::shared_ptr<saga::adaptor> adaptor, cpi::flags flags) 
    :   bulk_(NULL), proxy_(p), info_(info), adaptor_(adaptor), flags_(flags)
    {
    }

    // get_proxy() is needed for the adaptor_data<>
    proxy * cpi::get_proxy (void) const 
    { 
        return proxy_; 
    }

    TR1::shared_ptr<saga::adaptor> cpi::get_adaptor() const
    {
        return adaptor_;
    }

    void cpi::init_bulk(void)
    {
        delete bulk_;
        bulk_ = NULL;
    }

    saga::session& cpi::get_session()
    {
        return proxy_->get_session();
    }
    saga::session const& cpi::get_session() const
    {
        return proxy_->get_session();
    }

    std::vector<saga::uuid> cpi::execute_bulk(void)
    {
        if (NULL == bulk_)
        {
            SAGA_THROW("Bulk::exception: there is no bulk to be executed!",
                saga::IncorrectState);
        }
        return bulk_->execute_bulk();
    }

    saga::task_base::state cpi::get_state(saga::uuid task_uuid)
    {
        SAGA_THROW("cpi::get_state : Don't call get_state on cpi base object!",
            saga::NotImplemented);
        return saga::task_base::Unknown;
    }

    bool cpi::wait (double timeout, saga::uuid task_uuid)
    {
        SAGA_THROW("cpi::wait : Don't call wait on cpi base object!",
            saga::NotImplemented);
        return false;
    }

    void cpi::cancel(saga::uuid task_uuid)
    {
        SAGA_THROW("cpi::cancel : Don't call cancel on cpi base class",
            saga::NotImplemented);
    }

    // returns the uuid of the associated adaptor object
    saga::uuid const& cpi::get_adaptor_uuid (void) const
    {
        return adaptor_->get_uuid();
    }

    inline bool is_digit(char c)
    {
        return std::isdigit(c) ? true : false;
    }

    inline bool register_member(char const* envkey)
    {
        char *env = saga::detail::safe_getenv(envkey);
        if (NULL == env || !boost::all(env, &is_digit))
            return false;

        if (0 == std::strlen(env))
            return true;    // backwards compatible solution

        return 0 != boost::lexical_cast<int>(std::string(env));
    }

    // return, whether sync/async cpi functions should be implemented
    bool cpi::register_sync_member(char const* name)
    {
        // sync members are registered by default as long as they are not
        // explicitly omitted while registering async ones
        return !register_member("SAGA_RUN_ASYNC") || register_member("SAGA_RUN_SYNC");
    }

    bool cpi::register_async_member(char const* name)
    {
        // async members are registered as long as not explicitly disabled
        return /*NULL == saga::detail::safe_getenv("SAGA_RUN_ASYNC") ||*/ register_member("SAGA_RUN_ASYNC");
    }

    bool cpi::register_bulk_member(char const* name)
    {
        // bulk members are registered only if requested
        return register_member("SAGA_RUN_BULK");
    }

///////////////////////////////////////////////////////////////////////////////
}}} // namespace saga::impl::v1_0

