//  Copyright (c) 2006-2007 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "###suite###_###type###_adaptor.hpp"

#include <saga/saga/adaptors/config.hpp>
#include <saga/saga/adaptors/adaptor.hpp>
#include <saga/saga/adaptors/attribute.hpp>

using namespace ###suite###_###type###_adaptor;

#define ADAPTORS_###SUITE###_###TYPE###_TYPE "###suite###"

// struct cert_info_t {
//     bool         success;
//     std::string  errormessage;
//     std::string  identity;
//     std::string  path;
// };
// 
// cert_info_t get_cert_info (std::string proxy_path = "");

SAGA_ADAPTOR_REGISTER (context_adaptor);

///////////////////////////////////////////////////////////////////////////////
//  constructor
context_cpi_impl::context_cpi_impl (proxy                * p, 
                                    cpi_info       const & info,
                                    saga::ini::ini const & glob_ini, 
                                    saga::ini::ini const & adap_ini,
                                    TR1::shared_ptr <saga::adaptor> adaptor)
    : base_cpi (p, info, adaptor, cpi::Noflags)
{
  saga::adaptors::attribute attr (this);

  if ( attr.attribute_exists (saga::attributes::context_type) )
  {
    if ( ADAPTORS_###SUITE###_###TYPE###_TYPE != 
         attr.get_attribute (saga::attributes::context_type) )
    {
      SAGA_ADAPTOR_THROW ("Can't handle context types others than '###suite###'", 
                          saga::BadParameter);
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  destructor
context_cpi_impl::~context_cpi_impl (void)
{
}

// ///////////////////////////////////////////////////////////////////////////////
// //
// void context_cpi_impl::sync_set_defaults (saga::impl::void_t &)
// {    
//   saga::adaptors::attribute attr (this);
// 
//   if ( attr.attribute_exists (saga::attributes::context_type) )
//   {
//     if ( ADAPTORS_###SUITE###_###TYPE###_TYPE != 
//          attr.get_attribute (saga::attributes::context_type) )
//     {
//       SAGA_ADAPTOR_THROW ("Can't handle context types others than '###suite###'", 
//                           saga::BadParameter);
//     }
// 
//     if ( "" == attr.get_attribute (saga::attributes::context_type) )
//     {
//       SAGA_ADAPTOR_THROW ("Context Type cannot have an empty value", 
//                           saga::IncorrectState);
//     }
// 
//     cert_info_t ci;
// 
//     if ( attr.attribute_exists (saga::attributes::context_userproxy) ) 
//     {
//       // this call looks for a valid proxy file in the location described by
//       // saga::attributes::context_userproxy
//       ci = get_cert_info (attr.get_attribute (saga::attributes::context_userproxy));
//     }
//     else
//     {
//       // this call looks for a valid cert in a default location
//       ci = get_cert_info();    
//     }
// 
//     if ( true == ci.success )
//     {
//       // found a valid cert, copy information over
//       attr.set_attribute (saga::attributes::context_userproxy, ci.path);
//       attr.set_attribute (saga::attributes::context_userid,    ci.identity);
//       // ...
//     }
//     else
//     {
//       SAGA_ADAPTOR_THROW (ci.errormessage, saga::NoSuccess);
//     }
//   }
// }
// //
// ///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//
saga::impl::adaptor_selector::adaptor_info_list_type
context_adaptor::adaptor_register (saga::impl::session * s)
{
  // list of implemented cpi's
  saga::impl::adaptor_selector::adaptor_info_list_type infos;
  preference_type prefs; 

  context_cpi_impl::register_cpi (infos, prefs, adaptor_uuid_);

  // create a default security context if this is a default session
  if ( s->is_default_session () )
  {
    std::vector <std::pair <std::string, std::string> > entries;

    std::pair <std::string, std::string> entry (saga::attributes::context_type, 
                                                "###suite###");

    entries.push_back (entry);

    s->add_proto_context (entries);

  }

  return infos;
}
//
///////////////////////////////////////////////////////////////////////////////


// ///////////////////////////////////////////////////////////////////////////////
// //
// // init a cert, either from a given path, or from a default location
// //
// cert_info_t get_cert_info (std::string path)
// {
//   cert_info_t ci;
// 
//   ci.success      = false;
//   ci.errormessage = "";
//   
//   return ci;
// }
// //
// ///////////////////////////////////////////////////////////////////////////////

