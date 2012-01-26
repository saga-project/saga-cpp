//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <ctime>
#include <iostream>

#include <saga/saga/exception.hpp>
#include <saga/saga/url.hpp>
#include <saga/saga/adaptors/task.hpp>

#include <saga/impl/config.hpp>

#include "local_url.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace adaptors
{
    ///////////////////////////////////////////////////////////////////////////
    url_cpi_impl::url_cpi_impl (proxy* p, cpi_info const& info, 
            saga::ini::ini const& glob_ini, saga::ini::ini const& adap_ini,
            TR1::shared_ptr<saga::adaptor> adaptor)
    :   base_cpi(p, info, adaptor, cpi::Noflags)
    {
        // first usage of this adaptor
        saga::url url;
        {
            instance_data data (this);
            url = data->url_;
        }

        std::string scheme(url.get_scheme());
        if (!scheme.empty() && scheme != "any" && scheme != "file") 
        {
            SAGA_OSSTREAM strm;
            strm << "Cannot handle schemes other than 'any' or 'file': " 
                 << url.get_string();
            SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::adaptors::AdaptorDeclined);
        }
    }

    url_cpi_impl::~url_cpi_impl (void)
    {
    }

    // SAGA API function
    void url_cpi_impl::sync_translate(saga::url& retval, std::string required_scheme)
    {
        if (!required_scheme.empty() && required_scheme != "file")
        {
            SAGA_OSSTREAM strm;
            strm << "Cannot convert to schemes other than 'file', "
                    "requested scheme is: " << required_scheme;
            SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), 
                saga::adaptors::AdaptorDeclined);
        }

        saga::url url;
        {
            instance_data data (this);
            url = data->url_;
        }

        std::string scheme(url.get_scheme());
        if (scheme.empty() || scheme == "any")
            url.set_scheme("file");

        retval = url;
    }

#if ! defined(SAGA_DEFAULT_URL_ADAPTOR_NO_ASYNCS)
    ///////////////////////////////////////////////////////////////////////////
    saga::task url_cpi_impl::async_translate(std::string scheme)
    {
        return saga::adaptors::task("url_cpi_impl::sync_translate",
            shared_from_this(), &url_cpi_impl::sync_translate, scheme);
    }
#endif

///////////////////////////////////////////////////////////////////////////////
}

