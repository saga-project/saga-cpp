//  Copyright (c) 2005-2006 Andre Merzky (andre@merzky.net)
// 
//  Use, modification and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef SAGA_UTILS_UTILS_HPP
#define SAGA_UTILS_UTILS_HPP

#include <saga/saga/url.hpp>
#include <saga/saga/util.hpp>

#include <saga/saga/adaptors/utils/singleton.hpp>
#include <saga/saga/adaptors/utils/is_local_address.hpp>
#include <saga/saga/adaptors/utils/ini/ini.hpp>
#include <saga/saga/adaptors/utils/process/process.hpp>
 
namespace saga
{
  namespace adaptors
  {
    namespace utils
    {
      SAGA_EXPORT std::vector <std::string> split (std::string                       line,
                                                   char                              delim  = '\t',
                                                   int                               number = -1 );
      SAGA_EXPORT std::vector <std::string> split_commandline 
                                                  (std::string const               & line);
      SAGA_EXPORT std::vector <std::string> head  (std::size_t                       n, 
                                                   std::vector <std::string> const & in);
      SAGA_EXPORT std::vector <std::string> tail  (std::size_t                       n,
                                                   std::vector <std::string> const & in);
      SAGA_EXPORT std::vector <std::string> rev   (std::vector <std::string> const & in);
      SAGA_EXPORT std::vector <std::string> tac   (std::vector <std::string> const & in);
      SAGA_EXPORT std::vector <std::string> grep  (std::string                       pattern, 
                                                   std::vector <std::string> const & in);

    } // namespace utils
  } // namespace adaptors 
} // namespace saga

#endif // SAGA_UTILS_UTILS_HPP

