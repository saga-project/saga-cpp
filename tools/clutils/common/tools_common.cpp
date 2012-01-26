

#include <boost/algorithm/string.hpp>


#include "tools_common.hpp"

namespace saga_tools
{
  ////////////////////////////////////////////////////////////////////
  //
  // 
  //
  usage_printer::usage_printer (void)
    : cmd_ ("")
  {
  }

  ////////////////////////////////////////////////////////////////////
  //
  // 
  //
  usage_printer::~usage_printer (void)
  {
  }

  ////////////////////////////////////////////////////////////////////
  //
  // 
  //
  void usage_printer::set_cmd (std::string s)
  {
    cmd_ = s;
  }

  ////////////////////////////////////////////////////////////////////
  //
  // 
  //
  void usage_printer::set_name (std::string s)
  {
    name_ = s;
  }

  ////////////////////////////////////////////////////////////////////
  //
  // 
  //
  void usage_printer::set_long_common (std::string s)
  {
    common_long_ = s;
  }

  ////////////////////////////////////////////////////////////////////
  //
  // 
  //
  void usage_printer::set_short_common (std::string s)
  {
    common_short_ = s;
  }

  ////////////////////////////////////////////////////////////////////
  //
  // 
  //
  void usage_printer::set_long (std::string s)
  {
    long_ = s;
  }

  ////////////////////////////////////////////////////////////////////
  //
  // 
  //
  void usage_printer::set_short (std::string s)
  {
    short_ = s;
  }

  ////////////////////////////////////////////////////////////////////
  //
  // 
  //
  void usage_printer::print (std::string err)
  {
    if ( ! err.empty () )
    {
      std::cerr << "\n  Error: " << err << std::endl;
    }

    if ( cmd_.empty () )
    {
      // print full help
      std::cout << "\n\n  Usage: " << name_ << " " << common_short_ << " <command> <options> \n\n" 
                << common_long_ 
                << "\n  Commands:                 Options:\n\n"
                << std::flush;

      std::map <std::string, std::string> :: iterator it = map_.begin ();

      for ( it = map_.begin (); it != map_.end (); it++ )
      {
        std::cout << "    " << it->first <<  " \t    " << it->second << std::endl;
      }

      std::cout << std::endl;
    }
    else
    {
      // print help only for this command
      std::cout << "\n\n  Usage: " << name_ << " " << common_short_ << " <options> \n\n" 
                << common_long_ 
                << "\n  Options:\n\n"
                << std::flush;

      std::cout << "    " << map_[cmd_] << std::endl;

      std::cout << std::endl;
    }
              
    if ( ! err.empty () )
    {
      exit (-1);
    }
  }

  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  //
  // 
  //
  common::common (void)
    /// : state_basedir_ ("advertdb://localhost/tmp/saga_shell.db")
  {
  }
  
  ////////////////////////////////////////////////////////////////////
  //
  // 
  //
  common::~common (void)
  {
  }
  
  ////////////////////////////////////////////////////////////////////
  //
  // 
  //
  void common::init (unsigned int argc, 
                     char**       argv)
  {
    // init usage printer
    usage_.set_short_common ("-s <session_name> -c <config>");

    usage_.set_long_common  ("                                                                 \n"
                             "  Common Options:                                                \n"
                             "                                                                 \n"
                             "    -h | --help             print help message                   \n"
                             "    -s | --session=name     Use the named session for            \n"
                             "                            all operations                       \n"
                             "                            default: 'default'                   \n"
                             "    -c | --config=file      Use the specified file for           \n"
                             "                            saga tool state storage              \n"
                             "                            default: '~/.saga/saga_tools.state'  \n"
                             "                                                                 \n");

    // init configuration from env, or set defaults
    char* session_name_env = ::getenv ("SAGA_TOOL_SESSION");
    if ( NULL != session_name_env )
    {
      session_name_ = session_name_env;
    }
    else
    {
      session_name_ = "default";
    }


    char* config_file_env = ::getenv ("SAGA_TOOL_CONFIG");
    if ( NULL != config_file_env )
    {
      config_file_ = config_file_env;
    }
    else
    {
      config_file_ = "~/.saga/saga_tools.state";
    }

    eval_args (argc, argv);

    // FIXME: retrieve named session
    session_ = saga::get_default_session ();

    // dump ();
  }


