//  Copyright (c) 2008 Chris Miceli
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <fstream>
#include <string>
#include <cstdlib>

#include "test_saga.hpp"
#include "test_cleanup.hpp"

#include <saga/saga/adaptors/utils/ini/ini.hpp>
#include <saga/saga/exception.hpp>
#include <saga/impl/engine/init_ini_data.hpp>

#include <boost/filesystem/convenience.hpp>
#include <boost/tokenizer.hpp>


class handle_ini
{
  public:
    handle_ini();
    saga::ini::ini get_global_ini();
    /**
      * creates a adaptor .ini file in the given directory
      *
      * @param base_dir
      *      the directory where the config file should be created
      *
      * @param sec
      *      a pointer to an adaptor test config file section
      *
      * @return
      *      the config file's path as string.
      */  
    std::vector<boost::filesystem::path> create_adaptor_ini_file_  (std::string base_dir,
                                           saga::ini::section const& sec );

    /**
      * creates a simple .saga.ini file in the given directory
      *
      * @param base_dir
      *      the directory where the config file should be created
      *
      * @return
      *      the config file's path as string.
      *
      */  
    boost::filesystem::path create_saga_ini_file_     (std::string base_dir);

    /**
      * returns the [sec_name.properties] section of a test ini file. The
      * properties section for the file adaptor tests is for example called
      * [file_adaptor_test.properties]. If no properties section exists, NULL
      * is returned.
      *
      * @param sec_name
      *      the name of the section we want to get the properties for
      *
      * @return
      *      the corresponding section or NULL
      *
      */
    saga::ini::section *
      get_section_custom_properties_(saga::ini::section const& base_sec);
    
  /**
    * utility function: tests if a given section of the config file is a valid
    * section. a valid section contains at least the following entries:
    *   - enabled      (whether to include this section's tests or not)
    *   - adaptor_path (path to the adaptor we want to test with this section)
    *   - helper_path  (path to the test helper library of the adaptor)
    *   - adaptor_name (name of the adaptor)
    *
    * @param sec
    *      path to the section we want to test 
    *
    * @return
    *      true if the section is valid, false otherwise
    */
  bool is_valid_section(saga::ini::section const& sec);

  /**
    * utility function: tests if a given section of the config file is enabled
    *
    * @param sec
    *      path to the section we want to test 
    *
    * @return
    *      true if the section is enabled, false otherwise
    */
  bool is_enabled_section(saga::ini::section const& sec);

  private:
    saga::ini::ini config_file_;
    saga::ini::ini global_ini_;
};
