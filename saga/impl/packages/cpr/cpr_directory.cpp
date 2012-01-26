#if defined(__WAVE__)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/cpr_directory.cpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__)
#pragma wave option(preserve: 0, output: null)
#endif

#include <saga/saga/url.hpp>
#include <saga/saga/adaptors/task.hpp>

// needs tasks for async ops
#include <saga/impl/config.hpp>
#include <saga/impl/call.hpp>
#include <saga/impl/engine/task.hpp>
#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/engine/sync_async.hpp>

// own header file
#include <saga/impl/packages/cpr/cpr_directory.hpp>
#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/packages/cpr_directory_cpi_instance_data.hpp>

#ifdef SAGA_DEBUG
#include <saga/impl/packages/cpr/preprocessed/cpr_directory.cpp>
#else

#if defined(__WAVE__)
#pragma wave option(output: "preprocessed/cpr_directory.cpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga 
{
  namespace impl 
  {
    cpr_directory::cpr_directory (saga::session const& s, 
            saga::url dir_url, int mode) 
        : namespace_dir (s, saga::object::CPRDirectory)
    {
        // initialize the instance data
        typedef adaptors::v1_0::cpr_directory_cpi_instance_data 
            instance_data_type;
        typedef adaptors::instance_data <instance_data_type> 
            logical_dir_instance_data;

        // verify mode
        if (mode & saga::cpr::CreateParents)
            mode |= saga::cpr::Create;
            
        if (mode & saga::cpr::Create)
            mode |= saga::cpr::Write;     // needs to be writable at least

        // make sure, url is valid and directory path ends with a '/'
        std::string pathstr (dir_url.get_path());
        std::string::size_type pos = pathstr.find_last_of(SAGA_PATH_DELIMITERS);
        if (pathstr.empty() || pathstr.size()-1 != pos) 
        {
            pathstr += "/";
            dir_url.set_path(pathstr);
        }
        
        {
            logical_dir_instance_data data;
            data.init_data (this, TR1::shared_ptr <instance_data_type> (
                new instance_data_type (dir_url, mode)));
        } // lock goes out of scope
    }
    
    cpr_directory::cpr_directory (attribute const& rhs_attr)
    :   saga::impl::namespace_dir(saga::object::CPRDirectory),
        attribute_base(rhs_attr)
    {
    }
    
    void cpr_directory::init()
    {
        try {
            // initialize a CPI instance and execute required functions (such as
            // creating the directory etc.)
            this->initcpi(saga::adaptors::cpr_directory_cpi);
        }
        catch (saga::exception const& e) {
            // map special exceptions
            switch (e.get_error()) {
            case saga::adaptors::AdaptorDeclined:
            case saga::adaptors::NoAdaptor:
            case saga::adaptors::NoAdaptorInfo:
            case saga::adaptors::Unexpected:
                SAGA_THROW_PLAIN(e.get_object(), e.get_message(), saga::NoSuccess);
            // just rethrow otherwise
            default:
                throw;
            }
        }
    }
    
    cpr_directory::~cpr_directory (void) 
    {
        // free the instance data
        typedef adaptors::v1_0::cpr_directory_cpi_instance_data 
            instance_data_type;
        typedef adaptors::instance_data <instance_data_type> 
            logical_dir_instance_data;

        logical_dir_instance_data data;
        data.release_data (this);
    }

    ///////////////////////////////////////////////////////////////////////////
    // factory support
    void cpr_directory::create_impl_sync(saga::cpr::directory& retval)
    {
        this->init();
        retval = saga::cpr::directory(this);
    }

    saga::task cpr_directory::create_impl_async(saga::session const& s, 
        saga::url const& url, int mode)
    {
        TR1::shared_ptr<cpr_directory> impl(new cpr_directory(s, url, mode));

        bool (cpr_directory::*prep)(saga::cpr::directory&, saga::uuid) = NULL;
        return saga::adaptors::task("__init__", TR1::shared_ptr<v1_0::cpi>(), 
            impl.get(), &cpr_directory::create_impl_sync, prep);
    }

    SAGA_CALL_IMPL_IMPL_1 (cpr_directory, cpr_directory_cpi, is_checkpoint,   saga::url)
    SAGA_CALL_IMPL_IMPL_4 (cpr_directory, cpr_directory_cpi, find,            std::string, std::vector<std::string>, int, std::string)
    SAGA_CALL_IMPL_IMPL_3 (cpr_directory, cpr_directory_cpi, set_parent,      saga::url, saga::url, int)
    SAGA_CALL_IMPL_IMPL_2 (cpr_directory, cpr_directory_cpi, get_parent,      saga::url, int)
    SAGA_CALL_IMPL_IMPL_1 (cpr_directory, cpr_directory_cpi, get_file_num,    saga::url)
    SAGA_CALL_IMPL_IMPL_1 (cpr_directory, cpr_directory_cpi, list_files,      saga::url)
    SAGA_CALL_IMPL_IMPL_2 (cpr_directory, cpr_directory_cpi, add_file,        saga::url, saga::url)
    SAGA_CALL_IMPL_IMPL_2 (cpr_directory, cpr_directory_cpi, get_file,        saga::url, int)
    SAGA_CALL_IMPL_IMPL_3 (cpr_directory, cpr_directory_cpi, open_file,       saga::url, saga::url, int)
    SAGA_CALL_IMPL_IMPL_3 (cpr_directory, cpr_directory_cpi, open_file_idx,   saga::url, int,       int)
    SAGA_CALL_IMPL_IMPL_2 (cpr_directory, cpr_directory_cpi, remove_file,     saga::url, saga::url)
    SAGA_CALL_IMPL_IMPL_2 (cpr_directory, cpr_directory_cpi, remove_file_idx, saga::url, int)
    SAGA_CALL_IMPL_IMPL_3 (cpr_directory, cpr_directory_cpi, update_file,     saga::url, saga::url, saga::url)
    SAGA_CALL_IMPL_IMPL_3 (cpr_directory, cpr_directory_cpi, update_file_idx, saga::url, int,       saga::url)
    SAGA_CALL_IMPL_IMPL_3 (cpr_directory, cpr_directory_cpi, stage_file,      saga::url, saga::url, saga::url)
    SAGA_CALL_IMPL_IMPL_3 (cpr_directory, cpr_directory_cpi, stage_file_idx,  saga::url, int,       saga::url)
    SAGA_CALL_IMPL_IMPL_2 (cpr_directory, cpr_directory_cpi, stage_file_all,  saga::url, saga::url)
    SAGA_CALL_IMPL_IMPL_2 (cpr_directory, cpr_directory_cpi, open,            saga::url, int)
    SAGA_CALL_IMPL_IMPL_2 (cpr_directory, cpr_directory_cpi, open_dir,        saga::url, int)

    // Generate a exact deep copy of this object
    saga::object cpr_directory::clone() const
    {
        return saga::cpr::directory(
            new cpr_directory(static_cast<attribute_base const&>(*this)));
    }

  } // namespace impl

///////////////////////////////////////////////////////////////////////////////
} // namespace saga

#endif

