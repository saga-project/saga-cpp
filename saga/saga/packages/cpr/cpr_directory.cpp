#if defined(__WAVE__)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/cpr_directory.cpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2008 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__)
#pragma wave option(preserve: 0, output: null)
#endif

//  this is needed in every file including detail/attribute_impl.hpp and not 
//  belonging to the engine
#define SAGA_NO_IMPORT_ATTRIBUTE
#define SAGA_EXPORT_ATTRIBUTE       SAGA_CPR_PACKAGE_EXPORT

// include directory api and implementation
#include <string>
#include <vector>

#include <saga/saga/task.hpp>

// include the package we implement
#include <saga/saga/call.hpp>
#include <saga/saga/cpr.hpp>
#include <saga/impl/cpr.hpp>
#include <saga/saga/detail/call.hpp>

#include <saga/saga/detail/attribute_impl.hpp>

#ifdef SAGA_DEBUG
#include <saga/saga/packages/cpr/preprocessed/cpr_directory.cpp>
#else

#if defined(__WAVE__)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/cpr_directory.cpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga 
{
    namespace cpr 
    {
        
        namespace metrics
        {
            /// @cond
            saga::metrics::init_data const directory_metric_data[] = 
            {
                // cpr_directory specific metrics
                {
                    directory_created_entry,
                    "Metric fires on checkpoint creates, it carries the key of"
                    "the new checkpoint.",
                    saga::attributes::metric_mode_readonly,
                    "1", 
                    saga::attributes::metric_type_string,
                    ""
                },
                {
                    directory_modified_entry,
                    "Metric fires on checkpoint changes, it carries the key of"
                    "the modified checkpoint.",
                    saga::attributes::metric_mode_readonly,
                    "1", 
                    saga::attributes::metric_type_string,
                    ""
                },
                {
                    directory_deleted_entry,
                    "Metric fires on checkpoint deletes, it carries the key of" 
                    "the deleted checkpoint.",
                    saga::attributes::metric_mode_readonly,
                    "1", 
                    saga::attributes::metric_type_string,
                    ""
                },
            };
            /// @endcond
        } // namespace metrics

        directory::directory (session const& s, saga::url url, 
                              int mode)
        : name_space::directory (new saga::impl::cpr_directory (s, url, mode))
        {
            // initialize attribute implementation
            this->attribute_base::init (true);    // no constraints on attributes

            // initialize implementation object
            this->saga::object::get_impl()->init();

            // initialize metrics
            std::vector<saga::metric> metrics;
            for (unsigned int i = 0; 
                 i < sizeof(saga::cpr::metrics::directory_metric_data)/sizeof(saga::metrics::init_data);
                 ++i)
            {
                saga::metrics::init_data const* p = &saga::cpr::metrics::directory_metric_data[i];
                saga::metric m(*this, p->name, p->description, p->mode, p->unit, 
                               p->type, p->value);
                metrics.push_back(m);
            }
            this->monitorable_base::init (metrics);
        }

        directory::directory (saga::url url, int mode)
        : name_space::directory (new saga::impl::cpr_directory (detail::get_the_session (), url, mode))
        {
            // initialize attribute implementation
            this->attribute_base::init (true);    // no constraints on attributes

            // initialize implementation object
            this->saga::object::get_impl()->init();

            // initialize metrics
            std::vector<saga::metric> metrics;
            for (unsigned int i = 0; 
                 i < sizeof(saga::cpr::metrics::directory_metric_data)/sizeof(saga::metrics::init_data);
                 ++i)
            {
                saga::metrics::init_data const* p = &saga::cpr::metrics::directory_metric_data[i];
                saga::metric m(*this, p->name, p->description, p->mode, p->unit, 
                               p->type, p->value);
                metrics.push_back(m);
            }
            this->monitorable_base::init (metrics);
        }

        directory::directory(saga::object const& o)
        : name_space::directory(o)
        {
            if (this->get_type() != saga::object::CPRDirectory)
            {
                SAGA_THROW("Bad type conversion.", saga::BadParameter);
            }
        }

        directory::directory (void)
        {
        }

        directory::directory(saga::impl::cpr_directory *impl)
        : name_space::directory (impl)
        {
        }

        directory::~directory (void)
        {
        }

        directory &directory::operator= (saga::object const& o)
        {
            return this->saga::name_space::directory::operator=(o), *this;
        }

        saga::impl::cpr_directory* directory::get_impl() const
        { 
            typedef saga::object base_type;
            return static_cast<saga::impl::cpr_directory*>(this->base_type::get_impl()); 
        }

        TR1::shared_ptr <saga::impl::cpr_directory> directory::get_impl_sp() const
        { 
            typedef saga::object base_type;
            return TR1::static_pointer_cast<saga::impl::cpr_directory>(
                this->base_type::get_impl_sp()); 
        }

        // factory
        SAGA_CALL_CREATE_IMP_3(directory, impl::cpr_directory, session const&, saga::url, int)

        /// @cond
        SAGA_CALL_IMP_1    (directory, is_checkpoint,                         saga::url);
        SAGA_CALL_IMP_4    (directory, find,                                  std::string, std::vector<std::string>, int, std::string);
        SAGA_CALL_IMP_3    (directory, set_parent,                            saga::url, saga::url, int);
        SAGA_CALL_IMP_2    (directory, get_parent,                            saga::url, int);
        SAGA_CALL_IMP_1    (directory, get_file_num,                          saga::url);
        SAGA_CALL_IMP_1    (directory, list_files,                            saga::url);
        SAGA_CALL_IMP_2    (directory, add_file,                              saga::url, saga::url);
        SAGA_CALL_IMP_2    (directory, get_file,                              saga::url, int);
        SAGA_CALL_IMP_3    (directory, open_file,                             saga::url, saga::url, int);
        SAGA_CALL_IMP_3_EX (directory, open_file,     open_file_idx,          saga::url, int,       int);
        SAGA_CALL_IMP_2    (directory, remove_file,                           saga::url, saga::url);
        SAGA_CALL_IMP_2_EX (directory, remove_file,   remove_file_idx,        saga::url, int);
        SAGA_CALL_IMP_3    (directory, update_file,                           saga::url, saga::url, saga::url);
        SAGA_CALL_IMP_3_EX (directory, update_file,   update_file_idx,        saga::url, int,       saga::url);
        SAGA_CALL_IMP_3    (directory, stage_file,                            saga::url, saga::url, saga::url);
        SAGA_CALL_IMP_3_EX (directory, stage_file,    stage_file_idx,         saga::url, int,       saga::url);
        SAGA_CALL_IMP_2_EX (directory, stage_file,    stage_file_all,         saga::url, saga::url);

        // overload open methods from namespace_dir
        SAGA_CALL_IMP_2    (directory, open,                                  saga::url, int);
        SAGA_CALL_IMP_2    (directory, open_dir,                              saga::url, int);

        /// @endcond

    }   // namespace cpr

    namespace detail
    {
        /////////////////////////////////////////////////////////////////////////////
        //  implement the attribute functions (we need to explicitly specialize 
        //  the template because the functions are not implemented inline)
        template struct SAGA_CPR_PACKAGE_EXPORT_REPEAT attribute<cpr::directory>;

        template struct SAGA_CPR_PACKAGE_EXPORT attribute_priv<cpr::directory, task_base::Sync>;
        template struct SAGA_CPR_PACKAGE_EXPORT attribute_priv<cpr::directory, task_base::Async>;
        template struct SAGA_CPR_PACKAGE_EXPORT attribute_priv<cpr::directory, task_base::Task>;

        template struct SAGA_CPR_PACKAGE_EXPORT attribute_sync<cpr::directory>;
    }

    ///////////////////////////////////////////////////////////////////////////////////
} // namespace saga
#endif

