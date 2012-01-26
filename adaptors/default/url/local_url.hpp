//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(ADAPTORS_LOCAL_URL_FEB_10_2009_0259PM)
#define ADAPTORS_LOCAL_URL_FEB_10_2009_0259PM

#include <string>

#include <saga/saga/types.hpp>

#include <saga/saga/adaptors/config.hpp>
#include <saga/saga/adaptors/adaptor.hpp>
#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/url_cpi_instance_data.hpp>

#include <saga/impl/url_cpi.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace adaptors
{
    ///////////////////////////////////////////////////////////////////////////
    class url_cpi_impl 
    :   public saga::adaptors::v1_0::url_cpi<url_cpi_impl>
    {
    private:
        typedef saga::adaptors::v1_0::url_cpi<url_cpi_impl> base_cpi;
        
    public:
        /*! constructor of the url adaptor */
        url_cpi_impl (proxy* p, cpi_info const& info, 
            saga::ini::ini const& glob_ini, saga::ini::ini const& adap_ini,
            TR1::shared_ptr<saga::adaptor> adaptor);

        /*! destructor of the url adaptor */
        ~url_cpi_impl  (void);

        /*! implementation of SAGA::url functions */
        void sync_translate(saga::url& retval, std::string scheme);

#if ! defined(SAGA_DEFAULT_URL_ADAPTOR_NO_ASYNCS)
        saga::task async_translate(std::string scheme);
#endif 
    };

///////////////////////////////////////////////////////////////////////////////
} 

#endif 

