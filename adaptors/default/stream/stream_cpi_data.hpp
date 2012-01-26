//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ADAPTORS_DEFAULT_STREAM_CPI_DATA_HPP
#define ADAPTORS_DEFAULT_STREAM_CPI_DATA_HPP

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

#include <saga/saga/adaptors/packages/stream_cpi_instance_data.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace stream
{
    struct stream_cpi_data
      : saga::adaptors::v1_0::stream_cpi_instance_data
    {
        stream_cpi_data(saga::url const& url, 
                TR1::shared_ptr<boost::asio::ip::tcp::socket> sock)
          : saga::adaptors::v1_0::stream_cpi_instance_data(url, false), 
            sock_(sock)
        {
        }

        TR1::shared_ptr<boost::asio::ip::tcp::socket> sock_;
    };
}

#endif
