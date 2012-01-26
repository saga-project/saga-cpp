//  Copyright (c) 2005-2007 Hartmut Kaiser 
//  Copyright (c) 2005-2007 Andre Merzky   (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. 
//  (See accompanying file LICENSE or copy at 
//   http://www.boost.org/LICENSE_1_0.txt)

#ifndef ADAPTORS_###SUITE###_###TYPE###_ADVERT_DIRECTORY_HPP
#define ADAPTORS_###SUITE###_###TYPE###_ADVERT_DIRECTORY_HPP

// saga includes
#include <saga/saga.hpp>

// saga adaptor includes
#include <saga/saga/adaptors/adaptor.hpp>

// saga package includes
#include <saga/impl/packages/advert/advert_directory_cpi.hpp>


////////////////////////////////////////////////////////////////////////
namespace ###suite###_###type###
{
  ////////////////////////////////////////////////////////////////////
  //  This adaptor implements the functionality of the Saga API "advert".
  //  It defines the functions declared in its base class, advertdirectory_cpi.
  class advertdirectory_cpi_impl 
    : public saga::adaptors::v1_0::advert_directory_cpi <advertdirectory_cpi_impl>
  {
    public:    
      // constructor of the advert adaptor 
      advertdirectory_cpi_impl (proxy                           * p, 
                                cpi_info const                  & info, 
                                saga::ini::ini const            & glob_ini, 
                                saga::ini::ini const            & adap_ini,
                                TR1::shared_ptr <saga::adaptor>   adaptor);

      // destructor of the advert adaptor 
      ~advertdirectory_cpi_impl  (void);

//      // attribute functions
//      void sync_attribute_exists      (bool                        & ret, 
//                                       std::string                   key);
//      void sync_attribute_is_readonly (bool                        & ret, 
//                                       std::string                   key);
//      void sync_attribute_is_writable (bool                        & ret, 
//                                       std::string                   key);
//      void sync_attribute_is_vector   (bool                        & ret, 
//                                       std::string                   key);
//      void sync_attribute_is_extended (bool                        & ret, 
//                                       std::string                   key);
//      void sync_get_attribute         (std::string                 & ret, 
//                                       std::string                   key);
//      void sync_set_attribute         (saga::impl::void_t          & ret, 
//                                       std::string                   key, 
//                                       std::string                   val);
//      void sync_get_vector_attribute  (std::vector <std::string>   & ret, 
//                                       std::string                   key);
//      void sync_set_vector_attribute  (saga::impl::void_t          & ret,
//                                       std::string                   key, 
//                                       std::vector <std::string>     val);
//      void sync_remove_attribute      (saga::impl::void_t          & ret, 
//                                       std::string                   key);
//      void sync_list_attributes       (std::vector <std::string>   & ret);
//      void sync_find_attributes       (std::vector <std::string>   & ret,
//                                       std::string                   pattern);
//
//      // namespace_entry functions
//      void sync_get_url               (saga::url                   & ret);
//      void sync_get_cwd               (saga::url                   & ret);
//      void sync_get_name              (saga::url                   & ret);
//      void sync_read_link             (saga::url                   & ret);
//      void sync_is_dir                (bool                        & ret);
//      void sync_is_entry              (bool                        & ret);
//      void sync_is_link               (bool                        & ret);
//      void sync_copy                  (saga::impl::void_t          & ret, 
//                                       saga::url                     target, 
//                                       int                           flags);
//      void sync_link                  (saga::impl::void_t          & ret, 
//                                       saga::url                     target, 
//                                       int                           flags);
//      void sync_move                  (saga::impl::void_t          & ret, 
//                                       saga::url                     target, 
//                                       int                           flags);
//      void sync_remove                (saga::impl::void_t          & ret, 
//                                       int                           flags);
//      void sync_close                 (saga::impl::void_t          & ret, 
//                                       double                        timeout);
//
//      // namespace_dir functions      
//      void sync_change_dir            (saga::impl::void_t          & ret, 
//                                       saga::url                     target);
//      void sync_list                  (std::vector <saga::url>     & ret, 
//                                       std::string                   pattern, 
//                                       int                           flags);
//      void sync_find                  (std::vector <saga::url>     & ret, 
//                                       std::string                   pattern, 
//                                       int                           flags);
//      void sync_read_link             (saga::url                   & ret, 
//                                       saga::url                     target);
//      void sync_exists                (bool                        & ret, 
//                                       saga::url                     target);
//      void sync_is_dir                (bool                        & ret, 
//                                       saga::url                     target);
//      void sync_is_entry              (bool                        & ret, 
//                                       saga::url                     target);
//      void sync_is_link               (bool                        & ret, 
//                                       saga::url                     target);
//      void sync_get_num_entries       (std::size_t                 & ret);
//      void sync_get_entry             (saga::url                   & ret, 
//                                       std::size_t                   idx);
//      void sync_copy                  (saga::impl::void_t          & ret, 
//                                       saga::url                     source, 
//                                       saga::url                     target, 
//                                       int                           flags);
//      void sync_link                  (saga::impl::void_t          & ret, 
//                                       saga::url                     source, 
//                                       saga::url                     target, 
//                                       int                           flags);
//      void sync_move                  (saga::impl::void_t          & ret, 
//                                       saga::url                     source, 
//                                       saga::url                     target, 
//                                       int                           flags);
//      void sync_remove                (saga::impl::void_t          & ret, 
//                                       saga::url                     target, 
//                                       int                           flags);
//      void sync_make_dir              (saga::impl::void_t          & ret, 
//                                       saga::url                     target, 
//                                       int                           flags);
//      void sync_open                  (saga::name_space::entry     & ret, 
//                                       saga::url                     target, 
//                                       int                           flags);
//      void sync_open_dir              (saga::name_space::directory & ret, 
//                                       saga::url                     target, 
//                                       int                           flags);
//
//      // advert directory functi      ons
//      void sync_open                  (saga::advert::entry         & ret,
//                                       saga::url                     target, 
//                                       int                           flags);
//      void sync_open_dir              (saga::advert::directory     & ret,
//                                       saga::url                     target, 
//                                       int                           flags);
//      void sync_find                  (std::vector <saga::url>     & ret,
//                                       std::string                   pattern, 
//                                       std::vector <std::string>     patterns,
//                                       int                           flags);
//
//
//      // This adaptor implements the async functions 
//      // based on its own synchronous functions.
//
//      // attribute functions
//      saga::task async_attribute_exists      (std::string                 key);
//      saga::task async_attribute_is_readonly (std::string                 key);
//      saga::task async_attribute_is_writable (std::string                 key);
//      saga::task async_attribute_is_vector   (std::string                 key);
//      saga::task async_attribute_is_extended (std::string                 key);
//      saga::task async_get_attribute         (std::string                 key);
//      saga::task async_set_attribute         (std::string                 key, 
//                                              std::string                 val);
//      saga::task async_get_vector_attribute  (std::string                 key);
//      saga::task async_set_vector_attribute  (std::string                 key, 
//                                              std::vector <std::string>   val);
//      saga::task async_remove_attribute      (std::string                 key);
//      saga::task async_list_attributes       (void);
//      saga::task async_find_attributes       (std::string                 pattern);
//
//      // namespace_entry functions
//      saga::task async_get_url         (void);
//      saga::task async_get_cwd         (void);
//      saga::task async_get_name        (void);
//      saga::task async_read_link       (void);
//      saga::task async_is_dir          (void);
//      saga::task async_is_entry        (void);
//      saga::task async_is_link         (void);
//      saga::task async_copy            (saga::url                     target, 
//                                        int                           flags);
//      saga::task async_link            (saga::url                     target,
//                                        int                           flags);
//      saga::task async_move            (saga::url                     target,
//                                        int                           flags);
//      saga::task async_remove          (int                           flags);
//      saga::task async_close           (double                        timeout);
//
//      // namespace_dir functions
//      saga::task async_change_dir      (saga::url                     target);
//      saga::task async_list            (std::string                   pattern, 
//                                        int                           flags);
//      saga::task async_find            (std::string                   pattern,
//                                        int                           flags);
//      saga::task async_read_link       (saga::url                     target);
//      saga::task async_exists          (saga::url                     target);
//      saga::task async_is_dir          (saga::url                     target);
//      saga::task async_is_entry        (saga::url                     target);
//      saga::task async_is_link         (saga::url                     target);
//      saga::task async_get_num_entries (void);
//      saga::task async_get_entry       (std::size_t                   idx);
//      saga::task async_copy            (saga::url                     source, 
//                                        saga::url                     target, 
//                                        int                           flags);
//      saga::task async_link            (saga::url                     source, 
//                                        saga::url                     target, 
//                                        int                           flags);
//      saga::task async_move            (saga::url                     source, 
//                                        saga::url                     target, 
//                                        int                           flags);
//      saga::task async_remove          (saga::url                     target, 
//                                        int                           flags);
//      saga::task async_make_dir        (saga::url                     target,
//                                        int                           flags);
//      saga::task async_open            (saga::url                     target, 
//                                        int                           flags);
//      saga::task async_open_dir        (saga::url                     target, 
//                                        int                           flags);
//
//      // advert directory functions
//      saga::task async_find            (std::string                   pattern, 
//                                        std::vector <std::string>     patterns, 
//                                        int                           flags);
  }; // class advertdirectory_cpi_impl

} // namespace ###suite###_###type###
////////////////////////////////////////////////////////////////////////

#endif // ADAPTORS_###SUITE###_###TYPE###_ADVERTDIRECTORY_HPP

