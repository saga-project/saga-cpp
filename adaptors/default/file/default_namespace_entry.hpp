//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ADAPTORS_DEFAULT_FILE_DEFAULT_NAME_SPACE_ENTRY_HPP
#define ADAPTORS_DEFAULT_FILE_DEFAULT_NAME_SPACE_ENTRY_HPP

#include <vector>
#include <string>
#include <fstream>

#include <saga/saga/util.hpp>
#include <saga/saga/adaptors/adaptor.hpp>
#include <saga/saga/adaptors/instance_data.hpp>

#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/packages/filesystem/directory_cpi.hpp>

///////////////////////////////////////////////////////////////////////////////
template <typename Base>
class namespace_entry_cpi_impl : public Base
{
  private:
    typedef Base base_cpi;

  protected:
    typedef typename Base::instance_data instance_data;
    typedef typename Base::mutex_type mutex_type;

    void check_if_open (std::string const& functionname, 
        saga::url const& location);

    bool is_open_;

  public:
    /*! constructor of the namespace_entry adaptor */
    namespace_entry_cpi_impl  (saga::impl::proxy* p, 
                                saga::impl::v1_0::cpi_info const & info,
                                saga::ini::ini const & glob_ini, 
                                saga::ini::ini const & adap_ini,
                                TR1::shared_ptr<saga::adaptor> adaptor,
                                saga::impl::v1_0::cpi::flags flags);

    /*! destructor of the ns_entry adaptor */
    ~namespace_entry_cpi_impl (void);

    /* implementation of query methods */
    void sync_exists   (bool & exists);
    void sync_is_dir   (bool & is_dir);
    void sync_is_entry (bool & is_entry);
    void sync_is_file  (bool & is_file);
    void sync_is_link  (bool & is_link);

    /*! implementation of saga::directory::get_url */
    void sync_get_url  (saga::url& url);
    void sync_get_cwd  (saga::url& url);
    void sync_get_name (saga::url& url);

    void sync_copy (saga::impl::void_t & ret, saga::url source, int flags);
    void sync_link (saga::impl::void_t & ret, saga::url source, int flags);
    void sync_move (saga::impl::void_t & ret, saga::url source, int flags);
    void sync_remove (saga::impl::void_t & ret, int flags);
    void sync_close (saga::impl::void_t & ret, double);

#if ! defined (SAGA_DEFAULT_FILE_ADAPTOR_NO_ASYNCS)
    // This adaptor implements the async functions based on its own synchronous
    // functions for testing purposes only. Since there is no real need
    // to do so, we allow these to be 'switched off'.
    
    saga::task async_get_url();
    saga::task async_get_cwd();
    saga::task async_get_name();

    saga::task async_exists();
    saga::task async_is_entry();
    saga::task async_is_dir();
    saga::task async_is_link();
    
    saga::task async_copy(saga::url dest, int flags);
    saga::task async_link(saga::url target, int flags);
    saga::task async_move(saga::url dest, int flags);
    saga::task async_remove(int flags);
    saga::task async_close(double);

#if ! defined (SAGA_DEFAULT_FILE_ADAPTOR_NO_BULKS)
    // Bulk ops
    bool prepare_get_name(saga::url& url, saga::uuid task_uuid);
    std::vector<saga::uuid> bulk_get_name(
                            std::vector<TR1::reference_wrapper<saga::url> >&,
                            std::vector<saga::uuid> & params);
#endif // ! SAGA_DEFAULT_FILE_ADAPTOR_NO_BULKS
#endif // ! SAGA_DEFAULT_FILE_ADAPTOR_NO_ASYNCS

};  // class namespace_entry_cpi_impl

#endif // ADAPTORS_DEFAULT_FILE_DEFAULT_NAME_SPACE_ENTRY_HPP

