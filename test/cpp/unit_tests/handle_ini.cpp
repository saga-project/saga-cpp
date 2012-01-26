#include "handle_ini.hpp"


handle_ini::handle_ini()
{
    saga::ini::ini ini;
    saga::impl::init_ini_data(ini);
    saga::ini::section a_sec = ini.get_section("saga.adaptors");
    saga::ini::section_map s = a_sec.get_sections();
    saga::ini::section_map::iterator i;
    for(i = s.begin(); i != s.end(); ++i)
    {
        saga::ini::ini adaptor_ini;
        if(i->second.has_section("preferences"))
        {
            saga::ini::ini temp;
            temp.add_section(std::string("preferences"), i->second.get_section("preferences"));
            adaptor_ini.add_section(i->first, temp);
        }
        global_ini_.merge(adaptor_ini);
    }
    SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO)
    {
        std::cerr << "[INIT] |--> Stripped for preferences only" << std::endl;
        global_ini_.dump();
    }
}

saga::ini::ini handle_ini::get_global_ini()
{
    return global_ini_;
}

///////////////////////////////////////////////////////////////////////////////
//
saga::ini::section * 
handle_ini::get_section_custom_properties_(saga::ini::section const& base_sec)
{
  SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO) {
    std::cerr << "[INIT] |--> Additional properties for adaptor tests present... " 
              << std::flush;
  }  
  std::string adaptor_name(base_sec.get_entry("adaptor_name"));
  if( base_sec.has_section(adaptor_name) )
  {
    saga::ini::section adaptor_sec = base_sec.get_section(adaptor_name);
    if( adaptor_sec.has_section("test_properties") )
    {
      SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO) {
        std::cerr << " YES" << std::endl;
      }
      return new saga::ini::section(adaptor_sec.get_section("test_properties"));
    }
  }
  SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO) {
    std::cerr << " NO" << std::endl;
  }
  return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
std::vector<boost::filesystem::path> handle_ini::create_adaptor_ini_file_(std::string base_dir,
                                                saga::ini::ini const& sec)
{  
  namespace fs = boost::filesystem;
  std::vector<std::string>::iterator it;
  std::vector<fs::path> files;
  saga::ini::section_map adaptors(sec.get_sections());
  saga::ini::section_map::iterator sit;

  for(sit = adaptors.begin(); sit != adaptors.end(); sit++)
  {
    saga::ini::section adaptor_sec     (sit->second);
    saga::ini::section adaptor_test_sec;
    std::string adaptor_name = sit->first;


    if(adaptor_sec.has_section("test_properties"))
        adaptor_test_sec = adaptor_sec.get_section("test_properties");
    std::string adaptor_path = adaptor_test_sec.get_entry("adaptor_path");

    SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO) {
      std::cerr << "[INIT] |--> Creating temporary .ini file for " << adaptor_name
      << "... " << std::flush;
    }
#if BOOST_FILESYSTEM_VERSION == 3
    fs::path p (base_dir);
    p /= fs::path(adaptor_name + ".ini");
#else
    fs::path p (base_dir, fs::native);
    p /= fs::path(adaptor_name + ".ini", fs::native);
#endif
    std::fstream saga_ini_file(p.string().c_str(), std::ios::out);
    if( !saga_ini_file.is_open() )
    {
      std::cerr << "FAILED (Can't create file: " << p.string() << ")" << std::endl;
    }
    files.push_back(p);
    try {
      saga_ini_file << "[saga.adaptors." << adaptor_name << "]" << std::endl;
      saga_ini_file << "  name = " << adaptor_name << std::endl ;
      saga_ini_file << "  path = " << adaptor_path << std::endl ;

      SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO) {
        std::cerr << std::endl << "   [INIT] |--> Additional preferences for adaptors present... " 
        << std::flush;
      }
      if(adaptor_sec.has_section("adaptor_preferences"))
      {
        SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO) {
          std::cerr << " YES" << std::endl;
        }
        saga::ini::section pref_sec(adaptor_sec.get_section("adaptor_preferences"));
        saga_ini_file << "[saga.adaptors." << adaptor_name << ".preferences]" << std::endl;
        saga::ini::entry_map pref_map = pref_sec.get_entries();
        saga::ini::entry_map::iterator it;
        for(it = pref_map.begin(); it != pref_map.end(); ++it)
        {
          saga_ini_file << "  " << it->first << " = " << it->second << std::endl;
        }
      }//local parsing
      else
      {
        SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO) {
          std::cerr << " NO" << std::endl;
          std::cerr << "   [INIT] |--> Additional preferences globally for adaptors present... " 
          << std::flush;
        }
        if(global_ini_.has_section(adaptor_name) &&
           global_ini_.get_section(adaptor_name).has_section("preferences"))
        {
          SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO) {
            std::cerr << " YES" << std::endl;
          }
          saga::ini::section pref_sec = global_ini_.get_section(adaptor_name).get_section("preferences");
          saga_ini_file << "[saga.adaptors." << adaptor_name << ".preferences]" << std::endl;
          saga::ini::entry_map pref_map = pref_sec.get_entries();
          saga::ini::entry_map::iterator it;
          for(it = pref_map.begin(); it != pref_map.end(); ++it)
          {
            saga_ini_file << "  " << it->first << " = " << it->second << std::endl;
          }
        }
        else
        {
          SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO) {
            std::cerr << " NO" << std::endl;}
        }
      }//global parsing
      saga_ini_file.close();
    }//try to write adaptor ini files
    catch( std::exception const & e )
    {
      std::cerr << "FAILED ("<< e.what() << ")" << std::endl;
      exit(-1);
    }
    SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO) {
      std::cerr << "OK" << std::endl;
    }
  }
