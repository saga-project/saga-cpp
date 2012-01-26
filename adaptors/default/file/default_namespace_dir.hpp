//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ADAPTORS_DEFAULT_FILE_DEFAULT_NAME_SPACE_DIR_HPP
#define ADAPTORS_DEFAULT_FILE_DEFAULT_NAME_SPACE_DIR_HPP

#include <string>

#include <saga/saga/util.hpp>
#include <saga/saga/adaptors/adaptor.hpp>
#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/packages/namespace_dir_cpi_instance_data.hpp>

#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/packages/filesystem/directory_cpi.hpp>

#include <saga/saga/uuid.hpp>

#include "default_namespace_entry.hpp"

///////////////////////////////////////////////////////////////////////////////
template <typename Base>
class namespace_dir_cpi_impl 
:   public namespace_entry_cpi_impl<Base>
{
  private:
    typedef namespace_entry_cpi_impl<Base> base_cpi;
    std::vector<saga::url> entries_;

  protected:
    typedef typename base_cpi::instance_data instance_data;
    typedef typename Base::mutex_type mutex_type;

  public:
    /*! constructor of the namespace_dir cpi */
    namespace_dir_cpi_impl  (saga::impl::proxy * p, 
                              saga::impl::v1_0::cpi_info const & info,
                              saga::ini::ini const & glob_ini, 
                              saga::ini::ini const & adap_ini,
                              TR1::shared_ptr<saga::adaptor> adaptor,
                              saga::impl::v1_0::cpi::flags flags);

    /*! destructor of the namespace_dir cpi */
    ~namespace_dir_cpi_impl (void);

    /* implementation of query methods */
    void sync_exists   (bool & exists,   saga::url url);
    void sync_is_dir   (bool & is_dir,   saga::url url);
    void sync_is_entry (bool & is_entry, saga::url url);
    void sync_is_file  (bool & is_file,  saga::url url);
    void sync_is_link  (bool & is_link,  saga::url url);

    // overload from ns_entry
    void sync_remove (saga::impl::void_t & ret, int flags);

    /* implementation of saga::directory::open and saga::directory::open_directory */
    void sync_open     (saga::name_space::entry& entry, saga::url name, int openmode); 
    void sync_open_dir (saga::name_space::directory& entry, saga::url name, int openmode); 

    void sync_change_dir (saga::impl::void_t&, saga::url new_dir);
    void sync_make_dir(saga::impl::void_t & ret, saga::url url, int open_mode);
    void sync_list(std::vector <saga::url> & retval, std::string pattern, int);
    void sync_find(std::vector <saga::url> & retval, std::string entry, int);
    
    void sync_copy(saga::impl::void_t & ret, saga::url source, 
                   saga::url destination, int flags);
    void sync_move(saga::impl::void_t & ret, saga::url source, 
                   saga::url destination, int flags);
    void sync_remove(saga::impl::void_t & ret, saga::url url, int flags);
    void sync_link(saga::impl::void_t & ret, saga::url source, 
                   saga::url url, int open_mode);
                   
    void sync_read_link(saga::url& ret, saga::url source);

    void sync_get_num_entries(std::size_t& ret);
    void sync_get_entry(saga::url& ret, std::size_t entry);

    // wildcard functions
    void sync_copy_wildcard(saga::impl::void_t& ret, std::string source, 
                   saga::url destination, int flags);
    void sync_move_wildcard(saga::impl::void_t& ret, std::string source, 
                   saga::url destination, int flags);
    void sync_remove_wildcard(saga::impl::void_t& ret, std::string url, int flags);
    void sync_link_wildcard(saga::impl::void_t& ret, std::string source, 
                   saga::url url, int open_mode);

#if ! defined (SAGA_DEFAULT_FILE_ADAPTOR_NO_ASYNCS)
    // This adaptor implements the async functions based on its own synchronous
    // functions for testing purposes only. SInce there is no principal need
    // to do so, we allow these to be 'switched off'.
    saga::task async_open (saga::url name, int openmode); 
    saga::task async_open_dir (saga::url name, int openmode); 

    saga::task async_exists(saga::url url);
    saga::task async_is_entry(saga::url url);
    saga::task async_is_dir(saga::url url);
    saga::task async_is_link(saga::url url);

    saga::task async_change_dir(saga::url new_dir);
    saga::task async_list(std::string, int);
    saga::task async_find(std::string, int);

    saga::task async_copy(saga::url source, saga::url destination, int flags);
    saga::task async_move(saga::url source, saga::url destination, int flags);
    saga::task async_remove(saga::url url, int flags); 
    saga::task async_make_dir(saga::url url, int open_mode);  
    
    saga::task async_link(saga::url source, saga::url url, int open_mode);
    saga::task async_read_link(saga::url source); 
    
    saga::task async_get_num_entries();
    saga::task async_get_entry(std::size_t entry);

#if ! defined (SAGA_DEFAULT_FILE_ADAPTOR_NO_BULKS)
    // bulk ops
    bool prepare_copy(saga::impl::void_t & ret, saga::url source, 
                      saga::url destination, int open_mode, 
                      saga::uuid task_uuid=NULL);
    std::vector<saga::uuid> bulk_copy(
                      std::vector<TR1::reference_wrapper <saga::impl::void_t> >& rets,
                      std::vector<saga::url>& sources, 
                      std::vector<saga::url>& dests, 
                      std::vector<int>& modes, std::vector<saga::uuid>& uuids);

    bool prepare_move(saga::impl::void_t & ret, saga::url source, 
                      saga::url destination, int open_mode,
                      saga::uuid task_uuid=NULL);
    std::vector<saga::uuid> bulk_move(
                      std::vector<TR1::reference_wrapper <saga::impl::void_t> >& rets,
                      std::vector<saga::url>& sources, 
                      std::vector<saga::url>& dests, 
                      std::vector<int>& modes, std::vector<saga::uuid>& uuids);
    
    bool prepare_remove(saga::impl::void_t & ret, saga::url url, 
                        int open_mode, saga::uuid task_uuid=NULL);
    std::vector<saga::uuid> bulk_remove(
                    std::vector<TR1::reference_wrapper <saga::impl::void_t> >& rets,
                    std::vector<saga::url>& urls, 
                    std::vector<int>& modes, 
                    std::vector<saga::uuid>& uuids);

#endif // ! SAGA_DEFAULT_FILE_ADAPTOR_NO_BULKS
#endif // ! SAGA_DEFAULT_FILE_ADAPTOR_NO_ASYNCS

protected:
    void sync_copy_wildcard_helper(saga::url const& location, 
        std::string const& source, saga::url const& destination, int flags);
    void sync_move_wildcard_helper(saga::url const& location,
        std::string const& source, saga::url const& destination, int flags);
    void sync_remove_wildcard_helper(saga::url const& location, 
        std::string const& source, int flags);
// #if BOOST_VERSION >= 103500
    void sync_link_wildcard_helper(saga::url const& location,
        std::string const& source, saga::url const& symlink, int flags);
// #endif
};  // class namespace_dir_cpi_impl

#endif // ADAPTORS_DEFAULT_FILE_DEFAULT_NAME_SPACE_DIR_HPP

