//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_ENGINE_ATTRIBUTE_CPI_HPP
#define SAGA_IMPL_ENGINE_ATTRIBUTE_CPI_HPP

#include <string>

#include <saga/saga/util.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/exception.hpp>

#include <saga/impl/call.hpp>
#include <saga/impl/config.hpp>
#include <saga/impl/engine/cpi.hpp>
#include <saga/impl/engine/register_members.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl { namespace v1_0 {

    ///////////////////////////////////////////////////////////////////////////
    class attribute_cpi 
        : public cpi
    {
    public:
        typedef std::vector <std::string> strvec_type;

        attribute_cpi(proxy* p, cpi_info const& info, 
                TR1::shared_ptr<saga::adaptor> adaptor, cpi::flags flags) 
            : cpi (p, info, adaptor, flags) 
        { 
        }
         
        ~attribute_cpi(void) 
        {
        }

        SAGA_CALL_CPI_DECL_VIRT_1(attribute_cpi, std::string, get_attribute, std::string)
        SAGA_CALL_CPI_DECL_VIRT_2(attribute_cpi, saga::impl::void_t, set_attribute, std::string, std::string)
        SAGA_CALL_CPI_DECL_VIRT_1(attribute_cpi, strvec_type, get_vector_attribute, std::string)
        SAGA_CALL_CPI_DECL_VIRT_2(attribute_cpi, saga::impl::void_t, set_vector_attribute, std::string, strvec_type)
        SAGA_CALL_CPI_DECL_VIRT_1(attribute_cpi, saga::impl::void_t, remove_attribute, std::string)

        // inspection
        SAGA_CALL_CPI_DECL_VIRT_0(attribute_cpi, strvec_type, list_attributes)
        SAGA_CALL_CPI_DECL_VIRT_1(attribute_cpi, strvec_type, find_attributes, std::string)

        SAGA_CALL_CPI_DECL_VIRT_1(attribute_cpi, bool, attribute_exists, std::string)
        SAGA_CALL_CPI_DECL_VIRT_1(attribute_cpi, bool, attribute_is_readonly, std::string)
        SAGA_CALL_CPI_DECL_VIRT_1(attribute_cpi, bool, attribute_is_writable, std::string)
        SAGA_CALL_CPI_DECL_VIRT_1(attribute_cpi, bool, attribute_is_vector, std::string)
        SAGA_CALL_CPI_DECL_VIRT_1(attribute_cpi, bool, attribute_is_extended, std::string)

    }; // class attribute_cpi

///////////////////////////////////////////////////////////////////////////////
}}} // namespace saga::impl::v1_0

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors { namespace v1_0 {

///////////////////////////////////////////////////////////////////////////////
// register attribute CPI functions
    template <typename Derived>
    inline bool
    register_attribute_functions(
        std::vector<saga::impl::v1_0::cpi_info>& infos, 
        saga::impl::v1_0::cpi::maker_type maker, 
        saga::impl::v1_0::preference_type const& prefs, 
        saga::uuid const& cpi_uuid, saga::uuid const& adaptor_uuid, 
        std::string const& cpi_name)
    {
        bool retval = false;
        saga::impl::v1_0::cpi_info info(saga::adaptors::attribute_cpi,
            cpi_name, maker, prefs, cpi_uuid, adaptor_uuid);
          
        SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
            << "begin register_attribute_functions: " 
            << adaptor_uuid.string() << ":";

        typedef saga::impl::v1_0::attribute_cpi base_cpi;
        
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, get_attribute, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, set_attribute, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, get_vector_attribute, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, set_vector_attribute, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, remove_attribute, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, list_attributes, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, find_attributes, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, attribute_exists, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, attribute_is_readonly, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, attribute_is_writable, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, attribute_is_vector, prefs);
        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, attribute_is_extended, prefs);
      
        infos.push_back(info);

        SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
            "end register_attribute_functions";

        return retval;    // is true if at least one function got registered
    }

    /////////////////////////////////////////////////////////////////////////////
    template <typename Derived, typename Mutex = boost::recursive_mutex>
    class attribute_cpi 
        : public saga::impl::v1_0::attribute_cpi
    {
    protected:
        typedef saga::impl::v1_0::attribute_cpi base_type;
        typedef saga::impl::proxy proxy;
        typedef saga::impl::v1_0::cpi cpi;
        typedef saga::impl::v1_0::cpi_info cpi_info;
        typedef saga::impl::v1_0::preference_type preference_type;
        typedef Mutex mutex_type;
        
    public:
        attribute_cpi (proxy* p, cpi_info const& info, 
              TR1::shared_ptr<saga::adaptor> adaptor, cpi::flags flags) 
            : saga::impl::v1_0::attribute_cpi(p, info, adaptor, flags) 
        { 
        }
         
        ~attribute_cpi (void) 
        {
        }

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
            saga::uuid adaptor_uuid)
        {
            // register attribute CPI functions
            saga::uuid cpi_uuid;
            register_attribute_functions<Derived>(infos, 
                &saga::adaptors::v1_0::attribute_cpi<Derived>::cpi_maker, 
                prefs, cpi_uuid, adaptor_uuid, saga::adaptors::attribute_cpi);
        }

        ///////////////////////////////////////////////////////////////////////
        // generic up-cast of the impl pointer       
        TR1::shared_ptr<Derived> shared_from_this()
        {
            return TR1::static_pointer_cast<Derived>(
                this->base_type::shared_from_this());
        }

    }; // class attribute_cpi

///////////////////////////////////////////////////////////////////////////////
}}} // namespace saga::adaptors::v1_0

#endif // SAGA_IMPL_ENGINE_ATTRIBUTE_CPI_HPP

