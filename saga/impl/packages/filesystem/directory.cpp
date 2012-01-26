#if defined(__WAVE__)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/directory.cpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005 Michel Zandstra (michelzandstra@gmail.com)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__)
#pragma wave option(preserve: 0, output: null)
#endif
#include <saga/saga/util.hpp>
#include <saga/saga/url.hpp>

#include <saga/saga/adaptors/task.hpp>

// needs tasks for async ops
#include <saga/impl/call.hpp>
#include <saga/impl/engine/task.hpp>
#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/engine/sync_async.hpp>

// own header file
#include <saga/impl/packages/filesystem/directory.hpp>
#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/packages/directory_cpi_instance_data.hpp>

#ifdef SAGA_DEBUG
#include <saga/impl/packages/filesystem/preprocessed/directory.cpp>
#else

#if defined(__WAVE__)
#pragma wave option(output: "preprocessed/directory.cpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga 
{
  namespace impl 
  {
    directory::directory (saga::session const & s, saga::url dir_url, int mode) 
        : namespace_dir (s, saga::object::Directory)
    {
        // initialize the instance data
        typedef adaptors::v1_0::directory_cpi_instance_data instance_data_type;
        typedef adaptors::instance_data <instance_data_type> dir_instance_data;

        // verify mode
        int all_valid_flags = filesystem::Overwrite | filesystem::Recursive | 
            filesystem::Dereference | filesystem::Create | filesystem::Exclusive | 
            filesystem::Lock | filesystem::CreateParents | filesystem::Truncate | 
            filesystem::Append | filesystem::Read | filesystem::Write | 
            filesystem::ReadWrite | filesystem::Binary;

        if (mode & ~all_valid_flags)
        {
            SAGA_OSSTREAM strm;
            strm << "Unknown 'mode' used: " << std::hex << "0x" << mode;
            SAGA_THROW_NO_OBJECT(SAGA_OSSTREAM_GETSTRING(strm), saga::BadParameter);
        }

        // complement mode, if appropriate
        if (mode & saga::filesystem::CreateParents)
            mode |= saga::filesystem::Create;
            
        if (mode & saga::filesystem::Create)
            mode |= saga::filesystem::Write;     // needs to be writable at least

        // make sure, url is valid and directory path ends with a '/'
        std::string pathstr (dir_url.get_path());
        std::string::size_type pos = pathstr.find_last_of(SAGA_PATH_DELIMITERS);
        if (pathstr.empty() || pathstr.size()-1 != pos) 
        {
            pathstr += "/";
            dir_url.set_path(pathstr);
        }

        {
            dir_instance_data data;
            data.init_data (this, TR1::shared_ptr <instance_data_type> (
                new instance_data_type (dir_url, mode)));
        } // lock goes out of scope
    }

    void directory::init()
    {
        try {
            // initialize a CPI instance and execute required functions (such as
            // creating the directory etc.)
            this->initcpi(saga::adaptors::directory_cpi);
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
    
    directory::~directory (void) 
    {
        // free the instance data
        typedef adaptors::v1_0::directory_cpi_instance_data instance_data_type;
        typedef adaptors::instance_data <instance_data_type> dir_instance_data;

        dir_instance_data data;
        data.release_data (this);
    }

    ///////////////////////////////////////////////////////////////////////////
    // factory support
    void directory::create_impl_sync(filesystem::directory& retval)
    {
        this->init();
        retval = filesystem::directory(this);
    }

    saga::task directory::create_impl_async(saga::session const& s, 
        saga::url const& url, int mode)
    {
        TR1::shared_ptr<directory> impl(new directory(s, url, mode));

        bool (directory::*prep)(filesystem::directory&, saga::uuid) = NULL;
        return saga::adaptors::task("__init__", TR1::shared_ptr<v1_0::cpi>(), 
            impl.get(), &directory::create_impl_sync, prep);
    }

    // directory info
    SAGA_CALL_IMPL_IMPL_2 (directory, directory_cpi, get_size, saga::url, int)
    SAGA_CALL_IMPL_IMPL_1 (directory, directory_cpi, is_file, saga::url)

    // overload open methods of name space
    SAGA_CALL_IMPL_IMPL_2 (directory, directory_cpi, open, saga::url, int)
    SAGA_CALL_IMPL_IMPL_2 (directory, directory_cpi, open_dir, saga::url, int)

    // Generate a exact deep copy of this object
    saga::object directory::clone() const
    {
        saga::url location;
        int mode = 0;

        {
            typedef adaptors::v1_0::directory_cpi_instance_data instance_data_type;
            typedef adaptors::instance_data <instance_data_type> dir_instance_data;

            dir_instance_data data(const_cast<directory*>(this));
            location = data->location_;
            mode = data->mode_;
        }

        return saga::filesystem::file(get_session(), location, mode);
    }

  } // namespace impl

} // namespace saga
///////////////////////////////////////////////////////////////////////////////

#endif

