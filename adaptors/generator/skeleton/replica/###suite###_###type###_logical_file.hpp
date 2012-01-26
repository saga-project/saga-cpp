//  Copyright (c) 2005-2008 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ADAPTORS_###SUITE###_###TYPE###_REPLICA_LOGICALFILE_HPP
#define ADAPTORS_###SUITE###_###TYPE###_REPLICA_LOGICALFILE_HPP

#include <fstream>
#include <string>

#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>

#include <saga/saga/util.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/adaptors/adaptor.hpp>
#include <saga/saga/adaptors/packages/logical_file_cpi_instance_data.hpp>

#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/packages/replica/logical_file_cpi.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace ###suite###_###type###
{
  ///////////////////////////////////////////////////////////////////////////////
  //  This adaptor implements the functionality of the Saga API "replica".
  //  It defines the functions declared in its base class, logical_file_file_cpi.
  // 
  class logical_file_cpi_impl 
    : public saga::adaptors::v1_0::logical_file_cpi <logical_file_cpi_impl>
  {
    private:
      typedef saga::adaptors::v1_0::logical_file_cpi <logical_file_cpi_impl> base_cpi;

    public:
      // constructor of the file adaptor 
      logical_file_cpi_impl (proxy                * p, 
                             cpi_info       const & info, 
                             saga::ini::ini const & glob_ini, 
                             saga::ini::ini const & adap_ini,
                             TR1::shared_ptr <saga::adaptor> adaptor);

      // destructor of the file adaptor 
      ~logical_file_cpi_impl  (void);

      // attribute functions
      void sync_attribute_exists      (bool        & ret, std::string key);
      void sync_attribute_is_readonly (bool        & ret, std::string key);
      void sync_attribute_is_writable (bool        & ret, std::string key);
      void sync_attribute_is_vector   (bool        & ret, std::string key);
      void sync_attribute_is_extended (bool        & ret, std::string key);
      void sync_get_attribute         (std::string & ret, std::string key);
      void sync_set_attribute         (saga::impl::void_t & ret, 
                                       std::string key, 
                                       std::string val);
      void sync_get_vector_attribute  (std::vector <std::string> & ret, 
                                       std::string key);
      void sync_set_vector_attribute  (saga::impl::void_t & ret, 
                                       std::string, 
                                       std::vector<std::string>);
      void sync_remove_attribute      (saga::impl::void_t & ret, 
                                       std::string key);
      void sync_list_attributes       (std::vector<std::string> & keys);
      void sync_find_attributes       (std::vector<std::string> & ret, 
                                       std::string);

      // namespace_entry functions
      void sync_get_url   (saga::url & url);
      void sync_get_cwd   (saga::url & ret);
      void sync_get_name  (saga::url & ret);
      void sync_read_link (saga::url & ret);
      void sync_is_dir    (bool & ret);
      void sync_is_entry  (bool & ret);
      void sync_is_link   (bool & ret);
      void sync_copy      (saga::impl::void_t & ret, saga::url, int);
      void sync_link      (saga::impl::void_t & ret, saga::url, int);
      void sync_move      (saga::impl::void_t & ret, saga::url, int);
      void sync_remove    (saga::impl::void_t & ret, int);
      void sync_close     (saga::impl::void_t & ret, double);

      // logical_file functions
      void sync_list_locations  (std::vector <saga::url> & locations);
      void sync_add_location    (saga::impl::void_t & ret, saga::url location);
      void sync_remove_location (saga::impl::void_t & ret, saga::url location);
      void sync_update_location (saga::impl::void_t & ret, 
                                 saga::url oldlocation, 
                                 saga::url newlocation);
      void sync_replicate       (saga::impl::void_t & ret, saga::url, int);


      // attribute functions
      saga::task async_attribute_exists      (std::string key);
      saga::task async_attribute_is_readonly (std::string key);
      saga::task async_attribute_is_writable (std::string key);
      saga::task async_attribute_is_vector   (std::string key);
      saga::task async_attribute_is_extended (std::string key);
      saga::task async_get_attribute         (std::string key);
      saga::task async_set_attribute         (std::string key, std::string val);
      saga::task async_get_vector_attribute  (std::string key);
      saga::task async_set_vector_attribute  (std::string, std::vector<std::string>);
      saga::task async_remove_attribute      (std::string key);
      saga::task async_list_attributes       (void);
      saga::task async_find_attributes       (std::string);

      // namespace_entry functions
      saga::task async_get_url   (void);
      saga::task async_get_cwd   (void);
      saga::task async_get_name  (void);
      saga::task async_read_link (void);
      saga::task async_is_dir    (void);
      saga::task async_is_entry  (void);
      saga::task async_is_link   (void);
      saga::task async_copy      (saga::url, int);
      saga::task async_link      (saga::url, int);
      saga::task async_move      (saga::url, int);
      saga::task async_remove    (int);
      saga::task async_close     (double);

      // logical_file functions
      saga::task async_list_locations  (void);
      saga::task async_add_location    (saga::url location);
      saga::task async_remove_location (saga::url location);
      saga::task async_update_location (saga::url, saga::url);
      saga::task async_replicate       (saga::url, int);

  };  // class logical_file_cpi_impl

}   // namespace ###suite###_###type###
///////////////////////////////////////////////////////////////////////////////

#endif // ADAPTORS_###SUITE###_###TYPE###_REPLICA_LOGICALFILE_HPP

