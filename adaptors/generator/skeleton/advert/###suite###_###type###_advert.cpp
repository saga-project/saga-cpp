//  Copyright (c) 2005-2007 Hartmut Kaiser 
//  Copyright (c) 2005-2007 Andre Merzky   (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. 
//  (See accompanying file LICENSE or copy at 
//   http://www.boost.org/LICENSE_1_0.txt)
 

// saga includes
#include <saga/saga.hpp>

// adaptor includes
#include "###suite###_###type###_advert.hpp"


////////////////////////////////////////////////////////////////////////
namespace ###suite###_###type###
{

  ////////////////////////////////////////////////////////////////////////
  //  constructor
  advert_cpi_impl::advert_cpi_impl (proxy                           * p, 
                                    cpi_info const                  & info,
                                    saga::ini::ini const            & glob_ini, 
                                    saga::ini::ini const            & adap_ini,
                                    TR1::shared_ptr <saga::adaptor>   adaptor)
    : saga::adaptors::v1_0::advert_cpi <advert_cpi_impl> (p, info, adaptor, cpi::Noflags)
  {
    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
  }


  ////////////////////////////////////////////////////////////////////////
  //  destructor
  advert_cpi_impl::~advert_cpi_impl (void)
  {
  }



  ////////////////////////////////////////////////////////////////////////
  //  SAGA CPI functions 

//  ////////////////////////////////////////////////////////////////////////
//  // attribute functions
//  void advert_cpi_impl::sync_attribute_exists (bool        & ret, 
//                                               std::string   key)
//  {
//    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//  }
//
//  void advert_cpi_impl::sync_attribute_is_readonly (bool        & ret, 
//                                                    std::string   key)
//  {
//    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//  }
//
//  void advert_cpi_impl::sync_attribute_is_writable (bool        & ret, 
//                                                    std::string   key)
//  {
//    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//  }
//
//  void advert_cpi_impl::sync_attribute_is_vector (bool        & ret, 
//                                                  std::string   key)
//  {
//    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//  }
//
//  void advert_cpi_impl::sync_attribute_is_extended (bool        & ret, 
//                                                    std::string   key)
//  {
//    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//  }
//
//  ////////////////////////////////////////////////////////////////////////
//  void advert_cpi_impl::sync_get_attribute (std::string & ret, 
//                                            std::string   key)
//  {
//    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//  }
//
//  ////////////////////////////////////////////////////////////////////////
//  void advert_cpi_impl::sync_get_vector_attribute (std::vector <std::string> & ret, 
//                                                   std::string                 key)
//  {
//    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//  }
//
//  ////////////////////////////////////////////////////////////////////////
//  void advert_cpi_impl::sync_set_attribute (saga::impl::void_t & ret, 
//                                            std::string    key, 
//                                            std::string    val)
//  {
//    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//  }
//
//  ////////////////////////////////////////////////////////////////////////
//  void advert_cpi_impl::sync_set_vector_attribute (saga::impl::void_t            & ret, 
//                                                   std::string               key, 
//                                                   std::vector <std::string> val)
//  {
//    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//  }
//
//  ////////////////////////////////////////////////////////////////////////
//  void advert_cpi_impl::sync_remove_attribute (saga::impl::void_t & ret,
//                                               std::string    key)
//  {
//    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//  }
//
//  ////////////////////////////////////////////////////////////////////////
//  void advert_cpi_impl::sync_list_attributes (std::vector <std::string> & ret)
//  {
//    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//  }
//
//  ////////////////////////////////////////////////////////////////////////
//  void advert_cpi_impl::sync_find_attributes (std::vector<std::string> & ret, 
//                                              std::string                pattern)
//  {
//    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//  }
//
//  ////////////////////////////////////////////////////////////////////////
//  // namespace_entry functions
//  void advert_cpi_impl::sync_get_url (saga::url & url)
//  {
//    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//  }
//
//  void advert_cpi_impl::sync_get_cwd(saga::url& url)
//  {
//    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//  }
//
//  void advert_cpi_impl::sync_get_name (saga::url & url)
//  {
//    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//  }
//
//  void advert_cpi_impl::sync_read_link (saga::url & url)
//  {
//    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//  }
//
//  void advert_cpi_impl::sync_is_dir (bool & ret)
//  {
//    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//  }
//
//  void advert_cpi_impl::sync_is_entry (bool & ret)
//  {
//    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//  }
//
//  void advert_cpi_impl::sync_is_link (bool & ret)
//  {
//    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//  }
//
//  void advert_cpi_impl::sync_copy (saga::impl::void_t & ret, 
//                                   saga::url            target,
//                                   int                  flags)
//  {
//    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//  }
//
//  void advert_cpi_impl::sync_link (saga::impl::void_t & ret, 
//                                   saga::url            target,
//                                   int                  flags)
//  {
//    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//  }
//
//  void advert_cpi_impl::sync_move (saga::impl::void_t & ret, 
//                                   saga::url            target,
//                                   int                  flags)
//  {
//    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//  }
//
//  void advert_cpi_impl::sync_remove (saga::impl::void_t & ret, 
//                                     int                  flags)
//  {
//    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//  }
//
//  void advert_cpi_impl::sync_close (saga::impl::void_t & ret,
//                                    double               timeout)
//  {
//    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//  }
//
//
//  ////////////////////////////////////////////////////////////////////////
//  // advert functions
//  void advert_cpi_impl::sync_store_object (saga::impl::void_t & ret, 
//                                           saga::object         obj)
//  {
//    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//  }
//
//  void advert_cpi_impl::sync_retrieve_object (saga::object & ret, 
//                                              saga::session  s)
//  {
//    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//  }
//
//  void advert_cpi_impl::sync_store_string (saga::impl::void_t & ret, 
//                                           std::string          str)
//  {
//    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//  }
//
//  void advert_cpi_impl::sync_retrieve_string (std::string & ret)
//  {
//    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
//  }

} // namespace ###suite###_###type###
//////////////////////////////////////////////////////////////////////////

