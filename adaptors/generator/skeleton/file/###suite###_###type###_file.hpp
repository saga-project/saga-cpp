//  Copyright (c) 2008 Andre Merzky <andre@merzky.net>
// 
//  Distributed under the Boost Software License, 
//  Version 1.0. (See accompanying LICENSE file 
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ADAPTORS_###SUITE###_###TYPE###_ADAPTOR_FILE_HPP
#define ADAPTORS_###SUITE###_###TYPE###_ADAPTOR_FILE_HPP

#include <saga/saga/adaptors/adaptor.hpp>
#include <saga/impl/packages/filesystem/file_cpi.hpp>

#include "###suite###_###type###_adaptor.hpp"

////////////////////////////////////////////////////////////////////////////////
//
namespace ###suite###_###type###_adaptor 
{
  class file_cpi_impl : public saga::adaptors::v1_0::file_cpi<file_cpi_impl>
  {
    private:

      typedef saga::adaptors::v1_0::file_cpi<file_cpi_impl> file_cpi;


      /* instance data */
      typedef saga::adaptors::v1_0::file_cpi_instance_data instance_data_type;

      friend class saga::adaptors::instance_data<instance_data_type>;
      typedef      saga::adaptors::instance_data<instance_data_type> file_instance_data_t;


      /* adaptor data */
      typedef saga::adaptors::adaptor_data <file_adaptor> adaptor_data_t;


    public:

      /*! constructor for a file */
      file_cpi_impl (proxy                * p, 
                     cpi_info       const & info,
                     saga::ini::ini const & glob_ini,
                     saga::ini::ini const & adap_ini,
                     boost::shared_ptr <saga::adaptor> adaptor);

      /*! destructor of the file adaptor */
      ~file_cpi_impl (void);


//       ///////////////////////////////////////////////////////////////////////////
//       ///////////////////////// NAMESPACE::ENTRY METHODS ////////////////////////
//       ///////////////////////////////////////////////////////////////////////////
// 
//       void sync_get_url   (saga::url                   & url); 
//       void sync_get_cwd   (saga::url                   & cwd); 
//       void sync_get_name  (saga::url                   & name); 
// 
//       void sync_is_dir    (bool                        & is_dir);      
//       void sync_is_entry  (bool                        & is_file);   
//       void sync_is_link   (bool                        & is_link);      
//       void sync_read_link (saga::url                   & target); 
// 
//       void sync_copy      (saga::impl::void_t          & ret,   
//                            saga::url                     target, 
//                            int                           flags = saga::filesystem::None);
//       void sync_link      (saga::impl::void_t          & ret,    
//                            saga::url                     dest, 
//                            int                           flags = saga::filesystem::None);
//       void sync_move      (saga::impl::void_t          & ret,   
//                            saga::url                     dest, 
//                            int                           flags = saga::filesystem::None);
//       void sync_remove    (saga::impl::void_t          & ret,
//                            int                           flags = saga::filesystem::None);  
// 
//       void sync_close     (saga::impl::void_t          & ret, 
//                            double                        timeout = 0.0); 
// 
//       ///////////////////////////////////////////////////////////////////////////
//       ////////////////////////// FILESYSTEM::FILE METHODS ///////////////////////
//       ///////////////////////////////////////////////////////////////////////////
// 
//       void sync_get_size  (saga::off_t                 & size_out);
//       void sync_read      (saga::ssize_t               & len_out,
//                            saga::mutable_buffer          data, 
//                            saga::ssize_t                 len_in);
//       void sync_write     (saga::ssize_t               & len_out,
//                            saga::const_buffer            data,
//                            saga::ssize_t                 len_in);
//       void sync_seek      (saga::off_t                 & out,
//                            saga::off_t                   offset,
//                            saga::filesystem::seek_mode   whence);
// 
// 
//       ///////////////////////////////////////////////////////////////////////////
//       /////////////////////// PERMISSION INTERFACE METHODS //////////////////////
//       ///////////////////////////////////////////////////////////////////////////
// 
//       void sync_permissions_allow (saga::impl::void_t  & ret,
//                                    std::string           id,
//                                    int                   perm, 
//                                    int                   flags);
//       void sync_permissions_deny  (saga::impl::void_t & ret,
//                                    std::string          id,
//                                    int                  perm, 
//                                    int                  flags);
//       void sync_permissions_check (bool               & ret,
//                                    std::string          id,
//                                    int                  perm);
//       void sync_get_owner         (std::string        & out);
//       void sync_get_group         (std::string        & out);    

  }; // class file_cpi_impl

} // namespace ###suite###_###type###_adaptor

#endif // ADAPTORS_###SUITE###_###TYPE###_ADAPTOR_FILE_HPP