  ////////////////////////////////////////////////////////////////////
  //
  // 
  //
  void common::eval_args (unsigned int argc, 
                          char**       argv)
  {
    // if argv[0] is of the form 'saga-<module>-<cmd>', we push cmd as first
    // item on the arg stack. Also, we replace '-' with '_' in cmd. If argv[1] 
    // happens to have the same value, remove it.  
    //
    // So, the following five commands become equivalent:
    //
    //   saga-file          get_size file://host/path/name
    //   saga-file-get_size          file://host/path/name
    //   saga-file-get-size          file://host/path/name
    //   saga-file-get_size get_size file://host/path/name
    //   saga-file-get-size get_size file://host/path/name
    //
    // we need to make sure to parse only the file part of the path in argv,
    // thus the first split on '/'


    std::string cmd;
    
    std::vector <std::string> path_elem;
#if defined(SAGA_WINDOWS)
    boost::split (path_elem, argv[0], boost::is_any_of ("/\\"));
#else
    boost::split (path_elem, argv[0], boost::is_any_of ("/"));
#endif

    if ( path_elem.size () != 0 )
    {
      std::string name = path_elem[path_elem.size () - 1];
      
      usage_.set_name (name);

      std::vector <std::string> elem;
      
      boost::split (elem, name, boost::is_any_of ("-_"));


      if ( elem.size () > 2 )
      {
        cmd = elem[2];

        for ( unsigned int i = 3; i < elem.size (); i++ )
        {
          cmd += "_";
          cmd += elem[i];
        }
      }
    }

    // make cmd the first arg
    if ( ! cmd.empty () )
    {
      args_.push_back (cmd);
      usage_.set_cmd  (cmd);
    }


    bool parse = true;

    // now handle the other args, beyond argv[0]
    for ( unsigned int i = 1; i < argc; i++ )
    {
      std::string arg (argv[i]);

      // check for help option
      if (parse && arg == "-h" )
      {
        print_usage ();
        exit (0);
      }
      else if (parse && arg == "--help" )
      {
        print_usage ();
        exit (0);
      }


      // check for config option
      else if (parse && arg == "-c" )
      {
        if ( i >= argc )
        {
          usage_.print ("option '-c' needs an argument");
        }

        config_file_ = argv[i+1];
        i++;
      }
      else if ( parse && arg.substr (0, ::strlen ("--config=")) == "--config=" )
      {
        config_file_ = arg.substr (::strlen ("--config="), std::string::npos);

        if ( config_file_.empty () )
        {
          usage_.print ("option '--config' needs an argument");
        }
      }


      // check for session option
      else if ( parse && arg == "-s" )
      {
        if ( i >= argc )
        {
          usage_.print ("option '-s' needs an argument");
        }

        session_name_ = argv[i+1];
        i++;
      }
      else if ( parse && arg.substr (0, ::strlen ("--session=")) == "--session=" )
      {
        session_name_ = arg.substr (::strlen ("--session="), std::string::npos);

        if ( session_name_.empty () )
        {
          usage_.print ("option '--session' needs an argument");
        }
      }

      // keep remaining args, but ignore cmd if it is the first arg after
      // options
      else 
      {
        args_.push_back (arg);
        parse = false; // stop parsing for common flags
      }

    } // for

    return;
  }


  ////////////////////////////////////////////////////////////////////
  //
  // 
  //
  std::vector <std::string> & common::args (void)
  {
    return args_;
  }


  ////////////////////////////////////////////////////////////////////
  //
  // 
  //
  int common::nargs (void)
  {
    return args_.size () - 1;  // do not count cmd (args_[0])
  }


  ////////////////////////////////////////////////////////////////////
  //
  // 
  //
  void common::need_args (unsigned int n)
  {
    need_args_min (n);
    need_args_max (n);
  }

  ////////////////////////////////////////////////////////////////////
  //
  // 
  //
  // do not count cmd (args_[0])
  void common::need_args_min (unsigned int n)
  {
    if ( args_.size () < n+1 )
    {
      usage_.print ("insufficient number of arguments");
    }  
  }

