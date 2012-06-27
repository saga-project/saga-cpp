//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
// 
//  Use, modification and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  TODO: add stdin handling

#include <boost/config.hpp>
#include <saga/saga/adaptors/utils/process/process.hpp>

// for the throw macro...
#include <saga/saga/object.hpp>
#include <saga/impl/exception.hpp>

#if defined(BOOST_PROCESS_WIN32_API)
///////////////////////////////////////////////////////////////////////////////
//  this is a hack to make boost::process work (avoid multiple definitions)
//  (this will go away as soon as it is fixed in Boost.Process)
const boost::process::detail::file_handle::handle_type 
    boost::process::detail::file_handle::INVALID_VALUE = INVALID_HANDLE_VALUE;
#endif

namespace saga
{
  namespace adaptors
  {
    namespace utils
    {
      process::process (void)
        : cmd_  (""), 
          exit_ (0),
          done_ (false),
          fail_ (false)
      {
      }

      process::process (std::string cmd)
        : cmd_  (cmd), 
          exit_ (0),
          done_ (false),
          fail_ (false)
      {
      }

      process::process (std::vector <std::string> const& args)
        : args_ (args),
          exit_ (0),
          done_ (false),
          fail_ (false)
      {
      }

      process::process (std::map    <std::string, std::string> const& env)
        : env_  (env), 
          exit_ (0),
          done_ (false),
          fail_ (false)
      {
      }

      process::process (std::string               cmd, 
                        std::vector <std::string> const& args)
        : cmd_  (cmd), 
          args_ (args),
          exit_ (0),
          done_ (false),
          fail_ (false)
      {
      }

      process::process (std::string                            cmd, 
                        std::map    <std::string, std::string> const& env)
        : cmd_  (cmd), 
          env_  (env), 
          exit_ (0),
          done_ (false),
          fail_ (false)
      {
      }

      process::process (std::string                            cmd, 
                        std::vector <std::string>              const& args, 
                        std::map    <std::string, std::string> const& env)
        : cmd_  (cmd), 
          args_ (args),
          env_  (env), 
          exit_ (0),
          done_ (false),
          fail_ (false)
      {
      }

      void process::clear (void)
      {
        cmd_  = "";

        env_.clear ();
        args_.clear ();

        msg_   = "";
        exit_  = 0,
        fail_  = false;
        done_  = false;

        out_v_.clear ();
        err_v_.clear ();
      }

      void process::set_cmd (std::string cmd)
      {
        cmd_= cmd;
      }

      void process::clear_args (void)
      {
        args_.clear ();
      }

      void process::set_args (std::vector <std::string> const& args)
      {
        args_ = args;
      }

      void process::add_args (std::string arg_1, 
                              std::string arg_2)
      {
        args_.push_back (arg_1);
        args_.push_back (arg_2);
      }

      void process::add_arg (std::string arg)
      {
        args_.push_back (arg);
      }

      void process::clear_env (void)
      {
        env_.clear ();
      }

      void process::set_env (std::map <std::string, std::string> const& env)
      {
        env_ = env;
      }

      void process::add_env (std::string key, std::string val)
      {
        env_[key] = val;
      }

      ///////////////////////////////////////////////////////////////////////////////
      //
      // run a process, and return stdout.  In case of errors, throw a NoSuccess with
      // stderr as error message
      //
      std::vector <std::string> process::run_sync (bool io)
      {
        try 
        {
          // reset state
          fail_ = false;
          done_ = false;

          clear_out ();
          clear_err ();

          if ( cmd_ == "")
          {
            std::cout << " === no cmd\n";
            fail_ = true;
            msg_  = "no command to run";

            std::vector <std::string> out;
            return out;
          }


          boost::process::command_line cl (cmd_);
          std::string log (" >> ");
          log += cmd_ + " ";

          for ( unsigned int i = 0; i < args_.size (); i++ )
          {
            cl.argument (args_[i]);
            log += args_[i] + " ";
          }

          SAGA_LOG_ALWAYS (log.c_str ());

          boost::process::launcher l;

          if ( io )
          {
            l.set_stdin_behavior  (boost::process::redirect_stream);
            l.set_stdout_behavior (boost::process::redirect_stream);
            l.set_stderr_behavior (boost::process::redirect_stream);
          }

          std::map <std::string, std::string> :: iterator it;

          for ( it = env_.begin (); it != env_.end (); it++ )
          {
            l.set_environment ( (*it).first, (*it).second );
          }

          c_ = l.start (cl);

          if ( io )
          {
         // boost::process::postream & in   = c_.get_stdin  ();
            boost::process::pistream & out  = c_.get_stdout ();
            boost::process::pistream & err  = c_.get_stderr ();

            for ( std::string line; out.good () && getline (out, line); out_v_.push_back   (line) );
            for ( std::string line; err.good () && getline (err, line); err_v_.push_back   (line) );
          }

          boost::process::status status = c_.wait ();

          if ( ! status.exited () )
          {
            fail_ = true;
            msg_  = "exit fail";

            SAGA_LOG_DEBUG (msg_.c_str ());

            for ( std::size_t i = 0; i < err_v_.size (); i++ )
            {
              SAGA_LOG_DEBUG (err_v_[i].c_str ());
            }
          }
          else
          {
            exit_ = status.exit_status ();

            if ( ! status.exited () || exit_ )
            {
              fail_ = true;
              msg_  = "exit status != 0";

              SAGA_LOG_DEBUG (msg_.c_str ());

              for ( std::size_t i = 0; i < err_v_.size (); i++ )
              {
                SAGA_LOG_DEBUG (err_v_[i].c_str ());
              }
            }
            else
            {
              done_ = true;
            }
          }

        }
        catch ( const boost::process::not_found_error <std::string> & e )
        {
          fail_ = true;
          std::string msg("Could not find executable in path: ");
          msg += e.what();
          SAGA_THROW_NO_OBJECT (msg, saga::NoSuccess);
        }
        catch ( const boost::process::system_error & e )
        {
          fail_ = true;
          std::string msg("Execution failed: ");
          msg += e.what();
          SAGA_THROW_NO_OBJECT (msg, saga::NoSuccess);
        }

        return out_v_;
      }

      boost::process::pistream & process::get_out (void)
      {
        return c_.get_stdout ();
      }

      boost::process::pistream & process::get_err (void)
      {
        return c_.get_stderr ();
      }


      std::string process::get_out_s (void)
      {
        std::string out;

        for ( std::size_t i = 0; i < out_v_.size (); i++ )
        {
          out += out_v_[i] + "\n";
        }

        return out;
      }

      std::string process::get_err_s (void)
      {
        std::string err;

        for ( std::size_t i = 0; i < err_v_.size (); i++ )
        {
          err += err_v_[i] + "\n";
        }

        return err;
      }


      std::vector <std::string>  process::get_out_v (void)
      {
        return out_v_;
      }

      std::vector <std::string>  process::get_err_v (void)
      {
        return err_v_;
      }

      void process::clear_out (void)
      {
        out_v_.clear ();
      }

      void process::clear_err (void)
      {
        err_v_.clear ();
      }
      ///////////////////////////////////////////////////////////////////////////////

    } // namespace utils

  } // namespace adaptors

} // namespace saga

