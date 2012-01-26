//  Copyright (c) 2005-2009 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/assign/std/vector.hpp>

#include <saga/saga/adaptors/config.hpp>
#include <saga/saga/adaptors/adaptor.hpp>
#include <saga/saga/adaptors/task.hpp>

#include "default_replica_adaptor.hpp"
#include "logicaldirectory.hpp"
#include "logicalfile.hpp"
#include "context.hpp"

SAGA_ADAPTOR_REGISTER (replica::adaptor);

///////////////////////////////////////////////////////////////////////////////
namespace replica
{
//  register function for the SAGA engine
    saga::impl::adaptor_selector::adaptor_info_list_type
        adaptor::adaptor_register(saga::impl::session *s)
    {
        // list of implemented cpi's
        saga::impl::adaptor_selector::adaptor_info_list_type list;

        // create preferences
        preference_type prefs; 
        prefs.set(saga::attributes::context_type, "default_replica_db");

        // create logical file and logical directory cpi infos (each adaptor 
        // instance gets its own uuid) and add cpi_infos to list
        logical_file_cpi_impl::register_cpi(list, prefs, adaptor_uuid_);
        logical_directory_cpi_impl::register_cpi(list, prefs, adaptor_uuid_);
        context_cpi_impl::register_cpi(list, prefs, adaptor_uuid_);

        // create a default security context, if needed
        if (s->is_default_session())
        {
            typedef std::pair<std::string, std::string> entry_type;
            using namespace boost::assign;
            std::vector<entry_type> entries;

            entries += 
                entry_type(saga::attributes::context_type, "default_replica_db")
            ;
            
            s->add_proto_context(entries);
        }
        
        // and return list
        return list;
    }

///////////////////////////////////////////////////////////////////////////////
}   // namespace replica

