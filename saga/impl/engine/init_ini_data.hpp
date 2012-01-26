//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if!defined(SAGA_IMPL_INIT_INI_DATA_APR_17_2008_1152AM)
#define SAGA_IMPL_INIT_INI_DATA_APR_17_2008_1152AM

#include <string>
#include <iostream>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/adaptors/utils/ini/ini.hpp>

namespace saga { namespace impl {

///////////////////////////////////////////////////////////////////////////////
inline void handle_ini_file (saga::ini::ini    & ini, 
                             std::string const & loc)
{
  try { 
    ini.read (loc); 
  }
  catch ( saga::exception const & /*e*/ ) { 
    ;
  }
}
  
inline void handle_ini_file_env (saga::ini::ini & ini, 
                                 char const           * env_var, 
                                 char const           * file_suffix = NULL)
{
    const char *env = saga::detail::safe_getenv (env_var);
    if ( NULL != env ) 
    {
        namespace fs = boost::filesystem;
        
        #if BOOST_FILESYSTEM_VERSION == 3
        fs::path inipath (env);        
        #else
        fs::path inipath (env, fs::native);
        #endif
        
        if ( NULL != file_suffix )
        {
          #if BOOST_FILESYSTEM_VERSION == 3
          inipath /= fs::path (file_suffix);
          #else
          inipath /= fs::path (file_suffix, fs::native);
          #endif
        }

        SAGA_LOG(SAGA_VERBOSE_LEVEL_INFO) 
            << "handle_ini_file_env (" << env << ", " << file_suffix 
            << ") = " << inipath.string();

        handle_ini_file (ini, inipath.string ());
    }
}


// read system and user specified ini files - but not adaptor inis
SAGA_EXPORT inline void init_ini_data_base (saga::ini::ini & ini)
{
    namespace fs = boost::filesystem;

    // fallback: use compile time prefix
    handle_ini_file (ini, std::string (SAGA_DEFAULT_INI_PATH) + "/saga.ini");

    // look in the current directory first
    std::string cwd = fs::current_path().string() + "/.saga.ini";
    handle_ini_file (ini, cwd);

    // look for master ini in the SAGA_INI environment
    handle_ini_file_env (ini, "SAGA_INI");

    // afterwards in the standard locations
#if !defined(BOOST_WINDOWS)   // /etc/saga.ini doesn't make sense for Windows
    handle_ini_file     (ini, std::string ("/etc/saga.ini"));
#endif
    handle_ini_file_env (ini, "SAGA_LOCATION", "/share/saga/saga.ini");
    handle_ini_file_env (ini, "HOME",          "/.saga.ini");
    handle_ini_file_env (ini, "PWD",           "/.saga.ini");
}


///////////////////////////////////////////////////////////////////////////////
// global function to read adaptor ini information
SAGA_EXPORT inline void init_ini_data (saga::ini::ini & ini)
{
    namespace fs = boost::filesystem;

    // // set a compile time default saga location
    std::vector<std::string> lines;
    lines.push_back ("[saga]");
    lines.push_back ("location = ${SAGA_LOCATION:" SAGA_LOCATION "}");
    lines.push_back ("pid = " + boost::lexical_cast<std::string>(getpid()));

    std::vector<std::string> const& logging = get_logging_init_data();
    std::vector<std::string>::const_iterator end = logging.end();
    for (std::vector<std::string>::const_iterator it = logging.begin(); 
         it != end; ++it)
    {
        lines.push_back(*it);
    }

    ini.parse ("static default", lines);

    // read system and user ini files
    init_ini_data_base (ini);

    // now merge all information into one global structure
    std::string               ini_entry;
    std::string               ini_path;
    std::vector <std::string> ini_paths;

    // try to find new location (v1.0)
    if ( ini.has_entry ("saga.ini_path") )
    {
      ini_entry = "saga.ini_path";
    }

    // try to find old location (pre-v1.0)
    else if ( ini.has_entry ("info.ini_path") )
    {
      ini_entry = "info.ini_path";
    }

    // try to use found ini_entry
    if ( ! ini_entry.empty() )
      ini_path = ini.get_entry (ini_entry, "");

    // nothing?  use compile time fallback
    if ( ini_path.empty () )
    {
      SAGA_LOG(SAGA_VERBOSE_LEVEL_WARNING) 
          << "Cannot obtain ini path - trying default " 
          << SAGA_DEFAULT_INI_PATH;

      ini_path = SAGA_DEFAULT_INI_PATH;
    }

    SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB) 
        << "ini_path: " << ini_path;

