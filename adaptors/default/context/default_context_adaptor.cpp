//  Copyright (c) 2009 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "default_context_adaptor.hpp"

#include <unistd.h>
#include <pwd.h>

#include <saga/saga/adaptors/config.hpp>
#include <saga/saga/adaptors/adaptor.hpp>
#include <saga/saga/adaptors/attribute.hpp>

#include <boost/filesystem/operations.hpp>
#include <boost/format.hpp>

using namespace default_context_adaptor;

#define ADAPTORS_DEFAULT_CONTEXT_TYPE "UserPass"

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
    if ( ADAPTORS_DEFAULT_CONTEXT_TYPE != 
         attr.get_attribute (saga::attributes::context_type) )
    {
      SAGA_OSSTREAM strm;
      strm << "Can't handle context with types other than '" 
           << ADAPTORS_DEFAULT_CONTEXT_TYPE 
           << "' (got: " << attr.get_attribute (saga::attributes::context_type) << ")";

      SAGA_ADAPTOR_THROW (SAGA_OSSTREAM_GETSTRING (strm), saga::BadParameter);
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//  destructor
context_cpi_impl::~context_cpi_impl (void)
{
}

///////////////////////////////////////////////////////////////////////////////
//
void context_cpi_impl::sync_set_defaults (saga::impl::void_t &)
{    
  saga::adaptors::attribute attr (this);
 
  if ( attr.attribute_exists (saga::attributes::context_type) )
  {
    if ( ADAPTORS_DEFAULT_CONTEXT_TYPE != 
         attr.get_attribute (saga::attributes::context_type) )
    {
      SAGA_OSSTREAM strm;
      strm << "Can't handle context with types other than '" 
           << ADAPTORS_DEFAULT_CONTEXT_TYPE 
           << "' (got: " << attr.get_attribute (saga::attributes::context_type) << ")";

      SAGA_ADAPTOR_THROW (SAGA_OSSTREAM_GETSTRING(strm), saga::BadParameter);
    }

    // just ensure that username is set.  Passwd can be empty.
    if ( ! attr.attribute_exists (saga::attributes::context_userid) )
    { 
      struct passwd * passwd = ::getpwent ();

      if ( passwd          != NULL && 
           passwd->pw_name != NULL )
      {
        attr.set_attribute (saga::attributes::context_userid, passwd->pw_name);
      }
    }
  }
}
//
///////////////////////////////////////////////////////////////////////////////


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
                                                ADAPTORS_DEFAULT_CONTEXT_TYPE);
    entries.push_back (entry);

    s->add_proto_context (entries);

  }

  return infos;
}
//
///////////////////////////////////////////////////////////////////////////////


