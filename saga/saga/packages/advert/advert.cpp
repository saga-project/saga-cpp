#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/advert.cpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  job LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

//  this is needed in every file including detail/attribute_impl.hpp and not 
//  belonging to the engine
#define SAGA_NO_IMPORT_ATTRIBUTE
#define SAGA_EXPORT_ATTRIBUTE     SAGA_ADVERT_PACKAGE_EXPORT

//  this is needed in every file including detail/monitorable_impl.hpp and not 
//  belonging to the engine
#define SAGA_NO_IMPORT_MONITORABLE

#include <saga/saga/util.hpp>
#include <saga/saga/task.hpp>

// include file API and implementation
#include <saga/saga/advert.hpp>
#include <saga/impl/advert.hpp>
#include <saga/impl/engine/metric.hpp>
#include <saga/saga/detail/call.hpp>

#include <saga/saga/detail/attribute_impl.hpp>
#include <saga/saga/detail/monitorable_impl.hpp>

#ifdef SAGA_DEBUG
#include <saga/saga/packages/filesystem/preprocessed/advert.cpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/advert.cpp")
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga 
{
    namespace advert 
    {
        namespace metrics
        {
            ///@cond
            saga::metrics::init_data const advert_metric_data[] = 
            {
                // advert specific metrics
                {
                    advert_modified,
                    "Metric fires on advert entry changes, it carries the key of"
                    "the modified entry.",
                    attributes::metric_mode_readonly,
                    "1", 
                    attributes::metric_type_string,
                    ""
                },
                {
                    advert_deleted,
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
        
        
        entry::entry (session const& s, saga::url url, int mode)
        :   name_space::entry (new saga::impl::advert (s, url, mode))
        {
            // initialize attribute implementation
            this->attribute_base::init (true);    // no constraints on attributes

            // initialize implementation object
            this->saga::object::get_impl()->init();

            // initialize metrics
            std::vector<saga::metric> metrics;
            for (unsigned int i = 0; 
                 i < sizeof(saga::advert::metrics::advert_metric_data)/sizeof(saga::metrics::init_data);
                 ++i)
            {
                saga::metrics::init_data const* p = &saga::advert::metrics::advert_metric_data[i];
                saga::metric m(*this, p->name, p->description, p->mode, p->unit, 
                               p->type, p->value);
                metrics.push_back(m);
            }
            this->monitorable_base::init (metrics);
        }
        
        entry::entry (saga::url url, int mode)
        :   name_space::entry (new saga::impl::advert (detail::get_the_session (), url, mode))
        {
            // initialize attribute implementation
            this->attribute_base::init (true);    // no constraints on attributes

            // initialize implementation object
            this->saga::object::get_impl()->init();

            // initialize metrics
            std::vector<saga::metric> metrics;
            for (unsigned int i = 0; 
                 i < sizeof(saga::advert::metrics::advert_metric_data)/sizeof(saga::metrics::init_data);
                 ++i)
            {
                saga::metrics::init_data const* p = &saga::advert::metrics::advert_metric_data[i];
                saga::metric m(*this, p->name, p->description, p->mode, p->unit, 
                               p->type, p->value);
                metrics.push_back(m);
            }
            this->monitorable_base::init (metrics);
        }
        
        entry::entry (void) 
        {
        }
        
        entry::entry(saga::impl::advert *impl)
        : name_space::entry (impl)
        {
        }
        
        entry::entry (saga::object const& o)
        : name_space::entry(o)
        {
            if (this->get_type() != saga::object::Advert)
            {
                SAGA_THROW("Bad type conversion.", saga::BadParameter);
            }
        }
        
        entry::~entry (void)
        {
        }
        
        entry &entry::operator= (saga::object const& o)
        {
            return this->saga::name_space::entry::operator=(o), *this;
        }

        saga::impl::advert* entry::get_impl() const
        { 
            typedef saga::object base_type;
            return static_cast<saga::impl::advert*>(this->base_type::get_impl()); 
        }

        TR1::shared_ptr <saga::impl::advert> entry::get_impl_sp(void) const
        { 
            // FIXME: this needs documentation
            typedef saga::object base_type;
            return TR1::static_pointer_cast <saga::impl::advert> (
                this->base_type::get_impl_sp());
        }

        // factory
        SAGA_CALL_CREATE_IMP_3(entry, impl::advert, session const&, saga::url const&, int)

        // methods not covered by the namespace_entry
        SAGA_CALL_IMP_1 (entry, store_object, saga::object)
        SAGA_CALL_IMP_1 (entry, retrieve_object, saga::session)

        saga::task entry::retrieve_objectpriv (saga::task_base::Sync)
        { 
            return get_impl()->retrieve_object(saga::detail::get_the_session(), true); 
        }
        saga::task entry::retrieve_objectpriv (saga::task_base::Async)
        { 
            return saga::detail::run(get_impl()->retrieve_object(saga::detail::get_the_session())); 
        }
        saga::task entry::retrieve_objectpriv (saga::task_base::Task)
        { 
            return get_impl()->retrieve_object(saga::detail::get_the_session()); 
        }

        SAGA_CALL_IMP_1 (entry, store_string, std::string)
        SAGA_CALL_IMP_0 (entry, retrieve_string)

    }   // namespace advert

    namespace detail
    {
        /////////////////////////////////////////////////////////////////////////////
        //  implement the attribute functions (we need to explicitly specialize 
        //  the template because the functions are not implemented inline)
        template struct SAGA_ADVERT_PACKAGE_EXPORT_REPEAT attribute<advert::entry>;

        template struct SAGA_ADVERT_PACKAGE_EXPORT attribute_priv<advert::entry, task_base::Sync>;
        template struct SAGA_ADVERT_PACKAGE_EXPORT attribute_priv<advert::entry, task_base::Async>;
        template struct SAGA_ADVERT_PACKAGE_EXPORT attribute_priv<advert::entry, task_base::Task>;

        template struct SAGA_ADVERT_PACKAGE_EXPORT attribute_sync<advert::entry>;
    }

    ///////////////////////////////////////////////////////////////////////////////////
} // namespace saga

#endif

