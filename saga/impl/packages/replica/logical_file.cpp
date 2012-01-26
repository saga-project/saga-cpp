#if defined(__WAVE__)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/logical_file.cpp")
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

#include <saga/impl/packages/replica/logical_file.hpp>
#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/packages/logical_file_cpi_instance_data.hpp>

#ifdef SAGA_DEBUG
#include <saga/impl/packages/replica/preprocessed/logical_file.cpp>
#else

#if defined(__WAVE__)
#pragma wave option(preserve: 2, output: "preprocessed/logical_file.cpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga 
{ 
  namespace impl 
  {
    logical_file::logical_file (saga::session const& s, saga::url url, int mode)
        : namespace_entry (s, saga::object::LogicalFile)
    {
        // initialize the instance data
        typedef adaptors::v1_0::logical_file_cpi_instance_data instance_data_type;
        typedef adaptors::instance_data <instance_data_type> 
            logical_file_instance_data;

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

        // complement mode, if appropriate
        if (mode & saga::replica::CreateParents)
            mode |= saga::replica::Create;
            
        if (mode & saga::replica::Create)
            mode |= saga::replica::Write;     // needs to be writable at least

        {
            logical_file_instance_data data;
            data.init_data (this, TR1::shared_ptr <instance_data_type> (
                new instance_data_type (url, mode)));
        } // lock goes out of scope
    }
    
    logical_file::logical_file (attribute const& rhs_attr)
    :   saga::impl::namespace_entry(saga::object::LogicalFile),
        attribute_base(rhs_attr)
    {
    }
    
    void logical_file::init()
    {
        try {
            // initialize a CPI instance and execute required functions (such as
            // creating the file etc.)
            this->initcpi(saga::adaptors::logical_file_cpi);
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
    
    logical_file::~logical_file (void) 
    {
        // free the instance data
        typedef adaptors::v1_0::logical_file_cpi_instance_data instance_data_type;
        typedef adaptors::instance_data <instance_data_type> 
            logical_file_instance_data;

        logical_file_instance_data data;
        data.release_data (this);
    }

    ///////////////////////////////////////////////////////////////////////////
    // factory support
    void logical_file::create_impl_sync(replica::logical_file& retval)
    {
        this->init();
        retval = replica::logical_file(this);
    }

    saga::task logical_file::create_impl_async(saga::session const& s, 
        saga::url const& url, int mode)
    {
        TR1::shared_ptr<logical_file> impl(new logical_file(s, url, mode));

        bool (logical_file::*prep)(replica::logical_file&, saga::uuid) = NULL;
        return saga::adaptors::task("__init__", TR1::shared_ptr<v1_0::cpi>(), 
            impl.get(), &logical_file::create_impl_sync,prep);
    }

    // API methods not covered by namespace_entry
    SAGA_CALL_IMPL_IMPL_1(logical_file, logical_file_cpi, add_location, saga::url)
    SAGA_CALL_IMPL_IMPL_1(logical_file, logical_file_cpi, remove_location, saga::url)
    SAGA_CALL_IMPL_IMPL_2(logical_file, logical_file_cpi, update_location, saga::url, saga::url)
    SAGA_CALL_IMPL_IMPL_0(logical_file, logical_file_cpi, list_locations)
    SAGA_CALL_IMPL_IMPL_2(logical_file, logical_file_cpi, replicate, saga::url, int)

    // Generate a exact deep copy of this object
    saga::object logical_file::clone() const
    {
        return saga::replica::logical_file(
            new logical_file(static_cast<attribute_base const&>(*this)));
    }

  } // namespace impl

} // namespace saga
///////////////////////////////////////////////////////////////////////////////

#endif

