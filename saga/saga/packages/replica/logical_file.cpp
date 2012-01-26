#if defined(__WAVE__)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/logical_file.cpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  job LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__)
#pragma wave option(output: null)
#endif

//  this is needed in every file including detail/attribute_impl.hpp and not 
//  belonging to the engine
#define SAGA_NO_IMPORT_ATTRIBUTE
#define SAGA_EXPORT_ATTRIBUTE     SAGA_REPLICA_PACKAGE_EXPORT

#include <saga/saga/task.hpp>

// include file API and implementation
#include <saga/saga/replica.hpp>
#include <saga/impl/replica.hpp>
#include <saga/saga/detail/call.hpp>

#include <saga/saga/detail/attribute_impl.hpp>

#ifdef SAGA_DEBUG
#include <saga/saga/packages/replica/preprocessed/logical_file.cpp>
#else

#if defined(__WAVE__)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/logical_file.cpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga 
{    
    namespace replica {
 
        /////////////////////////////////////////////////////////////////////////////
        namespace metrics
        {
            /// @cond
            saga::metrics::init_data const logical_file_metric_data[] = 
            {
                // logical_file specific metrics
                {
                    saga::replica::metrics::logical_file_modified,
                    "Metric fires on logical file changes, it carries the key of"
                    "the modified logical file.",
                    attributes::metric_mode_readonly,
                    "1", 
                    attributes::metric_type_string,
                    ""
                },
                {
                    saga::replica::metrics::logical_file_deleted,
                    "Metric fires on logical file deletes, it carries the key of" 
                    "the deleted logical file.",
                    attributes::metric_mode_readonly,
                    "1", 
                    attributes::metric_type_string,
                    ""
                },
            };
            /// @endcond
        }

        logical_file::logical_file (session const& s, saga::url url, int mode)
        :   entry (new saga::impl::logical_file (s, url, mode))
        {
            // initialize attribute implementation
            this->attribute_base::init (true);    // no constraints on attributes

            // initialize implementation object
            this->saga::object::get_impl()->init();

            // initialize metrics
            std::vector<saga::metric> metrics;
            for (unsigned int i = 0; 
                 i < sizeof(metrics::logical_file_metric_data)/sizeof(saga::metrics::init_data);
                 ++i)
            {
                saga::metrics::init_data const* p = &metrics::logical_file_metric_data[i];
                saga::metric m(*this, p->name, p->description, p->mode, p->unit, 
                               p->type, p->value);
                metrics.push_back(m);
            }
            this->monitorable_base::init (metrics);
        }

        logical_file::logical_file (saga::url url, int mode)
        :   entry (new saga::impl::logical_file (detail::get_the_session (), url, mode))
        {
            // initialize attribute implementation
            this->attribute_base::init (true);    // no constraints on attributes

            // initialize implementation object
            this->saga::object::get_impl()->init();

            // initialize metrics
            std::vector<saga::metric> metrics;
            for (unsigned int i = 0; 
                 i < sizeof(metrics::logical_file_metric_data)/sizeof(saga::metrics::init_data);
                 ++i)
            {
                saga::metrics::init_data const* p = &metrics::logical_file_metric_data[i];
                saga::metric m(*this, p->name, p->description, p->mode, p->unit, 
                               p->type, p->value);
                metrics.push_back(m);
            }
            this->monitorable_base::init (metrics);
        }

        logical_file::logical_file(saga::object const& o)
        : entry(o)
        {
            if (this->get_type() != saga::object::LogicalFile)
            {
                SAGA_THROW("Bad type conversion.", saga::BadParameter);
            }
        }

        logical_file::logical_file (void) 
        {
        }

        logical_file::logical_file(saga::impl::logical_file *impl)
        : entry (impl)
        {
        }

        logical_file::~logical_file (void)
        {
        }

        logical_file &logical_file::operator= (saga::object const& o)
        {
            return this->saga::name_space::entry::operator=(o), *this;
        }

        saga::impl::logical_file* logical_file::get_impl() const
        { 
            typedef saga::object base_type;
            return static_cast<saga::impl::logical_file*>(this->base_type::get_impl()); 
        }

        TR1::shared_ptr <saga::impl::logical_file> logical_file::get_impl_sp(void) const
        { 
            // FIXME: this needs documentation
            typedef saga::object base_type;
            return TR1::static_pointer_cast <saga::impl::logical_file> (
                this->base_type::get_impl_sp());
        }

        // factory
        SAGA_CALL_CREATE_IMP_3(logical_file, impl::logical_file, session const&, saga::url, int)

        // methods not covered by the namespace_entry
        SAGA_CALL_IMP_1(logical_file, add_location, saga::url)
        SAGA_CALL_IMP_1(logical_file, remove_location, saga::url)
        SAGA_CALL_IMP_2(logical_file, update_location, saga::url, saga::url)
        SAGA_CALL_IMP_0(logical_file, list_locations)
        SAGA_CALL_IMP_2(logical_file, replicate, saga::url, int)
        
    }   // namespace replica

    namespace detail
    {
        /////////////////////////////////////////////////////////////////////////////
        //  implement the attribute functions (we need to explicitly specialize 
        //  the template because the functions are not implemented inline)
        template struct SAGA_REPLICA_PACKAGE_EXPORT_REPEAT attribute<replica::logical_file>;

        template struct SAGA_REPLICA_PACKAGE_EXPORT attribute_priv<replica::logical_file, task_base::Sync>;
        template struct SAGA_REPLICA_PACKAGE_EXPORT attribute_priv<replica::logical_file, task_base::Async>;
        template struct SAGA_REPLICA_PACKAGE_EXPORT attribute_priv<replica::logical_file, task_base::Task>;

        template struct SAGA_REPLICA_PACKAGE_EXPORT attribute_sync<replica::logical_file>;
    }

    ///////////////////////////////////////////////////////////////////////////////////
} // namespace saga

#endif

