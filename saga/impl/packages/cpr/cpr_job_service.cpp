#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/cpr_job_service.cpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 0, output: null)
#endif
#define SAGA_NO_IMPORT_ATTRIBUTE

#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/packages/cpr_job_service_cpi_instance_data.hpp>
#include <saga/saga/detail/attribute_impl.hpp>
#include <saga/impl/cpr.hpp>

#ifdef SAGA_DEBUG
#include <saga/impl/packages/cpr/preprocessed/job_service.cpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, output: "preprocessed/cpr_job_service.cpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga 
{ 
  namespace impl 
  {
    cpr_job_service::cpr_job_service (saga::session const& s, saga::url rm)
        : saga::impl::job_service (saga::object::CPRJobService, s)
    {
        // initialize the instance data
        typedef adaptors::v1_0::cpr_job_service_cpi_instance_data instance_data_type;
        typedef adaptors::instance_data <instance_data_type> cpr_job_service_instance_data;

        {
            cpr_job_service_instance_data data;
            data.init_data (this, TR1::shared_ptr <instance_data_type> (
                new instance_data_type (rm)));
        } // job_service_instance_data lock goes out of scope
    }

    void cpr_job_service::init()
    {
        try {
            // initialize a CPI instance and execute required functions
            this->initcpi(saga::adaptors::cpr_job_service_cpi);
        }
        catch (saga::exception const& e) {
            // map special exceptions
            switch (e.get_error()) {
            case saga::adaptors::AdaptorDeclined:
            case saga::adaptors::NoAdaptor:
            case saga::adaptors::NoAdaptorInfo:
            case saga::adaptors::Unexpected:
                SAGA_THROW_PLAIN(e.get_object(), e.get_message(), saga::NoSuccess);
            // just rethrow otherwise
            default:
                throw;
            }
        }
    }

    cpr_job_service::~cpr_job_service (void) 
    {
        // free the instance data
        typedef adaptors::v1_0::cpr_job_service_cpi_instance_data instance_data_type;
        typedef adaptors::instance_data <instance_data_type> cpr_job_service_instance_data;

        cpr_job_service_instance_data data;
        data.release_data (this);
    }

    ///////////////////////////////////////////////////////////////////////////
    // factory support
    void cpr_job_service::create_impl_sync(saga::cpr::service& retval)
    {
        this->init();
        retval = saga::cpr::service(this);
    }

    saga::task cpr_job_service::create_impl_async(saga::session const& s, 
        saga::url const& rm)
    {
        TR1::shared_ptr<cpr_job_service> impl(new cpr_job_service(s, rm));

        bool (cpr_job_service::*prep)(saga::cpr::service&, saga::uuid) = NULL;
        return saga::adaptors::task("__init__", TR1::shared_ptr<v1_0::cpi>(), 
            impl.get(), &cpr_job_service::create_impl_sync, prep);
    }

    // API methods of cpr_job_service
    SAGA_CALL_IMPL_IMPL_1 (cpr_job_service, cpr_job_service_cpi, 
                           create_job, saga::cpr::description);
    SAGA_CALL_IMPL_IMPL_2 (cpr_job_service, cpr_job_service_cpi, 
                           create_job_cpr, saga::cpr::description, saga::cpr::description);
    SAGA_CALL_IMPL_IMPL_5 (cpr_job_service, cpr_job_service_cpi, 
                           run_job, std::string, std::string,
                           saga::job::ostream &, 
                           saga::job::istream &, 
                           saga::job::istream &);
    SAGA_CALL_IMPL_IMPL_2 (cpr_job_service, cpr_job_service_cpi, 
                           run_job_noio, std::string, std::string);

}} // namespace saga::impl
///////////////////////////////////////////////////////////////////////////////


#endif

