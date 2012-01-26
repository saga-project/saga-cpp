//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/plugin/export_plugin.hpp>
#include <boost/asio.hpp>

#include <saga/saga/adaptors/config.hpp>
#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/adaptors/adaptor.hpp>
#include <saga/saga/exception.hpp>

#include "stream_adaptor.hpp"
#include "stream.hpp"
#include "stream_server.hpp"

SAGA_ADAPTOR_REGISTER (stream::saga_default_stream_adaptor);

///////////////////////////////////////////////////////////////////////////////
namespace stream
{
    /*
     * register function for saga_engine
     */
    saga::impl::adaptor_selector::adaptor_info_list_type 
      saga_default_stream_adaptor::adaptor_register (saga::impl::session *s)
    {
      // list of implemented cpi's
      saga::impl::adaptor_selector::adaptor_info_list_type list;

      // create preferences
      preference_type prefs; // (std::string ("security"), std::string ("none"));

      // create stream adaptor infos
      // and add cpi_infos to list
      stream::register_cpi(list, prefs, adaptor_uuid_);
      stream_server::register_cpi(list, prefs, adaptor_uuid_);

      // and return list
      return (list);
    }

}   // namespace stream
