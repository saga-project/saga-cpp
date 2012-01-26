//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/util.hpp>
#include <saga/saga/adaptors/config.hpp>
#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/adaptors/adaptor.hpp>

#include "default_file_adaptor.hpp"
#include "default_dir.hpp"
#include "default_file.hpp"

SAGA_ADAPTOR_REGISTER (file_adaptor);

//  register function for the SAGA engine
saga::impl::adaptor_selector::adaptor_info_list_type
    file_adaptor::adaptor_register(saga::impl::session *s)
{
    // list of descriptions for implemented cpi's
    saga::impl::adaptor_selector::adaptor_info_list_type infos;

    // create preferences
    preference_type prefs; // (std::string ("security"), std::string ("none"));

    // create file and directory cpi infos (each adaptor instance gets its own 
    // uuid) and add cpi_infos to list
    file_cpi_impl::register_cpi(infos, prefs, adaptor_uuid_);
    dir_cpi_impl::register_cpi(infos, prefs, adaptor_uuid_);

    // and return list
    return infos;
}

