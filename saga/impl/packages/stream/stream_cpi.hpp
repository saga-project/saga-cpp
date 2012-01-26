#ifndef SAGA_IMPL_PACKAGES_COMM_STREAM_STREAM_CPI_HPP
#define SAGA_IMPL_PACKAGES_COMM_STREAM_STREAM_CPI_HPP

//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Use, modification and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <vector>

#include <saga/saga/types.hpp>
#include <saga/saga/exception.hpp>

#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/packages/stream_cpi_instance_data.hpp>
#include <saga/saga/adaptors/attribute_cpi.hpp>
#include <saga/saga/adaptors/attribute_cache_cpi.hpp>

#include <saga/impl/call.hpp>
#include <saga/impl/config.hpp>
#include <saga/impl/engine/register_members.hpp>

////////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl { namespace v1_0 {

    ///////////////////////////////////////////////////////////////////////////
    class stream_cpi : public attribute_cpi
    {
      public:
        typedef saga::stream::state state; 

        stream_cpi (proxy * p, cpi_info const& info, 
                TR1::shared_ptr<saga::adaptor> adaptor, cpi::flags flags)
          :  attribute_cpi (p, info, adaptor, flags)
        {
        }

        virtual ~stream_cpi (void) 
        {
        }

        cpi::type get_type() const { return cpi::Stream; }

        SAGA_CALL_CPI_DECL_VIRT_0(stream_cpi, saga::url,   get_url)
        SAGA_CALL_CPI_DECL_VIRT_0(stream_cpi, saga::context, get_context)

        SAGA_CALL_CPI_DECL_VIRT_1(stream_cpi, saga::impl::void_t, connect, double)
        SAGA_CALL_CPI_DECL_VIRT_2(stream_cpi, saga::stream::activity, wait, saga::stream::activity, double)
        SAGA_CALL_CPI_DECL_VIRT_1(stream_cpi, saga::impl::void_t,  close, double)
        SAGA_CALL_CPI_DECL_VIRT_2(stream_cpi, saga::ssize_t, read, saga::mutable_buffer, saga::ssize_t)
        SAGA_CALL_CPI_DECL_VIRT_2(stream_cpi, saga::ssize_t, write, saga::const_buffer, saga::ssize_t)

    }; // class stream_cpi

////////////////////////////////////////////////////////////////////////////////
}}} // namespace saga::impl::v1_0

////////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors { namespace v1_0 {

///////////////////////////////////////////////////////////////////////////////
// register stream CPI functions
    template <typename Derived>
    inline bool
    register_stream_functions(
        std::vector<saga::impl::v1_0::cpi_info>& infos, 
        saga::impl::v1_0::cpi::maker_type maker, 
        saga::impl::v1_0::preference_type const& prefs, 
        saga::uuid const& cpi_uuid, saga::uuid const& adaptor_uuid, 
        std::string const& cpi_name)
    {
        bool retval = false;
        saga::impl::v1_0::cpi_info info(saga::adaptors::stream_cpi, 
            cpi_name, maker, prefs, cpi_uuid, adaptor_uuid);
          
        SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
            << "begin register_stream_functions (" << cpi_name << "): " 
            << adaptor_uuid.string() << ":";

        typedef saga::impl::v1_0::stream_cpi base_cpi;
        
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, get_url, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, get_context, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, connect, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, wait, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, read, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, write, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, close, prefs);
      
        infos.push_back(info);

        SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
            << "end register_stream_functions";

        return retval;    // is true if at least one function got registered
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Derived, typename Mutex = boost::recursive_mutex>
    class stream_cpi 
    :   public attribute_cache_cpi<Derived, saga::impl::v1_0::stream_cpi>
    {
    protected:
        typedef attribute_cache_cpi<Derived, saga::impl::v1_0::stream_cpi>
            base_type;
        typedef saga::impl::proxy proxy;
        typedef saga::impl::v1_0::cpi cpi;
        typedef saga::impl::v1_0::cpi_info cpi_info;
        typedef saga::impl::v1_0::preference_type preference_type;
        typedef Mutex mutex_type;

        mutex_type mtx_;     // secure access to cpi_instance_data

    public:
        typedef saga::stream::state state; 

        stream_cpi (proxy * p, cpi_info const& info, 
                TR1::shared_ptr<saga::adaptor> adaptor, cpi::flags flags)
          :  base_type(p, info, adaptor, flags)
        {
        }

        virtual ~stream_cpi (void) 
        {
        }

        ///////////////////////////////////////////////////////////////////////
        //  instance data
        //    data associated with the API object instance, i.e. visible to all 
        //    CPI object instances associated with the API object instance 
        //    regardless of the shared library (adaptor) they are implemented in.
        typedef saga::adaptors::v1_0::stream_cpi_instance_data 
            stream_cpi_instance_data;
        friend class saga::adaptors::instance_data<stream_cpi_instance_data>;
        typedef      saga::adaptors::instance_data<stream_cpi_instance_data> 
            instance_data_type;

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
            // register stream CPI functions
            saga::uuid cpi_uuid;
            register_stream_functions<Derived>(infos,
                &saga::adaptors::v1_0::stream_cpi<Derived>::cpi_maker, 
                prefs, cpi_uuid, adaptor_uuid, saga::adaptors::stream_cpi);

            // register attribute CPI functions
            register_attribute_functions<Derived>(infos,
                &saga::adaptors::v1_0::stream_cpi<Derived>::cpi_maker, 
                prefs, cpi_uuid, adaptor_uuid, saga::adaptors::stream_cpi);
        }

        ///////////////////////////////////////////////////////////////////////
        // generic up-cast of the impl pointer       
        TR1::shared_ptr<Derived> shared_from_this()
        {
            return TR1::static_pointer_cast<Derived>(
                this->base_type::shared_from_this());
        }

    }; // class stream_cpi

////////////////////////////////////////////////////////////////////////////////
}}} // namespace saga::adaptors::v1_0

#endif // SAGA_IMPL_PACKAGES_COMM_STREAM_STREAM_CPI_HPP

