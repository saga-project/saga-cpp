#ifndef SAGA_PACKAGES_DATA_CPR_CHECKPOINT_HPP
#define SAGA_PACKAGES_DATA_CPR_CHECKPOINT_HPP

#if defined(__WAVE__)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/cpr_checkpoint.hpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
//  Copyright (c) 2008 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__)
#pragma wave option(preserve: 0, output: null)
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif 

#include <string>
#include <vector>

// include dependent spec sections
#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/session.hpp>
#include <saga/saga/call.hpp>
#include <saga/saga/filesystem.hpp>
#include <saga/saga/detail/attribute.hpp>

#include <saga/saga/packages/namespace/namespace_entry.hpp>
#include <saga/saga/packages/cpr/config.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660 4244)
#endif

#ifdef SAGA_DEBUG
#include <saga/saga/packages/cpr/preprocessed/cpr_checkpoint.hpp>
#else

#if defined(__WAVE__)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/cpr_checkpoint.hpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga
{
    /*! \brief The <I>Checkpoint and Recovery</I> (CPR) API package
     *
     *
     */
    namespace cpr 
    {
        
        namespace metrics 
        {
            //  metric names for a advert entries
            char const* const checkpoint_modified = "checkpoint.Modified";
            char const* const checkpoint_deleted  = "checkpoint.Deleted";
        }
        
        
        /*! \brief The flags enum is inherited from the namespace package.    
         *
         *   A number of cpr specific flags are added to it. All added 
         * flags are used for the opening of checkpoint and checkpoint directory 
         * instances, and are not applicable to the operations inherited from
         * the namespace package. 
         */
        enum flags 
        {
            // Unknown         = /*  -1, */  saga::name_space::Unknown       ,
            None            = /*   0, */  saga::name_space::None          ,
            Overwrite       = /*   1, */  saga::name_space::Overwrite     ,
            Recursive       = /*   2, */  saga::name_space::Recursive     ,
            Dereference     = /*   4, */  saga::name_space::Dereference,
            Create          = /*   8, */  saga::name_space::Create        ,
            Exclusive       = /*  16, */  saga::name_space::Exclusive     ,
            Lock            = /*  32, */  saga::name_space::Lock          ,
            CreateParents   = /*  64, */  saga::name_space::CreateParents ,
            Truncate        =    128,
            Append          =    256, 
            Read            =    512,
            Write           =   1024,
            ReadWrite       =   Read | Write
            // Binary          =   2048      // reserved
        };             
        
        namespace attributes 
        {
            /*! \brief <b>[rw]</b> time of creation
             */
            char const* const cpr_time             = "Time";

            /*! \brief <b>[ro]</b> number of files in checkpoint
             */
            char const* const cpr_nfliles          = "NFiles";

            /*! \brief <b>[ro]</b> checkpoint mode (Full, Incremental)
             */
            char const* const cpr_mode             = "Mode";

            /*! \brief <b>[rw]</b> possible Mode values
             */
            char const* const cpr_mode_full        = "Full";
            char const* const cpr_mode_incremental = "Incremental";

            /*! \brief <b>[ro]</b> Reference for incremental checkpoints.  For full
             * checkpoints, points to previous checkpoint (Incremental or full).
             */
            char const* const cpr_parent           = "Parent";

            /*! \brief <b>[ro]</b> checkpoints following this checkpoint (if known)
             */
            char const* const cpr_children         = "Children";
        }

        /*! \brief The saga::replica class implements the saga::attributes interface.     
         *
         *  A checkpoint represents merely an entry in a name space which 
         * has (a) an associated set of registered (physical) checkpoint files of that
         * checkpoint, and (b) an associated set of meta data describing that
         * checkpoint. Both sets can be empty. To access the content of a checkpoint,
         * a saga::file needs to be created with one of the registered file locations. 
         */
        class SAGA_CPR_PACKAGE_EXPORT checkpoint 
        : public saga::name_space::entry,
        public saga::detail::attribute<checkpoint>
        {
        protected:
            /// @cond
            /** These methods are not within API scope */
            friend class saga::impl::cpr_checkpoint;
            friend struct saga::detail::attribute<checkpoint>;  // needs to access get_impl()
            typedef saga::detail::attribute<checkpoint> attribute_base;
            typedef saga::name_space::entry::monitorable_base monitorable_base;
            /// @endcond
            
        private:
            /// @cond
            // factory
            SAGA_CALL_CREATE_PRIV_3(session const&, saga::url, int)

            /** These methods are not within API scope */
            SAGA_CALL_PRIV_2 (set_parent,  saga::url, int)
            SAGA_CALL_PRIV_1 (get_parent,  int)
            SAGA_CALL_PRIV_0 (get_file_num)
            SAGA_CALL_PRIV_0 (list_files)
            SAGA_CALL_PRIV_1 (add_file,    saga::url)
            SAGA_CALL_PRIV_1 (get_file,    int)
            SAGA_CALL_PRIV_2 (open_file,   int,       int)
            SAGA_CALL_PRIV_2 (open_file,   saga::url, int)
            SAGA_CALL_PRIV_1 (remove_file, int)
            SAGA_CALL_PRIV_1 (remove_file, saga::url)
            SAGA_CALL_PRIV_2 (update_file, int,       saga::url)
            SAGA_CALL_PRIV_2 (update_file, saga::url, saga::url)
            SAGA_CALL_PRIV_2 (stage_file,  int,       saga::url)
            SAGA_CALL_PRIV_2 (stage_file,  saga::url, saga::url)
            SAGA_CALL_PRIV_1 (stage_file,  saga::url)
            /// @endcond

        protected:
            /// @cond
            /** These methods are not within API scope */
            TR1::shared_ptr <saga::impl::cpr_checkpoint> get_impl_sp(void) const;
            saga::impl::cpr_checkpoint* get_impl (void) const;
            explicit checkpoint(saga::impl::cpr_checkpoint *impl);
            /// @endcond

        public:
            /*! \brief Creates the object.      
             *
             *
             */
            checkpoint (session const& s, saga::url url, int mode = Read);

            /*! \brief Creates the object.      
             *
             *
             */
            explicit checkpoint (saga::url url, int mode = Read);

            /*! \brief Creates the object.      
             *
             *
             */
            explicit checkpoint (saga::object const& o);

            /*! \brief Creates the object.      
             *
             *
             */
            checkpoint (void);

            /*! \brief Destroys the object. 
             *
             *
             */
            ~checkpoint (void);

            /*! \brief Factory function resembling checkpoint(session, url, int) 
             *         constructor
             */
            static checkpoint create(session const& s, saga::url url, 
                int mode = Read)
            {
                return checkpoint(s, url, mode);
            }
            SAGA_CALL_CREATE_3_DEF_1(session const&, saga::url, int, Read)

            /*! \brief Factory function resembling checkpoint(url, int) constructor
             */
            static checkpoint create(saga::url url, int mode = Read)
            {
                return checkpoint(url, mode);
            }
            template <typename Tag>
            static saga::task create(saga::url url, int mode = Read)
            {
                return create<Tag>(detail::get_the_session(), url, mode);
            }

            /*! \brief Brief %description starts here     
             *
             *
             */
            checkpoint& operator=(saga::object const& o);
            
            /*! \brief set the previous checkpoint url
             *  \param generation number of generations to go backwards
             *
             *
             */
            void set_parent (saga::url url, int generation = 1) 
            {
                saga::task t = set_parentpriv(url, generation, saga::task_base::Sync()); 
                t.get_result ();
            }
            /// @cond
            SAGA_CALL_PUB_2_DEF_1 (set_parent, saga::url, int, 1)
            /// @endcond
            
            /*! \brief get the previous checkpoint url
             *  \param generation number of generations to go backwards
             *
             *
             */
            saga::url get_parent (int generation = 1) 
            {
                saga::task t = get_parentpriv(generation, saga::task_base::Sync()); 
                return t.get_result <saga::url> ();
            }
            /// @cond
            SAGA_CALL_PUB_1_DEF_1 (get_parent, int, 1)
            /// @endcond
            
            
            /*! \brief list the available checkpoint files
             *
             *
             */
            std::vector <saga::url> list_files (void) 
            {
                saga::task t = list_filespriv (saga::task_base::Sync()); 
                return t.get_result <std::vector <saga::url> > ();
            }
            /// @cond
            SAGA_CALL_PUB_0_DEF_0 (list_files)
            /// @endcond
            
            /*! \brief register a checkpoint file
             *  \param file url
             *
             *
             */
            int add_file (saga::url url) 
            {
                saga::task t = add_filepriv(url, saga::task_base::Sync()); 
                return t.get_result <int> ();
            }
            /// @cond
            SAGA_CALL_PUB_1_DEF_0 (add_file, saga::url)
            /// @endcond
            
            
            /*! \brief get the url for a specific checkpoint file
             *  \param file id
             *
             *
             */
            saga::url get_file (int id) 
            {
                saga::task t = get_filepriv(id, saga::task_base::Sync()); 
                return t.get_result <saga::url> ();
            }
            /// @cond
            SAGA_CALL_PUB_1_DEF_0 (get_file, int)
            /// @endcond
            
            
            /*! \brief open a specific checkpoint file by id
             *  \param file id
             *  \param open flags
             *
             *
             */
            saga::filesystem::file open_file (int id, 
                                              int flags = CreateParents | Lock | ReadWrite) 
            {
                saga::task t = open_filepriv(id, flags, saga::task_base::Sync()); 
                return t.get_result <saga::filesystem::file> ();
            }
            /// @cond
            SAGA_CALL_PUB_2_DEF_1 (open_file, int, int, CreateParents | Lock | ReadWrite )
            /// @endcond
            
            /*! \brief open a specific checkpoint file by name
             *  \param file url
             *  \param open flags
             *
             *
             */
            saga::filesystem::file open_file (saga::url url, 
                                              int flags = CreateParents | Lock | ReadWrite) 
            {
                saga::task t = open_filepriv(url, flags, saga::task_base::Sync()); 
                return t.get_result <saga::filesystem::file> ();
            }
            /// @cond
            SAGA_CALL_PUB_2_DEF_1 (open_file, saga::url, int, CreateParents | Lock | ReadWrite)
            /// @endcond
            
            
            /*! \brief remove a specific checkpoint file by id
             *  \param checkpoint url
             *  \param file id
             *
             *
             */
            void remove_file (int id)
            {
                saga::task t = remove_filepriv(id, saga::task_base::Sync()); 
                t.get_result ();
            }
            /// @cond
            SAGA_CALL_PUB_1_DEF_0 (remove_file, int)
            /// @endcond
            
            /*! \brief remove a specific checkpoint file by name
             *  \param file url
             *
             *
             */
            void remove_file (saga::url url)
            {
                saga::task t = remove_filepriv(url, saga::task_base::Sync()); 
                t.get_result ();
            }
            /// @cond
            SAGA_CALL_PUB_1_DEF_0 (remove_file, saga::url)
            /// @endcond
            
            
            /*! \brief update a specific checkpoint file by id
             *  \param file id
             *  \param new url
             *
             *
             */
            void update_file (int id, saga::url new_url)
            {
                saga::task t = update_filepriv(id, new_url, saga::task_base::Sync()); 
                t.get_result ();
            }
            /// @cond
            SAGA_CALL_PUB_2_DEF_0 (update_file, int, saga::url)
            /// @endcond
            
            /*! \brief update a specific checkpoint file by name
             *  \param old url
             *  \param new url
             *
             *
             */
            void update_file (saga::url old_url, saga::url new_url)
            {
                saga::task t = update_filepriv(old_url, new_url, saga::task_base::Sync()); 
                t.get_result ();
            }
            /// @cond
            SAGA_CALL_PUB_2_DEF_0 (update_file, saga::url, saga::url)
            /// @endcond
            
            
            /*! \brief stage a specific checkpoint file by id
             *  \param file id
             *  \param target url
             *
             *
             */
            void stage_file (int id, saga::url new_url)
            {
                saga::task t = stage_filepriv(id, new_url, saga::task_base::Sync()); 
                t.get_result ();
            }
            /// @cond
            SAGA_CALL_PUB_2_DEF_0 (stage_file, int, saga::url)
            /// @endcond
            
            /*! \brief stage a specific checkpoint file by name
             *  \param source url
             *  \param target url
             *
             *
             */
            void stage_file (saga::url old_url, saga::url new_url)
            {
                saga::task t = stage_filepriv(old_url, new_url, saga::task_base::Sync()); 
                t.get_result ();
            }
            /// @cond
            SAGA_CALL_PUB_2_DEF_0 (stage_file, saga::url, saga::url)
            /// @endcond
            
            /*! \brief stage all checkpoint files
             *  \param target url
             *
             *
             */
            void stage_file (saga::url new_url)
            {
                saga::task t = stage_filepriv(new_url, saga::task_base::Sync()); 
                t.get_result ();
            }
            /// @cond
            SAGA_CALL_PUB_1_DEF_0 (stage_file, saga::url)
            /// @endcond
        }; 
        
    }}   

#if defined(__WAVE__)
#pragma wave option(output: null)
#endif

#endif // !defined(SAGA_DEBUG)

// re-enable warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // !defined(SAGA_PACKAGES_DATA_CPR_CHECKPOINT_HPP)

