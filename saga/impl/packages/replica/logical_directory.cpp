#if defined(__WAVE__)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/logical_directory.cpp")
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
#include <saga/impl/packages/replica/logical_directory.hpp>
#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/packages/logical_directory_cpi_instance_data.hpp>

#ifdef SAGA_DEBUG
#include <saga/impl/packages/replica/preprocessed/logical_directory.cpp>
#else

#if defined(__WAVE__)
#pragma wave option(output: "preprocessed/logical_directory.cpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga 
{
  namespace impl 
  {
    logical_directory::logical_directory (saga::session const& s, 
            saga::url dir_url, int mode) 
        : namespace_dir (s, saga::object::LogicalDirectory)
    {
        // initialize the instance data
        typedef adaptors::v1_0::logical_directory_cpi_instance_data 
            instance_data_type;
        typedef adaptors::instance_data <instance_data_type> 
            logical_dir_instance_data;

        // verify mode
        int all_valid_flags = replica::Overwrite | replica::Recursive | 
            replica::Dereference | replica::Create | replica::Exclusive | 
            replica::Lock | replica::CreateParents | replica::Read | 
            replica::Write | replica::ReadWrite;

        if (mode & ~all_valid_flags)
        {
            SAGA_OSSTREAM strm;
            strm << "Unknown 'mode' used: " << std::hex << "0x" << mode;
            SAGA_THROW_NO_OBJECT(SAGA_OSSTREAM_GETSTRING(strm), saga::BadParameter);
        }

        if (mode & saga::replica::CreateParents)
            mode |= saga::replica::Create;
            
        if (mode & saga::replica::Create)
            mode |= saga::replica::Write;     // needs to be writable at least

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
    
    logical_directory::logical_directory (attribute const& rhs_attr)
    :   saga::impl::namespace_dir(saga::object::LogicalDirectory),
        attribute_base(rhs_attr)
    {
    }
    
    void logical_directory::init()
    {
        try {
            // initialize a CPI instance and execute required functions (such as
            // creating the directory etc.)
            this->initcpi(saga::adaptors::logical_directory_cpi);
        }
        catch (saga::exception const& e) {
            // map special exceptions
            switch (e.get_error()) {
            case saga::adaptors::AdaptorDeclined:
            case saga::adaptors::NoAdaptor:
            case saga::adaptors::NoAdaptorInfo:
            case saga::adaptors::Unexpected:
                SAGA_THROW_PLAIN(e.get_object(), e.what(), saga::NoSuccess);
            // just rethrow otherwise
            default:
                throw;
            }
        }
    }
    
    logical_directory::~logical_directory (void) 
    {
        // free the instance data
        typedef adaptors::v1_0::logical_directory_cpi_instance_data 
            instance_data_type;
        typedef adaptors::instance_data <instance_data_type> 
            logical_dir_instance_data;

        logical_dir_instance_data data;
        data.release_data (this);
    }

    ///////////////////////////////////////////////////////////////////////////
    // factory support
    void logical_directory::create_impl_sync(replica::logical_directory& retval)
    {
        this->init();
        retval = replica::logical_directory(this);
    }

    saga::task logical_directory::create_impl_async(saga::session const& s, 
        saga::url const& url, int mode)
    {
        TR1::shared_ptr<logical_directory> impl(new logical_directory(s, url, mode));

        bool (logical_directory::*prep)(replica::logical_directory&, saga::uuid) = NULL;
        return saga::adaptors::task("__init__", TR1::shared_ptr<v1_0::cpi>(), 
            impl.get(), &logical_directory::create_impl_sync, prep);
    }

    // logical_file info
    SAGA_CALL_IMPL_IMPL_1 (logical_directory, logical_directory_cpi, is_file, saga::url)

    // overload open methods of name space
    SAGA_CALL_IMPL_IMPL_2 (logical_directory, logical_directory_cpi, open, saga::url, int)
    SAGA_CALL_IMPL_IMPL_2 (logical_directory, logical_directory_cpi, open_dir, saga::url, int)
    SAGA_CALL_IMPL_IMPL_3 (logical_directory, logical_directory_cpi, find, std::string, std::vector<std::string>, int)

    // Generate a exact deep copy of this object
    saga::object logical_directory::clone() const
    {
        return saga::replica::logical_directory(
            new logical_directory(static_cast<attribute_base const&>(*this)));
    }

  } // namespace impl

///////////////////////////////////////////////////////////////////////////////
} // namespace saga

#endif

