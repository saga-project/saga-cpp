
// INSERT YOUR LICENSE BOILER PLATE HERE

#include <saga/saga.hpp>
#include <saga/saga/adaptors/task.hpp>

#include "example_shell_rpc_rpc.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace example_shell_rpc
{
  /*
   * constructor
   */
  rpc_cpi_impl::rpc_cpi_impl (proxy * p, 
                              cpi_info       const & info, 
                              saga::ini::ini const & glob_ini, 
                              saga::ini::ini const & adap_ini,
                              TR1::shared_ptr <saga::adaptor> adaptor)
    : base_cpi (p, info, adaptor, cpi::Noflags), 
      closed_  (false)
  {
    // get the remote method URL from instance data
    instance_data idata (this);

    u_ = idata->funcname_;

    if ( u_.get_scheme () != "shell-rpc" )
    {
      SAGA_ADAPTOR_THROW ("Scheme not supported - only support 'shell-rpc'", 
                          saga::BadParameter);
    }

    // extract the method name (aka path to executable)
    exe_ = u_.get_path ();

    // extract the resource manager component (hostname)
    rm_   = "any://localhost/";
    host_ = "localhost";

    if ( ! u_.get_host ().empty () )
    {
      rm_.set_host (u_.get_host ());
      host_ = u_.get_host ();
    }


    // try to create a job service as backend for our shell based rpc execution.
    try 
    {
      js_ = saga::job::service (rm_);
    }
    catch ( const saga::exception & e )
    {
      SAGA_ADAPTOR_THROW ((std::string ("Could not create job backend on ") 
                           + rm_.get_string ()).c_str (), 
                          saga::BadParameter);
    }


    SAGA_LOG_INFO ((std::string (" accept : ") +  u_.get_string ()).c_str ());
    SAGA_LOG_INFO ((std::string (" rm     : ") + rm_.get_string ()).c_str ());
    SAGA_LOG_INFO ((std::string (" on     : ") + host_            ).c_str ());
  }

  /*
   * destructor
   */
  rpc_cpi_impl::~rpc_cpi_impl (void)
  {
  }

  /*
   * SAGA API functions
   */
  void rpc_cpi_impl::sync_call (saga::impl::void_t &, 
                                std::vector <saga::rpc::parameter> & args)
  {
    if ( closed_ )
    {
      SAGA_ADAPTOR_THROW ("rpc instance is already closed", 
                          saga::IncorrectState);
    }


    // build up the command to be executed, from the exe (see above), + using
    // all input parameter data as command parameters to the exe.  While cycling
    // through the list, find the last Out parameter (to store results)
    std::string cmd = exe_;
    bool have_out   = false;
    saga::rpc::parameter pout;

    for ( unsigned int i = 0; i < args.size (); i++ )
    {
      if ( args[i].get_mode () == saga::rpc::In )
      {
        cmd += " ";
        cmd += (const char*) (args[i].get_data ());
      }
      else if ( args[i].get_mode () == saga::rpc::Out )
      {
        pout     = args[i];
        have_out = true;
      }
    }

    SAGA_LOG_INFO ((std::string (" cmd    : ") + cmd).c_str ());


    // run the command, and catch its io
    saga::job::ostream in;
    saga::job::istream out;
    saga::job::istream err;

    saga::job::job j = js_.run_job (cmd, host_, in, out, err);

    // get job state
    saga::job::state state = j.get_state ();

    if ( state != saga::job::Running && state != saga::job::Done )
    {
      SAGA_ADAPTOR_THROW ((std::string ("Could not execute shell rpc call ") + cmd).c_str (), 
                          saga::NoSuccess);
    }

    // we only need results if we have an out parameter - otherwise we are done
    if ( ! have_out )
    {
      return;
    }

    // read stdout, and put in ouput parameter found above
    char buffer[1024];
    std::string res;

    while ( out.good () ) 
    {
      // -1 to allow for a closing \0
      out.read (buffer, sizeof (buffer) - 1);
      int n = out.gcount ();

      if ( n > 0 )
      {
        buffer[n] = '\0';
        res += buffer;
      }
    }

    // make sure pout buffer is large enough
    if ( (ssize_t) res.size () > pout.get_size () )
    {
      SAGA_ADAPTOR_THROW ("Output parameter is too small to store returned output",
                          saga::BadParameter);
    }

    char * data = (char*) pout.get_data ();

    // -1 to remove the trailing line break
    ::memcpy (data, res.c_str (), res.size () - 1);

    // null-terminate string
    data[res.size ()] = '\0';
  }


  void rpc_cpi_impl::sync_close (saga::impl::void_t &, 
                                 double timeout)
  {
    closed_ = true;
  }

}   // namespace example_shell_rpc
///////////////////////////////////////////////////////////////////////////////

