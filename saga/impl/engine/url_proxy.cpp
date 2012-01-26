//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/url_cpi_instance_data.hpp>

#include <saga/impl/engine/url_proxy.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl
{
    url_proxy::url_proxy(saga::session const& s, saga::url const& url)
      : proxy (saga::object::URL, s)
    {
         // initialize the instance data
        typedef adaptors::v1_0::url_cpi_instance_data instance_data_type;
        typedef adaptors::instance_data <instance_data_type> url_instance_data;

        url_instance_data data;
        data.init_data (this, TR1::shared_ptr<instance_data_type>(
            new instance_data_type(url)));
    }

    void url_proxy::init()
    {
        try {
            // initialize a CPI instance and execute required functions (such as
            // creating the file etc.)
            this->initcpi(saga::adaptors::url_cpi);
        }
        catch (saga::exception const& e) {
            // map special exceptions
            switch (e.get_error()) {
            case saga::adaptors::AdaptorDeclined:
            case saga::adaptors::NoAdaptor:
            case saga::adaptors::NoAdaptorInfo:
            case saga::adaptors::Unexpected:
                SAGA_THROW_PLAIN(e.get_object(), e.what(), saga::NoSuccess);
            // just rethrow otherwise
            default:
                throw;
            }
        }
    }

    url_proxy::~url_proxy()
    {
        // free the instance data
        typedef adaptors::v1_0::url_cpi_instance_data instance_data_type;
        typedef adaptors::instance_data<instance_data_type> url_instance_data;

        url_instance_data data;
        data.release_data(this);
    }

    SAGA_CALL_IMPL_IMPL_1(url_proxy, url_cpi, translate, std::string)

}}