  ////////////////////////////////////////////////////////////////////
  //
  // 
  //
  // do not count cmd (args_[0])
  void common::need_args_max (unsigned int n)
  {
    if ( args_.size () > n+1 )
    {
      usage_.print ("too many arguments");
    }  
  }


  ////////////////////////////////////////////////////////////////////
  //
  // 
  //
  void common::print_usage (std::string err)
  {
    usage_.print (err);
  }

  ////////////////////////////////////////////////////////////////////
  //
  // 
  //
  void common::load_state (std::string session_name)
  {
    std::string session_to_load = session_name;

    if ( session_to_load == "default" )
    {
      session_to_load = session_name_;
    }

    std::cout << "loading session " << session_to_load << std::endl;

    /// try
    /// {
    ///   state_basedir_.change_dir ("/sessions/");
    ///   if ( ! state_basedir_.is_entry (session_to_load + "/session.adv") )
    ///   {
    ///     fail (saga::no_success (session_to_load + ": no such session"), -1);
    ///   }

    ///   saga::advert::entry ad = state_basedir_.open (session_to_load + "/session.adv");
    ///   /// session_ = ad.retrieve_object <saga::session> ();
    /// }
    /// catch ( const saga::exception & e )
    /// {
    ///   std::cerr << "Could not load session: " << e.what () << std::endl;
    ///   fail (-1);
    /// }
  }


  ////////////////////////////////////////////////////////////////////
  //
  // 
  //
  void common::save_state (std::string session_name)
  {
    std::cout << "saving session " << session_name << std::endl;
  }

  ////////////////////////////////////////////////////////////////////
  //
  // 
  //
  void common::dump (void)
  {
    std::cout << "session name: " << session_name_ << std::endl;
    std::cout << "config  file: " << config_file_  << std::endl;
    if ( args_.size () == 0 )
    {
      std::cout << "args        : none" << std::endl;
    }
    else
    {
      std::cout << "args        : " << args_[0]      << std::endl;

      for ( unsigned int i = 1; i < args_.size (); i++ )
      {
        std::cout << "              " << args_[i] << std::endl;
      }
    }
  }

  ////////////////////////////////////////////////////////////////////
  //
  // 
  //
  void common::dump_state (std::string session_name)
  {
    std::cout << "dumping tool state" << std::endl;
  }

  ////////////////////////////////////////////////////////////////////
  //
  // 
  //
  void common::session (saga::session s)
  {
    std::cout << "setting new session" << std::endl;
    session_ = s;
  }

  ////////////////////////////////////////////////////////////////////
  //
  // 
  //
  saga::session common::session (void)
  {
    return session_;
  }

  ////////////////////////////////////////////////////////////////////
  //
  // 
  //
  usage_printer & common::usage (void)
  {
    return usage_;
  }

  ////////////////////////////////////////////////////////////////////
  //
  // 
  //
  void common::dump_history (void)
  {
    std::cout << "loading tool state" << std::endl;
  }

  ////////////////////////////////////////////////////////////////////
  //
  // 
  //
  void common::add_history (void)
  {
    std::cout << "loading tool state" << std::endl;
  }

  // If the specified history is negative, it is counted as that many
  // entries from the back.  Positive values are interpreted as absolute
  // indices.
  void common::get_history (int idx)
  {

    std::cout << "loading tool state" << std::endl;
  }


  ////////////////////////////////////////////////////////////////////
  //
  // 
  //
  void common::fail (int n)
  {
    std::cerr << " encountered error - exit\n" << std::endl;
    exit (n);
  }


  ////////////////////////////////////////////////////////////////////
  //
  // 
  //
  void common::fail (const std::string &  e, 
                     int n)
  {
    std::cerr << " encountered error: \n" << e << std::endl;
    exit (n);
  }


  ////////////////////////////////////////////////////////////////////
  //
  // 
  //
  void common::fail (const std::exception & e, 
                     int n)
  {
    std::cerr << " std exception caught: \n" 
              << e.what () << std::endl;
    exit (n);
  }


  ////////////////////////////////////////////////////////////////////
  //
  // 
  //
  void common::fail (const saga::exception & e, 
                     int n)
  {
    std::cerr << " SAGA exception caught: \n" 
              << e.what () << std::endl;
    exit (n);
  }
}

