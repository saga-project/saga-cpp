//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
// 
//  Use, modification and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef _SAGA_PROCESS_H_
#define _SAGA_PROCESS_H_ 1

#include <map>
#include <vector>
#include <iostream>

// other includes from SAGA (this pulls export definitions, which we actually
// want, but can't include directly for some reason
#include <saga/saga/util.hpp>

// boost process
#include <boost/process.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable : 4251 4231 4660)
#endif

namespace saga
{
  namespace adaptors
  {
    namespace utils
    {
      class SAGA_EXPORT process
      {
        private:
          std::string                            cmd_;
          std::vector <std::string>              args_;
          std::map    <std::string, std::string> env_;

          boost::process::child c_;

          std::vector <std::string> out_v_;
          std::vector <std::string> err_v_;

          int         exit_;
          bool        done_;
          bool        fail_;
          std::string msg_;


        public:
          process (void);

          process (std::string                            cmd); 
          process (std::vector <std::string>              const& args);
          process (std::map    <std::string, std::string> const& env);

          process (std::string                            cmd, 
                   std::vector <std::string>              const& args);
          process (std::string                            cmd, 
                   std::map    <std::string, std::string> const& env);

          process (std::string                            cmd, 
                   std::vector <std::string>              const& args, 
                   std::map    <std::string, std::string> const& env);

          void clear      (void);
          void set_cmd    (std::string cmd);

          void clear_args (void);
          void set_args   (std::vector <std::string> const& args);
          void add_arg    (std::string arg);
          void add_args   (std::string arg_1, 
                           std::string arg_2);

          void clear_env  (void);
          void set_env    (std::map <std::string, std::string> const& env);
          void add_env    (std::string key, std::string val);

          std::vector <std::string>  run_sync  (bool io = true);

          void                       clear_out (void);
          void                       clear_err (void);

          boost::process::pistream & get_out   (void);
          boost::process::pistream & get_err   (void);

          std::string                get_out_s (void);
          std::string                get_err_s (void);

          std::vector <std::string>  get_out_v (void);
          std::vector <std::string>  get_err_v (void);

          int         exitcode (void) { return exit_; }
          bool        done     (void) { return done_; }
          bool        fail     (void) { return fail_; }
          std::string msg      (void) { return msg_;  }
      };

    } // namespace utils

  } // namespace adaptors

} // namespace saga

#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // _SAGA_PROCESS_H_

