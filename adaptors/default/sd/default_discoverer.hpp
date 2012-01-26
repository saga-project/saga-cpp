//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ADAPTORS_DEFAULT_SD_DISCOVERER_HPP
#define ADAPTORS_DEFAULT_SD_DISCOVERER_HPP

#include <string>

#include <saga/saga/types.hpp>

#include <saga/saga/adaptors/config.hpp>
#include <saga/saga/adaptors/adaptor.hpp>
#include <saga/saga/adaptors/packages/discoverer_cpi_instance_data.hpp>
#include <saga/saga/adaptors/instance_data.hpp>

#include <saga/impl/packages/sd/discoverer_cpi.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace sd
{

///////////////////////////////////////////////////////////////////////////////
/**
  * This adaptor implements the functionality of the Saga API "discoverer".
  * It defines the functions declared in its base class, discoverer_cpi.
  *
  * @note This adaptor is written for test purposes only; it is possible that
  * it does not function exactly like the Saga API dictates.
  * @see The documentation of the Saga API
  */
    class discoverer_cpi_impl 
    :   public saga::adaptors::v1_0::discoverer_cpi<discoverer_cpi_impl>
    {
    private:
        typedef saga::adaptors::v1_0::discoverer_cpi<discoverer_cpi_impl> 
            base_cpi;
        
    public:
        // constructor of the discoverer CPI 
        discoverer_cpi_impl (proxy* p, cpi_info const& info, 
            saga::ini::ini const& glob_ini, saga::ini::ini const& adap_ini,
            TR1::shared_ptr<saga::adaptor> adaptor);

        /*! destructor of the rpc adaptor */
        ~discoverer_cpi_impl  (void);

        /*! implementation of SAGA::rpc functions */
        void sync_list_services3 (std::vector<saga::sd::service_description>&, 
            std::string svc_filter, std::string vo_filter, 
            std::string data_filter);

        void sync_list_services2 (std::vector<saga::sd::service_description>&, 
            std::string svc_filter, std::string data_filter);

#if ! defined(SAGA_DEFAULT_SD_ADAPTOR_NO_ASYNCS)
        saga::task async_list_services3 (
            std::string svc_filter, std::string vo_filter, 
            std::string data_filter);

        saga::task async_list_services2 (
            std::string svc_filter, std::string data_filter);
#endif 
    };  

///////////////////////////////////////////////////////////////////////////////
}   // namespace sd

#endif

