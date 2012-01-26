//  Copyright (c) 2006 Ole Christian Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test_cleanup.hpp"
#include <errno.h>

namespace cleanup
{
  suite::suite () 
    : boost::unit_test::test_suite ("cleanup dummy test") 
  {
  }
  
  suite::~suite()
  { 
      std::cerr << "[CLEANUP] Removing temporary .ini files..." << std::endl; 
    
      std::vector<boost::filesystem::path> ::iterator it = files_.begin();
      while( it != files_.end() )
      {
          SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO)
          {
              std::cerr << "[CLEANUP] |--> " << it->string() << std::endl;
          }
          boost::filesystem::remove(*it);
          it++;
      }

#if !defined(SAGA_WINDOWS)      
      std::cerr << "[CLEANUP] Removing any remaining jobs..." << std::endl; 
      std::ifstream pids ("test_helper.pids", std::ios::in);

      std::string tmp;
      
      while ( getline(pids, tmp) ) {
          int pid = atoi(tmp.c_str());
          std::cerr << "trying to kill pid: " << pid << std::flush;
          if ( 0 == ::kill((pid_t) pid, SIGKILL) )
          {
            std::cerr << " success. " << std::endl;
          }
          else
          {
            std::cerr << " failure: " << ::strerror (errno) << std::endl;
          }
      }
      pids.close();
#endif

      // Remove pid file now that all should be killed
      boost::filesystem::remove(boost::filesystem::path("test_helper.pids"));
  }
  
  void suite::add_file(boost::filesystem::path config_file)
  {
     SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO)
     {
        std::cerr << "Added " << config_file.string() << std::endl;
     }
     files_.push_back(config_file); 
  }
  
  void suite::add_files(std::vector<boost::filesystem::path> config_files)
  {
     std::vector<boost::filesystem::path>::iterator it;
     for(it = config_files.begin(); it != config_files.end(); it++)
     {
       SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO)
       {
          std::cerr << "Added " << (*it).string() << std::endl;
       }
       files_.push_back(*it); 
     }
  }

  void suite::add_directory(boost::filesystem::path directory)
  {
     SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO)
     {
        std::cerr << "Added " << directory.string() << std::endl;
     }
     directories_.push_back(directory); 
  }

  void suite::add_library_handle(boost::plugin::dll dll)
  {
     library_handles_.push_back(dll);
  }
  
  void suite::add_helper_object(saga::test::test_helper_utils* util)
  {
     helper_objects_.push_back(util);
  }

}//namespace cleanup
