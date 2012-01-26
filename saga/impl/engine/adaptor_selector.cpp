//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <string>

using namespace std;

#include <saga/impl/config.hpp>
#include <saga/impl/engine/cpi.hpp>
#include <saga/impl/engine/engine.hpp>
#include <saga/impl/engine/adaptor_selector.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl 
{
    /// @cond
    /** Hide from Doxygen */

    namespace adaptor_selector_helpers 
    {
        using v1_0::preference_type;
        using v1_0::cpi_info;

        /*
         * match_prefs:
         *
         * Searches through the prefs, to see if for all of the items in the
         * needed_prefs there is a matching item in the supplied_prefs.
         *
         * Returns true if so, false if not.
         */
        bool match_prefs (preference_type const& session_prefs, 
                          preference_type const& adaptor_prefs) 
        {
            if (adaptor_prefs.data_.empty())
                return true;            // adaptor does not define any constraints
//             we can remove this back-door now as we agreed on to construct a
//             default constructed session as a 'default' session
//             if (session_prefs.data_.empty())
//                 return true;            // session doesn't allow any specifics, assume everything is ok

            preference_type::const_iterator adaptor_end = adaptor_prefs.data_.end();

            // look through session requirements (allowances) to match with
            // adaptor capabilities
            preference_type::const_iterator end = session_prefs.data_.end();
            for (preference_type::const_iterator it = session_prefs.data_.begin(); 
                 it != end; ++it) 
            {
                if (adaptor_prefs.data_.find(*it) != adaptor_end)
                    return true;      // adaptor supports this context type
            }

            // adaptor doesn't support any of the allowed context types
            return false;
        }

        /////////////////////////////////////////////////////////////////////////////
        inline bool 
        match_adaptor_infos(cpi_info        const & info,
                            std::string     const & op_name, 
                            preference_type const & prefs, 
                            v1_0::op_info         & oi)
        {
            // if the adaptor prefs doesn't match, we don't need to check for the ops
            if (!match_prefs (prefs, info.get_prefs()))
                return false;

            // initialization is always implemented (it's the constructor)
            if (op_name == "__init__")
                return true;

            typedef cpi_info::op_list_type::const_iterator op_list_iterator;

            cpi_info::op_list_type const& ops = info.get_ops();
            op_list_iterator op_it = ops.find(v1_0::op_info(op_name));

            if (op_it != ops.end()) 
            {
// FIXME: do we need per operation preferences? I assume not.
//                 if (match_prefs(prefs, (*op_it).get_prefs())) 
                {
                    oi = *op_it;
                    return true;
                }
            }
            return false;
        } 

    } // namespace adaptor_selector_helpers

    /////////////////////////////////////////////////////////////////////////////  
    bool 
    adaptor_selector::match_adaptor (cpi_info        const & info, 
                                     std::string     const & ops_cpi_name, 
                                     std::string     const & op_name, 
                                     preference_type const & prefs, 
                                     op_info               & oi)
    {
        // search adaptors for wanted cpi
        if (info.get_ops_cpi_name() == ops_cpi_name && 
            adaptor_selector_helpers::match_adaptor_infos(info, op_name, prefs, oi))
        {
            return true;
        }
        return false;
    }

    /*
     * select_adaptor:
     *
     * selects a suitable adaptor from the adaptors based on the values defined
     * in the preferences. It returns the maker function of a suitable adaptor.
     *
     * When no suitable adaptor is found, NULL is returned
     */
    adaptor_selector::cpi_info const &
    adaptor_selector::select_adaptor (adaptor_info_list_type const & cpis, 
                                      adaptor_info_list_type const & no_adaptors, 
                                      std::string            const & cpi_name, 
                                      std::string            const & ops_cpi_name, 
                                      std::string            const & op_name, 
                                      preference_type        const & prefs,
                                      op_info                      & oi) 
    {
        /* find adaptors implementing the supplied interface */
        typedef adaptor_info_list_type::const_iterator adaptor_info_list_iterator;

        adaptor_info_list_iterator no_begin = no_adaptors.begin();
        adaptor_info_list_iterator no_end   = no_adaptors.end();

        adaptor_info_list_iterator cpi_end = cpis.end();

        for (adaptor_info_list_iterator cpi_it = cpis.begin(); 
             cpi_it != cpi_end; ++cpi_it)
        {
            // make sure this cpi info belongs to the cpi instance in question
            // if we have to load the adaptor for the constructor (__init__), then we
            // might want to look for the ops_cpi_name as well
            if (cpi_name != (*cpi_it).get_cpi_name() && 
                 (op_name != "__init__" || 
                  ops_cpi_name != (*cpi_it).get_ops_cpi_name())
               )
            {
                continue;
            }

            // make sure this adaptor is not on the no-no list...
            if ((std::find_if (no_begin, no_end, v1_0::same_cpi (*cpi_it)) == no_end) &&
                match_adaptor(*cpi_it, ops_cpi_name, op_name, prefs, oi))
            {
                return *cpi_it;
            }
        }

        SAGA_THROW_VERBATIM(saga::object(), 
                            "Could not select any matching adaptor for: " + cpi_name + "::" + 
                            op_name, saga::adaptors::NoAdaptor);

        static adaptor_selector::cpi_info dummy;
        return dummy;
    }

    // function to test, whether a given cpi matches the expectations
    bool adaptor_selector::test_adaptor(adaptor_info_list_type const & cpis, 
                                        v1_0::cpi              const * cpi_instance, 
                                        std::string            const & ops_cpi_name, 
                                        std::string            const & op_name, 
                                        preference_type        const & prefs, 
                                        op_info                      & oi)
    {
        // find adaptors implementing the supplied interface 
        typedef adaptor_info_list_type::const_iterator adaptor_info_list_iterator;

        // loop through the list of registered functions for this cpi instance 
        // and check, whether it implements the required function
        cpi_info const & info (cpi_instance->get_adaptor_info());
        saga::uuid cpi_uuid (info.get_cpi_id());

        adaptor_info_list_iterator cpi_end = cpis.end();

        for (adaptor_info_list_iterator cpi_it = cpis.begin(); 
             cpi_it != cpi_end; ++cpi_it)
        {
            // make sure this cpi info belongs to the cpi instance in question
            if (cpi_uuid != (*cpi_it).get_cpi_id())
            {
                continue;
            }

            // now test, whether preferences match and function is implemented
            if (match_adaptor(*cpi_it, ops_cpi_name, op_name, prefs, oi))
                return true;
        }
        return false;
    }

    ///@endcond

    ///////////////////////////////////////////////////////////////////////////////
}} // namespace saga::impl


