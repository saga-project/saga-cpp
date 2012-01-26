#if defined(__WAVE__)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/cpr_checkpoint.cpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2008 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  job LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__)
#pragma wave option(output: null)
#endif

//  this is needed in every file including detail/attribute_impl.hpp and not 
//  belonging to the engine
#define SAGA_NO_IMPORT_ATTRIBUTE
#define SAGA_EXPORT_ATTRIBUTE       SAGA_CPR_PACKAGE_EXPORT

#include <saga/saga/task.hpp>

// include file API and implementation
#include <saga/saga/cpr.hpp>
#include <saga/impl/cpr.hpp>
#include <saga/saga/detail/call.hpp>

#include <boost/assign/list_inserter.hpp>
#include <boost/assign/std/vector.hpp>

#include <saga/saga/detail/attribute_impl.hpp>

#ifdef SAGA_DEBUG
#include <saga/saga/packages/filesystem/preprocessed/cpr_checkpoint.cpp>
#else

#if defined(__WAVE__)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/cpr_checkpoint.cpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga 
{
    namespace cpr 
    {
        namespace metrics
        {
            /// @cond
            saga::metrics::init_data const checkpoint_metric_data[] = 
            {
                // cpr_checkpoint specific metrics
                {
                    checkpoint_modified,
                    "Metric fires on checkpoint changes, it carries the key of"
                    "the modified checkpoint file.",
                    saga::attributes::metric_mode_readonly,
                    "1", 
                    saga::attributes::metric_type_string,
                    ""
                },
                {
                    checkpoint_deleted,
                    "Metric fires on checkpoint deletes, it carries the key of" 
                    "the deleted checkpoint file.",
                    saga::attributes::metric_mode_readonly,
                    "1", 
                    saga::attributes::metric_type_string,
                    ""
                },
            };
            /// @endcond
            
        } // namespace metrics

        checkpoint::checkpoint (session const& s, saga::url url, int mode)
        :   entry (new saga::impl::cpr_checkpoint (s, url, mode))
        {
            // initialize attribute implementation
            this->attribute_base::init (true);    // no constraints on attributes

            // initialize implementation object
            this->saga::object::get_impl()->init();

            // initialize metrics
            std::vector<saga::metric> metrics;
            for (unsigned int i = 0; 
                 i < sizeof(saga::cpr::metrics::checkpoint_metric_data)/sizeof(saga::metrics::init_data);
                 ++i)
            {
                saga::metrics::init_data const* p = &saga::cpr::metrics::checkpoint_metric_data[i];
                saga::metric m(*this, p->name, p->description, p->mode, p->unit, 
                               p->type, p->value);
                metrics.push_back(m);
            }
            this->monitorable_base::init (metrics);

            // initialize attributes
            using namespace boost::assign;
            std::vector<std::string> valid_keys;
            valid_keys += 
                    attributes::cpr_time,
                    attributes::cpr_nfliles,
                    attributes::cpr_mode,
                    attributes::cpr_parent,
                    attributes::cpr_children
                ;

            // initialize list of valid keys
            this->init_keynames (valid_keys);

            strmap_type attributes_scalar_rw;
            insert(attributes_scalar_rw)
                    (attributes::cpr_time, "")
                    (attributes::cpr_nfliles, "")
                    (attributes::cpr_mode, "")
                    (attributes::cpr_parent, "")
                    (attributes::cpr_children, "")
                ;

            // initialize attribute implementation
            this->attribute_base::init (strmap_type(), attributes_scalar_rw);
            this->attribute_base::init (false, true);   // cache only implementation
        }

        checkpoint::checkpoint (saga::url url, int mode)
        :   entry (new saga::impl::cpr_checkpoint (detail::get_the_session (), url, mode))
        {
            // initialize attribute implementation
            this->attribute_base::init (true);    // no constraints on attributes

            // initialize implementation object
            this->saga::object::get_impl()->init();

            // initialize metrics
            std::vector<saga::metric> metrics;
            for (unsigned int i = 0; 
                 i < sizeof(saga::cpr::metrics::checkpoint_metric_data)/sizeof(saga::metrics::init_data);
                 ++i)
            {
                saga::metrics::init_data const* p = &saga::cpr::metrics::checkpoint_metric_data[i];
                saga::metric m(*this, p->name, p->description, p->mode, p->unit, 
                               p->type, p->value);
                metrics.push_back(m);
            }

            this->monitorable_base::init (metrics);

            // initialize attributes
            using namespace boost::assign;
            std::vector<std::string> valid_keys;
            valid_keys += 
                    attributes::cpr_time,
                    attributes::cpr_nfliles,
                    attributes::cpr_mode,
                    attributes::cpr_parent,
                    attributes::cpr_children
                ;

            // initialize list of valid keys
            this->init_keynames (valid_keys);

            strmap_type attributes_scalar_rw;
            insert(attributes_scalar_rw)
                    (attributes::cpr_time, "")
                    (attributes::cpr_nfliles, "")
                    (attributes::cpr_mode, "")
                    (attributes::cpr_parent, "")
                    (attributes::cpr_children, "")
                ;

            // initialize attribute implementation
            this->attribute_base::init (strmap_type(), attributes_scalar_rw);
            this->attribute_base::init (false, true);   // cache only implementation
        }

        checkpoint::checkpoint(saga::object const& o)
        : entry(o)
        {
            if (this->get_type() != saga::object::CPRCheckpoint)
            {
                SAGA_THROW("Bad type conversion.", saga::BadParameter);
            }
        }

        checkpoint::checkpoint (void) 
        {
        }

        checkpoint::checkpoint(saga::impl::cpr_checkpoint *impl)
        : entry (impl)
        {
        }

        checkpoint::~checkpoint (void)
        {
        }

        checkpoint &checkpoint::operator= (saga::object const& o)
        {
            return this->saga::name_space::entry::operator=(o), *this;
        }

        saga::impl::cpr_checkpoint* checkpoint::get_impl() const
        { 
            typedef saga::object base_type;
            return static_cast<saga::impl::cpr_checkpoint*>(this->base_type::get_impl()); 
        }

        TR1::shared_ptr <saga::impl::cpr_checkpoint> checkpoint::get_impl_sp(void) const
        { 
            // FIXME: this needs documentation
            typedef saga::object base_type;
            return TR1::static_pointer_cast <saga::impl::cpr_checkpoint> (
                this->base_type::get_impl_sp());
        }

        /// @cond
        // factory
        SAGA_CALL_CREATE_IMP_3(checkpoint, impl::cpr_checkpoint, session const&, saga::url, int)

        SAGA_CALL_IMP_2    (checkpoint, set_parent,                   saga::url, int);
        SAGA_CALL_IMP_1    (checkpoint, get_parent,                   int);
        SAGA_CALL_IMP_0    (checkpoint, get_file_num);
        SAGA_CALL_IMP_0    (checkpoint, list_files);
        SAGA_CALL_IMP_1    (checkpoint, add_file,                     saga::url);
        SAGA_CALL_IMP_1    (checkpoint, get_file,                     int);
        SAGA_CALL_IMP_2    (checkpoint, open_file,                    saga::url, int);
        SAGA_CALL_IMP_2_EX (checkpoint, open_file,   open_file_idx,   int,       int);
        SAGA_CALL_IMP_1    (checkpoint, remove_file,                  saga::url);
        SAGA_CALL_IMP_1_EX (checkpoint, remove_file, remove_file_idx, int);
        SAGA_CALL_IMP_2    (checkpoint, update_file,                  saga::url, saga::url);
        SAGA_CALL_IMP_2_EX (checkpoint, update_file, update_file_idx, int,       saga::url);
        SAGA_CALL_IMP_2    (checkpoint, stage_file,                   saga::url, saga::url);
        SAGA_CALL_IMP_2_EX (checkpoint, stage_file,  stage_file_idx,  int,       saga::url);
        SAGA_CALL_IMP_1_EX (checkpoint, stage_file,  stage_file_all,  saga::url);
        /// @endcond
        
    }   // namespace cpr

    namespace detail
    {
        /////////////////////////////////////////////////////////////////////////////
        //  implement the attribute functions (we need to explicitly specialize 
        //  the template because the functions are not implemented inline)
        template struct SAGA_CPR_PACKAGE_EXPORT_REPEAT attribute<cpr::checkpoint>;

        template struct SAGA_CPR_PACKAGE_EXPORT attribute_priv<cpr::checkpoint, task_base::Sync>;
        template struct SAGA_CPR_PACKAGE_EXPORT attribute_priv<cpr::checkpoint, task_base::Async>;
        template struct SAGA_CPR_PACKAGE_EXPORT attribute_priv<cpr::checkpoint, task_base::Task>;

        template struct SAGA_CPR_PACKAGE_EXPORT attribute_sync<cpr::checkpoint>;
    }

    ///////////////////////////////////////////////////////////////////////////////////
} // namespace saga

#endif

