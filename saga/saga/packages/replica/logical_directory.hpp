#ifndef SAGA_SAGA_PACKAGES_DATA_REPLICA_LOGICAL_DIRECTORY_HPP
#define SAGA_SAGA_PACKAGES_DATA_REPLICA_LOGICAL_DIRECTORY_HPP

#if defined(__WAVE__)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/logical_directory.hpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
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

#include <saga/saga/packages/namespace/namespace_dir.hpp>
#include <saga/saga/packages/replica/logical_file.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

#ifdef SAGA_DEBUG
#include <saga/saga/packages/replica/preprocessed/logical_directory.hpp>
#else

#if defined(__WAVE__)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/logical_directory.hpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga 
{
    namespace replica 
    {
        
        /////////////////////////////////////////////////////////////////////////////
        
        namespace metrics 
        {
            ///////////////////////////////////////////////////////////////////////////
            //  metric names for a advert entries
            char const* const logical_directory_created_entry = "logical_directory.CreatedEntry";
            char const* const logical_directory_modified_entry = "logical_directory.ModifiedEntry";
            char const* const logical_directory_deleted_entry = "logical_directory.DeletedEntry";
        }
        
        
        /*! \brief A logical directory represents a directory entry in the
         * name space of logical files.   
         *
         *   Several replica system implementations have the notion of containers,
         * which, for our purposes, represent directories which can 
         * have, just as logical files, associated sets of meta data. In the presented
         * API, logical directories and containers are the same.
         */
        class SAGA_REPLICA_PACKAGE_EXPORT logical_directory 
        : public saga::name_space::directory,
        public saga::detail::attribute<logical_directory>
        {
        protected:
            /// @cond
            /** These methods are not within API scope */
            friend class saga::impl::logical_directory;
            friend struct saga::detail::attribute<logical_directory>;  // needs to access get_impl()
            typedef saga::detail::attribute<logical_directory> attribute_base;
            typedef saga::name_space::directory::monitorable_base monitorable_base;
            /// @endcond                  
            
        private:
            /// @cond
            // factory
            SAGA_CALL_CREATE_PRIV_3(session const&, saga::url, int)

            /** These methods are not within API scope */
            SAGA_CALL_PRIV_1(is_file, saga::url)

            // overload open methods from namespace_dir
            SAGA_CALL_PRIV_2(open, saga::url, int)
            SAGA_CALL_PRIV_2(open_dir, saga::url, int)
            SAGA_CALL_PRIV_3(find, std::string, std::vector<std::string>, int)
            /// @endcond
            
        protected:
            /// @cond
            /** These methods are not within API scope */
            TR1::shared_ptr <saga::impl::logical_directory> get_impl_sp() const;
            saga::impl::logical_directory* get_impl() const;
            explicit logical_directory(saga::impl::logical_directory *impl);
            /// @endcond
            
        public:
            /*! \brief Creates the object.
             *
             *
             */
            logical_directory (session const& s, saga::url url, 
                               int mode = saga::replica::ReadWrite);

            /*! \brief Creates the object.
             *
             *
             */
            explicit logical_directory (saga::url url, 
                                        int mode = saga::replica::ReadWrite);

            /*! \brief Creates the object.
             *
             *
             */
            explicit logical_directory (saga::object const& o);

            /*! \brief Creates the object.
             *
             *
             */
            logical_directory (void);

            /*! \brief Destroys the object.
             *
             *
             */
            ~logical_directory (void);

            /*! \brief Factory function resembling logical_directory(session, url, int) 
             *         constructor
             */
            static logical_directory create(session const& s, saga::url url, 
                int mode = Read)
            {
                return logical_directory(s, url, mode);
            }
            SAGA_CALL_CREATE_3_DEF_1(session const&, saga::url, int, Read)

            /*! \brief Factory function resembling logical_directory(url, int) constructor
             */
            static logical_directory create(saga::url url, int mode = Read)
            {
                return logical_directory(url, mode);
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
            logical_directory& operator=(saga::object const& o);

            /*! \brief Alias for is_entry of saga::ns_directory     
             *         
             *
             *
             */
            bool is_file(saga::url url) 
            {
                saga::task t = is_filepriv(url, saga::task_base::Sync()); 
                return t.get_result<bool>();
            }
            SAGA_CALL_PUB_1_DEF_0(is_file, saga::url)
            
            /*! \brief Creates a new logical_file instance.
             *         
             *  \param url File to be opened 
             *  \param flags Flags definition operation modus 
             *  \return Opened file instance
             *
             *
             */
            saga::replica::logical_file open(saga::url url, 
                                             int flags = saga::replica::Read) 
            {
                saga::task t = openpriv(url, flags, saga::task_base::Sync()); 
                return t.get_result<saga::replica::logical_file>();
            }
            SAGA_CALL_PUB_2_DEF_1(open, saga::url, int, saga::replica::Read)
            
            /*! \brief Creates a new logical_directory instance     
             *         
             *  \param url Name of directory to open 
             *  \param flags Flags definition operation modus 
             *  \return Opened directory instance 
             *
             *
             */
            saga::replica::logical_directory open_dir(saga::url url, 
                                                      int flags
                                                      = saga::replica::Read) 
            {
                saga::task t = open_dirpriv(url, flags, saga::task_base::Sync()); 
                return t.get_result<saga::replica::logical_directory>();
            }
            SAGA_CALL_PUB_2_DEF_1(open_dir, saga::url, int, saga::replica::Read)
            
            /*! \brief Find entries in the current directory and below, with matching
             *         names and matching meta data      
             *         
             *  \param name_pattern Pattern for names of entries to be found 
             *  \param key_pattern Pattern for meta data of entries to be found 
             *  \param flags Flags defining the operation modus 
             *  \return Array of names matching both pattern 
             *
             *
             */
            std::vector<saga::url> find(std::string name_pattern, 
                                        std::vector<std::string> key_pattern, 
                                        int flags = saga::replica::Recursive) 
            {
                saga::task t = findpriv(name_pattern, key_pattern, flags,
                                        saga::task_base::Sync()); 
                return t.get_result<std::vector<saga::url> >();
            }
            SAGA_CALL_PUB_3_DEF_1(find, std::string, std::vector<std::string>, 
                                  int, saga::replica::None)
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

#endif // SAGA_SAGA_PACKAGES_DATA_REPLICA_LOGICAL_DIRECTORY_HPP
