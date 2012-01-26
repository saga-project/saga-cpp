//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005 Michel Zandstra (michelzandstra@gmail.com)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <ctype.h>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/tokenizer.hpp>
#include <boost/assert.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/plugin/dll.hpp>

#include <saga/saga/base.hpp>
#include <saga/saga/exception.hpp>
#include <saga/saga/adaptors/task_declaration.hpp>

#include <saga/impl/config.hpp>
#include <saga/impl/engine/engine.hpp>
#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/engine/init_ini_data.hpp>

#include <saga/saga/adaptors/adaptor.hpp>
#include <saga/saga/uuid.hpp>

#ifdef SAGA_HAVE_DYNAMIC_ADAPTOR_DATA
# include "dynamic_adaptor_data.hpp"
#else
# include "static_adaptor_data.hpp"
#endif

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// We need to ensure that the mutex controlling the access to the dl_xxx 
// functions is defined before the session object, as this is the only way
// to destruct it after the global session instance.
namespace boost { namespace plugin 
{
    // protect access to dl_xxx functions (on Linux, etc.)
    manage_mutex dl_mutex;
}}

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl 
{
    void log_saga_version_once()
    {
        SAGA_LOG(SAGA_VERBOSE_LEVEL_CRITICAL) 
            << "saga version: " << SAGA_VERSION 
            << " (" << SAGA_VERSION_DATE << ")";
    }

    void log_saga_version()
    {
        static boost::once_flag once_flag = BOOST_ONCE_INIT;
        boost::call_once(&log_saga_version_once, once_flag);
    }

    ///////////////////////////////////////////////////////////////////////////
    engine::engine ()
      : is_initialized(false)
    {
        boost::plugin::dl_mutex.initialize_mutex();  // force dl_open mutex to be initialized
    }

    void engine::late_initialize(saga::impl::session const *s)
    {
        if (!is_initialized) {
            is_initialized = true;
            init();
            log_saga_version();
            load(const_cast<saga::impl::session*>(s));
        }
    }

    engine::~engine (void)
    {
        tidy();
    }

    void engine::tidy()
    {
        // make sure the adaptor list get's cleared last
        adaptor_infos_.clear();
        adaptors_.clear();
        modules_.clear();
    }

    // test, whether the given adaptor instance has been loaded already
    bool engine::is_adaptor_loaded(std::string const& instance_name,
        std::string const& adaptor_name) const
    {
        adaptor_instance_map_type::const_iterator end = adaptors_.end();
        for (adaptor_instance_map_type::const_iterator it = adaptors_.begin();
             it != end; ++it)
        {
            if (instance_name == (*it).second.instance_name &&
                adaptor_name == (*it).second.adaptor_name)
            {
                return true;    // this adaptor has been loaded already.
            }
        }
        return false;
    }

    // add this adaptor to the list of modules from which to generate logs, if
    // appropriate
    void engine::register_adaptor_logging(std::string const& adaptor_name)
    {
        // add this adaptor to the list of modules from which to generate logs 
        std::string logadaptors = ini.get_entry ("saga.logging.modules", "*");
        if (logadaptors == "*") {
            // log from all adaptors
            std::string module_name(SAGA_MANGLE_ADAPTOR_NAME_STR(adaptor_name));
            modules_to_log_.insert(module_name);
        }
        else {
            // restrict logging to listed adaptors only
            std::vector<std::string> names;
            std::string split_at (",");
            boost::split (names, logadaptors, boost::is_any_of(split_at)); 

            std::string mangled_name(SAGA_MANGLE_ADAPTOR_NAME_STR(adaptor_name));
            std::vector<std::string>::const_iterator end = names.end();
            for (std::vector<std::string>::const_iterator it = names.begin();
                  it != end; ++it)
            {
                if (adaptor_name == *it || mangled_name == *it)
                {
                    modules_to_log_.insert(mangled_name);
                }
            }
        }
    }

  bool engine::load_adaptor (saga::impl::session * current_session, 
                             std::string           instance_name, 
                             std::string           adaptor_name, 
                             boost::filesystem::path const & lib, 
                             bool                  load_globally,
                             std::string const   & unmangled_name)
  {
    if (is_adaptor_loaded(instance_name, adaptor_name))
        return false;

    namespace fs = boost::filesystem;
    if (fs::extension(lib) == SAGA_SHARED_LIB_EXTENSION)
    {
      try {
        // add this adaptor to the list of modules from which to generate logs 
        register_adaptor_logging(unmangled_name);

        /* get the handle of the library */
        boost::plugin::dll d (lib.string(), adaptor_name, 
            load_globally ? BOOST_PLUGIN_DLOPEN_FLAGS_GLOBAL : 
                BOOST_PLUGIN_DLOPEN_FLAGS_LOCAL);

        SAGA_LOG(SAGA_VERBOSE_LEVEL_INFO)
            << "loaded module: " << d.get_name() << " (using " 
            << (load_globally ? "global" : "local") << " load flags)";

        // get the factory
        boost::plugin::plugin_factory<saga::adaptor> pf (d);

        // invoke the loader function of the library, so that the 
        // maker function and description table get registered 
        // with the registry 

        // create the adaptor registration object
        TR1::shared_ptr<saga::adaptor> adap (pf.create("adaptor")); 

        // bail out if the name reported by the adaptor set the adaptor name
        if (SAGA_MANGLE_ADAPTOR_NAME_STR(adap->get_name()) != adaptor_name)
        {
//             adaptor_name = adap->get_name();
            SAGA_LOG(SAGA_VERBOSE_LEVEL_WARNING)
                << "adaptor name: " << adaptor_name 
                << " does not match name reported by library: "
                << adap->get_name();
        }

        // call the register function, and store the adaptors it returns
        adaptor_selector::adaptor_info_list_type new_infos = 
            adap->adaptor_register (current_session); 

        // initialize the adaptor instance using a copy of the global
        // preferences
        saga::ini::ini glob_ini;
        glob_ini.merge(ini);

        // Adaptor preferences are read from two different locations: (a) from
        // any section specified via the 'preferences' key for that adaptor, if
        // that is set in the adapotor's ini file, and (b) from a 'preferences'
        // subsection in that file.  Thus, the more specific preferences from
        // the default adaptor preferences section overwrite the previous
        // general preferences, when neccessary.
        // FIXME: About the comment -- Is this so?!


        // read the specific adaptor settings first, as they may contain the
        // pointer to the more general adaptor settings
        saga::ini::ini adap_ini;
        std::string adaptor_section ("saga.adaptors." + instance_name);

        // if such a pointer to more general settings is present, read those.
        adap_ini = glob_ini.get_section (adaptor_section);

        // we now read the default adaptor ini again, to overwrite the global
        // setting with the adaptor specific settings.
        std::string merge_prefs = adap_ini.get_entry ("preferences", "");
        if ( ! merge_prefs.empty ()
            && glob_ini.has_section_full ( merge_prefs ) )
          adap_ini.merge ( glob_ini.get_section ( merge_prefs ) );

        SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_DEBUG) 
        {
            adap_ini.dump(0, 
                ::boost::logging::get_logger_base(saga::impl::logger())->
                    read_msg().gather().out());
        }

        if (!adap->init(current_session, glob_ini, adap_ini))
        {
            SAGA_LOG(SAGA_VERBOSE_LEVEL_WARNING)
                << "adaptor name: " << adaptor_name 
                << " dynamic loading has been canceled by the adaptor instance: "
                << instance_name;
            return false;
        }

        // store adaptor description info
        std::copy (new_infos.begin(), new_infos.end(), 
            std::back_inserter(adaptor_infos_));

        // store adaptor instance
        if (!new_infos.empty()) {
          // some adaptors do not register any functions (yes, this happens!)
          saga::uuid id (new_infos.front().get_adaptor_id());
          if (adaptors_.find(id) == adaptors_.end())
          {
            std::pair<adaptor_instance_map_type::iterator, bool> p =
                adaptors_.insert(std::make_pair(id, adaptor_instance_data_type()));
            if (p.second)
            {
                (*p.first).second.adaptor = adap;
                (*p.first).second.instance_name = instance_name;
                (*p.first).second.adaptor_name = adaptor_name;
            }
          }
        }

        // hold on on to this shared library
        modules_.push_back (d); 

        SAGA_LOG(SAGA_VERBOSE_LEVEL_INFO)
            << "loaded adaptor: " << adaptor_name << " ("
            << instance_name << ") from file: " << lib.string();
      }
      catch ( std::logic_error const & e ) 
      {
        // report error, and skip the library
        SAGA_LOG(SAGA_VERBOSE_LEVEL_WARNING)
            << "Could not load adaptor: " << e.what();
        return false;
      }
      return true;    // adaptor got loaded
    }
    return false;
  }
  
  bool engine::load_static_adaptor (saga::impl::session *current_session, 
      std::string instance_name, std::string adaptor_name,
      boost::plugin::get_plugins_list_type get_factory)
  {
    if (is_adaptor_loaded(instance_name, adaptor_name))
        return false;

    try {
      // add this adaptor to the list of modules from which to generate logs 
      register_adaptor_logging(instance_name);

      // get the factory
      boost::plugin::static_plugin_factory<saga::adaptor> pf (get_factory);

      // invoke the loader function of the library, so that the 
      // maker function and description table get registered 
      // with the registry 

      // create the adaptor registration object
      TR1::shared_ptr<saga::adaptor> adap (pf.create("adaptor")); 

      // bail out if the name reported by the adaptor set the adaptor name
      if (SAGA_MANGLE_ADAPTOR_NAME_STR(adap->get_name()) != adaptor_name)
      {
//           adaptor_name = adap->get_name();
          SAGA_LOG(SAGA_VERBOSE_LEVEL_WARNING)
              << "adaptor name: " << adaptor_name 
              << " does not match name reported by library: "
              << adap->get_name();
      }

      // call the register function, and store the adaptors it returns
      adaptor_selector::adaptor_info_list_type new_infos = 
          adap->adaptor_register (current_session); 

      // initialize the adaptor instance using the preferences from the ini files
      saga::ini::ini glob_ini;
      if (ini.has_section ("preferences"))
      {
          glob_ini = ini.get_section ("preferences");
      }

      saga::ini::ini adap_ini;
      std::string adaptor_section("saga.adaptors." + instance_name);
      if (ini.has_section_full(adaptor_section))
      {
          adap_ini = ini.get_section (adaptor_section);
      }

      if ( adap_ini.has_entry ("preferences") )
      {
        if ( ini.has_section_full (adap_ini.get_entry ("preferences")) )
        {
          adap_ini = ini.get_section (adap_ini.get_entry ("preferences"));
        }
      }

      if (!adap->init(current_session, glob_ini, adap_ini))
      {
          SAGA_LOG(SAGA_VERBOSE_LEVEL_WARNING)
              << "adaptor name: " << adaptor_name 
              << " static loading has been canceled by the adaptor instance: "
              << instance_name;
          return false;
      }

      // store adaptor description info
      std::copy (new_infos.begin(), new_infos.end(), 
          std::back_inserter(adaptor_infos_));

      // store adaptor instance
      if (!new_infos.empty()) {
        // some adaptors do not register any functions (yes, this happens!)
        saga::uuid id (new_infos.front().get_adaptor_id());
        if (adaptors_.find(id) == adaptors_.end())
        {
          std::pair<adaptor_instance_map_type::iterator, bool> p =
              adaptors_.insert(std::make_pair(id, adaptor_instance_data_type()));
          if (p.second)
          {
              (*p.first).second.adaptor = adap;
              (*p.first).second.instance_name = instance_name;
              (*p.first).second.adaptor_name = adaptor_name;
          }
        }
      }

      SAGA_LOG(SAGA_VERBOSE_LEVEL_INFO)
          << "loaded adaptor: " << adaptor_name << " ("
          << instance_name << ")";
    }
    catch (std::logic_error const& e) 
    {
      // report error, and skip the library
      SAGA_LOG(SAGA_VERBOSE_LEVEL_WARNING)
          << "Could not load adaptor: " << e.what();
      return false;
    }
    return true;    // adaptor got loaded
  }
  
  /**
    * Reads all available ini files, in following order:
    *  /etc/saga.ini
    *  $SAGA_LOCATION/share/saga/saga.ini
    *  $HOME/.saga.ini
    *  $PWD/.saga.ini
    *  $SAGA_INI
    */
  void engine::init (void)
  {
      try {
        saga::impl::init_ini_data(ini);
      }
      catch (...) {
        SAGA_LOG_WARN("Could not read ini files - most likely, we will not be"
                      "able to load dynamic adaptors");
      }

      // initialize logging
      init_logging(ini);

      // enable logging for engine, if requested
      std::string engine_logging(ini.get_entry ("saga.logging.engine", "0"));
      try {
          int enable_engine_logging = boost::lexical_cast<int>(engine_logging);
          if (enable_engine_logging) {
              std::string saga_core("core");
              modules_to_log_.insert(SAGA_MANGLE_ENGINE_NAME_STR(saga_core));
          }
          modules_to_log_.insert("unknown");        // always log messages from unknown modules
      }
      catch (boost::bad_lexical_cast const&) {
          // disabled by default
      }
  }
  
  /**
    * Reads all adaptor info files (*.ini) ini path, and tries
    * to open the shared libraries specified in there. It uses 
    * boost::plugin to handle the shared libraries.  If it can 
    * open  a library,  it invokes the loader function of this 
    * library to register it's  maker function and description 
    * table with the registry.
    *
    * @throws saga::exception is thrown if file "adaptorlist" 
    *         could not be opened, or does not exist
    */
  void engine::load (saga::impl::session *current_session)
  {
    std::string saga_location;
    saga::ini::section_map s;

    bool have_dynamic_adaptors = true;

    try {
        // get saga.location
        saga_location = ini.get_entry ("saga.location");

        // now load all known adaptors
        saga::ini::section a_sec = ini.get_section("saga.adaptors");
        s = a_sec.get_sections();
    }
    catch (saga::does_not_exist const& /*e*/) 
    {
        have_dynamic_adaptors = false;
        // SAGA_THROW("No adaptor(s) found (reported error: " + e.what() + ")", 
        //  saga::NoSuccess);
    }

    // now, load adaptors
    SAGA_LOG_INFO("loading static adaptors")
    load_static_adaptors(s, current_session);      // load all static adaptors

    if ( have_dynamic_adaptors )
    {
        SAGA_LOG_INFO("loading dynamic adaptors")
        load_dynamic_adaptors(s, current_session, saga_location);     // load all dynamic adaptors (except already loaded)
    }
    else
    {
        SAGA_LOG_INFO("skip loading dynamic adaptors")
    }

    if (adaptors_.empty()) {
        // no adaptors loaded
        SAGA_LOG_CRITICAL("No adaptor(s) found/loaded, SAGA will be non-functional, mostly"); 
    }
  }

  /////////////////////////////////////////////////////////////////////////////
  void engine::load_static_adaptors(saga::ini::section_map const& s,
      saga::impl::session *current_session)
  {
      static_adaptor_load_data_type* load_data = static_adaptor_load_data;
      for (/**/; NULL != load_data->name; ++load_data)
      {
          try 
          {
              // try to load this adaptor
              load_static_adaptor (current_session, load_data->name, 
                  SAGA_MANGLE_ADAPTOR_NAME_STR(std::string(load_data->name)), 
                  load_data->get_factory);

              // log output
              SAGA_LOG(SAGA_VERBOSE_LEVEL_INFO) 
                  << "static loading ok: " << load_data->name;
          }
          catch(saga::exception const& e) 
          {
              SAGA_LOG(SAGA_VERBOSE_LEVEL_WARNING) 
                  << "static loading failed: " << load_data->name << ": " 
                  << e.what();
          }
      }
  }

  /////////////////////////////////////////////////////////////////////////////
  void engine::load_dynamic_adaptors(saga::ini::section_map const& s,
                                     saga::impl::session *current_session, 
                                     std::string saga_location)
  {
    saga::ini::section_map::const_iterator end = s.end();
    for (saga::ini::section_map::const_iterator i = s.begin (); i != end; ++i) 
    {
      namespace fs = boost::filesystem;

      // the section name is the instance name of the adaptor
      std::string instance_name (i->second.get_name());
      std::string unmangled_name;
      std::string adaptor_name;

      if (i->second.has_entry("name"))
      {
        adaptor_name = SAGA_MANGLE_ADAPTOR_NAME_STR(i->second.get_entry("name"));
        unmangled_name = i->second.get_entry("name");
      }
      else
      {
        adaptor_name = SAGA_MANGLE_ADAPTOR_NAME_STR(instance_name);
        unmangled_name = instance_name;
        SAGA_LOG(SAGA_VERBOSE_LEVEL_INFO) 
            << "adaptor instance: " << instance_name 
            << ": no adaptor name given, using instance name instead";
      }

      bool enabled = true;
      if (i->second.has_entry("enabled"))
      {
        std::string tmp (i->second.get_entry("enabled"));
        boost::to_lower (tmp);

        if ( tmp == "no" || tmp == "false" || tmp == "0" )
        {
          enabled = false;
        }
      }

      if ( enabled )
      {
        // read required load flag settings from the adaptor ini, this might 
        // be necessary to allow to load a certain adaptor using local load
        // settings (see dlopen() flag: RTDL_LOCAL)
        bool load_globally = true;
        if (i->second.has_entry("loadflags"))
        {
          std::string flags_str(i->second.get_entry("loadflags"));
          boost::to_lower (flags_str);
          if (flags_str == "local")
          {
              load_globally = false;
          }
          else if (flags_str == "global")
          {
              load_globally = true;
          }
        }

        fs::path a_path = "";
        try 
        {
          if ( i->second.has_entry ("path") )
          {
            #if BOOST_FILESYSTEM_VERSION == 3
            a_path = fs::path(i->second.get_entry ("path"));
            #else
            a_path = fs::path(i->second.get_entry ("path"), fs::native);
            #endif
          }
          
          // AM: if no explicit path is given, we leave the path empty, and trust the
          // correct LD_LIBRARY_PATH settings.  Note that the previous fallback of
          // $SAGA_LOCATION/lib is very likely in the LD_LIBRARY_PATH, as we are
          // obviously running from libsaga_core, which should live exactly there.
          //
          // else
          // {
          // #if BOOST_FILESYSTEM_VERSION == 3
          //   a_path = fs::path(saga_location + "/lib/");
          // #else
          //   a_path = fs::path(saga_location + "/lib/", fs::native);
          // #endif
          // }

          // AM: this if'ed load call below does nothing, as a_path cannot
          // possibly have a shared lib extension - but the whole of
          // load_adaptor is if'ed by a test for that extension... 
          //
          // if (!load_adaptor (current_session, instance_name,  adaptor_name, 
          //                    a_path,          load_globally,  unmangled_name))
          // {
          // build path to adaptor to load
          std::string libname (adaptor_name + SAGA_SHARED_LIB_EXTENSION);
          #if BOOST_FILESYSTEM_VERSION == 3
          a_path = fs::path (libname);
          #else
          a_path = fs::path (libname, fs::native);
          #endif

          if (!load_adaptor (current_session,  instance_name,  adaptor_name, 
                             a_path,           load_globally,  unmangled_name))
          {
            // FIXME: How come we log if adaptor is disabled or on successful
            // load, but not on load failure?

            continue;   // next please :-P
          }

          // } // AM: see if comment above

          SAGA_LOG(SAGA_VERBOSE_LEVEL_INFO) 
              << "dynamic loading ok: " << a_path.string() << " - " << i->first;
        }
        catch (saga::exception const& e) 
        {
          SAGA_LOG(SAGA_VERBOSE_LEVEL_WARNING) 
              << "dynamic loading failed: " << a_path.string () 
              << " - " << i->first << ": " << e.what();
          // FIXME: use default adaptor location
        }
      } 
      else
      {
        SAGA_LOG(SAGA_VERBOSE_LEVEL_INFO) 
            << "dynamic loading disabled for: " << adaptor_name
            << " - " << i->first;
      }
    } // for
  }

  /////////////////////////////////////////////////////////////////////////////
  //  This is a helper struct, which during destruction pushes the 
  //  adaptor_info 'info' into the given list 'no_no_list'. The bool
  //  'update' will be set (from the outside) to false if everything goes 
  //  fine.
  struct update_no_no_list
  {
      update_no_no_list (v1_0::cpi_info const & info_,
                         adaptor_selector::adaptor_info_list_type & no_adaptor_infos)
        : update(true), 
          info(info_), 
          no_no_list(no_adaptor_infos)
      {}

      ~update_no_no_list()
      {
          if ( update )
              no_no_list.push_back (info);
      }

      bool update;
      v1_0::cpi_info const & info;
      adaptor_selector::adaptor_info_list_type &no_no_list;
  };

  // Creates an adaptor object implementing the interface
  v1_0::cpi* engine::get_adaptor(std::string     const & ops_cpi_name, 
                                 std::string     const & op_name, 
                                 preference_type const & prefs,
                                 adaptor_selector::adaptor_info_list_type & no_adaptor_infos,  
                                 proxy         * proxy_, 
                                 v1_0::op_info & oi) const
  {
    // we try to select an adaptor.  If we find one, we try to init it.  If
    // that fails, we add it to the no_adaptor list, and select a new (a
    // different one) etc.  If we don't find any new adaptor (e.g no adaptors
    // loaded, or all are in the no_adaptor list by now), we finish.
    //
    // Note: 'maker' is the adaptor entry point which creates a cpi instance,
    // i.e. is the cpi factory function of the adaptor
    try {
      // select_adaptor throws on error
      cpi_info const & adap_info = selector_.select_adaptor (adaptor_infos_,
                                                        no_adaptor_infos, 
                                                        proxy_->get_cpi_name(), 
                                                        ops_cpi_name, 
                                                        op_name, 
                                                        prefs, 
                                                        oi);

      BOOST_ASSERT (adap_info.get_cpi_name () != "NONE");

      // update no_no_list on exit, if appropriate (on exception)      
      update_no_no_list on_exit (adap_info, no_adaptor_infos);
      cpi::maker_type maker = adap_info.get_maker ();

      if (NULL == maker)
      {
        // could not get a maker: ignore this adaptor, select another
        SAGA_THROW("saga::engine::get_adaptor: could not get a "
          "factory function for CPI: " + ops_cpi_name , 
          saga::adaptors::Unexpected);
      }
      else 
      {
        // find corresponding adaptor instance
        adaptor_instance_map_type::const_iterator it = 
          adaptors_.find (adap_info.get_adaptor_id ());

        if (it == adaptors_.end()) 
        {
          SAGA_THROW("saga::engine::get_adaptor: could not (re-)"
            "find adaptor instance for CPI: " + ops_cpi_name, 
            saga::adaptors::Unexpected);
        }

        // call maker to create adaptor instance, 
        // and return adaptor on success
        saga::ini::ini glob_ini;
        if (ini.has_section ("preferences"))
        {
          glob_ini = ini.get_section ("preferences");
        }

        saga::ini::ini adap_ini;
        std::string adaptor_section("saga.adaptors." + (*it).second.instance_name);

        if (ini.has_section_full(adaptor_section))
        {
          adap_ini = ini.get_section (adaptor_section);
        }

        SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
            << "Trying to create CPI: "    << ops_cpi_name 
            << " for operation: "          << op_name 
            << " from proxy for CPI: "      << proxy_->get_cpi_name()
            << " using adaptor instance: " << (*it).second.instance_name
            << " (" << adap_info.get_adaptor_id ().string () << ")";

        v1_0::cpi* result = maker (proxy_, adap_info, glob_ini, adap_ini, (*it).second.adaptor);

        if (NULL != result)
        {
          SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
              << "Successfully created CPI: "  << ops_cpi_name
              << " using adaptor instance: " << (*it).second.instance_name;
          on_exit.update = false;   // don't push adap_info into no_no_list
        }
        else
        {
          SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
              << "Failed to create CPI: "  << ops_cpi_name
              << " using adaptor instance: " << (*it).second.instance_name;
        }
        return result;
      }
    }
    catch (saga::exception const & e) 
    {
      if ( e.get_error () != (saga::error)saga::adaptors::NoAdaptor )
      {
        throw;    // some unexpected error happened
      }
    }

    return NULL;
  }

  bool engine::test_adaptor(
      v1_0::cpi const* adaptor, std::string const& cpi_name, 
      std::string const& op_name, preference_type const& prefs, 
      v1_0::op_info& oi) const
  {
    bool result = selector_.test_adaptor (adaptor_infos_, adaptor, 
                                         cpi_name, op_name, prefs, oi);
    SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
        << "Testing adaptor " << adaptor->get_adaptor ()->get_name () 
        << " for " << cpi_name 
        << " -> "  << op_name 
        << " : "   << result;

    return result;
  }

  /////////////////////////////////////////////////////////////////////////
  // logging support
  bool engine::module_needs_logging(char const* modulename) const
  {
      std::string name(modulename);

      return modules_to_log_.find(name) != modules_to_log_.end() ||
          modules_to_log_.find(SAGA_MANGLE_ADAPTOR_NAME_STR(name)) != modules_to_log_.end();
  }
} } // namespace saga::impl
///////////////////////////////////////////////////////////////////////////////

