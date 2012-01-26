#if defined(__WAVE__)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/cpr_checkpoint.cpp")
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

#include <saga/impl/packages/cpr/cpr_checkpoint.hpp>
#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/packages/cpr_checkpoint_cpi_instance_data.hpp>

#ifdef SAGA_DEBUG
#include <saga/impl/packages/cpr/preprocessed/cpr_checkpoint.cpp>
#else

#if defined(__WAVE__)
#pragma wave option(preserve: 2, output: "preprocessed/cpr_checkpoint.cpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga 
{ 
  namespace impl 
  {
    cpr_checkpoint::cpr_checkpoint (saga::session const& s, saga::url url, int mode)
        : namespace_entry (s, saga::object::CPRCheckpoint)
    {
        // initialize the instance data
        typedef adaptors::v1_0::cpr_checkpoint_cpi_instance_data instance_data_type;
        typedef adaptors::instance_data <instance_data_type> 
            cpr_checkpoint_instance_data;

        // verify mode
        if (mode & saga::cpr::CreateParents)
            mode |= saga::cpr::Create;
            
        if (mode & saga::cpr::Create)
            mode |= saga::cpr::Write;     // needs to be writable at least

        {
            cpr_checkpoint_instance_data data;
            data.init_data (this, TR1::shared_ptr <instance_data_type> (
                new instance_data_type (url, mode)));
        } // lock goes out of scope
    }
    
    cpr_checkpoint::cpr_checkpoint (attribute const& rhs_attr)
    :   saga::impl::namespace_entry(saga::object::CPRCheckpoint),
        attribute_base(rhs_attr)
    {
    }
    
    void cpr_checkpoint::init()
    {
        try {
            // initialize a CPI instance and execute required functions (such as
            // creating the file etc.)
            this->initcpi(saga::adaptors::cpr_checkpoint_cpi);
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
    
    cpr_checkpoint::~cpr_checkpoint (void) 
    {
        // free the instance data
        typedef adaptors::v1_0::cpr_checkpoint_cpi_instance_data instance_data_type;
        typedef adaptors::instance_data <instance_data_type> 
            cpr_checkpoint_instance_data;

        cpr_checkpoint_instance_data data;
        data.release_data (this);
    }

    ///////////////////////////////////////////////////////////////////////////
    // factory support
    void cpr_checkpoint::create_impl_sync(saga::cpr::checkpoint& retval)
    {
        this->init();
        retval = saga::cpr::checkpoint(this);
    }

    saga::task cpr_checkpoint::create_impl_async(saga::session const& s, 
        saga::url const& url, int mode)
    {
        TR1::shared_ptr<cpr_checkpoint> impl(new cpr_checkpoint(s, url, mode));

        bool (cpr_checkpoint::*prep)(saga::cpr::checkpoint&, saga::uuid) = NULL;
        return saga::adaptors::task("__init__", TR1::shared_ptr<v1_0::cpi>(), 
            impl.get(), &cpr_checkpoint::create_impl_sync, prep);
    }

    // API methods not covered by namespace_entry
    SAGA_CALL_IMPL_IMPL_2(cpr_checkpoint, cpr_checkpoint_cpi, set_parent,      saga::url, int)
    SAGA_CALL_IMPL_IMPL_1(cpr_checkpoint, cpr_checkpoint_cpi, get_parent,      int)
    SAGA_CALL_IMPL_IMPL_0(cpr_checkpoint, cpr_checkpoint_cpi, get_file_num)    
    SAGA_CALL_IMPL_IMPL_0(cpr_checkpoint, cpr_checkpoint_cpi, list_files)
    SAGA_CALL_IMPL_IMPL_1(cpr_checkpoint, cpr_checkpoint_cpi, add_file,        saga::url)
    SAGA_CALL_IMPL_IMPL_1(cpr_checkpoint, cpr_checkpoint_cpi, get_file,        int)
    SAGA_CALL_IMPL_IMPL_2(cpr_checkpoint, cpr_checkpoint_cpi, open_file,       saga::url, int)
    SAGA_CALL_IMPL_IMPL_2(cpr_checkpoint, cpr_checkpoint_cpi, open_file_idx,   int,       int)
    SAGA_CALL_IMPL_IMPL_1(cpr_checkpoint, cpr_checkpoint_cpi, remove_file,     saga::url)
    SAGA_CALL_IMPL_IMPL_1(cpr_checkpoint, cpr_checkpoint_cpi, remove_file_idx, int)
    SAGA_CALL_IMPL_IMPL_2(cpr_checkpoint, cpr_checkpoint_cpi, update_file,     saga::url, saga::url)
    SAGA_CALL_IMPL_IMPL_2(cpr_checkpoint, cpr_checkpoint_cpi, update_file_idx, int,       saga::url)
    SAGA_CALL_IMPL_IMPL_2(cpr_checkpoint, cpr_checkpoint_cpi, stage_file,      saga::url, saga::url)
    SAGA_CALL_IMPL_IMPL_2(cpr_checkpoint, cpr_checkpoint_cpi, stage_file_idx,  int,       saga::url)
    SAGA_CALL_IMPL_IMPL_1(cpr_checkpoint, cpr_checkpoint_cpi, stage_file_all,  saga::url)

    // Generate a exact deep copy of this object
    saga::object cpr_checkpoint::clone() const
    {
        return saga::cpr::checkpoint(
            new cpr_checkpoint(static_cast<attribute_base const&>(*this)));
    }

  } // namespace impl

} // namespace saga
///////////////////////////////////////////////////////////////////////////////

#endif

