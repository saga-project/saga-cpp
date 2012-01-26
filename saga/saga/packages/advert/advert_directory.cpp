#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/directory.cpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 0, output: null)
#endif

//  this is needed in every file including detail/attribute_impl.hpp and not 
//  belonging to the engine
#define SAGA_NO_IMPORT_ATTRIBUTE
#define SAGA_EXPORT_ATTRIBUTE     SAGA_ADVERT_PACKAGE_EXPORT

// include directory API and implementation
#include <string>
#include <vector>

// include the package we implement
#include <saga/saga/call.hpp>
#include <saga/saga/advert.hpp>
#include <saga/saga/adaptors/task.hpp>
#include <saga/impl/advert.hpp>
#include <saga/saga/detail/call.hpp>

#include <saga/saga/detail/attribute_impl.hpp>

#ifdef SAGA_DEBUG
#include <saga/saga/packages/advert/preprocessed/directory.cpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/directory.cpp")
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga 
{
    namespace advert 
    {
        
        namespace metrics
        {
            ///@cond
            saga::metrics::init_data const directory_metric_data[] = 
            {
                // directory specific metrics
                {
                    directory_created_entry,
                    "Metric fires on advert entry creates, it carries the key of"
                    "the new entry.",
                    attributes::metric_mode_readonly,
                    "1", 
                    attributes::metric_type_string,
                    ""
                },
                {
                    directory_modified_entry,
                    "Metric fires on advert entry changes, it carries the key of"
                    "the modified entry.",
                    attributes::metric_mode_readonly,
                    "1", 
                    attributes::metric_type_string,
                    ""
                },
                {
                    directory_deleted_entry,
                    "Metric fires on advert entry deletes, it carries the key of" 
                    "the deleted entry.",
                    attributes::metric_mode_readonly,
                    "1", 
                    attributes::metric_type_string,
                    ""
                },
            };
            ///@endcond
            
            ///////////////////////////////////////////////////////////////////////////
        }   // namespace saga::metrics

        directory::directory (session const& s, saga::url url, int mode)
        :   name_space::directory (new saga::impl::advert_directory (s, url, mode))
        {
            // initialize attribute implementation
            this->attribute_base::init (true);    // no constraints on attributes

            // initialize implementation object
            this->saga::object::get_impl()->init();

            // initialize metrics
            std::vector<saga::metric> metrics;
            for (unsigned int i = 0; 
                 i < sizeof(saga::advert::metrics::directory_metric_data)/sizeof(saga::metrics::init_data);
                 ++i)
            {
                saga::metrics::init_data const* p = &saga::advert::metrics::directory_metric_data[i];
                saga::metric m(*this, p->name, p->description, p->mode, p->unit, 
                               p->type, p->value);
                metrics.push_back(m);
            }
            this->monitorable_base::init (metrics);
        }

        directory::directory (saga::url url, int mode)
        : name_space::directory (new saga::impl::advert_directory (detail::get_the_session (), url, mode))
        {
            // initialize attribute implementation
            this->attribute_base::init (true);    // no constraints on attributes

            // initialize implementation object
            this->saga::object::get_impl()->init();

            // initialize metrics
            std::vector<saga::metric> metrics;
            for (unsigned int i = 0; 
                 i < sizeof(saga::advert::metrics::directory_metric_data)/sizeof(saga::metrics::init_data);
                 ++i)
            {
                saga::metrics::init_data const* p = &saga::advert::metrics::directory_metric_data[i];
                saga::metric m(*this, p->name, p->description, p->mode, p->unit, 
                               p->type, p->value);
                metrics.push_back(m);
            }
            this->monitorable_base::init (metrics);
        }

        directory::directory (void)
        {
        }

        directory::directory(saga::impl::advert_directory *impl)
        :   name_space::directory (impl)
        {
        }

        directory::directory (saga::object const& o)
        : name_space::directory(o)
        {
            if (this->get_type() != saga::object::AdvertDirectory)
            {
                SAGA_THROW("Bad type conversion.", saga::BadParameter);
            }
        }

        directory::~directory (void)
        {
        }

        directory &directory::operator= (saga::object const& o)
        {
            return this->saga::name_space::directory::operator=(o), *this;
        }

        saga::impl::advert_directory* directory::get_impl() const
        { 
            typedef saga::object base_type;
            return static_cast<saga::impl::advert_directory*>(this->base_type::get_impl()); 
        }

        TR1::shared_ptr <saga::impl::advert_directory> directory::get_impl_sp() const
        { 
            typedef saga::object base_type;
            return TR1::static_pointer_cast<saga::impl::advert_directory>(
                this->base_type::get_impl_sp()); 
        }

        // factory
        SAGA_CALL_CREATE_IMP_3(directory, impl::advert_directory, session const&, saga::url const&, int)

        // overload open methods from namespace_dir
        SAGA_CALL_IMP_2 (directory, open, saga::url, int)
        SAGA_CALL_IMP_2 (directory, open_dir, saga::url, int)
        SAGA_CALL_IMP_3 (directory, find, std::string, std::vector<std::string>, int)

    }   // namespace advert

    namespace detail
    {
        ///////////////////////////////////////////////////////////////////////////
        //  implement the attribute functions (we need to explicitly specialize 
        //  the template because the functions are not implemented inline)
        template struct SAGA_ADVERT_PACKAGE_EXPORT_REPEAT attribute<advert::directory>;

        template struct SAGA_ADVERT_PACKAGE_EXPORT attribute_priv<advert::directory, task_base::Sync>;
        template struct SAGA_ADVERT_PACKAGE_EXPORT attribute_priv<advert::directory, task_base::Async>;
        template struct SAGA_ADVERT_PACKAGE_EXPORT attribute_priv<advert::directory, task_base::Task>;

        template struct SAGA_ADVERT_PACKAGE_EXPORT attribute_sync<advert::directory>;
    }

///////////////////////////////////////////////////////////////////////////////
} // namespace saga

#endif

