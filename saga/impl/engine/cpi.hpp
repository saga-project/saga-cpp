//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005 Michel Zandstra (michelzandstra@gmail.com)
//  Copyright (c) 2006 Stephan Hirmer (shirmer@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_ENGINE_CPI_HPP
#define SAGA_IMPL_ENGINE_CPI_HPP

// flag that this file belongs to the SAGA engine
#define SAGA_ENGINE 1

#include <string>
#include <vector>

#include <saga/saga/util.hpp>
#include <saga/saga/task.hpp>

#include <saga/saga/exception.hpp>
#include <saga/saga/metric.hpp>
#include <saga/saga/impl_base.hpp>

#include <saga/impl/config.hpp>
#include <saga/saga/uuid.hpp>

#include <saga/saga/adaptors/bulk_container_base.hpp>
#include <saga/saga/adaptors/instance_data_base.hpp>
#include <saga/saga/adaptors/utils/utils.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl { namespace v1_0 { 

    /**
     * Base class for all other cpi-classes
     * Any cpi_xxx class should inherit from this class, otherwise the
     * Saga engine cannot handle them.
     */
    class cpi : public TR1::enable_shared_from_this<cpi>
    {
    protected:
        typedef saga::impl::v1_0::cpi_info cpi_info;

    public:
        enum flags
        {
            Unknown = -1,
            Noflags =  0,
            Fixme   =  1
        };

        // cpi types
        enum type
        {
            UnknownCPI        = -1,
            Context           = saga::object::Context,
            Url               = saga::object::URL,
            NSEntry           = saga::object::NSEntry,
            NSDirectory       = saga::object::NSDirectory,
            File              = saga::object::File,
            Directory         = saga::object::Directory,
            LogicalFile       = saga::object::LogicalFile,
            LogicalDirectory  = saga::object::LogicalDirectory,
            Job               = saga::object::Job,
            JobService        = saga::object::JobService,
            StreamServer      = saga::object::StreamServer,
            Stream            = saga::object::Stream,
            RPC               = saga::object::RPC,
            Advert            = saga::object::Advert,
            AdvertDirectory   = saga::object::AdvertDirectory,
            ServiceDiscoverer = saga::object::ServiceDiscoverer,
            CPRJob            = saga::object::CPRJob,
            CPRJobService     = saga::object::CPRJobService,
            CPRCheckpoint     = saga::object::CPRCheckpoint,
            CPRDirectory      = saga::object::CPRDirectory,
            EntityNavigator   = saga::object::EntityNavigator
        };

    protected:
        // proxy must reset proxy_ pointer when it is destroyed, to avoid
        // dangling pointers.
        friend class saga::impl::proxy;

        saga::adaptors::bulk_container_base* bulk_;
        saga::impl::proxy* proxy_;
        std::vector <saga::metric> metrics_;
        cpi_info const& info_;
        TR1::shared_ptr<saga::adaptor> adaptor_;
        flags flags_;

    protected:
        typedef void (cpi::*sync_func)();
        typedef saga::task (cpi::*async_func)();
        typedef bool (cpi::*prep_func)();

    public:
        SAGA_EXPORT cpi (saga::impl::proxy* p, cpi_info const& info,
            TR1::shared_ptr<saga::adaptor> adaptor, flags flags);

        SAGA_EXPORT virtual ~cpi (void) 
        {
            if (bulk_ != NULL) delete bulk_;
        }

        // returns the uuid of the associated adaptor object
        SAGA_EXPORT virtual saga::uuid const& get_adaptor_uuid (void) const;

        /**
        * pointer to an adaptor-maker function
        */
        typedef cpi * (*maker_type) (proxy * p, 
                                     cpi_info       const & cpi,
                                     saga::ini::ini const & adap_ini, 
                                     saga::ini::ini const & glob_ini, 
                                     TR1::shared_ptr <saga::adaptor> adaptor);

        /**
        * Returns the implemented CPI version 
        */
        SAGA_EXPORT virtual int get_version (void) const 
            { return SAGA_CPIVERSION_1_0; }

        /**
        * Used by the proxy to get the flags from the adaptor
        *
        * @return the flags of the adaptor, 0 on default
        * 
        */
        SAGA_EXPORT virtual flags get_flags (void) const 
            { return flags_; }

        /**
        */
        SAGA_EXPORT virtual cpi_info const& get_adaptor_info() const 
            { return info_; }

        SAGA_EXPORT virtual saga::metric get_metric (std::string const & name) const 
        { 
            std::vector <saga::metric> :: const_iterator i = metrics_.begin ();
            std::vector <saga::metric> :: const_iterator end = metrics_.end ();

            for (/**/; i != end; ++i) 
            {
                if (i->get_attribute (saga::attributes::metric_name) == name)
                    return (*i);
            }

            SAGA_THROW("Could not find metric: '" + name + "'.",
                saga::BadParameter);
            return saga::metric();
        }

        SAGA_EXPORT virtual const std::vector<saga::metric>& list_metrics (void) const 
        { 
            return (metrics_); 
        }

        SAGA_EXPORT virtual type get_type() const = 0;
         
        /*!
          * initializes a bulk operation. 
          * This function has to be called before the preparing functions are
          * called. Resets the bulk adaptor.
          */
        SAGA_EXPORT void init_bulk(void);
        
        SAGA_EXPORT virtual saga::task_base::state get_state(saga::uuid task_uuid);
        SAGA_EXPORT virtual bool wait(double timeout, saga::uuid task_uuid);
        SAGA_EXPORT virtual void cancel(saga::uuid task_uuid);
        /*!
        * executes a bulk adaptor.
        */      
        SAGA_EXPORT virtual std::vector<saga::uuid> execute_bulk(void);
      
        // get_proxy() is needed for the adaptor_data<>
        SAGA_EXPORT proxy *get_proxy (void) const;
        SAGA_EXPORT saga::session& get_session();
        SAGA_EXPORT saga::session const& get_session() const;
        SAGA_EXPORT TR1::shared_ptr<saga::adaptor> get_adaptor() const;

        // return, whether sync/async cpi functions should be implemented
        SAGA_EXPORT static bool register_sync_member(char const* name);
        SAGA_EXPORT static bool register_async_member(char const* name);
        SAGA_EXPORT static bool register_bulk_member(char const* name);

        // return pointers to CPI function implementations
        SAGA_EXPORT bool get_sync_func(char const* name, sync_func* func);
        SAGA_EXPORT bool get_async_func(char const* name, async_func* func);
        SAGA_EXPORT bool get_prep_func(char const* name, prep_func* func);

    }; // class cpi

///////////////////////////////////////////////////////////////////////////////
}}} // namespace saga:impl::v1_0

#endif  // SAGA_IMPL_ENGINE_CPI_HPP

