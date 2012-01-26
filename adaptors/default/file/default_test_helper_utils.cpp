//  Copyright (c) 2006 Ole Christian Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <fstream>

#include <saga/saga/util.hpp>
#include <saga/saga/adaptors/adaptor.hpp>
#include <saga/saga/path_leaf.hpp>

#include "default_test_helper_utils.hpp"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

SAGA_TEST_HELPER_REGISTER(file_test_helper_utils_impl); 

///////////////////////////////////////////////////////////////////////////////
//
void file_test_helper_utils_impl::delete_temp_file (saga::url path)
{
    mutex_type::scoped_lock lock(mtx_);
    boost::filesystem::remove(boost::filesystem::path(path.get_path()));
}
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
void file_test_helper_utils_impl::delete_temp_dir  (saga::url path)
{
    mutex_type::scoped_lock lock(mtx_);
    boost::filesystem::remove_all(boost::filesystem::path(path.get_path()));
}
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
saga::url 
file_test_helper_utils_impl::create_temp_file_for_exception( const saga::error & e )
{
  mutex_type::scoped_lock lock(mtx_);
  std::string unique_path = get_unique_path_name();
  saga::url unique_url;
  
  switch (e) {
    
     case saga::adaptors::Success:
     {
       unique_url = saga::url(unique_path);
       unique_url.set_scheme("file");
       unique_url.set_host("localhost");
       
#if BOOST_FILESYSTEM_VERSION == 3
       boost::filesystem::path p (saga::url::unescape(unique_url.get_path()));
#else
       boost::filesystem::path p (saga::url::unescape(unique_url.get_path()), 
                                boost::filesystem::native);
#endif
       
       std::fstream file;
       
       file.open (p.string().c_str (), std::ios::out);
       file.close();
     }
       break;
      
     case saga::IncorrectState:
     {
       unique_url = saga::url(unique_path);
       unique_url.set_scheme("file");
       unique_url.set_host("localhost");
       
#if BOOST_FILESYSTEM_VERSION == 3
       boost::filesystem::path p (saga::url::unescape(unique_url.get_path()));
#else
       boost::filesystem::path p (saga::url::unescape(unique_url.get_path()), 
                                boost::filesystem::native);
#endif
       
       std::fstream file;
       
       file.open (p.string().c_str (), std::ios::out);
       file.close();
     }
       break;
      
     case saga::BadParameter:
     {
       unique_url = saga::url(unique_path);
       unique_url.set_scheme("file");
       unique_url.set_host("localhost");
       
#if BOOST_FILESYSTEM_VERSION == 3
       boost::filesystem::path p (saga::url::unescape(unique_url.get_path()));
#else
       boost::filesystem::path p (saga::url::unescape(unique_url.get_path()), 
                                boost::filesystem::native);
#endif
       
       std::fstream file;
       
       file.open (p.string().c_str (), std::ios::out);
       file.close();
     }
       break;
     case saga::DoesNotExist:
     {
       unique_url = saga::url(unique_path);
       unique_url.set_scheme("file");
       unique_url.set_host("localhost");
     }
       break;
       
     case saga::IncorrectURL:
     {
       unique_url = saga::url("file:local:-host/~.../");
     
     }
       break;
       
     case saga::PermissionDenied:
     {
       // create a file with insufficient permissions and return the url
     }
       break;
       
     default:
     {
       unique_url = saga::url("");
     }
       break;
   }
  
  return unique_url;
}
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
saga::url
file_test_helper_utils_impl::create_temp_dir_for_exception( const saga::error & e )
{
  mutex_type::scoped_lock lock(mtx_);
  std::string unique_path = get_unique_path_name();
  saga::url unique_url;
  
  switch (e) {
    
     case saga::adaptors::Success:
     {
        unique_url = saga::url(unique_path);
        unique_url.set_scheme("file");
        unique_url.set_host("localhost"); 
        unique_path = saga::url::unescape(unique_path);
        std::string::size_type pos = unique_path.find_first_of(SAGA_PATH_DELIMITERS);
        if (unique_path.size()-1 != pos)
            unique_path += "/";
        boost::filesystem::path p (saga::url::unescape(unique_url.get_path()));
        try {
           boost::filesystem::create_directory(unique_path);
        } 
        catch(boost::filesystem::filesystem_error const& e) {
           std::cerr << e.what() << std::endl;
        }
     }
       break;
     //Just make a normal one and let tests take care of rest
     case saga::AlreadyExists:
     {
        unique_url = saga::url(unique_path);
        unique_url.set_scheme("file");
        unique_url.set_host("localhost"); 
        unique_path = saga::url::unescape(unique_path);
        std::string::size_type pos = unique_path.find_first_of(SAGA_PATH_DELIMITERS);
        if (unique_path.size()-1 != pos)
            unique_path += "/";
        boost::filesystem::path p (saga::url::unescape(unique_url.get_path()));
        try {
           boost::filesystem::create_directory(unique_path);
        } 
        catch(boost::filesystem::filesystem_error const& e) {
           std::cerr << e.what() << std::endl;
        }
     }
       break;
      
     case saga::IncorrectState:
     {
        unique_url = saga::url(unique_path);
        unique_url.set_scheme("file");
        unique_url.set_host("localhost"); 
        unique_path = saga::url::unescape(unique_path);
        std::string::size_type pos = unique_path.find_first_of(SAGA_PATH_DELIMITERS);
        if (unique_path.size()-1 != pos)
            unique_path += "/";
        boost::filesystem::path p(unique_path);
        try {
           saga::detail::remove_trailing_dot(p);
           boost::filesystem::create_directory(p);
        } 
        catch(boost::filesystem::filesystem_error const& e) {
           std::cerr << e.what() << std::endl;
        }
     }
       break;
      
     case saga::BadParameter:
     {
        unique_url = saga::url(unique_path);
        unique_url.set_scheme("file");
        unique_url.set_host("localhost"); 
        unique_path = saga::url::unescape(unique_path);
        std::string::size_type pos = unique_path.find_first_of(SAGA_PATH_DELIMITERS);
        if (unique_path.size()-1 != pos)
            unique_path += "/";
        boost::filesystem::path p(unique_path);
        try {
           saga::detail::remove_trailing_dot(p);
           boost::filesystem::create_directory(p);
        } 
        catch(boost::filesystem::filesystem_error const& e) {
           std::cerr << e.what() << std::endl;
        }
     }
       break;
     case saga::DoesNotExist:
     {
        unique_url = saga::url(unique_path);
        unique_url.set_scheme("file");
        unique_url.set_host("localhost"); 
        std::string::size_type pos = unique_path.find_first_of(SAGA_PATH_DELIMITERS);
        if (unique_path.size()-1 != pos)
            unique_path += "/";
     }
       break;
       
     case saga::IncorrectURL:
     {
       unique_url = saga::url("file:local:-host/~.../");
     
     }
       break;
       
     case saga::PermissionDenied:
     {
       // create a file with insufficent permissions and return the url
     }
       break;
       
     default:
     {
       unique_url = std::string("");
     }
       break;
   }
//   return unique_path;
   return unique_url;
}
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
saga::url file_test_helper_utils_impl::create_temp_file_name (bool create)
{
  mutex_type::scoped_lock lock(mtx_);
  std::string unique_path = get_unique_path_name();
  saga::url unique_url(unique_path);
  unique_url.set_scheme("file");
  unique_url.set_host("localhost");
  
  if(create) {
#if BOOST_FILESYSTEM_VERSION == 3
       boost::filesystem::path p (saga::url::unescape(unique_url.get_path()));
#else
       boost::filesystem::path p (saga::url::unescape(unique_url.get_path()), 
                                boost::filesystem::native);
#endif

    
    std::fstream file;
    file.open (p.string().c_str (), std::ios::out);
    file.close();
  }
  return unique_url;
}
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
saga::url file_test_helper_utils_impl::create_temp_dir_name (bool create)
{
  mutex_type::scoped_lock lock(mtx_);
  std::string unique_path = get_unique_path_name();
  saga::url unique_url(unique_path);
  unique_url.set_scheme("file");
  unique_url.set_host("localhost"); 

  std::string::size_type pos = unique_path.find_first_of(SAGA_PATH_DELIMITERS);
  if (unique_path.size()-1 != pos)
      unique_path += "/";
  if(create) {
    unique_path = saga::url::unescape(unique_path);
    boost::filesystem::path p(unique_url.get_path());
    try {
        boost::filesystem::create_directory(unique_path);
    } 
    catch(boost::filesystem::filesystem_error const& e) {
        std::cerr << e.what() << std::endl;
    }
  }
  return unique_url;
}

