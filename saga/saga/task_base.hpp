//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_TASK_BASE_HPP
#define SAGA_TASK_BASE_HPP

#include <saga/saga/util.hpp>
#include <boost/mpl/has_xxx.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga 
{
    namespace attributes
    {
        // textual representation of task states, required as metric values
        char const* const task_state_unknown = "Unknown";
        char const* const task_state_new = "New";
        char const* const task_state_running = "Running";
        char const* const task_state_done = "Done";
        char const* const task_state_canceled = "Canceled";
        char const* const task_state_failed = "Failed";
    }

    /*! \brief Brief %description starts here     
     *
     *
     */
    struct task_base 
    {
        /*! \brief A task can be in one of several possible states: New, Running, Done, Canceled and Failed.     
         *
         *  New:  This state identifies a newly constructed task instance which has not yet
         * run. This state corresponds to the BES state ’Pending’. This state is initial. 
         *
         * Running:  The run() method has been invoked on the task, either explicitly or 
         * implicitly. This state corresponds to the BES state ’Running’. This state is initial. 
         *
         * Done:  The synchronous or asynchronous operation has finished successfully. It 
         * corresponds to the BES state ’Finished’. This state is final. 
         *
         * Canceled: The asynchronous operation has been canceled, i.e. cancel() has been 
         * called on the task instance. It corresponds to the BES state ’Canceled’. This state is final. 
         *
         * Failed:  The synchronous or asynchronous operation has finished unsuccessfully. 
         * It corresponds to the BES state ’Failed’. This state is final. 
         */
        enum state 
        {
            Unknown   = -1,
            New       =  1,
            Running   =  2,
            Done      =  3,
            Canceled  =  4,
            Failed    =  5
        };

        /// @cond
        /** These methods are not within API scope */
        // tagging definitions
        struct Sync { struct sync_tag; };
        struct Async { struct sync_tag; };
        struct Task  { struct sync_tag; };
        /// @endcond
    };

    /// @cond
    namespace detail
    {
        BOOST_MPL_HAS_XXX_TRAIT_DEF(sync_tag)
    }

    namespace adaptors
    {
        task_base::state SAGA_EXPORT task_state_value_to_enum(std::string const& val);
        std::string SAGA_EXPORT task_state_enum_to_value(int s);
    }
    /// @endcond
///////////////////////////////////////////////////////////////////////////////
} // namespace saga

#endif  // SAGA_TASK_BASE_HPP

