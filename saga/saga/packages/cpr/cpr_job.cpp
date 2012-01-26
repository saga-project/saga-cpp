#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/cpr_job.cpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2008 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

//  this is needed in every file including detail/attribute_impl.hpp and not 
//  belonging to the engine
#define SAGA_NO_IMPORT_ATTRIBUTE
#define SAGA_EXPORT_ATTRIBUTE       SAGA_CPR_PACKAGE_EXPORT

#define SAGA_NO_IMPORT_PERMISSIONS
#define SAGA_EXPORT_PERMISSIONS     SAGA_CPR_PACKAGE_EXPORT

#include <boost/assign/list_inserter.hpp>
#include <boost/assign/std/vector.hpp>

// include cpr API and implementation
#include <saga/saga/cpr.hpp>
#include <saga/impl/cpr.hpp>
#include <saga/saga/detail/call.hpp>

#include <saga/saga/detail/attribute_impl.hpp>
#include <saga/saga/detail/permissions_impl.hpp>

#ifdef SAGA_DEBUG
#include <saga/saga/packages/cpr/preprocessed/cpr_job.cpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/cpr_job.cpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga 
{ 
    namespace cpr 
    {
        namespace metrics
        {
            /// @cond
            saga::metrics::init_data const metric_data[] = 
            {
            // FIXME: are we inherting metrics from job?
            // additional metrics for a cpr::job
            {
            saga::cpr::metrics::checkpoint,
            "metric fires when application level checkpoint is requested.",
            saga::attributes::metric_mode_readonly,
            "STRING", 
            saga::attributes::metric_type_string,
            ""
            },
            {
            saga::cpr::metrics::checkpointed,
            "metric fires when application level checkpoint is finished.",
            saga::attributes::metric_mode_readonly,
            "TRIGGER", 
            saga::attributes::metric_type_trigger,
            ""
            },
            {
            saga::cpr::metrics::recover,
            "metric fires when application level recover is requested.",
            saga::attributes::metric_mode_readonly,
            "STRING", 
            saga::attributes::metric_type_string,
            ""
            },
            {
            saga::cpr::metrics::recovered,
            "metric fires when application level recover is finished.",
            saga::attributes::metric_mode_readonly,
            "TRIGGER", 
            saga::attributes::metric_type_trigger,
            ""
            }
            };
            /// @endcond
            
        }
        
        
        
        /////////////////////////////////////////////////////////////////////////////
        job::job()
        {
        }
        
        job::job(saga::impl::cpr_job* impl)
            : saga::job::job (impl)
        {
            SAGA_LOG_BLURB("saga::cpr::job ctor start");
            using namespace boost::assign;
            
            // initialize metrics
            std::vector<saga::metric> metrics;
            for (unsigned int i = 0; 
                 i < sizeof(saga::cpr::metrics::metric_data)/sizeof(saga::metrics::init_data);
                 ++i)
            {
                saga::metrics::init_data const* p = &saga::cpr::metrics::metric_data[i];
                saga::metric m(*this, p->name, p->description, p->mode, p->unit, 
                               p->type, p->value);
                metrics.push_back(m);
            }
            this->monitorable_base::init (metrics);

            SAGA_LOG_BLURB("saga::cpr::job ctor done"); 
        }
        
        job::~job (void)
        {
        }
        
        /////////////////////////////////////////////////////////////////////////////
        job::job (saga::object const& o)
        : saga::job::job (o)
        {
            if (this->get_type() != saga::object::CPRJob)
            {
                SAGA_THROW("Bad type conversion.", saga::BadParameter);
            }
        }
        
        job &job::operator= (saga::object const& o)
        {
            return saga::job::job::operator=(o), *this;
        }

        /////////////////////////////////////////////////////////////////////////////
        saga::impl::cpr_job* job::get_impl() const
        { 
            typedef saga::object base_type;
            return static_cast<saga::impl::cpr_job*>(this->base_type::get_impl()); 
        }

        TR1::shared_ptr <saga::impl::cpr_job> job::get_impl_sp(void) const
        { 
            // FIXME: this needs documentation
            typedef saga::object base_type;
            return TR1::static_pointer_cast <saga::impl::cpr_job> (
                this->base_type::get_impl_sp());
        }

        // job cpr management
        /// @cond
        SAGA_CALL_IMP_1 (job, checkpoint,    saga::url);
        SAGA_CALL_IMP_1 (job, recover,       saga::url);

        SAGA_CALL_IMP_1 (job, cpr_stage_in,  saga::url);
        SAGA_CALL_IMP_1 (job, cpr_stage_out, saga::url);

        SAGA_CALL_IMP_0 (job, cpr_list);
        SAGA_CALL_IMP_0 (job, cpr_last);
        /// @endcond
    }   // namspace cpr

} // namespace saga
///////////////////////////////////////////////////////////////////////////////////

#endif

