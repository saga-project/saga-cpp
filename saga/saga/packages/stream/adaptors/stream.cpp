//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/task.hpp>
#include <saga/saga/session.hpp>
#include <saga/saga/packages/stream/adaptors/stream.hpp>

#include <saga/impl/stream.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors {

    stream::stream (saga::adaptors::v1_0::stream_cpi_instance_data* data, 
              saga::session const& s)
      : saga::stream::stream(new saga::impl::stream(data, s))
    {
    }

    stream::stream (saga::adaptors::v1_0::stream_cpi_instance_data* data)
      : saga::stream::stream(
          new saga::impl::stream(data, saga::detail::get_the_session()))
    {
    }

    stream::~stream()
    {
    }

    std::string get_state_name(saga::stream::state s)
    {
        switch (s) {
        case saga::stream::New:     return "New";
        case saga::stream::Open:    return "Open";
        case saga::stream::Closed:  return "Closed";
        case saga::stream::Dropped: return "Dropped";
        case saga::stream::Error:   return "Error";
        case saga::stream::Unknown:
        default:
            break;
        }
        return "<Unknown>";
    }
    
///////////////////////////////////////////////////////////////////////////////
}}    // namespace saga::adaptors

