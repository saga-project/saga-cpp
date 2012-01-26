//  Copyright (c) 2009 STFC
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef SAGA_IMPL_PACKAGES_SD_DISCOVERER_HPP

#define SAGA_IMPL_PACKAGES_SD_DISCOVERER_HPP

#include <string>
#include <vector>

#include <saga/saga/session.hpp>
#include <saga/impl/call.hpp>
#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/packages/sd/discoverer_cpi.hpp>

namespace saga
{
  namespace impl
  {

    class discoverer : public proxy
    {
      typedef v1_0::discoverer_cpi discoverer_cpi;
      typedef v1_0::preference_type preference_type;
  
      //initialize newly attached CPI instance
      void init();

      public:
       discoverer (saga::session const & s, saga::url loc);
       ~discoverer (void);

      // factory support
      void create_impl_sync(saga::sd::discoverer& retval);
      static saga::task create_impl_async(saga::session const& s,
                                          saga::url const& loc);

       SAGA_CALL_IMPL_DECL_2 (list_services2,
                              std::string, std::string)
       SAGA_CALL_IMPL_DECL_3 (list_services3,
                              std::string, std::string, std::string)
     };
  } // namespace impl
} // namespace saga

/////////////////////////////////////////////////////////////////////////////////////////////

#endif // !define(SAGA_IMPL_PACKAGES_SD_DISCOVERER_HPP)
 
