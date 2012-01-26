//  Copyright (c) 2008, 2009 João Abecasis
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_ADAPTORS_CONDOR_JOB_IS_LOCALHOST_HPP_INCLUDED
#define SAGA_ADAPTORS_CONDOR_JOB_IS_LOCALHOST_HPP_INCLUDED

#include <saga/saga-defs.hpp>
#include <saga/saga/adaptors/utils.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/host_name.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/spirit/core/non_terminal/impl/static.hpp>

#include <set>
#include <string>

#include <saga/saga/adaptors/utils/is_local_address.hpp>

namespace saga { namespace adaptors { namespace utils 
{
    namespace detail 
    {
        struct is_local_address_data
        {
            typedef boost::asio::io_service io_service;
            typedef boost::asio::ip::tcp::resolver resolver;
            typedef boost::asio::ip::tcp::resolver::query query;
            typedef boost::asio::ip::tcp::resolver::iterator iterator;

#if !defined(SAGA_WINDOWS)
            std::string readline (FILE* fd)
            {
              char c[1];
              std::string line = "";

              size_t n = ::fread (&c, sizeof (char), 1, fd);

              while ( 1 == n )
              {
                // on EOL, return
                if ( c[0] == '\n' )
                {
                  return (line);
                }

                // append any other char to line
                line += c[0];

                n = ::fread (&c, sizeof (char), 1, fd);
              }

              // on read error, return what we have
              return (line);
            }
#endif            

            is_local_address_data()
                : resolver_(service_)
                , hostname_(boost::asio::ip::host_name(ec_))
            {
                iterator iter = resolve("localhost"), end;
                for (; iter != end; ++iter)
                    local_addresses_.insert(get_address(iter));

                if (!hostname_.empty())
                {
                    iter = resolve(hostname_);
                    for (; iter != end; ++iter)
                        local_addresses_.insert(get_address(iter));
                }

#if !defined(SAGA_WINDOWS)
                // we also accept whatever 'hostname' tells us to be a valid
                // local hostname
                {
                  FILE* fd = ::popen ("hostname", "r");

                  if ( fd != NULL )
                  {
                    std::string line = this->readline (fd);

                    while ( ! line.empty () )
                    {
                      std::vector <std::string> elems = saga::adaptors::utils::split (line);

                      for ( unsigned int i = 0; i < elems.size (); i++ )
                      {
                        local_addresses_.insert (elems[i]);
                      }
                     
                      line = this->readline (fd);
                    }

                    ::pclose (fd);
                  }
                }
#endif
            }

            iterator resolve(std::string host)
            {
                return resolver_.resolve(query(host, "0"), ec_);
            }

            static std::string get_address(iterator iter)
            {
                return iter->endpoint().address().to_string();
            }

            boost::system::error_code ec_;

            io_service service_;
            resolver resolver_;

            std::string hostname_;
            std::set<std::string> local_addresses_;
        };

        class is_local_address_tag
        {
            is_local_address_tag() {}
            friend bool saga::adaptors::utils::is_local_address(std::string const &);
        };

    }

    bool is_local_address(std::string const & host)
    {
        if (host.empty() || host == "localhost")
            return true;

        detail::is_local_address_data & data =
            boost::spirit::static_<
                detail::is_local_address_data, detail::is_local_address_tag>();

        for (detail::is_local_address_data::iterator iter = data.resolve(host), end;
                iter != end; ++iter)
            if (data.local_addresses_.count(data.get_address(iter)))
                return true;

        return false;
    }

}}} // namespace saga::adaptors::utils

#endif // include guard

