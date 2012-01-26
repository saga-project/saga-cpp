//  Copyright (c) 2005-2009 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/assign/std/vector.hpp>

#include <saga/saga/adaptors/config.hpp>
#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/adaptors/adaptor.hpp>

#include "default_cpr_adaptor.hpp"
#include "cpr_directory.hpp"
#include "cpr_checkpoint.hpp"
#include "cpr_job.hpp"
#include "cpr_job_service.hpp"
#include "cpr_context.hpp"

SAGA_ADAPTOR_REGISTER (cpr::adaptor);

///////////////////////////////////////////////////////////////////////////////
namespace cpr
{
//  register function for the SAGA engine
    saga::impl::adaptor_selector::adaptor_info_list_type
        adaptor::adaptor_register(saga::impl::session *s)
    {
        // list of implemented cpi's
        saga::impl::adaptor_selector::adaptor_info_list_type list;

        // create preferences
        preference_type prefs; // (std::string ("security"), std::string ("none"));

        // create logical file and logical directory cpi infos (each adaptor 
        // instance gets its own uuid) and add cpi_infos to list
        cpr_checkpoint_cpi_impl::register_cpi  (list, prefs, adaptor_uuid_);
        cpr_directory_cpi_impl::register_cpi   (list, prefs, adaptor_uuid_);
        cpr_job_cpi_impl::register_cpi         (list, prefs, adaptor_uuid_);
        cpr_job_service_cpi_impl::register_cpi (list, prefs, adaptor_uuid_);
        context_cpi_impl::register_cpi         (list, prefs, adaptor_uuid_);

        // create a default security context, if needed
        if ( s->is_default_session () )
        {
            typedef std::pair <std::string, std::string> entry_type;
            using namespace boost::assign;
            std::vector <entry_type> entries;

            entries += 
                entry_type (saga::attributes::context_type,     "UserPass"),
                entry_type (saga::attributes::context_userid,   "anonymous"),
                entry_type (saga::attributes::context_userpass, "anonymous")
            ;
            
            s->add_proto_context (entries);
        }
        
        // and return list
        return list;
    }

///////////////////////////////////////////////////////////////////////////////
}   // namespace cpr

