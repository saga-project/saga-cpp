//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SAGA_PACKAGES_STREAM_ADAPTORS_STREAM_HPP)
#define SAGA_PACKAGES_STREAM_ADAPTORS_STREAM_HPP

#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/session.hpp>
#include <saga/saga/context.hpp>

#include <saga/saga/stream.hpp>
#include <saga/saga/adaptors/packages/stream_cpi_instance_data.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors {

    class SAGA_STREAM_PACKAGE_EXPORT stream
    :   public saga::stream::stream
    {
    public:
        stream (saga::adaptors::v1_0::stream_cpi_instance_data* data, 
            saga::session const& s);
        stream (saga::adaptors::v1_0::stream_cpi_instance_data* data);

        ~stream();
    };

    std::string SAGA_STREAM_PACKAGE_EXPORT get_state_name(saga::stream::state s);

///////////////////////////////////////////////////////////////////////////////
}}    // namespace saga::adaptors

#endif // !defined(SAGA_PACKAGES_STREAM_ADAPTORS_STREAM_HPP)
