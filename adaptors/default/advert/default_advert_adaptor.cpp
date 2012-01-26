//  Copyright (c) 2005-2009 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/assign/std/vector.hpp>

#include <saga/saga/adaptors/config.hpp>
#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/adaptors/adaptor.hpp>

#include "default_advert_adaptor.hpp"
#include "advertdirectory.hpp"
#include "advert.hpp"
#include "context.hpp"

SAGA_ADAPTOR_REGISTER (advert::adaptor);

///////////////////////////////////////////////////////////////////////////////
namespace advert
{
    //  register function for the SAGA engine
    saga::impl::adaptor_selector::adaptor_info_list_type
        adaptor::adaptor_register(saga::impl::session *s)
    {
        // list of implemented cpi's
        saga::impl::adaptor_selector::adaptor_info_list_type list;

        // create preferences
        preference_type prefs; 
        prefs.set(saga::attributes::context_type, "default_advert_db");

        // create advert and advert directory cpi infos (each adaptor 
        // instance gets its own uuid) and add cpi_infos to list
        advert_cpi_impl::register_cpi(list, prefs, adaptor_uuid_);
        advertdirectory_cpi_impl::register_cpi(list, prefs, adaptor_uuid_);
        context_cpi_impl::register_cpi(list, prefs, adaptor_uuid_);

        // create a default security context, if needed
        if (s->is_default_session())
        {
            typedef std::pair<std::string, std::string> entry_type;
            using namespace boost::assign;
            std::vector<entry_type> entries;

            entries += 
                entry_type(saga::attributes::context_type, "default_advert_db")
            ;
            
            s->add_proto_context(entries);
        }
        
        // and return list
        return list;
    }

///////////////////////////////////////////////////////////////////////////////
}   // namespace advert