#if BOOST_FILESYSTEM_VERSION == 3
  fs::path p (base_dir);
#else
  fs::path p (base_dir, fs::native);
#endif
  return files;
}

///////////////////////////////////////////////////////////////////////////////
//
boost::filesystem::path handle_ini::create_saga_ini_file_(std::string base_dir)
{
  std::fstream saga_ini_file;
  namespace fs = boost::filesystem;
  
  SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO) {
    std::cerr << "[INIT] Creating .saga.ini file in " << base_dir << " " 
              << std::flush;
  }  
#if BOOST_FILESYSTEM_VERSION == 3
  fs::path p (base_dir);
  p /= fs::path(".saga.ini");
#else
  fs::path p (base_dir, fs::native);
  p /= fs::path(".saga.ini", fs::native);

#endif
  
  saga_ini_file.open(p.string().c_str(), std::ios::out);
  if( !saga_ini_file.is_open() )
  {
    std::cerr << "FAILED (Could not open file)" << std::endl;
    exit(-1);
  }
  
  try {
    saga_ini_file << "[saga]"         << std::endl
                  << "  ini_path = ." << std::endl  ;
    saga_ini_file.close();
  }
  catch( std::exception const & e )
  {
    std::cerr << "FAILED (" << e.what() << ")" << std::endl;
    exit(-1);
  }
  SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO) {
    std::cerr << "OK" << std::endl;
  }
  return p;
}

bool handle_ini::is_valid_section(saga::ini::section const& sec)
{
    bool ret_val = false;

    if( sec.has_entry("enabled") &&
        sec.has_entry("adaptor_path") &&
        sec.has_entry("adaptor_name") )
    {
        ret_val = true;
    }
    return ret_val;
}

bool handle_ini::is_enabled_section(saga::ini::section const& sec)
{
    bool ret_val = false;

    std::string is_enabled(sec.get_entry("enabled"));
    if( is_enabled == "True" || is_enabled == "TRUE" || is_enabled == "true" )
    {
        ret_val = true;
    }
  return ret_val;
}
