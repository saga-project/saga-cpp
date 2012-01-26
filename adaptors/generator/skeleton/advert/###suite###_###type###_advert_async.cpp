//  Copyright (c) 2005-2007 Hartmut Kaiser 
//  Copyright (c) 2005-2007 Andre Merzky   (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. 
//  (See accompanying file LICENSE or copy at 
//   http://www.boost.org/LICENSE_1_0.txt)
 
////////////////////////////////////////////////////////////////////////
// This skeleton implements the async functions based on its 
// own synchronous functions.

// saga includes
#include <saga/saga.hpp>

// adaptor includes
#include "###suite###_###type###_advert.hpp"


////////////////////////////////////////////////////////////////////////
namespace ###suite###_###type###
{

//  ////////////////////////////////////////////////////////////////////////
//  // attribute functions
//  saga::task 
//    advert_cpi_impl::async_attribute_exists (std::string   key)
//  {
//    return saga::adaptors::task ("advert_cpi_impl::async_attribute_exists",
//                                 shared_from_this (),
//                                 &advert_cpi_impl::sync_attribute_exists,
//                                 key); 
//  }
//
//  saga::task 
//    advert_cpi_impl::async_attribute_is_readonly (std::string   key)
//  {
//    return saga::adaptors::task ("advert_cpi_impl::async_attribute_is_readonly",
//                                 shared_from_this (),
//                                 &advert_cpi_impl::sync_attribute_is_readonly,
//                                 key); 
//  }
//
//  saga::task 
//    advert_cpi_impl::async_attribute_is_writable (std::string   key)
//  {
//    return saga::adaptors::task ("advert_cpi_impl::async_attribute_is_writable",
//                                 shared_from_this (),
//                                 &advert_cpi_impl::sync_attribute_is_writable,
//                                 key); 
//  }
//
//  saga::task 
//    advert_cpi_impl::async_attribute_is_vector (std::string   key)
//  {
//    return saga::adaptors::task ("advert_cpi_impl::async_attribute_is_vector",
//                                 shared_from_this (),
//                                 &advert_cpi_impl::sync_attribute_is_vector,
//                                 key); 
//  }
//
//  saga::task 
//    advert_cpi_impl::async_attribute_is_extended (std::string   key)
//  {
//    return saga::adaptors::task ("advert_cpi_impl::async_attribute_is_extended",
//                                 shared_from_this (),
//                                 &advert_cpi_impl::sync_attribute_is_extended,
//                                 key); 
//  }
//
//  saga::task 
//    advert_cpi_impl::async_get_attribute (std::string   key)
//  {
//    return saga::adaptors::task ("advert_cpi_impl::async_get_attribute",
//                                 shared_from_this (),
//                                 &advert_cpi_impl::sync_get_attribute,
//                                 key); 
//  }
//
//  saga::task 
//    advert_cpi_impl::async_set_attribute (std::string    key, 
//                                          std::string    val)
//  {
//    return saga::adaptors::task ("advert_cpi_impl::async_set_attribute",
//                                 shared_from_this (),
//                                 &advert_cpi_impl::sync_set_attribute,
//                                 key, 
//                                 val); 
//  }
//
//  saga::task 
//    advert_cpi_impl::async_get_vector_attribute (std::string                 key)
//  {
//    return saga::adaptors::task ("advert_cpi_impl::async_get_vector_attribute",
//                                 shared_from_this (),
//                                 &advert_cpi_impl::sync_get_vector_attribute,
//                                 key); 
//  }
//
//  saga::task 
//    advert_cpi_impl::async_set_vector_attribute (std::string               key, 
//                                                 std::vector <std::string> val)
//  {
//    return saga::adaptors::task ("advert_cpi_impl::async_set_vector_attribute",
//                                 shared_from_this (),
//                                 &advert_cpi_impl::sync_set_vector_attribute,
//                                 key,
//                                 val); 
//  }
//
//  saga::task 
//    advert_cpi_impl::async_remove_attribute (std::string    key)
//  {
//    return saga::adaptors::task ("advert_cpi_impl::async_remove_attribute",
//                                 shared_from_this (),
//                                 &advert_cpi_impl::sync_remove_attribute,
//                                 key); 
//  }
//
//  saga::task 
//    advert_cpi_impl::async_list_attributes (void)
//  {
//    return saga::adaptors::task ("advert_cpi_impl::async_list_attributes",
//                                 shared_from_this (),
//                                 &advert_cpi_impl::sync_list_attributes);
//  }
//
//  saga::task 
//    advert_cpi_impl::async_find_attributes (std::string                pattern)
//  {
//    return saga::adaptors::task ("advert_cpi_impl::async_find_attributes",
//                                 shared_from_this (),
//                                 &advert_cpi_impl::sync_find_attributes,
//                                 pattern); 
//  }
//
//  ////////////////////////////////////////////////////////////////////////
//  // namespace_entry functions
//  saga::task 
//    advert_cpi_impl::async_get_url (void)
//  {
//    return saga::adaptors::task ("advert_cpi_impl::async_get_url",
//                                 shared_from_this (),
//                                 &advert_cpi_impl::sync_get_url);
//  }
//
//  saga::task 
//    advert_cpi_impl::async_get_cwd (void)
//  {
//    return saga::adaptors::task ("advert_cpi_impl::async_get_cwd",
//                                 shared_from_this (),
//                                 &advert_cpi_impl::sync_get_cwd);
//  }
//
//  saga::task 
//    advert_cpi_impl::async_get_name (void)
//  {
//    return saga::adaptors::task ("advert_cpi_impl::async_get_name",
//                                 shared_from_this (),
//                                 &advert_cpi_impl::sync_get_name);
//  }
//
//  saga::task 
//    advert_cpi_impl::async_read_link (void)
//  {
//    return saga::adaptors::task ("advert_cpi_impl::async_read_link",
//                                 shared_from_this (),
//                                 &advert_cpi_impl::sync_read_link);
//  }
//
//  saga::task advert_cpi_impl::async_is_dir (void)
//  {
//    return saga::adaptors::task ("advert_cpi_impl::async_is_dir",
//                                 shared_from_this (),
//                                 &advert_cpi_impl::sync_is_dir);
//  }
//
//  saga::task advert_cpi_impl::async_is_entry (void)
//  {
//    return saga::adaptors::task ("advert_cpi_impl::async_is_entry",
//                                 shared_from_this (),
//                                 &advert_cpi_impl::sync_is_entry);
//  }
//
//  saga::task advert_cpi_impl::async_is_link (void)
//  {
//    return saga::adaptors::task ("advert_cpi_impl::async_is_link",
//                                 shared_from_this (),
//                                 &advert_cpi_impl::sync_is_link);
//  }
//
//  saga::task advert_cpi_impl::async_copy (saga::url target,
//                                          int       flags)
//  {
//    return saga::adaptors::task ("advert_cpi_impl::async_copy",
//                                 shared_from_this (),
//                                 &advert_cpi_impl::sync_copy,
//                                 target,
//                                 flags); 
//  }
//
//  saga::task advert_cpi_impl::async_link (saga::url target,
//                                          int       flags)
//  {
//    return saga::adaptors::task ("advert_cpi_impl::async_link",
//                                 shared_from_this (),
//                                 &advert_cpi_impl::sync_link,
//                                 target,
//                                 flags); 
//  }
//
//  saga::task advert_cpi_impl::async_move (saga::url target,
//                                          int       flags)
//  {
//    return saga::adaptors::task ("advert_cpi_impl::async_move",
//                                 shared_from_this (),
//                                 &advert_cpi_impl::sync_move,
//                                 target,
//                                 flags); 
//  }
//
//  saga::task advert_cpi_impl::async_remove (int flags)
//  {
//    return saga::adaptors::task ("advert_cpi_impl::async_remove",
//                                 shared_from_this (),
//                                 &advert_cpi_impl::sync_remove,
//                                 flags); 
//  }
//
//  saga::task advert_cpi_impl::async_close (double timeout)
//  {
//    return saga::adaptors::task ("advert_cpi_impl::async_close",
//                                 shared_from_this (),
//                                 &advert_cpi_impl::sync_close,
//                                 timeout); 
//  }
//
//  ////////////////////////////////////////////////////////////////////////
//  // logicalfile functions
//  saga::task 
//    advert_cpi_impl::async_store_object (saga::object   obj)
//  {
//    return saga::adaptors::task ("advert_cpi_impl::async_store_object",
//                                 shared_from_this (),
//                                 &advert_cpi_impl::sync_store_object,
//                                 obj); 
//  }
//
//  saga::task 
//    advert_cpi_impl::async_retrieve_object (saga::session  s)
//  {
//    return saga::adaptors::task ("advert_cpi_impl::async_retrieve_object",
//                                 shared_from_this (),
//                                 &advert_cpi_impl::sync_retrieve_object,
//                                 s); 
//  }
//
//  saga::task 
//    advert_cpi_impl::async_store_string (std::string    str)
//  {
//    return saga::adaptors::task ("advert_cpi_impl::async_store_string",
//                                 shared_from_this (),
//                                 &advert_cpi_impl::sync_store_string,
//                                 str); 
//  }
//
//  saga::task 
//    advert_cpi_impl::async_retrieve_string (void)
//  {
//    return saga::adaptors::task ("advert_cpi_impl::async_retrieve_string",
//                                 shared_from_this (),
//                                 &advert_cpi_impl::sync_retrieve_string);
//  }

} // namespace ###suite###_###type###
////////////////////////////////////////////////////////////////////////

