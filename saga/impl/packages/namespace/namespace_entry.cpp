#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/namespace_entry.cpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005 Michel Zandstra (michelzandstra@gmail.com)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 0, output: null)
#endif
#include <saga/saga/util.hpp>
#include <saga/saga/url.hpp>

#include <saga/saga/adaptors/task.hpp>

// needs tasks for async ops
#include <saga/impl/call.hpp>
#include <saga/impl/engine/task.hpp>
#include <saga/impl/engine/sync_async.hpp>

// own header file
#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/packages/namespace_entry_cpi_instance_data.hpp>

#include <saga/impl/packages/namespace/namespace_entry.hpp>

#ifdef SAGA_DEBUG
#include <saga/impl/packages/namespace/preprocessed/namespace_entry.cpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: "preprocessed/namespace_entry.cpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga 
{ 
  namespace impl 
  {
    namespace_entry::namespace_entry (saga::session const & s, 
            saga::url ns_url, int mode, saga::object::type t) 
      : proxy (t, s)
      , need_to_free_instance_data (false)
    {
        // initialize the instance data
        typedef adaptors::v1_0::namespace_entry_cpi_instance_data 
            instance_data_type;
        typedef adaptors::instance_data <instance_data_type>       
            namespace_entry_instance_data;

        // verify mode
        int all_valid_flags = name_space::Overwrite | name_space::Recursive | 
            name_space::Dereference | name_space::Create | name_space::Exclusive | 
            name_space::Lock | name_space::CreateParents | name_space::Read | 
            name_space::Write | name_space::ReadWrite;

        if (mode & ~all_valid_flags)
        {
            SAGA_OSSTREAM strm;
            strm << "Unknown 'mode' used: " << std::hex << "0x" << mode;
            SAGA_THROW_NO_OBJECT(SAGA_OSSTREAM_GETSTRING(strm), saga::BadParameter);
        }

        // complement mode, if appropriate
        if (mode & saga::name_space::CreateParents)
            mode |= saga::name_space::Create;
            
        if (mode & saga::name_space::Create)
            mode |= saga::name_space::Write;     // needs to be writable at least

        // make sure url is valid 
        {
            namespace_entry_instance_data data;
            data.init_data (this, TR1::shared_ptr <instance_data_type> 
                            (new instance_data_type (ns_url.get_url(), mode)));
        } // lock goes out of scope

        need_to_free_instance_data = true;
    }
    
    void namespace_entry::init()
    {
        try {
            // initialize a CPI instance and execute required functions
            this->initcpi("namespace_entry_cpi");
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
    
    namespace_entry::namespace_entry (saga::object::type t) 
      : proxy (t, saga::detail::get_the_session()), 
        need_to_free_instance_data(false)
    {
        // no need to init instance data here, this is done in the derived class
    }

    namespace_entry::namespace_entry (saga::session const & s, saga::object::type t) 
    :   proxy (t, s), need_to_free_instance_data(false)
    {
        // no need to init instance data here, this is done in the derived class
    }

    namespace_entry::~namespace_entry (void) 
    {
        // release all attached adaptors now
        saga::impl::proxy::release_cpis();

        if (need_to_free_instance_data) {
            // free the instance data
            typedef adaptors::v1_0::namespace_entry_cpi_instance_data instance_data_type;
            typedef adaptors::instance_data <instance_data_type> namespace_entry_instance_data;

            namespace_entry_instance_data data;
            data.release_data (this);
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    // factories
    void namespace_entry::create_impl_sync(name_space::entry& retval)
    {
        this->init();
        retval = name_space::entry(this);
    }

    saga::task namespace_entry::create_impl_async(saga::session const& s, 
        saga::url const& url, int mode)
    {
        TR1::shared_ptr<namespace_entry> impl(new namespace_entry(s, url, mode));

        bool (namespace_entry::*prep)(name_space::entry&, saga::uuid) = NULL;
        return saga::adaptors::task("__init__", TR1::shared_ptr<v1_0::cpi>(), 
            impl.get(), &namespace_entry::create_impl_sync, prep);
    }

    // basic properties
    SAGA_CALL_IMPL_IMPL_0(namespace_entry, namespace_entry_cpi, get_url)
    SAGA_CALL_IMPL_IMPL_0(namespace_entry, namespace_entry_cpi, get_cwd)
    SAGA_CALL_IMPL_IMPL_0(namespace_entry, namespace_entry_cpi, get_name)
  
    // navigation/query methods
    SAGA_CALL_IMPL_IMPL_0(namespace_entry, namespace_entry_cpi, read_link)
    SAGA_CALL_IMPL_IMPL_0(namespace_entry, namespace_entry_cpi, is_dir)
    SAGA_CALL_IMPL_IMPL_0(namespace_entry, namespace_entry_cpi, is_entry)
    SAGA_CALL_IMPL_IMPL_0(namespace_entry, namespace_entry_cpi, is_link)
  
    // entry management methods
    SAGA_CALL_IMPL_IMPL_2(namespace_entry, namespace_entry_cpi, copy, saga::url, int)
    SAGA_CALL_IMPL_IMPL_2(namespace_entry, namespace_entry_cpi, link, saga::url, int)
    SAGA_CALL_IMPL_IMPL_2(namespace_entry, namespace_entry_cpi, move, saga::url, int)
    SAGA_CALL_IMPL_IMPL_1(namespace_entry, namespace_entry_cpi, remove, int)
    SAGA_CALL_IMPL_IMPL_1(namespace_entry, namespace_entry_cpi, close, double)
  
    // permissions with flags
    SAGA_CALL_IMPL_IMPL_3(namespace_entry, namespace_entry_cpi, permissions_allow, std::string, int, int)
    SAGA_CALL_IMPL_IMPL_3(namespace_entry, namespace_entry_cpi, permissions_deny, std::string, int, int)

    // Generate a exact deep copy of this object
    saga::object namespace_entry::clone() const
    {
        saga::url location;
        int mode = 0;

        {
            typedef adaptors::v1_0::namespace_entry_cpi_instance_data 
                instance_data_type;
            typedef adaptors::instance_data <instance_data_type>
                namespace_entry_instance_data;

            namespace_entry_instance_data data(const_cast<namespace_entry*>(this));
            location = data->location_;
            mode = data->mode_;
        }

        // saga::session const & s, saga::url loc, int mode, saga::object::type t
        return saga::name_space::entry(get_session(), location, mode, get_type());
    }

///////////////////////////////////////////////////////////////////////////////
}} // namespace saga::impl

#endif // SAGA_DEBUG

