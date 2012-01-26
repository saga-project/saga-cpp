//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_SAGA_ADAPTORS_ADAPTOR_HPP
#define SAGA_SAGA_ADAPTORS_ADAPTOR_HPP

#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>

#include <boost/version.hpp>
#include <boost/plugin.hpp>
#include <boost/plugin/export_plugin.hpp>
#include <boost/thread.hpp>
#include <boost/tokenizer.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/stringize.hpp>

#include <saga/saga/adaptors/adaptor_data.hpp>
#include <saga/saga/adaptors/adaptor_suite.hpp>
#include <saga/saga/adaptors/adaptor_suite_data.hpp>
#include <saga/saga/adaptors/utils/utils.hpp>

#include <saga/impl/engine/adaptor_selector.hpp>
#include <saga/saga/uuid.hpp>

///////////////////////////////////////////////////////////////////////////////
#ifndef SAGA_ADAPTOR_NAME
# ifndef SAGA_ENGINE
#   error "SAGA_ADAPTOR_NAME needs to be defined for saga adaptor macros"
# else
#   define SAGA_ADAPTOR_NAME engine   // needed for get_name()
# endif
#endif

///////////////////////////////////////////////////////////////////////////////
#ifndef SAGA_ADAPTOR_LIB_NAME
#define SAGA_ADAPTOR_LIB_NAME                                                 \
        BOOST_PP_CAT(libsaga_adaptor_, SAGA_ADAPTOR_NAME )                    \
    /**/
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga 
{
    // definition of cpi names
    namespace adaptors
    {
        // general usage
        char const* const attribute_cpi        = "attribute_cpi";
        char const* const permissions_cpi      = "permissions_cpi";

        char const* const context_cpi          = "context_cpi";
        char const* const url_cpi              = "url_cpi";

        // file package
        char const* const namespace_entry_cpi  = "namespace_entry_cpi";
        char const* const namespace_dir_cpi    = "namespace_dir_cpi";
        char const* const file_cpi             = "file_cpi";
        char const* const directory_cpi        = "directory_cpi";
        
        // locical_file package
        char const* const logical_file_cpi      = "logical_file_cpi";
        char const* const logical_directory_cpi = "logical_directory_cpi";
        
        // job package
        char const* const job_cpi              = "job_cpi";
        char const* const job_service_cpi      = "job_service_cpi";
        
        // stream package
        char const* const stream_cpi           = "stream_cpi";
        char const* const stream_server_cpi    = "stream_server_cpi";

        // RPC package
        char const* const rpc_cpi              = "rpc_cpi";
        
        // information services package
        char const* const advert_cpi           = "advert_cpi";
        char const* const advert_directory_cpi = "advert_directory_cpi";
        
        // Service discovery package
        char const* const discoverer_cpi       = "discoverer_cpi";

        // cpr package
        char const* const cpr_job_cpi          = "cpr_job_cpi";
        char const* const cpr_job_service_cpi  = "cpr_job_service_cpi";
        char const* const cpr_checkpoint_cpi   = "cpr_checkpoint_cpi";
        char const* const cpr_directory_cpi    = "cpr_directory_cpi";

        //Information System Navigator
        char const* const navigator_cpi        = "navigator_cpi";
        
        // return the name of the implementing CPI for a saga object type
        inline std::string 
        get_cpi_name(saga::object::type type) 
        {
            switch (type) {
            case saga::object::Context:           return context_cpi;
            case saga::object::URL:               return url_cpi;
            case saga::object::NSEntry:           return namespace_entry_cpi;
            case saga::object::NSDirectory:       return namespace_dir_cpi;
            case saga::object::File:              return file_cpi;
            case saga::object::Directory:         return directory_cpi;
            case saga::object::LogicalFile:       return logical_file_cpi;
            case saga::object::LogicalDirectory:  return logical_directory_cpi;
            case saga::object::Job:               return job_cpi;
            case saga::object::JobService:        return job_service_cpi;
            case saga::object::Stream:            return stream_cpi;
            case saga::object::StreamServer:      return stream_server_cpi;
            case saga::object::RPC:               return rpc_cpi;
            case saga::object::Advert:            return advert_cpi;
            case saga::object::AdvertDirectory:   return advert_directory_cpi;
            case saga::object::ServiceDiscoverer: return discoverer_cpi;
            case saga::object::CPRJob:            return cpr_job_cpi;
            case saga::object::CPRJobService:     return cpr_job_service_cpi;
            case saga::object::CPRCheckpoint:     return cpr_checkpoint_cpi;
            case saga::object::CPRDirectory:      return cpr_directory_cpi;
            case saga::object::EntityNavigator:   return navigator_cpi;
            default:
                break;
            }
            return "Unknown";
        }
    }
    
    /// @cond
    /** These methods are not within API scope */
    class adaptor
    {
    private:
        // this mutex is used to synchronize access to the adaptor member data
        typedef boost::recursive_mutex mutex_type;
        mutex_type mtx_;

    protected:
        // this uuid identifies the adaptor object
        saga::uuid adaptor_uuid_;
    
    public:
        virtual ~adaptor (void) {}
        virtual impl::adaptor_selector::adaptor_info_list_type 
                adaptor_register (saga::impl::session *) = 0;

        // support functions for the adaptor_data helper class
        void lock_data() 
        {
#if BOOST_VERSION < 103500
            boost::detail::thread::lock_ops<mutex_type>::lock(mtx_);
#else
            mtx_.lock();
#endif
        }
        void unlock_data() 
        {
#if BOOST_VERSION < 103500
            boost::detail::thread::lock_ops<mutex_type>::unlock(mtx_);
#else
            mtx_.unlock();
#endif
        }
        
        // access the identifying uuid of this adaptor
        saga::uuid const & get_uuid() const
        {
            return adaptor_uuid_;
        }

        // access the identifying name of this adaptor
        virtual std::string get_name() const = 0;


        // get adaptor_suite, if any is available
        template <class Adaptor_Suite>
        Adaptor_Suite & get_adaptor_suite (void)
        {
          return saga::adaptors::utils::get_singleton <Adaptor_Suite> ();
        }

        // initialize the adaptor using the preferences, return false to 
        // cancel adaptor loading
        virtual bool init(saga::impl::session *,
            saga::ini::ini const& glob_ini, saga::ini::ini const& adap_ini) 
        { return true; }
    };
    /// @endcond
///////////////////////////////////////////////////////////////////////////////
} // namespace saga

///////////////////////////////////////////////////////////////////////////////
//  Forward the SAGA_ADAPTOR_REGISTER macro to the correct BOOST_PLUGIN macro 
//  sequence
#define SAGA_ADAPTOR_REGISTER(ActualType)                                     \
        BOOST_PLUGIN_EXPORT_LIST (SAGA_ADAPTOR_LIB_NAME)                      \
        BOOST_PLUGIN_EXPORT      (SAGA_ADAPTOR_LIB_NAME, saga::adaptor,       \
                                  ActualType,        "adaptor");              \
    /**/

#endif // SAGA_SAGA_ADAPTORS_ADAPTOR_HPP

