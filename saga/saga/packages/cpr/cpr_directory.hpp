#ifndef SAGA_SAGA_PACKAGES_DATA_CPR_DIRECTORY_HPP
#define SAGA_SAGA_PACKAGES_DATA_CPR_DIRECTORY_HPP

#if defined(__WAVE__)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/cpr_directory.hpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
//  Copyright (c) 2008 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__)
#pragma wave option(output: null)
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif 

// include stl
#include <string>

// include dependent spec sections
#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/call.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/object.hpp>
#include <saga/saga/session.hpp>
#include <saga/saga/detail/attribute.hpp>

#include <saga/saga/packages/cpr/cpr_checkpoint.hpp>
#include <saga/saga/packages/namespace/namespace_dir.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660 4244)
#endif

#ifdef SAGA_DEBUG
#include <saga/saga/packages/filesystem/preprocessed/cpr_directory.hpp>
#else

#if defined(__WAVE__)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/cpr_directory.hpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga 
{
    
    namespace cpr 
    {
        
        namespace metrics 
        {
            //  metric names for a advert entries
            char const* const directory_created_entry  = "directory.CreatedEntry";
            char const* const directory_modified_entry = "directory.ModifiedEntry";
            char const* const directory_deleted_entry  = "directory.DeletedEntry";
        }

        /*! \brief A checkpoint directory contains, you guess it, checkpoints.
         *
         */
        class SAGA_CPR_PACKAGE_EXPORT directory 
          : public saga::name_space::directory,
            public saga::detail::attribute<directory>
        {
        protected:
            /// @cond
            /** These methods are not within API scope */
            friend class saga::impl::cpr_directory;
            friend struct saga::detail::attribute<directory>;  // needs to access get_impl()
            typedef saga::detail::attribute<directory> attribute_base;
            typedef saga::name_space::directory::monitorable_base monitorable_base;
            /// @endcond

        private:
            /// @cond
            // factory
            SAGA_CALL_CREATE_PRIV_3(session const&, saga::url, int)

            /** These methods are not within API scope */
            SAGA_CALL_PRIV_1 (is_checkpoint, saga::url)
            SAGA_CALL_PRIV_4 (find,          std::string, std::vector<std::string>, int, std::string)
            SAGA_CALL_PRIV_3 (set_parent,    saga::url,   saga::url, int)
            SAGA_CALL_PRIV_2 (get_parent,    saga::url,   int)
            SAGA_CALL_PRIV_1 (get_file_num,  saga::url)
            SAGA_CALL_PRIV_1 (list_files,    saga::url)
            SAGA_CALL_PRIV_2 (add_file,      saga::url,   saga::url)
            SAGA_CALL_PRIV_2 (get_file,      saga::url,   int)
            SAGA_CALL_PRIV_3 (open_file,     saga::url,   int,       int)
            SAGA_CALL_PRIV_3 (open_file,     saga::url,   saga::url, int)
            SAGA_CALL_PRIV_2 (remove_file,   saga::url,   int)
            SAGA_CALL_PRIV_2 (remove_file,   saga::url,   saga::url)
            SAGA_CALL_PRIV_3 (update_file,   saga::url,   int,       saga::url)
            SAGA_CALL_PRIV_3 (update_file,   saga::url,   saga::url, saga::url)
            SAGA_CALL_PRIV_3 (stage_file,    saga::url,   int,       saga::url)
            SAGA_CALL_PRIV_3 (stage_file,    saga::url,   saga::url, saga::url)
            SAGA_CALL_PRIV_2 (stage_file,    saga::url,   saga::url)

            // overload open methods from namespace_dir
            SAGA_CALL_PRIV_2 (open,          saga::url,   int)
            SAGA_CALL_PRIV_2 (open_dir,      saga::url,   int)
            /// @endcond

        protected:
            /// @cond
            /** These methods are not within API scope */
            TR1::shared_ptr <saga::impl::cpr_directory> get_impl_sp() const;
            saga::impl::cpr_directory* get_impl() const;
            explicit directory(saga::impl::cpr_directory *impl);
            /// @endcond

        public:
            /*! \brief Creates the object.     
             *
             *
             */
            directory (session const& s, saga::url url, 
                       int mode = saga::cpr::ReadWrite);

            /*! \brief Creates the object.       
             *
             *
             */
            explicit directory (saga::url url, 
                                int mode = saga::cpr::ReadWrite);

            /*! \brief Creates the object.       
             *
             *
             */
            explicit directory (saga::object const& o);

            /*! \brief Creates the object.       
             *
             *
             */
            directory (void);

            /*! \brief Destroys the object.     
             *
             *
             */
            ~directory (void);

            /*! \brief Factory function resembling directory(session, url, int) 
             *         constructor
             */
            static directory create(session const& s, saga::url url, 
                int mode = Read)
            {
                return directory(s, url, mode);
            }
            SAGA_CALL_CREATE_3_DEF_1(session const&, saga::url, int, Read)

            /*! \brief Factory function resembling directory(url, int) constructor
             */
            static directory create(saga::url url, int mode = Read)
            {
                return directory(url, mode);
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
            directory& operator=(saga::object const& o);
            
            /*! \brief Alias for is_entry of saga::ns_directory     
             *        
             *
             *
             */
            bool is_checkpoint (saga::url url) 
            {
                saga::task t = is_checkpointpriv (url, saga::task_base::Sync()); 
                return t.get_result<bool>();
            }
            /// @cond
            SAGA_CALL_PUB_1_DEF_0 (is_checkpoint, saga::url)
            /// @endcond
            
            /*! \brief Creates a new checkpoint instance.
             *         
             *  \param url File to be opened 
             *  \param flags Flags definition operation modus 
             *  \return Opened file instance
             *
             *
             */
            saga::cpr::checkpoint open (saga::url url, 
                                        int flags = saga::cpr::Read) 
            {
                saga::task t = openpriv (url, flags, saga::task_base::Sync ()); 
                return t.get_result <saga::cpr::checkpoint> ();
            }
            /// @cond
            SAGA_CALL_PUB_2_DEF_1 (open, saga::url, int, saga::cpr::Read)
            /// @endcond
            
            /*! \brief Creates a new directory instance     
             *         
             *  \param url Name of directory to open 
             *  \param flags Flags definition operation modus 
             *  \return Opened directory instance 
             *
             *
             */
            saga::cpr::directory open_dir (saga::url url, 
                                           int flags = saga::cpr::Read) 
            {
                saga::task t = open_dirpriv(url, flags, saga::task_base::Sync()); 
                return t.get_result<saga::cpr::directory>();
            }
            /// @cond
            SAGA_CALL_PUB_2_DEF_1 (open_dir, saga::url, int, saga::cpr::None)
            /// @endcond
            
            /*! \brief Find entries in the current directory and below, with matching
             *         names and matching meta data      
             *  \param name_pattern Pattern for names of entries to be found 
             *  \param meta_pattern Pattern for meta data of entries to be found 
             *  \param flags Flags defining the operation modus 
             *  \return Array of names matching both pattern 
             *
             *
             */
            std::vector<saga::url> find (std::string name_pattern, 
                                         std::vector<std::string> const & meta_pattern = std::vector <std::string>(), 
                                         int         flags = saga::cpr::None, 
                                         std::string spec  = "") 
            {
                saga::task t = findpriv (name_pattern, meta_pattern, flags, spec,
                                         saga::task_base::Sync()); 
                return t.get_result<std::vector<saga::url> >();
            }
            /// @cond
            SAGA_CALL_PUB_4_DEF_3  (find, std::string, 
                                    std::vector<std::string>, 
                                    std::vector<std::string>(),
                                    int, saga::cpr::None, 
                                    std::string, "");
            /// @endcond
            
            /*! \brief set the previous checkpoint url
             *  \param location of checkpoint
             *  \param generation number of generations to go backwards
             *
             *
             */
            void set_parent (saga::url checkpoint, saga::url parent, int generation = 1) 
            {
                set_parentpriv (checkpoint, parent, generation, 
                                saga::task_base::Sync()); 
            }
            /// @cond
            SAGA_CALL_PUB_3_DEF_1 (set_parent, saga::url, saga::url, int, 1)
            /// @endcond
            
            /*! \brief get the previous checkpoint url
             *  \param generation number of generations to go backwards
             *
             *
             */
            saga::url get_parent (saga::url checkpoint, int generation = 1) 
            {
                saga::task t = get_parentpriv (checkpoint, generation, 
                                               saga::task_base::Sync()); 
                return t.get_result <saga::url> ();
            }
            /// @cond
            SAGA_CALL_PUB_2_DEF_1 (get_parent, saga::url, int, 1)
            /// @endcond
            
            
            /*! \brief get number of checkpoint files
             *  \param checkpoint url
             *
             *
             */
            int get_file_num (saga::url checkpoint) 
            {
                saga::task t = get_file_numpriv(checkpoint, saga::task_base::Sync()); 
                return t.get_result <int> ();
            }
            /// @cond
            SAGA_CALL_PUB_1_DEF_0 (get_file_num, saga::url)
            /// @endcond
            
            /*! \brief list the available checkpoint files
             *  \param checkpoint url
             *
             *
             */
            std::vector <saga::url> list_files (saga::url checkpoint) 
            {
                saga::task t = list_filespriv (checkpoint, saga::task_base::Sync()); 
                return t.get_result <std::vector <saga::url> > ();
            }
            /// @cond
            SAGA_CALL_PUB_1_DEF_0 (list_files, saga::url)
            /// @endcond
            
            /*! \brief register a checkpoint file
             *  \param checkpoint url
             *  \param file url
             *
             *
             */
            int add_file (saga::url checkpoint, saga::url url) 
            {
                saga::task t = add_filepriv(checkpoint, url, saga::task_base::Sync()); 
                return t.get_result <int> ();
            }
            /// @cond
            SAGA_CALL_PUB_2_DEF_0 (add_file, saga::url, saga::url)
            /// @endcond
            
            
            /*! \brief get the url for a specific checkpoint file
             *  \param checkpoint url
             *  \param file id
             *
             *
             */
            saga::url get_file (saga::url checkpoint, int id) 
            {
                saga::task t = get_filepriv(checkpoint, id, saga::task_base::Sync()); 
                return t.get_result <saga::url> ();
            }
            /// @cond
            SAGA_CALL_PUB_2_DEF_0 (get_file, saga::url, int)
            /// @endcond
            
            
            /*! \brief open a specific checkpoint file by id
             *  \param checkpoint url
             *  \param file id
             *  \param open flags
             *
             *
             */
            saga::filesystem::file open_file (saga::url checkpoint, int id, 
                                              int flags = CreateParents | Lock | ReadWrite) 
            {
                saga::task t = open_filepriv(checkpoint, id, flags, saga::task_base::Sync()); 
                return t.get_result <saga::filesystem::file> ();
            }
            /// @cond
            SAGA_CALL_PUB_3_DEF_1 (open_file, saga::url, int, int, 
                                   CreateParents | Lock | ReadWrite); 
            /// @endcond
            
            /*! \brief open a specific checkpoint file by name
             *  \param checkpoint url
             *  \param file url
             *  \param open flags
             *
             *
             */
            saga::filesystem::file open_file (saga::url checkpoint, saga::url url, 
                                              int flags = CreateParents | Lock | ReadWrite) 
            {
                saga::task t = open_filepriv(checkpoint, url, flags, saga::task_base::Sync()); 
                return t.get_result <saga::filesystem::file> ();
            }
            /// @cond
            SAGA_CALL_PUB_3_DEF_1 (open_file, saga::url, saga::url, int,
                                   CreateParents | Lock | ReadWrite); 
            /// @endcond
            
            
            /*! \brief remove a specific checkpoint file by id
             *  \param checkpoint url
             *  \param file id
             *
             *
             */
            void remove_file (saga::url checkpoint, int id)
            {
                saga::task t = remove_filepriv(checkpoint, id, saga::task_base::Sync()); 
                t.get_result ();
            }
            /// @cond
            SAGA_CALL_PUB_2_DEF_0 (remove_file, saga::url, int)
            /// @endcond
            
            /*! \brief remove a specific checkpoint file by name
             *  \param checkpoint url
             *  \param file url
             *
             *
             */
            void remove_file (saga::url checkpoint, saga::url url)
            {
                saga::task t = remove_filepriv(checkpoint, url, saga::task_base::Sync()); 
                t.get_result ();
            }
            /// @cond
            SAGA_CALL_PUB_2_DEF_0 (remove_file, saga::url, saga::url)
            /// @endcond
            
            
            /*! \brief update a specific checkpoint file by id
             *  \param checkpoint url
             *  \param file id
             *  \param new url
             *
             *
             */
            void update_file (saga::url checkpoint, int id, saga::url new_url)
            {
                saga::task t = update_filepriv(checkpoint, id, new_url, saga::task_base::Sync()); 
                t.get_result ();
            }
            /// @cond
            SAGA_CALL_PUB_3_DEF_0 (update_file, saga::url, int, saga::url)
            /// @endcond
            
            /*! \brief update a specific checkpoint file by name
             *  \param checkpoint url
             *  \param old url
             *  \param new url
             *
             *
             */
            void update_file (saga::url checkpoint, saga::url old_url, saga::url new_url)
            {
                saga::task t = update_filepriv(checkpoint, old_url, new_url, saga::task_base::Sync()); 
                t.get_result ();
            }
            /// @cond
            SAGA_CALL_PUB_3_DEF_0 (update_file, saga::url, saga::url, saga::url)
            /// @endcond
            
            
            /*! \brief stage a specific checkpoint file by id
             *  \param checkpoint url
             *  \param file id
             *  \param target url
             *
             *
             */
            void stage_file (saga::url checkpoint, int id, saga::url new_url)
            {
                saga::task t = stage_filepriv(checkpoint, id, new_url, saga::task_base::Sync()); 
                t.get_result ();
            }
            /// @cond
            SAGA_CALL_PUB_3_DEF_0 (stage_file, saga::url, int, saga::url)
            /// @endcond
            
            /*! \brief stage a specific checkpoint file by name
             *  \param checkpoint url
             *  \param source url
             *  \param target url
             *
             *
             */
            void stage_file (saga::url checkpoint, saga::url old_url, saga::url new_url)
            {
                saga::task t = stage_filepriv(checkpoint, old_url, new_url, saga::task_base::Sync()); 
                t.get_result ();
            }
            /// @cond
            SAGA_CALL_PUB_3_DEF_0 (stage_file, saga::url, saga::url, saga::url)
            /// @endcond
            
            /*! \brief stage all checkpoint files
             *  \param checkpoint url
             *  \param target url
             *
             *
             */
            void stage_file (saga::url checkpoint, saga::url new_url)
            {
                saga::task t = stage_filepriv(checkpoint, new_url, saga::task_base::Sync()); 
                t.get_result ();
            }
            /// @cond
            SAGA_CALL_PUB_2_DEF_0 (stage_file, saga::url, saga::url)
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

#endif // SAGA_SAGA_PACKAGES_DATA_CPR_DIRECTORY_HPP
