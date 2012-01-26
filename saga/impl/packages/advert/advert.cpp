#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/advert.cpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 0, output: null)
#endif

#include <saga/saga/url.hpp>

#include <saga/saga/adaptors/task.hpp>

#include <saga/impl/packages/advert/advert.hpp>
#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/packages/advert_cpi_instance_data.hpp>

#ifdef SAGA_DEBUG
#include <saga/impl/packages/advert/preprocessed/advert.cpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, output: "preprocessed/advert.cpp")
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl 
{
    advert::advert (saga::session const& s, saga::url url, int mode)
    :   namespace_entry (s, saga::object::Advert)
    {
        // initialize the instance data
        typedef adaptors::v1_0::advert_cpi_instance_data instance_data_type;
        typedef adaptors::instance_data <instance_data_type> 
            advert_instance_data;

        // verify mode
        int all_valid_flags = saga::advert::Overwrite | saga::advert::Recursive | 
            saga::advert::Dereference | saga::advert::Create | saga::advert::Exclusive | 
            saga::advert::Lock | saga::advert::CreateParents | saga::advert::Read | 
            saga::advert::Write | saga::advert::ReadWrite;

        if (mode & ~all_valid_flags)
        {
            SAGA_OSSTREAM strm;
            strm << "Unknown 'mode' used: " << std::hex << "0x" << mode;
            SAGA_THROW_NO_OBJECT(SAGA_OSSTREAM_GETSTRING(strm), saga::BadParameter);
        }

        // complement mode, if appropriate
        if (mode & saga::advert::CreateParents)
            mode |= saga::advert::Create;
            
        if (mode & saga::advert::Create)
            mode |= saga::advert::Write;     // needs to be writable at least

        {
            advert_instance_data data;
            data.init_data (this, TR1::shared_ptr <instance_data_type> (
                new instance_data_type (url, mode)));
        } // lock goes out of scope
    }
    
    advert::advert (attribute const& rhs_attr)
    :   saga::impl::namespace_entry(saga::object::Advert),
        attribute_base(rhs_attr)
    {
    }
    
    void advert::init()
    {
        try {
            // initialize a CPI instance and execute required functions (such as
            // creating the file etc.)
            this->initcpi(saga::adaptors::advert_cpi);
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
    
    advert::~advert (void) 
    {
        // free the instance data
        typedef adaptors::v1_0::advert_cpi_instance_data instance_data_type;
        typedef adaptors::instance_data <instance_data_type> 
            advert_instance_data;

        advert_instance_data data;
        data.release_data (this);
    }

    ///////////////////////////////////////////////////////////////////////////
    // factory support
    void advert::create_impl_sync(saga::advert::entry& retval)
    {
        this->init();
        retval = saga::advert::entry(this);
    }

    saga::task advert::create_impl_async(saga::session const& s, 
        saga::url const& url, int mode)
    {
        TR1::shared_ptr<advert> impl(new advert(s, url, mode));

        bool (advert::*prep)(saga::advert::entry&, saga::uuid) = NULL;
        return saga::adaptors::task("__init__", TR1::shared_ptr<v1_0::cpi>(), 
            impl.get(), &advert::create_impl_sync, prep);
    }

    // API methods not covered by namespace_entry
    SAGA_CALL_IMPL_IMPL_1(advert, advert_cpi, store_object, saga::object)
    SAGA_CALL_IMPL_IMPL_1(advert, advert_cpi, retrieve_object, saga::session)
    SAGA_CALL_IMPL_IMPL_1(advert, advert_cpi, store_string, std::string)
    SAGA_CALL_IMPL_IMPL_0(advert, advert_cpi, retrieve_string)

    // Generate a exact deep copy of this object
    saga::object advert::clone() const
    {
        return saga::advert::entry(
            new advert(static_cast<attribute_base const&>(*this)));
    }

}} // namespace saga::impl
///////////////////////////////////////////////////////////////////////////////

#endif

