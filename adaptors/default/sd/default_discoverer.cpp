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

#include "default_discoverer.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace sd
{

/*
 * constructor
 */
discoverer_cpi_impl::discoverer_cpi_impl (proxy* p, cpi_info const& info, 
        saga::ini::ini const& glob_ini, saga::ini::ini const& adap_ini,
        TR1::shared_ptr<saga::adaptor> adaptor)
:   base_cpi(p, info, adaptor, cpi::Noflags)
{
}

/*
 * destructor
 */
discoverer_cpi_impl::~discoverer_cpi_impl (void)
{
}

/*
 * SAGA API functions
 */
void discoverer_cpi_impl::sync_list_services3 (
    std::vector<saga::sd::service_description>& retval, std::string svc_filter, 
    std::string data_filter, std::string vo_filter)
{
}

void discoverer_cpi_impl::sync_list_services2 (
    std::vector<saga::sd::service_description>& retval, std::string svc_filter, 
    std::string data_filter)
{
}

#if ! defined(SAGA_DEFAULT_SD_ADAPTOR_NO_ASYNCS)
///////////////////////////////////////////////////////////////////////////////
// The async call is something special because it creates a new connection to 
// the XmlRpc server which is used for this call only. Note that the async 
// close has no relation to this and does not influence any of the connections
// created by the async call.
saga::task discoverer_cpi_impl::async_list_services3 (
    std::string svc_filter, std::string data_filter, std::string vo_filter)
{
    return saga::adaptors::task("discoverer_cpi_impl::sync_list_services3", 
        shared_from_this(), &discoverer_cpi_impl::sync_list_services3, 
        svc_filter, data_filter, vo_filter);
}

saga::task discoverer_cpi_impl::async_list_services2 (
    std::string svc_filter, std::string data_filter)
{
   return discoverer_cpi_impl::async_list_services3(svc_filter, data_filter, "");
 
}
#endif

///////////////////////////////////////////////////////////////////////////////
}   // namespace rpc
