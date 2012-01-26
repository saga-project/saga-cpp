#ifndef SAGA_IMPL_PACKAGES_DATA_FILE_FILE_CPI_HPP
#define SAGA_IMPL_PACKAGES_DATA_FILE_FILE_CPI_HPP

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/file_cpi.hpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005 Michel Zandstra [michelzandstra@gmail.com]
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif 

#include <saga/saga/util.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/exception.hpp>
#include <saga/saga/filesystem.hpp>

#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/packages/file_cpi_instance_data.hpp>

#include <saga/impl/call.hpp>
#include <saga/impl/config.hpp>
#include <saga/impl/engine/cpi.hpp>
#include <saga/impl/engine/register_members.hpp>
#include <saga/impl/packages/namespace/namespace_entry_cpi.hpp>

#ifdef SAGA_DEBUG
#include <saga/impl/packages/namespace/preprocessed/file_cpi.hpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: "preprocessed/file_cpi.hpp")
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl { namespace v1_0 {

    /**
     * This class declares all functions of "file" defined in the SAGA API.
     * It extends the base class saga::cpi
     *
     * @see For more information, read the GGF SAGA API Specification
     */
    class file_cpi 
        : public namespace_entry_cpi
    {
    public:
       
        file_cpi (proxy * p, cpi_info const& info, 
                TR1::shared_ptr<saga::adaptor> adaptor, cpi::flags flags) 
            : namespace_entry_cpi (p, info, adaptor, flags) 
        { 
        }
         
        virtual ~file_cpi (void) 
        { 
        }

        cpi::type get_type() const { return cpi::File; }

        SAGA_CALL_CPI_DECL_VIRT_0 (file_cpi, saga::off_t, get_size)
        SAGA_CALL_CPI_DECL_VIRT_2 (file_cpi, saga::ssize_t, read, saga::mutable_buffer, saga::ssize_t)
        SAGA_CALL_CPI_DECL_VIRT_2 (file_cpi, saga::ssize_t, write, saga::const_buffer, saga::ssize_t)
        SAGA_CALL_CPI_DECL_VIRT_2 (file_cpi, saga::off_t, seek, saga::off_t, saga::filesystem::seek_mode)

        // scattered I/O
        SAGA_CALL_CPI_DECL_VIRT_1 (file_cpi, saga::impl::void_t, read_v, std::vector<saga::filesystem::iovec>)
        SAGA_CALL_CPI_DECL_VIRT_1 (file_cpi, saga::impl::void_t, write_v, std::vector<saga::filesystem::const_iovec>)

        // pattern based I/O
        SAGA_CALL_CPI_DECL_VIRT_1 (file_cpi, saga::ssize_t, size_p, std::string)
        SAGA_CALL_CPI_DECL_VIRT_2 (file_cpi, saga::ssize_t, read_p, std::string, saga::mutable_buffer)
        SAGA_CALL_CPI_DECL_VIRT_2 (file_cpi, saga::ssize_t, write_p, std::string, saga::const_buffer)

        // extended based I/O
        SAGA_CALL_CPI_DECL_VIRT_0 (file_cpi, std::vector<std::string>, modes_e)
        SAGA_CALL_CPI_DECL_VIRT_2 (file_cpi, saga::ssize_t, size_e, std::string, std::string)
        SAGA_CALL_CPI_DECL_VIRT_3 (file_cpi, saga::ssize_t, read_e, std::string, std::string, saga::mutable_buffer)
        SAGA_CALL_CPI_DECL_VIRT_3 (file_cpi, saga::ssize_t, write_e, std::string, std::string, saga::const_buffer)

    }; // class file_cpi

///////////////////////////////////////////////////////////////////////////////
}}} // namespace saga::impl::v1_0

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors { namespace v1_0 {

///////////////////////////////////////////////////////////////////////////////
// register file CPI functions
    template <typename Derived>
    inline bool
    register_file_functions(
        std::vector<saga::impl::v1_0::cpi_info>& infos, 
        saga::impl::v1_0::cpi::maker_type maker, 
        saga::impl::v1_0::preference_type const& prefs, 
        saga::uuid const& cpi_uuid, saga::uuid const& adaptor_uuid, 
        std::string const& cpi_name)
    {
        bool retval = false;
        saga::impl::v1_0::cpi_info info(saga::adaptors::file_cpi, cpi_name, 
            maker, prefs, cpi_uuid, adaptor_uuid);
          
        SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
            << "begin register_file_functions (" << cpi_name << "): " 
            << adaptor_uuid.string() << ":";

        typedef saga::impl::v1_0::file_cpi base_cpi;
        
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, get_size, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, read, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, write, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, seek, prefs);
      
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, read_v, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, write_v, prefs);

        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, size_p, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, read_p, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, write_p, prefs);

        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, modes_e, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, size_e, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, read_e, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, write_e, prefs);

        infos.push_back(info);

        SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
            << "end register_file_functions";

        return retval;    // is true if at least one function got registered
    }
    
    ///////////////////////////////////////////////////////////////////////////
    template <typename Derived, typename Mutex = boost::recursive_mutex>
    class file_cpi
        : public saga::impl::v1_0::file_cpi
    {
    protected:
        typedef saga::impl::v1_0::file_cpi base_type;
        typedef saga::impl::proxy proxy;
        typedef saga::impl::v1_0::cpi cpi;
        typedef saga::impl::v1_0::cpi_info cpi_info;
        typedef saga::impl::v1_0::preference_type preference_type;
        typedef Mutex mutex_type;

        mutex_type mtx_;     // secure access to cpi_instance_data

    public:
        file_cpi (proxy * p, cpi_info const& info, 
                TR1::shared_ptr<saga::adaptor> adaptor, cpi::flags flags) 
            : base_type(p, info, adaptor, flags) 
        { 
        }
         
        virtual ~file_cpi (void) 
        { 
        }

        ///////////////////////////////////////////////////////////////////////
        //  instance data
        //    data associated with the API object instance, i.e. visible to all 
        //    CPI object instances associated with the API object instance 
        //    regardless of the shared library (adaptor) they are implemented in.
        typedef saga::adaptors::v1_0::file_cpi_instance_data file_instance_data;
        friend class saga::adaptors::instance_data<file_instance_data>;
        typedef      saga::adaptors::instance_data<file_instance_data> 
            instance_data;

        ///////////////////////////////////////////////////////////////////////
        // generic factory function
        static cpi* cpi_maker (proxy                * p, 
                               cpi_info       const & info,
                               saga::ini::ini const & glob_ini, 
                               saga::ini::ini const & adap_ini,
                               TR1::shared_ptr<saga::adaptor> adaptor)
        {
            return new Derived (p, info, glob_ini, adap_ini, adaptor);
        }

        ///////////////////////////////////////////////////////////////////////
        // register implemented functions
        static void 
        register_cpi(std::vector<cpi_info>& infos, preference_type prefs, 
            saga::uuid adaptor_uuid)
        {
            // register namespace_entry CPI functions
            saga::uuid cpi_uuid;
            register_namespace_entry_functions<Derived>(infos, 
                &saga::adaptors::v1_0::file_cpi<Derived>::cpi_maker, 
                prefs, cpi_uuid, adaptor_uuid, saga::adaptors::file_cpi);

            // register file CPI functions
            register_file_functions<Derived>(infos,
                &saga::adaptors::v1_0::file_cpi<Derived>::cpi_maker, 
                prefs, cpi_uuid, adaptor_uuid, saga::adaptors::file_cpi);

            // register attribute CPI functions
            register_attribute_functions<Derived>(infos, 
                &saga::adaptors::v1_0::namespace_entry_cpi<Derived>::cpi_maker, 
                prefs, cpi_uuid, adaptor_uuid, saga::adaptors::file_cpi);

            // register permissions CPI functions
            register_permissions_functions<Derived>(infos, 
                &saga::adaptors::v1_0::permissions_cpi<Derived>::cpi_maker, 
                prefs, cpi_uuid, adaptor_uuid, saga::adaptors::file_cpi);
        }

        ///////////////////////////////////////////////////////////////////////
        // generic up-cast of the impl pointer       
        TR1::shared_ptr<Derived> shared_from_this()
        {
            return TR1::static_pointer_cast<Derived>(
                this->base_type::shared_from_this());
        }

    }; // class file_cpi
    
///////////////////////////////////////////////////////////////////////////////
}}} // namespace saga::adaptors::v1_0

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif // !defined(SAGA_DEBUG)

#endif // SAGA_IMPL_PACKAGES_DATA_FILE_FILE_CPI_HPP

