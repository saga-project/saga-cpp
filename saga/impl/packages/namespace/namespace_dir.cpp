#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/namespace_dir.cpp")
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
#include <saga/saga/session.hpp>

#include <saga/saga/adaptors/task.hpp>

// needs tasks for async ops
#include <saga/impl/call.hpp>
#include <saga/impl/engine/task.hpp>
#include <saga/impl/engine/proxy.hpp>
#include <saga/impl/engine/sync_async.hpp>

// own header file
#include <saga/impl/packages/namespace/namespace_dir.hpp>
#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/packages/namespace_dir_cpi_instance_data.hpp>

#ifdef SAGA_DEBUG
#include <saga/impl/packages/namespace/preprocessed/namespace.cpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: "preprocessed/namespace_dir.cpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga 
{ 
  namespace impl 
  {
    namespace_dir::namespace_dir (saga::session const& s, 
          saga::url dir_url, int mode, saga::object::type t) 
      : namespace_entry (s, t), need_to_free_instance_data(false)
    {
        // initialize the instance data
        typedef adaptors::v1_0::namespace_dir_cpi_instance_data instance_data_type;
        typedef adaptors::instance_data <instance_data_type> namespace_dir_instance_data;

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
            mode |= saga::name_space::Write;      // needs to be writable at least

        // make sure, url is valid and directory path ends with a '/'
        std::string pathstr (dir_url.get_path());
        std::string::size_type pos = pathstr.find_last_of(SAGA_PATH_DELIMITERS);
        if (pathstr.size() > 0 && pathstr.size()-1 != pos) 
        {
            pathstr += "/";
            dir_url.set_path(pathstr);
        }

        {
            namespace_dir_instance_data data;
            data.init_data (this, TR1::shared_ptr <instance_data_type> (
                new instance_data_type (dir_url, mode)));
            need_to_free_instance_data = true;
        } // lock goes out of scope

        need_to_free_instance_data = true;
    }
    
    void namespace_dir::init()
    {
        try {
            // initialize a CPI instance and execute required functions
            this->initcpi("namespace_dir_cpi");
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
               throw e;
            }
        }
    }

    namespace_dir::namespace_dir (saga::session const& s, saga::object::type t) 
        : namespace_entry (s, t), need_to_free_instance_data(false)
    {
    }

    namespace_dir::namespace_dir (saga::object::type t) 
        : namespace_entry (t), need_to_free_instance_data(false)
    {
    }

    namespace_dir::~namespace_dir (void) 
    {
      // release all attached adaptors now
      saga::impl::proxy::release_cpis();

      // release all attached adaptors now
      saga::impl::proxy::release_cpis();
        if (need_to_free_instance_data) {
            // free the instance data
            typedef adaptors::v1_0::namespace_dir_cpi_instance_data instance_data_type;
            typedef adaptors::instance_data <instance_data_type> namespace_dir_instance_data;

            namespace_dir_instance_data data;
            data.release_data (this);
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    // factories
    void namespace_dir::create_impl_sync(name_space::directory& retval)
    {
        this->init();
        retval = name_space::directory(this);
    }

    saga::task namespace_dir::create_impl_async(saga::session const& s, 
        saga::url url, int mode)
    {
        TR1::shared_ptr<namespace_dir> impl(new namespace_dir(s, url, mode));

        bool (namespace_dir::*prep)(name_space::directory&, saga::uuid) = NULL;
        return saga::adaptors::task("__init__", TR1::shared_ptr<v1_0::cpi>(), 
            impl.get(), &namespace_dir::create_impl_sync, prep);
    }

    ///////////////////////////////////////////////////////////////////////////
    // navigation/query methods
    SAGA_CALL_IMPL_IMPL_1(namespace_dir, namespace_dir_cpi, change_dir, saga::url)
    SAGA_CALL_IMPL_IMPL_2(namespace_dir, namespace_dir_cpi, list, std::string, int)
    SAGA_CALL_IMPL_IMPL_2(namespace_dir, namespace_dir_cpi, find, std::string, int)
  
    SAGA_CALL_IMPL_IMPL_1(namespace_dir, namespace_dir_cpi, read_link, saga::url)
    SAGA_CALL_IMPL_IMPL_1(namespace_dir, namespace_dir_cpi, exists, saga::url)
    SAGA_CALL_IMPL_IMPL_1(namespace_dir, namespace_dir_cpi, is_dir, saga::url)
    SAGA_CALL_IMPL_IMPL_1(namespace_dir, namespace_dir_cpi, is_entry, saga::url)
    SAGA_CALL_IMPL_IMPL_1(namespace_dir, namespace_dir_cpi, is_link, saga::url)
  
    // Deal with entries by entry number
    SAGA_CALL_IMPL_IMPL_0(namespace_dir, namespace_dir_cpi, get_num_entries)
    SAGA_CALL_IMPL_IMPL_1(namespace_dir, namespace_dir_cpi, get_entry, std::size_t)
  
    // entry management methods
    SAGA_CALL_IMPL_IMPL_3(namespace_dir, namespace_dir_cpi, copy, saga::url, saga::url, int)
    SAGA_CALL_IMPL_IMPL_3(namespace_dir, namespace_dir_cpi, link, saga::url, saga::url, int)
    SAGA_CALL_IMPL_IMPL_3(namespace_dir, namespace_dir_cpi, move, saga::url, saga::url, int)
    SAGA_CALL_IMPL_IMPL_2(namespace_dir, namespace_dir_cpi, remove, saga::url, int)
    SAGA_CALL_IMPL_IMPL_2(namespace_dir, namespace_dir_cpi, make_dir, saga::url, int)
  
    // open methods
    SAGA_CALL_IMPL_IMPL_2(namespace_dir, namespace_dir_cpi, open, saga::url, int)
    SAGA_CALL_IMPL_IMPL_2(namespace_dir, namespace_dir_cpi, open_dir, saga::url, int)

    // entry management methods
    inline bool has_wildcard(std::string const& pattern)
    {
        int bracket_count = 0;
        int brace_count = 0;
        std::string::const_iterator end = pattern.end();
        for (std::string::const_iterator it = pattern.begin(); it != end; ++it)
        {
            char c = *it;
            switch (c) {
            case '*':
            case '?':
                return true;
                
            case '[':
                ++bracket_count;
                break;
                
            case ']':
                if (0 == --bracket_count)
                    return true;
                break;
                
            case '{':
                ++brace_count;
                break;
                
            case '}':
                if (0 == --brace_count)
                    return true;
                break;
                
            default:
                break;
            }
        }
        return false;
    }
    
//     SAGA_CALL_IMPL_IMPL_3(namespace_dir, namespace_dir_cpi, copy_wildcard, std::string, saga::url, int)
    saga::task 
    namespace_dir::copy_wildcard(std::string par1, saga::url par2, int par3, bool is_sync)
    {
        bool tried_fallback = false;
        try {
            saga::task t = saga::impl::execute_sync_async(this,
                "namespace_dir_cpi", "copy_wildcard", "namespace_dir::copy_wildcard", 
                is_sync, &namespace_dir_cpi::sync_copy_wildcard, 
                &namespace_dir_cpi::async_copy_wildcard, par1, par2, par3);

            if (saga::task_base::Failed == t.get_state() && !has_wildcard(par1)) {
                tried_fallback = true;
                t = saga::impl::execute_sync_async(this,
                    "namespace_dir_cpi", "copy", "namespace_dir::copy", 
                    is_sync, &namespace_dir_cpi::sync_copy, 
                    &namespace_dir_cpi::async_copy, saga::url(par1), par2, par3);
            }
            return t;
        }
        catch (saga::not_implemented const& /*e*/) {
            if (!tried_fallback && !has_wildcard(par1)) {
                return saga::impl::execute_sync_async(this,
                    "namespace_dir_cpi", "copy", "namespace_dir::copy", 
                    is_sync, &namespace_dir_cpi::sync_copy, 
                    &namespace_dir_cpi::async_copy, saga::url(par1), par2, par3);
            }
            throw;    // rethrow if contains wildcards
        }
    }

//    SAGA_CALL_IMPL_IMPL_3(namespace_dir, namespace_dir_cpi, link_wildcard, std::string, saga::url, int)
    saga::task 
    namespace_dir::link_wildcard(std::string par1, saga::url par2, int par3, bool is_sync)
    {
        bool tried_fallback = false;
        try {
            saga::task t = saga::impl::execute_sync_async(this,
                "namespace_dir_cpi", "link_wildcard", "namespace_dir::link_wildcard", 
                is_sync, &namespace_dir_cpi::sync_link_wildcard, 
                &namespace_dir_cpi::async_link_wildcard, par1, par2, par3);

            if (saga::task_base::Failed == t.get_state() && !has_wildcard(par1)) {
                tried_fallback = true;
                t = saga::impl::execute_sync_async(this,
                    "namespace_dir_cpi", "link", "namespace_dir::link", 
                    is_sync, &namespace_dir_cpi::sync_link, 
                    &namespace_dir_cpi::async_link, saga::url(par1), par2, par3);
            }
            return t;
        }
        catch (saga::not_implemented const& /*e*/) {
            if (!tried_fallback && !has_wildcard(par1)) {
                return saga::impl::execute_sync_async(this,
                    "namespace_dir_cpi", "link", "namespace_dir::link", 
                    is_sync, &namespace_dir_cpi::sync_link, 
                    &namespace_dir_cpi::async_link, saga::url(par1), par2, par3);
            }
            throw;    // rethrow if contains wildcards
        }
    }

//     SAGA_CALL_IMPL_IMPL_3(namespace_dir, namespace_dir_cpi, move_wildcard, std::string, saga::url, int)
    saga::task 
    namespace_dir::move_wildcard(std::string par1, saga::url par2, int par3, bool is_sync)
    {
        bool tried_fallback = false;
        try {
            saga::task t = saga::impl::execute_sync_async(this,
                "namespace_dir_cpi", "move_wildcard", "namespace_dir::move_wildcard", 
                is_sync, &namespace_dir_cpi::sync_move_wildcard, 
                &namespace_dir_cpi::async_move_wildcard, par1, par2, par3);

            if (saga::task_base::Failed == t.get_state() && !has_wildcard(par1)) {
                tried_fallback = true;
                t = saga::impl::execute_sync_async(this,
                    "namespace_dir_cpi", "move", "namespace_dir::move", 
                    is_sync, &namespace_dir_cpi::sync_move, 
                    &namespace_dir_cpi::async_move, saga::url(par1), par2, par3);
            }
            return t;
        }
        catch (saga::not_implemented const& /*e*/) {
            if (!tried_fallback && !has_wildcard(par1)) {
                return saga::impl::execute_sync_async(this,
                    "namespace_dir_cpi", "move", "namespace_dir::move", 
                    is_sync, &namespace_dir_cpi::sync_move, 
                    &namespace_dir_cpi::async_move, saga::url(par1), par2, par3);
            }
            throw;    // rethrow if contains wildcards
        }
    }

//     SAGA_CALL_IMPL_IMPL_2(namespace_dir, namespace_dir_cpi, remove_wildcard, std::string, int)
    saga::task 
    namespace_dir::remove_wildcard(std::string par1, int par2, bool is_sync)
    {
        bool tried_fallback = false;
        try {
            saga::task t = saga::impl::execute_sync_async(this,
                "namespace_dir_cpi", "remove_wildcard", "namespace_dir::remove_wildcard", 
                is_sync, &namespace_dir_cpi::sync_remove_wildcard, 
                &namespace_dir_cpi::async_remove_wildcard, par1, par2);

            if (saga::task_base::Failed == t.get_state() && !has_wildcard(par1)) {
                tried_fallback = true;
                t = saga::impl::execute_sync_async(this,
                    "namespace_dir_cpi", "remove", "namespace_dir::remove", 
                    is_sync, &namespace_dir_cpi::sync_remove, 
                    &namespace_dir_cpi::async_remove, saga::url(par1), par2);
            }
            return t;
        }
        catch (saga::not_implemented const& /*e*/) {
            if (!tried_fallback && !has_wildcard(par1)) {
                return saga::impl::execute_sync_async(this,
                    "namespace_dir_cpi", "remove", "namespace_dir::remove", 
                    is_sync, &namespace_dir_cpi::sync_remove, 
                    &namespace_dir_cpi::async_remove, saga::url(par1), par2);
            }
            throw;    // rethrow if contains wildcards
        }
    }

    // permissions with flags
    SAGA_CALL_IMPL_IMPL_4(namespace_dir, namespace_dir_cpi, permissions_allow, saga::url, std::string, int, int)
    SAGA_CALL_IMPL_IMPL_4(namespace_dir, namespace_dir_cpi, permissions_deny, saga::url, std::string, int, int)

//    SAGA_CALL_IMPL_IMPL_4(namespace_dir, namespace_dir_cpi, permissions_allow_wildcard, std::string, std::string, int, int)
    saga::task 
    namespace_dir::permissions_allow_wildcard(std::string par1, 
        std::string par2, int par3, int par4, bool is_sync)
    {
        bool tried_fallback = false;
        try {
            saga::task t = saga::impl::execute_sync_async(this,
                "namespace_dir_cpi", "permissions_allow_wildcard", 
                "namespace_dir::permissions_allow_wildcard", 
                is_sync, &namespace_dir_cpi::sync_permissions_allow_wildcard, 
                &namespace_dir_cpi::async_permissions_allow_wildcard, 
                par1, par2, par3, par4);

            if (saga::task_base::Failed == t.get_state() && !has_wildcard(par1)) {
                tried_fallback = true;
                t = saga::impl::execute_sync_async(this,
                    "namespace_dir_cpi", "permissions_allow", 
                    "namespace_dir::permissions_allow", 
                    is_sync, &namespace_dir_cpi::sync_permissions_allow, 
                    &namespace_dir_cpi::async_permissions_allow, 
                    saga::url(par1), par2, par3, par4);
            }
            return t;
        }
        catch (saga::not_implemented const& /*e*/) {
            if (!tried_fallback && !has_wildcard(par1)) {
                return saga::impl::execute_sync_async(this,
                    "namespace_dir_cpi", "permissions_allow", 
                    "namespace_dir::permissions_allow", 
                    is_sync, &namespace_dir_cpi::sync_permissions_allow, 
                    &namespace_dir_cpi::async_permissions_allow, 
                    saga::url(par1), par2, par3, par4);
            }
            throw;    // rethrow if contains wildcards
        }
    }

//     SAGA_CALL_IMPL_IMPL_4(namespace_dir, namespace_dir_cpi, permissions_deny_wildcard, std::string, std::string, int, int)
    saga::task 
    namespace_dir::permissions_deny_wildcard(std::string par1, 
        std::string par2, int par3, int par4, bool is_sync)
    {
        bool tried_fallback = false;
        try {
            saga::task t = saga::impl::execute_sync_async(this,
                "namespace_dir_cpi", "permissions_deny_wildcard", 
                "namespace_dir::permissions_deny_wildcard", 
                is_sync, &namespace_dir_cpi::sync_permissions_deny_wildcard, 
                &namespace_dir_cpi::async_permissions_deny_wildcard, 
                par1, par2, par3, par4);

            if (saga::task_base::Failed == t.get_state() && !has_wildcard(par1)) {
                tried_fallback = true;
                t = saga::impl::execute_sync_async(this,
                    "namespace_dir_cpi", "permissions_deny", 
                    "namespace_dir::permissions_deny", 
                    is_sync, &namespace_dir_cpi::sync_permissions_deny, 
                    &namespace_dir_cpi::async_permissions_deny, 
                    saga::url(par1), par2, par3, par4);
            }
            return t;
        }
        catch (saga::not_implemented const& /*e*/) {
            if (!tried_fallback && !has_wildcard(par1)) {
                return saga::impl::execute_sync_async(this,
                    "namespace_dir_cpi", "permissions_deny", 
                    "namespace_dir::permissions_deny", 
                    is_sync, &namespace_dir_cpi::sync_permissions_deny, 
                    &namespace_dir_cpi::async_permissions_deny, 
                    saga::url(par1), par2, par3, par4);
            }
            throw;    // rethrow if contains wildcards
        }
    }

    // Generate a exact deep copy of this object
    saga::object namespace_dir::clone() const
    {
        saga::url location;
        int mode = 0;

        {
            typedef adaptors::v1_0::namespace_dir_cpi_instance_data instance_data_type;
            typedef adaptors::instance_data <instance_data_type> namespace_dir_instance_data;

            namespace_dir_instance_data data(const_cast<namespace_dir*>(this));
            location = data->location_;
            mode = data->mode_;
        }

        // saga::session const & s, saga::url loc, int mode, saga::object::type t
        return saga::name_space::directory(get_session(), location, mode, get_type());
    }

///////////////////////////////////////////////////////////////////////////////
}} // namespace saga::impl

#endif

