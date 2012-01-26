//  Copyright (c) 2006 Ole Christian Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/version.hpp>

#if !defined(SAGA_TEST_HELPER_UTILS_HPP)
#define SAGA_TEST_HELPER_UTILS_HPP

#if defined(BOOST_WINDOWS)
# include <windows.h>
#else
# include <cstdlib>
#endif

#include <string>

#include <saga/saga/url.hpp>
#include <saga/saga/error.hpp>

#include <boost/config.hpp>
#include <boost/version.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/convenience.hpp>

#include <saga/saga/adaptors/utils/ini/ini.hpp>
#include <saga/saga/exception.hpp>
#include <saga/impl/exception.hpp> // SAGA_THROW
#ifndef  MAX_PATH
# define MAX_PATH _POSIX_PATH_MAX
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace test {

inline std::string escape_url (std::string in)
{
    std::string out;
    for (std::string::size_type i = 0; i < in.size(); ++i)
    {
      // leave sequences which look like escape sequences alone
      if (in[i] == '%') {
        if ( i + 2 < in.size()) {
          if ( std::isxdigit(in[i+1]) && std::isxdigit(in[i+2]))
          {
            // found such a sequence
            out += in[i] + in[i+1] + in[i+2];
            i += 2;
          }
          else {
            out += "%25";
          }
        }
        else {
          out += "%25";
        }
      }
      else {
        switch(in[i]) {
        case ' ': out += "%20"; break;
        case '&': out += "%26"; break;
        case '<': out += "%3C"; break;
        case '>': out += "%3E"; break;
        case '"': out += "%22"; break;
        case '#': out += "%23"; break;
        case '$': out += "%24"; break;
        case '\'': out += "%27"; break;
        case '+': out += "%2B"; break;
        case ',': out += "%2C"; break;
        case ';': out += "%3B"; break;
        case '=': out += "%3D"; break;
        case '?': out += "%3F"; break;
        case '@': out += "%40"; break;
        case '[': out += "%5B"; break;
        case '\\': out += "%5C"; break;
        case ']': out += "%5D"; break;
        case '^': out += "%5E"; break;
        case '`': out += "%60"; break;
        case '{': out += "%7B"; break;
        case '|': out += "%7C"; break;
        case '}': out += "%7D"; break;
        case '~': out += "%7E"; break;
        default: out += in[i]; 
            break;
        }
      }
    }
    return out;
}


/**
  * This class is an interface for the test helper library, every adaptor 
  * should implement. It's basically a collection of methods used by the
  * various unit test cases.
  *
  */
class test_helper_utils
{
private:
  saga::ini::section * properties_;
  
public:
  /**
    * constructor - does nothing right now.
    */
  test_helper_utils() {}

  /**
    * destructor
    */  
  virtual ~test_helper_utils() {}
  
  void set_properties(saga::ini::section * properties)
  {
    properties_ = properties;
  }

  /*
   * the implementation of this interface should generate a file... ;-)
   *
   * TODO: Detailed description
   *
   */
  virtual saga::url create_temp_file_for_exception( const saga::error & e ) = 0;

  /*
   * the implementation of this interface should generate a dir... ;-)
   *
   * TODO: Detailed description
   *
   */
  virtual saga::url create_temp_dir_for_exception( const saga::error & e ) = 0;

  /**
    * the implementation of this interface should create a unique temporary
    * local/remote file if the create flag is set to TRUE and return the 
    * path as a string. If the create flag is set to FASLE, only the unique
    * name is returned but no file is created.
    *
    * @param create
    *      if set to TRUE unique file should be created, otherwise not
    *
    * @return
    *      a unique file name
    */
  virtual saga::url create_temp_file_name(bool create = true) = 0;
  
  /**
    * the implementation of this interface should create a unique temporary
    * local/remote directory if the create flag is set to TRUE and return the 
    * path as a string. If the create flasg is set to FASLE, only the unique
    * name is returned but no file is created.
    *
    * @param create
    *      if set to TRUE unique directory should be created, otherwise not
    *
    * @return
    *      a unique directory name
    */
  virtual saga::url create_temp_dir_name(bool create = true) = 0;

  /**
    * the implementation of this interface should delete the given file
    *
    * @param path
    *      path to the file to delete
    *
    */
  virtual void delete_temp_file(saga::url path) = 0;

  /**
    * the implementation of this interface should delete the given directory
    *
    * @param path
    *      path to the directory to delete
    *
    */
  virtual void delete_temp_dir(saga::url path) = 0;
  
  
protected:
  /**
    * creates a unique file or directory name
    *
    * @return
    *      a unique name
    */    
  inline std::string get_unique_path_name()
  {
    char tmp_name    [MAX_PATH + 1];
    
  #if defined(BOOST_WINDOWS)
    char tmp_dir_path[MAX_PATH + 1];
    if ( 0 == GetTempPathA (sizeof (tmp_dir_path), tmp_dir_path) ) 
    {
      SAGA_THROW("Unable to create temporary file name", saga::NoSuccess);
      return std::string();
    }

    strcat (tmp_dir_path, "SAGA\\");
    DWORD cchBuff = MAX_PATH;          // size of user name
    char chBuffer[MAX_PATH + 1];       // buffer for expanded string
    
    GetUserName (chBuffer, &cchBuff);  // Get and display the user name.
    strcat (tmp_dir_path, chBuffer);
    
    // create directory
    boost::filesystem::path tempdir (tmp_dir_path, boost::filesystem::native);
    boost::filesystem::create_directories (tempdir);

    // create file name
    if ( 0 == GetTempFileNameA (tmp_dir_path, "tmp", 0, tmp_name) ) 
    {
      SAGA_THROW("Unable to create temporary file name", saga::NoSuccess);
      return std::string();
    }

    std::string t (tmp_name);
    std::string::size_type p = t.find_first_of("\\");
    while (p != std::string::npos) {
        t[p] = '/';
        p = t.find_first_of("\\", p);
    }

  #else
    
    snprintf (tmp_name, MAX_PATH, "/tmp/saga_XXXXXXXX");
    int fd = mkstemp (tmp_name);
    if ( fd <= 0 )
    {
      SAGA_THROW("Unable to create temporary file name", saga::NoSuccess);
      return (boost::filesystem::path().string());
    }
    close (fd);

    std::string t (tmp_name);
  #endif
    
    boost::filesystem::remove (t.c_str());    
#if BOOST_FILESYSTEM_VERSION == 3
    boost::filesystem::path retval (escape_url(t));
#else
    boost::filesystem::path retval (escape_url(t), boost::filesystem::native);
#endif
    return (retval.string());
  }
};

///////////////////////////////////////////////////////////////////////////////
}}  // saga::test

///////////////////////////////////////////////////////////////////////////////
#define SAGA_TEST_HELPER_REGISTER(ActualType)                                 \
    BOOST_PLUGIN_EXPORT(SAGA_ADAPTOR_LIB_NAME, saga::test::test_helper_utils, \
                        ActualType,            "test_helper_utils");          \
    /**/

#endif //SAGA_TEST_HELPER_UTILS_HPP

