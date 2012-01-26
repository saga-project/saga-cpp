//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SAGA_IMPL_URL_CPI_FEB_10_2009_1203PM)
#define SAGA_IMPL_URL_CPI_FEB_10_2009_1203PM

#include <string>

#include <saga/saga/util.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/exception.hpp>

#include <saga/saga/adaptors/instance_data.hpp>
#include <saga/saga/adaptors/url_cpi_instance_data.hpp>

#include <saga/impl/call.hpp>
#include <saga/impl/config.hpp>
#include <saga/impl/engine/register_members.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl { namespace v1_0 
{
    ///////////////////////////////////////////////////////////////////////////
    class url_cpi : public cpi
    {
    public:
        url_cpi (proxy * p, cpi_info const& info, 
                TR1::shared_ptr<saga::adaptor> adaptor, cpi::flags flags) 
          : cpi(p, info, adaptor, flags) 
        {}

        ~url_cpi() 
        {}

        cpi::type get_type() const { return cpi::Url; }

        SAGA_CALL_CPI_DECL_VIRT_1(url_cpi, saga::url, translate, std::string)
    }; 

///////////////////////////////////////////////////////////////////////////////
}}} // namespace saga::impl::v1_0

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors { namespace v1_0 
{
    ///////////////////////////////////////////////////////////////////////////
    //  register namespace_entry CPI functions
    template <typename Derived>
    inline bool
    register_url_functions(
        std::vector<saga::impl::v1_0::cpi_info>& infos, 
        saga::impl::v1_0::cpi::maker_type maker, 
        saga::impl::v1_0::preference_type const& prefs, 
        saga::uuid const& cpi_uuid, saga::uuid const& adaptor_uuid, 
        std::string const& cpi_name)
    {
        bool retval = false;
        saga::impl::v1_0::cpi_info info(adaptors::url_cpi, 
            cpi_name, maker, prefs, cpi_uuid, adaptor_uuid);

        SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
            << "begin register_url_functions (" << cpi_name << "): "   
            << adaptor_uuid.string() << ":";

        typedef saga::impl::v1_0::url_cpi base_cpi;

        SAGA_REGISTER_MEMBER(retval, info, base_cpi, Derived, translate, prefs);

        infos.push_back(info);

        SAGA_LOG(SAGA_VERBOSE_LEVEL_BLURB)
            << "end register_url_functions";

        return retval;    // is true if at least one function got registered
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Derived, typename Mutex = boost::recursive_mutex>
    class url_cpi : public saga::impl::v1_0::url_cpi
    {
    protected:
        typedef saga::impl::v1_0::url_cpi base_type;
        typedef saga::impl::proxy proxy;
        typedef saga::impl::v1_0::cpi cpi;
        typedef saga::impl::v1_0::cpi_info cpi_info;
        typedef saga::impl::v1_0::preference_type preference_type;
        typedef Mutex mutex_type;

        mutex_type mtx_;     // secure access to cpi_instance_data

    public:
        url_cpi (proxy* p, cpi_info const& info, 
                TR1::shared_ptr <saga::adaptor> adaptor, cpi::flags flags) 
            : base_type(p, info, adaptor, flags) 
        {}
        ~url_cpi() 
        {}

        ///////////////////////////////////////////////////////////////////////
        //  instance data
        //    data associated with the API object instance, i.e. visible to all 
        //    CPI object instances associated with the API object instance 
        //    regardless of the shared library (adaptor) they are implemented in.
        typedef saga::adaptors::v1_0::url_cpi_instance_data 
            url_instance_data;
        friend class saga::adaptors::instance_data<url_instance_data>;
        typedef      saga::adaptors::instance_data<url_instance_data> 
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
            std::string const& cpi_name = saga::adaptors::url_cpi)
        {
            // register namespace_entry CPI functions
            saga::uuid cpi_uuid;
            register_url_functions<Derived>(infos, 
                &saga::adaptors::v1_0::url_cpi<Derived>::cpi_maker, 
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

