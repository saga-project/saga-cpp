//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/adaptors/config.hpp>
#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/adaptors/adaptor.hpp>

#include "default_rpc_adaptor.hpp"
#include "default_rpc.hpp"

SAGA_ADAPTOR_REGISTER (rpc::rpc_adaptor);

///////////////////////////////////////////////////////////////////////////////
namespace rpc
{

    // register function for the SAGA engine
    saga::impl::adaptor_selector::adaptor_info_list_type 
    rpc_adaptor::adaptor_register (saga::impl::session *s)
    {
        // list of implemented cpi's
        saga::impl::adaptor_selector::adaptor_info_list_type list;

        // create preferences
        preference_type prefs; // (std::string ("security"), std::string ("none"));

        // create rpc cpi infos (each adaptor 
        // instance gets its own uuid) and add cpi_infos to list
        rpc_cpi_impl::register_cpi(list, prefs, adaptor_uuid_);

        // and return list
        return list;
    }

///////////////////////////////////////////////////////////////////////////////
}   // namespace rpc
