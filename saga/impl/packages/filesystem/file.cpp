#if defined(__WAVE__)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/file.cpp")
#endif
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__)
#pragma wave option(preserve: 0, output: null)
#endif

#include <saga/saga/url.hpp>

#include <saga/saga/adaptors/task.hpp>

#include <saga/impl/packages/filesystem/file.hpp>
#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/packages/file_cpi_instance_data.hpp>

#ifdef SAGA_DEBUG
#include <saga/impl/packages/filesystem/preprocessed/file.cpp>
#else

#if defined(__WAVE__)
#pragma wave option(preserve: 2, output: "preprocessed/file.cpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga 
{ 
  namespace impl 
  {
    file::file (saga::session const & s, saga::url file_url, int mode, 
            saga::off_t pointer)
      : namespace_entry (s, saga::object::File)
    {
        // initialize the instance data
        typedef adaptors::v1_0::file_cpi_instance_data instance_data_type;
        typedef adaptors::instance_data <instance_data_type> file_instance_data;

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

        // make sure url is valid 
        {
            file_instance_data data;
            data.init_data (this, TR1::shared_ptr <instance_data_type> (
                new instance_data_type (file_url.get_url(), mode, pointer)));
        } // lock goes out of scope
    }
    
    void file::init()
    {
        try {
            // initialize a CPI instance and execute required functions (such as
            // creating the file etc.)
            this->initcpi(saga::adaptors::file_cpi);
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
    
    file::~file (void) 
    {
        // free the instance data
        typedef adaptors::v1_0::file_cpi_instance_data instance_data_type;
        typedef adaptors::instance_data <instance_data_type> file_instance_data;

        file_instance_data data;
        data.release_data (this);
    }

    ///////////////////////////////////////////////////////////////////////////
    // factory support
    void file::create_impl_sync(filesystem::file& retval)
    {
        this->init();
        retval = filesystem::file(this);
    }

    saga::task file::create_impl_async(saga::session const& s, 
        saga::url const& url, int mode)
    {
        TR1::shared_ptr<file> impl(new file(s, url, mode));

        bool (file::*prep)(filesystem::file&, saga::uuid) = NULL;
        return saga::adaptors::task("__init__", TR1::shared_ptr<v1_0::cpi>(), 
            impl.get(), &file::create_impl_sync, prep);
    }

    // API methods not covered by namespace_entry
    SAGA_CALL_IMPL_IMPL_0 (file, file_cpi, get_size)

//    SAGA_CALL_IMPL_IMPL_2 (file, file_cpi, read, saga::mutable_buffer, saga::ssize_t)
    saga::task 
    file::read (saga::mutable_buffer buff, saga::ssize_t length, bool is_sync)
    {
        // handle default arguments
        if (0 == length && std::size_t(-1) != buff.get_size())
            length = buff.get_size();

        // standard procedure from here
        return saga::impl::execute_sync_async(this,
            "file_cpi", "read", "file_cpi::read", is_sync,
            &file_cpi::sync_read, &file_cpi::async_read, buff, length);
    }
//     SAGA_CALL_IMPL_IMPL_2 (file, file_cpi, write, saga::const_buffer, saga::ssize_t)
    saga::task 
    file::write (saga::const_buffer buff, saga::ssize_t length, bool is_sync)
    {
        // handle default arguments
        if (0 == length && std::size_t(-1) != buff.get_size())
            length = buff.get_size();

        // standard procedure from here
        return saga::impl::execute_sync_async(this,
            "file_cpi", "write", "file_cpi::write", is_sync,
            &file_cpi::sync_write, &file_cpi::async_write, buff, length);
    }
    SAGA_CALL_IMPL_IMPL_2 (file, file_cpi, seek, saga::off_t, saga::filesystem::seek_mode)

    // scattered I/O
    SAGA_CALL_IMPL_IMPL_1 (file, file_cpi, read_v, std::vector<saga::filesystem::iovec>)
    SAGA_CALL_IMPL_IMPL_1 (file, file_cpi, write_v, std::vector<saga::filesystem::const_iovec>)

    // pattern based I/O
    SAGA_CALL_IMPL_IMPL_1 (file, file_cpi, size_p, std::string)
    SAGA_CALL_IMPL_IMPL_2 (file, file_cpi, read_p, std::string, saga::mutable_buffer)
    SAGA_CALL_IMPL_IMPL_2 (file, file_cpi, write_p, std::string, saga::const_buffer)

    // extended based I/O
    SAGA_CALL_IMPL_IMPL_0 (file, file_cpi, modes_e)
    SAGA_CALL_IMPL_IMPL_2 (file, file_cpi, size_e,  std::string, std::string)
    SAGA_CALL_IMPL_IMPL_3 (file, file_cpi, read_e,  std::string, std::string, saga::mutable_buffer)
    SAGA_CALL_IMPL_IMPL_3 (file, file_cpi, write_e, std::string, std::string, saga::const_buffer)

    // Generate a exact deep copy of this object
    saga::object file::clone() const
    {
        saga::url location;
        int mode = 0;
        saga::off_t pointer = 0;

        {
            typedef adaptors::v1_0::file_cpi_instance_data instance_data_type;
            typedef adaptors::instance_data <instance_data_type> file_instance_data;

            file_instance_data data(const_cast<file*>(this));
            location = data->location_;
            mode = data->mode_;
            pointer = data->pointer_;
        }

        return saga::filesystem::file(get_session(), location, mode, pointer);
    }

  } // namespace impl
} // namespace saga
///////////////////////////////////////////////////////////////////////////////

#endif

