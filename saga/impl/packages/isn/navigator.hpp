//  Copyright (c) 2009 STFC
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef SAGA_IMPL_PACKAGES_ISN_NAVIGATOR_HPP
#define SAGA_IMPL_PACKAGES_ISN_NAVIGATOR_HPP

#include <string>
#include <vector>

#include <saga/saga/session.hpp>
#include <saga/impl/call.hpp>
#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/packages/isn/navigator_cpi.hpp>

namespace saga { namespace isn { class navigator; } }

namespace saga
{
  namespace impl
  {

    class navigator : public proxy
    {
      typedef v1_0::navigator_cpi navigator_cpi;
      typedef v1_0::preference_type preference_type;
  
      //initialize newly attached CPI instance
      void init();

      public:
       navigator (std::string model, saga::session const & s, saga::url loc);
       ~navigator (void);

      // factory support
      void create_impl_sync(saga::isn::navigator& retval);
      static saga::task create_impl_async(std::string const& model,
                                          saga::session const& s,
                                          saga::url const& loc);

       SAGA_CALL_IMPL_DECL_1 (list_related_entity_names,
                              std::string)

       SAGA_CALL_IMPL_DECL_4 (get_related_entities,
                              std::string,
                              std::string,
                              std::string,
                              std::vector<saga::isn::entity_data>)

       SAGA_CALL_IMPL_DECL_2 (get_entities,
                              std::string,
                              std::string)
     };
  } // namespace impl
} // namespace saga

/////////////////////////////////////////////////////////////////////////////////////////////

#endif // !define(SAGA_IMPL_PACKAGES_ISN_NAVIGATOR_HPP)
 