    // split of the separate paths from the given path list
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

    boost::char_separator<char> sep (SAGA_INI_PATH_DELIMITER);

    tokenizer           tok (ini_path, sep);
    tokenizer::iterator tok_end = tok.end ();

    for (tokenizer::iterator tit = tok.begin (); tit != tok_end; ++tit)
    {
      ini_paths.push_back (*tit);
    }

    // have all path elements, now find ini files in there...
    std::vector<std::string>::iterator ini_end = ini_paths.end();

    for (std::vector<std::string>::iterator it = ini_paths.begin(); 
         it != ini_end; ++it)
    {
      try {
        fs::directory_iterator nodir;
        
        #if BOOST_FILESYSTEM_VERSION == 3
        fs::path this_path (*it);
        #else
        fs::path this_path (*it, fs::native);
        #endif

        if ( fs::exists (this_path) )
        {
    #if BOOST_FILESYSTEM_VERSION == 3
          fs::path tmp_path(*it);
          for (fs::directory_iterator dir(tmp_path); dir != nodir; ++dir)
    #else
          for (fs::directory_iterator dir(fs::path(*it, fs::native)); dir != nodir; ++dir)
    #endif
          {
            if (fs::extension (*dir) == ".ini")
            {
              // read and merge the ini file into the main ini hierarchy
              try {
#if BOOST_FILESYSTEM_VERSION == 3
                SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB) 
                    << "merging " << (*dir).path().string();
                ini.merge ((*dir).path().string());
#else
                SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB) 
                    << "merging " << (*dir).string();
                ini.merge ((*dir).string ());
#endif
              }
              catch (saga::exception const& e) 
              {
#if BOOST_FILESYSTEM_VERSION == 3
                SAGA_LOG(SAGA_VERBOSE_LEVEL_WARNING) 
                    << "Cannot read ini file " << (*dir).path().string() 
                    << " - skipping (" << e.what() << ")";
#else
                SAGA_LOG(SAGA_VERBOSE_LEVEL_WARNING) 
                    << "Cannot read ini file " << (*dir).string () 
                    << " - skipping (" << e.what() << ")";
#endif
              }
            }
          }
        }
        else
        {
          SAGA_LOG_BLURB("ini path does not exist");
          SAGA_LOG_BLURB(this_path.string ().c_str ());
        }
      }
      catch (fs::filesystem_error const& e) 
      {
        SAGA_LOG(SAGA_VERBOSE_LEVEL_ERROR) 
            << "error while reading ini files: " << e.what();
      }
      catch ( ... )
      {
        SAGA_LOG(SAGA_VERBOSE_LEVEL_ERROR)
          << "unknown error while reading ini files";
      }
    }

    // read system and user ini files _again_, to allow the user to overwrite
    // the settings from the default adaptor inis.  This is inefficient (FIXME)
    init_ini_data_base (ini);

    SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_BLURB) 
    {
        ini.dump (0, 
            ::boost::logging::get_logger_base(saga::impl::logger())->
                  read_msg().gather().out());
    }

    if ( ! ini.has_section_full ("saga.adaptors") ) 
    {
      SAGA_LOG(SAGA_VERBOSE_LEVEL_ERROR) 
          << "Did not find any [saga.adaptors] section in any of the "
          << "SAGA configuration files.";
      return;
    }
}

///////////////////////////////////////////////////////////////////////////////
}}  // namespace saga::impl

#endif // guard

