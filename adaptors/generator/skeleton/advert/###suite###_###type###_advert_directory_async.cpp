//  Copyright (c) 2005-2007 Hartmut Kaiser 
//  Copyright (c) 2005-2007 Andre Merzky   (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. 
//  (See accompanying file LICENSE or copy at 
//   http://www.boost.org/LICENSE_1_0.txt)
 
////////////////////////////////////////////////////////////////////////
// This adaptor implements the async functions 
// based on its own synchronous functions.

// saga includes
#include <saga/saga.hpp>

// adaptor includes
#include "###suite###_###type###_advert_directory.hpp"


////////////////////////////////////////////////////////////////////////
namespace ###suite###_###type###
{

//  ////////////////////////////////////////////////////////////////////////
//  // attribute functions
//  saga::task 
//    advertdirectory_cpi_impl::async_attribute_exists (std::string key)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_attribute_exists",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_attribute_exists,
//                                 key); 
//  }
//
//  saga::task 
//    advertdirectory_cpi_impl::async_attribute_is_readonly (std::string key)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_attribute_is_readonly",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_attribute_is_readonly,
//                                 key); 
//  }
//
//  saga::task 
//    advertdirectory_cpi_impl::async_attribute_is_writable (std::string key)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_attribute_is_writable",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_attribute_is_writable,
//                                 key); 
//  }
//
//  saga::task 
//    advertdirectory_cpi_impl::async_attribute_is_vector (std::string key)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_attribute_is_vector",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_attribute_is_vector,
//                                 key); 
//  }
//
//  saga::task 
//    advertdirectory_cpi_impl::async_attribute_is_extended (std::string key)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_attribute_is_extended",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_attribute_is_extended,
//                                 key); 
//  }
//
//  saga::task 
//    advertdirectory_cpi_impl::async_get_attribute (std::string key)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_get_attribute",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_get_attribute,
//                                 key); 
//  }
//
//  saga::task 
//    advertdirectory_cpi_impl::async_set_attribute (std::string key, 
//                                                   std::string val)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_set_attribute",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_set_attribute,
//                                 key, 
//                                 val); 
//  }
//
//  saga::task 
//    advertdirectory_cpi_impl::async_get_vector_attribute (std::string key)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_get_vector_attribute",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_get_vector_attribute,
//                                 key); 
//  }
//
//  saga::task 
//    advertdirectory_cpi_impl::async_set_vector_attribute (std::string               key, 
//                                                          std::vector <std::string> val)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_set_vector_attribute",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_set_vector_attribute,
//                                 key, 
//                                 val); 
//  }
//
//  saga::task 
//    advertdirectory_cpi_impl::async_remove_attribute (std::string key)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_remove_attribute",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_remove_attribute,
//                                 key); 
//  }
//
//  saga::task 
//    advertdirectory_cpi_impl::async_list_attributes (void)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_list_attributes",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_list_attributes);
//  }
//
//  saga::task 
//    advertdirectory_cpi_impl::async_find_attributes (std::string pattern)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_find_attributes",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_find_attributes,
//                                 pattern); 
//  }
//
//  ////////////////////////////////////////////////////////////////////////
//  // namespace_entry functions
//  saga::task 
//    advertdirectory_cpi_impl::async_get_url (void)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_get_url",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_get_url);
//  }
//
//  saga::task 
//    advertdirectory_cpi_impl::async_get_cwd (void)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_get_cwd",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_get_cwd);
//  }
//
//  saga::task 
//    advertdirectory_cpi_impl::async_get_name (void)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_get_name",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_get_name);
//  }
//
//  saga::task 
//    advertdirectory_cpi_impl::async_is_dir (void)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_is_dir",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_is_dir);
//  }
//
//  saga::task advertdirectory_cpi_impl::async_is_entry (void)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_is_entry",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_is_entry);
//  }
//
//  saga::task advertdirectory_cpi_impl::async_is_link (void)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_is_link",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_is_link);
//  }
//
//  saga::task 
//    advertdirectory_cpi_impl::async_read_link (void)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_read_link",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_read_link);
//  }
//
//  saga::task advertdirectory_cpi_impl::async_copy (saga::url target, 
//                                                   int       flags)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_copy",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_copy,
//                                 target, 
//                                 flags); 
//  }
//
//  saga::task advertdirectory_cpi_impl::async_link (saga::url target, 
//                                                   int       flags)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_link",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_link,
//                                 target, 
//                                 flags); 
//  }
//
//  saga::task advertdirectory_cpi_impl::async_move (saga::url target, 
//                                                   int       flags)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_move",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_move,
//                                 target, 
//                                 flags); 
//  }
//
//  saga::task advertdirectory_cpi_impl::async_remove (int flags)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_remove",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_remove,
//                                 flags); 
//  }
//
//  saga::task 
//    advertdirectory_cpi_impl::async_close (double timeout)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_close",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_close,
//                                 timeout); 
//  }
//
//  ////////////////////////////////////////////////////////////////////////
//  //  namespace_dir functions
//  saga::task 
//    advertdirectory_cpi_impl::async_list (std::string pattern, 
//                                          int         flags)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_list",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_list,
//                                 pattern, 
//                                 flags); 
//  }
//
//  saga::task 
//    advertdirectory_cpi_impl::async_find (std::string pattern, 
//                                          int         flags)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_find",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_find,
//                                 pattern, 
//                                 flags); 
//  }
//
//  saga::task 
//    advertdirectory_cpi_impl::async_exists (saga::url entry)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_exists",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_exists,
//                                 entry); 
//  }
//
//  saga::task 
//    advertdirectory_cpi_impl::async_is_dir (saga::url entry)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_is_dir",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_is_dir,
//                                 entry); 
//  }
//
//  saga::task 
//    advertdirectory_cpi_impl::async_is_entry (saga::url entry)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_is_entry",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_is_entry,
//                                 entry); 
//  }
//
//  saga::task 
//    advertdirectory_cpi_impl::async_is_link (saga::url entry)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_is_link",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_is_link,
//                                 entry); 
//  }
//
//  saga::task 
//    advertdirectory_cpi_impl::async_read_link (saga::url entry)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_read_link",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_read_link, 
//                                 entry);
//  }
//
//  saga::task 
//    advertdirectory_cpi_impl::async_get_num_entries (void)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_get_num_entries",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_get_num_entries);
//  }
//
//  saga::task 
//    advertdirectory_cpi_impl::async_get_entry (std::size_t  num)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_get_entry",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_get_entry,
//                                 num); 
//  }
//
//  saga::task 
//    advertdirectory_cpi_impl::async_copy (saga::url source,
//                                          saga::url target, 
//                                          int       flags)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_copy",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_copy,
//                                 source,
//                                 target,
//                                 flags); 
//  }
//
//  saga::task 
//    advertdirectory_cpi_impl::async_link (saga::url source,
//                                          saga::url target, 
//                                          int       flags)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_link",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_link,
//                                 source,
//                                 target, 
//                                 flags);
//  }
//
//  saga::task 
//    advertdirectory_cpi_impl::async_move (saga::url source,
//                                          saga::url target, 
//                                          int       flags)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_move",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_move,
//                                 source,
//                                 target, 
//                                 flags);
//  }
//
//  saga::task 
//    advertdirectory_cpi_impl::async_remove (saga::url entry, 
//                                            int       flags)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_remove",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_remove,
//                                 entry, 
//                                 flags); 
//  }
//
//  saga::task 
//    advertdirectory_cpi_impl::async_change_dir (saga::url dir)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_change_dir",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_change_dir,
//                                 dir); 
//  }
//
//  saga::task 
//    advertdirectory_cpi_impl::async_make_dir (saga::url dir, 
//                                              int       flags)
//  {
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_make_dir",
//                                 shared_from_this (),
//                                 &advertdirectory_cpi_impl::sync_make_dir,
//                                 dir, 
//                                 flags); 
//  }
//
//  saga::task 
//    advertdirectory_cpi_impl::async_open (saga::url  entry, 
//                                          int        flags)
//  {
//    typedef void (advertdirectory_cpi_impl::*sync_open_type)(
//        saga::advert::entry& ret, saga::url entry, int flags);
//
//    sync_open_type f = (sync_open_type)&advertdirectory_cpi_impl::sync_open;
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_open", 
//                                 shared_from_this (),
//                                 f, 
//                                 entry, flags);
//  }
//
//  saga::task 
//    advertdirectory_cpi_impl::async_open_dir (saga::url entry, 
//                                              int       flags)
//  {
//    typedef void (advertdirectory_cpi_impl::*sync_open_dir_type)(
//        saga::advert::directory& ret, saga::url entry, int flags);
//
//    sync_open_dir_type f = (sync_open_dir_type)&advertdirectory_cpi_impl::sync_open_dir;
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_open_dir", 
//                                 shared_from_this (),
//                                 f, 
//                                 entry, flags);
//  }
//
//
//  ////////////////////////////////////////////////////////////////////////
//  // advert directory functions
//  saga::task 
//    advertdirectory_cpi_impl::async_find (std::string              pattern, 
//                                          std::vector<std::string> patterns, 
//                                          int                      flags)
//  {
//    typedef void (advertdirectory_cpi_impl::*sync_find_type)(
//        std::vector<saga::url>&, std::string, std::vector<std::string>, int);
//
//    sync_find_type f = (sync_find_type)&advertdirectory_cpi_impl::sync_find;
//    return saga::adaptors::task ("advertdirectory_cpi_impl::async_find",
//                                 shared_from_this (),
//                                 f, 
//                                 pattern, patterns, flags); 
//  }

} // namespace ###suite###_###type###
////////////////////////////////////////////////////////////////////////

