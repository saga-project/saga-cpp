//  Copyright (c) 2005-2008 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/adaptors/config.hpp>
#include <saga/saga/adaptors/adaptor.hpp>
#include <saga/saga/adaptors/task.hpp>

#include "###suite###_###type###_adaptor.hpp"
#include "###suite###_###type###_logical_directory.hpp"
#include "###suite###_###type###_logical_file.hpp"

SAGA_ADAPTOR_REGISTER (###suite###_###type###::adaptor);

///////////////////////////////////////////////////////////////////////////////
namespace ###suite###_###type###
{
  //  register function for the SAGA engine
  saga::impl::adaptor_selector::adaptor_info_list_type
  adaptor::adaptor_register (saga::impl::session *s)
  {
    // list of implemented cpi's
    saga::impl::adaptor_selector::adaptor_info_list_type list;

    // create preferences
    preference_type prefs; // (std::string ("security"), std::string ("none"));

    // create logical file and logical directory cpi infos (each adaptor 
    // instance gets its own uuid) and add cpi_infos to list
    logical_file_cpi_impl::register_cpi      (list, prefs, adaptor_uuid_);
    logical_directory_cpi_impl::register_cpi (list, prefs, adaptor_uuid_);

    // and return list
    return list;
  }

}   // namespace ###suite###_###type###
///////////////////////////////////////////////////////////////////////////////

