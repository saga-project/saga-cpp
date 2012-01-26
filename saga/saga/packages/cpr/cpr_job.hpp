#ifndef SAGA_PACKAGES_CPR_CPRJOB_HPP
#define SAGA_PACKAGES_CPR_CPRJOB_HPP

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/cpr_job.hpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
//  Copyright (c) 2008 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#include <string>
#include <vector>
#include <iosfwd>

// include dependent spec sections
#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/session.hpp>
#include <saga/saga/call.hpp>
#include <saga/saga/job.hpp>

#include <saga/saga/packages/cpr/config.hpp>

#include <saga/saga/detail/attribute.hpp>
#include <saga/saga/detail/permissions.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

#ifdef SAGA_DEBUG
#include <saga/saga/packages/cpr/preprocessed/cpr_job.hpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/cpr_job.hpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga
{
    /*! \brief The <I>Checkpoint and Recovery</I> (CPR) API package
     *
     *
     */
    namespace cpr {
        
        /*! \brief  Available %attributes defined for the CPR API package
         *   
         */     
        namespace attributes 
        {
            //  inherited from saga::job::job?
        }

        /*! \brief   Available %metrics defined for the CPR API package.
         *
         *   Most cp::job metrics are inherited from the job::job class, and only 
         *   four are added to simplify application level checkpoiint management:
         *   checkpoint, checkpointed, recover, and recovered. <br>
         *   <br><b>Usage example:</b><br><br>
         *   <code>//TODO</code>
         */    
        namespace metrics 
        { 
            /*! \brief Fires if the job gets a checkpoint request.
             */
            char const* const checkpoint   = "cpr_job.Checkpoint";

            /*! \brief Fires if the job finishes a checkpoint request.
             */
            char const* const checkpointed = "cpr_job.Checkpointed";

            /*! \brief Fires if the job gets a recover request.
             */
            char const* const recover      = "cpr_job.Recover";

            /*! \brief Fires if the job finishes a recover request.
             */
            char const* const recovered    = "cpr_job.Recovered";
        }

        // /*! \brief The state is equivalent to the inherited 
        //  * saga::job::state, but adds the Checkpointing and Recovering state.
        //  *
        //  * Checkpointing identifies a job instance which is in
        //  *   the process of checkpointing.
        //  *
        //  * Recovering identifies a job instance which is in
        //  *   the process of recovering.
        //  */
        enum state 
        {
            Unknown        =  saga::job::Unknown,   // -1
            New            =  saga::job::New,       //  0
            Running        =  saga::job::Running,   //  1
            Failed         =  saga::job::Failed,    //  2
            Done           =  saga::job::Done,      //  3
            Canceled       =  saga::job::Canceled,  //  4
            Suspended      =  saga::job::Suspended, //  5
            Checkpointing  =  6,
            Recovering     =  7
        };    

        /*! \brief The job is a saga::job with additional checkpoint and
         * recovery related methods.
         *
         */   
        class SAGA_CPR_PACKAGE_EXPORT job 
            : public saga::job::job
        {
        private:
            friend struct saga::detail::attribute<job>;  // needs to access get_impl()
            friend struct saga::detail::permissions<job>;

            /// @cond
            // job cpr management
            SAGA_CALL_PRIV_1 (checkpoint,    saga::url)
            SAGA_CALL_PRIV_1 (recover,       saga::url)

            SAGA_CALL_PRIV_1 (cpr_stage_in,  saga::url)
            SAGA_CALL_PRIV_1 (cpr_stage_out, saga::url)

            SAGA_CALL_PRIV_0 (cpr_last)
            SAGA_CALL_PRIV_0 (cpr_list)
            /// @endcond

        protected:
            /// @cond
            /** These methods are not within API scope */
            typedef saga::detail::attribute<job> attribute_base;
            TR1::shared_ptr <saga::impl::cpr_job> get_impl_sp(void) const;
            saga::impl::cpr_job* get_impl (void) const;
            explicit job (saga::impl::cpr_job *);
            /// @endcond

        public:
            /*! \brief Creates the object.     
             *
             *
             */
            job();

            /*! \brief Creates the object.     
             *
             *
             */
            explicit job (saga::object const& o);

            /*! \brief Destroys the object.     
             *
             *
             */
            ~job();

            /*! \brief Brief %description starts here     
             *
             *
             */
            job &operator= (saga::object const& o);

            /*! \brief checkpoint a job
             *  \return void
             *
             *
             */
            void checkpoint (saga::url u = saga::url ()) 
            {
                checkpointpriv (u, saga::task_base::Sync()); 
            }
            /// @cond
            SAGA_CALL_PUB_1_DEF_1 (checkpoint, saga::url, saga::url ())
            /// @endcond

            /*! \brief recover a job
             *  \return void
             *
             *
             */
            void recover (saga::url u = saga::url ()) 
            {
                recoverpriv (u, saga::task_base::Sync()); 
            }
            /// @cond
            SAGA_CALL_PUB_1_DEF_1 (recover, saga::url, saga::url ())
            /// @endcond

            /*! \brief stage-in a checkpoint file
             *  \return void
             *
             *
             */
            void cpr_stage_in (saga::url u = saga::url ()) 
            {
                cpr_stage_inpriv (u, saga::task_base::Sync()); 
            }
            /// @cond
            SAGA_CALL_PUB_1_DEF_1 (cpr_stage_in, saga::url, saga::url ())
            /// @endcond

            /*! \brief stage-out a checkpoint file
             *  \return void
             *
             *
             */
            void cpr_stage_out (saga::url u = saga::url ()) 
            {
                cpr_stage_outpriv (u, saga::task_base::Sync()); 
            }
            /// @cond
            SAGA_CALL_PUB_1_DEF_1 (cpr_stage_out, saga::url, saga::url ())
            /// @endcond

            /*! \brief get list of known checkpoints
             *  \return list of known checkpoint URLs
             *
             *
             */
            std::vector <saga::url> cpr_list (void) 
            {
                saga::task t = cpr_listpriv(saga::task_base::Sync()); 
                return t.get_result <std::vector <saga::url> > ();
            }
            /// @cond
            SAGA_CALL_PUB_0_DEF_0 (cpr_list)
            /// @endcond

            /*! \brief get last known checkpoint
             *  \return last known checkpoint URL
             *
             *
             */
            saga::url cpr_last (void) 
            {
                saga::task t = cpr_lastpriv(saga::task_base::Sync()); 
                return t.get_result <saga::url> ();
            }
            /// @cond
            SAGA_CALL_PUB_0_DEF_0 (cpr_last)
            /// @endcond
        };

    }   // namespace job
}   // namespace saga

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif // !defined(SAGA_DEBUG)

// re-enable warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // !defined(SAGA_PACKAGES_CPR_CPRJOB_HPP)

