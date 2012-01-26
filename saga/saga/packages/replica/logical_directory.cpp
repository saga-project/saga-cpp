#if defined(__WAVE__)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/logical_directory.cpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__)
#pragma wave option(preserve: 0, output: null)
#endif

//  this is needed in every file including detail/attribute_impl.hpp and not 
//  belonging to the engine
#define SAGA_NO_IMPORT_ATTRIBUTE
#define SAGA_EXPORT_ATTRIBUTE     SAGA_REPLICA_PACKAGE_EXPORT

// include directory api and implementation
#include <string>
#include <vector>

#include <saga/saga/task.hpp>

// include the package we implement
#include <saga/saga/call.hpp>
#include <saga/saga/replica.hpp>
#include <saga/impl/replica.hpp>
#include <saga/saga/detail/call.hpp>

#include <saga/saga/detail/attribute_impl.hpp>

#ifdef SAGA_DEBUG
#include <saga/saga/packages/replica/preprocessed/logical_directory.cpp>
#else

#if defined(__WAVE__)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/logical_directory.cpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga 
{
    
    namespace replica {
        
        /////////////////////////////////////////////////////////////////////////////
        namespace metrics
        {
            /// @cond
            saga::metrics::init_data const logical_directory_metric_data[] = 
            {
                // logical_directory specific metrics
                {
                    saga::replica::metrics::logical_directory_created_entry,
                    "Metric fires on logical file creates, it carries the key of"
                    "the new logical file.",
                    saga::attributes::metric_mode_readonly,
                    "1", 
                    saga::attributes::metric_type_string,
                    ""
                },
                {
                    saga::replica::metrics::logical_directory_modified_entry,
                    "Metric fires on logical file changes, it carries the key of"
                    "the modified logical file.",
                    saga::attributes::metric_mode_readonly,
                    "1", 
                    saga::attributes::metric_type_string,
                    ""
                },
                {
                    saga::replica::metrics::logical_directory_deleted_entry,
                    "Metric fires on logical file deletes, it carries the key of" 
                    "the deleted logical file.",
                    saga::attributes::metric_mode_readonly,
                    "1", 
                    saga::attributes::metric_type_string,
                    ""
                },
            };
            /// @endcond
        }
        
        logical_directory::logical_directory (session const& s, saga::url url, 
                                              int mode)
        : name_space::directory (new saga::impl::logical_directory (s, url, mode))
        {
            // initialize attribute implementation
            this->attribute_base::init (true);    // no constraints on attributes

            // initialize implementation object
            this->saga::object::get_impl()->init();

            // initialize metrics
            std::vector<saga::metric> metrics;
            for (unsigned int i = 0; 
                 i < sizeof(metrics::logical_directory_metric_data)/sizeof(saga::metrics::init_data);
                 ++i)
            {
                saga::metrics::init_data const* p = &metrics::logical_directory_metric_data[i];
                saga::metric m(*this, p->name, p->description, p->mode, p->unit, 
                               p->type, p->value);
                metrics.push_back(m);
            }
            this->monitorable_base::init (metrics);
        }

        logical_directory::logical_directory (saga::url url, int mode)
        : name_space::directory (new saga::impl::logical_directory (detail::get_the_session (), url, mode))
        {
            // initialize attribute implementation
            this->attribute_base::init (true);    // no constraints on attributes

            // initialize implementation object
            this->saga::object::get_impl()->init();

            // initialize metrics
            std::vector<saga::metric> metrics;
            for (unsigned int i = 0; 
                 i < sizeof(metrics::logical_directory_metric_data)/sizeof(saga::metrics::init_data);
                 ++i)
            {
                saga::metrics::init_data const* p = &metrics::logical_directory_metric_data[i];
                saga::metric m(*this, p->name, p->description, p->mode, p->unit, 
                               p->type, p->value);
                metrics.push_back(m);
            }
            this->monitorable_base::init (metrics);
        }

        logical_directory::logical_directory(saga::object const& o)
        : name_space::directory(o)
        {
            if (this->get_type() != saga::object::LogicalDirectory)
            {
                SAGA_THROW("Bad type conversion.", saga::BadParameter);
            }
        }

        logical_directory::logical_directory (void)
        {
        }

        logical_directory::logical_directory(saga::impl::logical_directory *impl)
        : name_space::directory (impl)
        {
        }

        logical_directory::~logical_directory (void)
        {
        }
        
        logical_directory &logical_directory::operator= (saga::object const& o)
        {
            return this->saga::name_space::directory::operator=(o), *this;
        }

        saga::impl::logical_directory* logical_directory::get_impl() const
        { 
            typedef saga::object base_type;
            return static_cast<saga::impl::logical_directory*>(this->base_type::get_impl()); 
        }

        TR1::shared_ptr <saga::impl::logical_directory> logical_directory::get_impl_sp() const
        { 
            typedef saga::object base_type;
            return TR1::static_pointer_cast<saga::impl::logical_directory>(
                this->base_type::get_impl_sp()); 
        }

        // factory
        SAGA_CALL_CREATE_IMP_3(logical_directory, impl::logical_directory, session const&, saga::url, int)

        // get file info
        SAGA_CALL_IMP_1 (logical_directory, is_file,  saga::url)

        // overload open methods from namespace_dir
        SAGA_CALL_IMP_2 (logical_directory, open, saga::url, int)
        SAGA_CALL_IMP_2 (logical_directory, open_dir, saga::url, int)
        SAGA_CALL_IMP_3 (logical_directory, find, std::string, std::vector<std::string>, int)
    }   // namespace replica

    namespace detail
    {
        /////////////////////////////////////////////////////////////////////////////
        //  implement the attribute functions (we need to explicitly specialize 
        //  the template because the functions are not implemented inline)
        template struct SAGA_REPLICA_PACKAGE_EXPORT_REPEAT attribute<replica::logical_directory>;

        template struct SAGA_REPLICA_PACKAGE_EXPORT attribute_priv<replica::logical_directory, task_base::Sync>;
        template struct SAGA_REPLICA_PACKAGE_EXPORT attribute_priv<replica::logical_directory, task_base::Async>;
        template struct SAGA_REPLICA_PACKAGE_EXPORT attribute_priv<replica::logical_directory, task_base::Task>;

        template struct SAGA_REPLICA_PACKAGE_EXPORT attribute_sync<replica::logical_directory>;
    }

    ///////////////////////////////////////////////////////////////////////////////////
} // namespace saga
#endif

