//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_CONTEXT_CPI_HPP
#define SAGA_IMPL_CONTEXT_CPI_HPP

#include <string>

#include <saga/saga/util.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/exception.hpp>

#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/context_cpi_instance_data.hpp>
#include <saga/saga/adaptors/attribute_cache_cpi.hpp>

#include <saga/impl/call.hpp>
#include <saga/impl/config.hpp>
#include <saga/impl/engine/register_members.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl { namespace v1_0 {

    ///////////////////////////////////////////////////////////////////////////
    class context_cpi 
        : public attribute_cpi
    {
    public:
      context_cpi (proxy * p, cpi_info const& info, 
            TR1::shared_ptr <saga::adaptor> adaptor, cpi::flags flags) 
        : attribute_cpi (p, info, adaptor, flags) 
      {
      }
       
      ~context_cpi (void) 
      { 
      }

      cpi::type get_type() const { return cpi::Context; }

      SAGA_CALL_CPI_DECL_VIRT_0(context_cpi, saga::impl::void_t, set_defaults)

    }; // class context_cpi

///////////////////////////////////////////////////////////////////////////////
}}} // namespace saga::impl::v1_0

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors { namespace v1_0 {

///////////////////////////////////////////////////////////////////////////////
//  register namespace_entry CPI functions
    template <typename Derived>
    inline bool
    register_context_functions(
        std::vector<saga::impl::v1_0::cpi_info>& infos, 
        saga::impl::v1_0::cpi::maker_type maker, 
        saga::impl::v1_0::preference_type const& prefs, 
        saga::uuid const& cpi_uuid, saga::uuid const& adaptor_uuid, 
        std::string const& cpi_name)
    {
        bool retval = false;
        saga::impl::v1_0::cpi_info info(adaptors::context_cpi, 
            cpi_name, maker, prefs, cpi_uuid, adaptor_uuid);

        SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
            << "begin register_context_functions (" << cpi_name << "): "   
            << adaptor_uuid.string() << ":";
        
        typedef saga::impl::v1_0::context_cpi base_cpi;
        
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, set_defaults, prefs);

        infos.push_back(info);

        SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
            << "end register_context_functions";
        
        return retval;    // is true if at least one function got registered
    }
    
    ///////////////////////////////////////////////////////////////////////////
    template <typename Derived, typename Mutex = boost::recursive_mutex>
    class context_cpi 
        : public attribute_cache_cpi<Derived, saga::impl::v1_0::context_cpi>
    {
    protected:
        typedef attribute_cache_cpi<Derived, saga::impl::v1_0::context_cpi> 
            base_type;
        typedef saga::impl::proxy proxy;
        typedef saga::impl::v1_0::cpi cpi;
        typedef saga::impl::v1_0::cpi_info cpi_info;
        typedef saga::impl::v1_0::preference_type preference_type;
        typedef Mutex mutex_type;

        mutex_type mtx_;     // secure access to cpi_instance_data

    public:
        context_cpi (proxy* p, cpi_info const& info, 
                TR1::shared_ptr <saga::adaptor> adaptor, cpi::flags flags) 
            : base_type(p, info, adaptor, flags) 
        {
        }
        
        ~context_cpi (void) 
        { 
        }

        ///////////////////////////////////////////////////////////////////////
        //  instance data
        //    data associated with the API object instance, i.e. visible to all 
        //    CPI object instances associated with the API object instance 
        //    regardless of the shared library (adaptor) they are implemented in.
        typedef saga::adaptors::v1_0::context_cpi_instance_data 
            ctx_instance_data;
        friend class saga::adaptors::instance_data<ctx_instance_data>;
        typedef      saga::adaptors::instance_data<ctx_instance_data> 
            instance_data;

        ///////////////////////////////////////////////////////////////////////
        // generic factory function
        static cpi* cpi_maker (proxy* p, cpi_info const& info,
            saga::ini::ini const& glob_ini, saga::ini::ini const& adap_ini,
            TR1::shared_ptr<saga::adaptor> adaptor)
        {
            return new Derived (p, info, glob_ini, adap_ini, adaptor);
        }

        ///////////////////////////////////////////////////////////////////////
        // register implemented functions
        static void 
        register_cpi(std::vector<cpi_info>& infos, preference_type prefs, 
            saga::uuid adaptor_uuid, 
            std::string const& cpi_name = saga::adaptors::context_cpi)
        {
            // register namespace_entry CPI functions
            saga::uuid cpi_uuid;
            register_context_functions<Derived>(infos, 
                &saga::adaptors::v1_0::context_cpi<Derived>::cpi_maker, 
                prefs, cpi_uuid, adaptor_uuid, cpi_name);

            // register attribute CPI functions
            register_attribute_functions<Derived>(infos, 
                &saga::adaptors::v1_0::context_cpi<Derived>::cpi_maker, 
                prefs, cpi_uuid, adaptor_uuid, cpi_name);
        }

        ///////////////////////////////////////////////////////////////////////
        // generic up-cast of the impl pointer       
        TR1::shared_ptr<Derived> shared_from_this()
        {
            return TR1::static_pointer_cast<Derived>(
                this->base_type::shared_from_this());
        }
    }; 

///////////////////////////////////////////////////////////////////////////////
}}} // namespace saga::adaptors::v1_0

#endif 

