#ifndef SAGA_PACKAGES_DATA_REPLICA_LOGICAL_FILE_HPP
#define SAGA_PACKAGES_DATA_REPLICA_LOGICAL_FILE_HPP

#if defined(__WAVE__)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/logical_file.hpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
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
#include <saga/saga/detail/attribute.hpp>

#include <saga/saga/packages/namespace/namespace_entry.hpp>
#include <saga/saga/packages/replica/config.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

#ifdef SAGA_DEBUG
#include <saga/saga/packages/replica/preprocessed/logical_file.hpp>
#else

#if defined(__WAVE__)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/logical_file.hpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga
{    
    /*! \brief The <I>Replica Management</I> API package
     *
     *   Detailed %description here...
     */  
    namespace replica 
    {
        
        /////////////////////////////////////////////////////////////////////////////
        /*! \brief Available %metrics defined for the Replica Management API package
         *
         */
        namespace metrics 
        {
            char const* const logical_file_modified = "logical_file.Modified";
            char const* const logical_file_deleted = "logical_file.Deleted";
        }
        
        /*! \brief The flags enum is inherited from the namespace package.    
         *
         *   A number of replica specific flags are added to it. All added 
         * flags are used for the opening of logical file and logical directory 
         * instances, and are not applicable to the operations inherited from
         * the namespace package. 
         */
        enum flags 
        {
            Unknown         = /*  -1, */  saga::name_space::Unknown       ,
            None            = /*   0, */  saga::name_space::None          ,
            Overwrite       = /*   1, */  saga::name_space::Overwrite     ,
            Recursive       = /*   2, */  saga::name_space::Recursive     ,
            Dereference     = /*   4, */  saga::name_space::Dereference,
            Create          = /*   8, */  saga::name_space::Create        ,
            Exclusive       = /*  16, */  saga::name_space::Exclusive     ,
            Lock            = /*  32, */  saga::name_space::Lock          ,
            CreateParents   = /*  64, */  saga::name_space::CreateParents ,
            // 128, reserved for Truncate
            // 256,  reserved for Append 
            Read            =    512,
            Write           =   1024,
            ReadWrite       =   Read | Write
            // 2048, reserved for Binary
        };             
        
        /*! \brief The saga::replica class implements the saga::attributes interface.     
         *
         *  A logical file represents merely an entry in a name space which 
         * has (a) an associated set of registered (physical) replicas of that
         * file, and (b) an associated set of meta data describing that logical
         * file. Both sets can be empty. To access the content of a logical file,
         * a saga::file needs to be created with one of the registered replica locations. 
         */
        class SAGA_REPLICA_PACKAGE_EXPORT logical_file 
          : public saga::name_space::entry,
            public saga::detail::attribute<logical_file>
        {
        protected:
            /// @cond
            /** These methods are not within API scope */
            friend class saga::impl::logical_file;
            friend struct saga::detail::attribute<logical_file>;  // needs to access get_impl()
            typedef saga::detail::attribute<logical_file> attribute_base;
            typedef saga::name_space::entry::monitorable_base monitorable_base;
            /// @endcond
            
        private:
            /// @cond
            // factory
            SAGA_CALL_CREATE_PRIV_3(session const&, saga::url, int)

            /** These methods are not within API scope */
            SAGA_CALL_PRIV_1(add_location, saga::url)
            SAGA_CALL_PRIV_1(remove_location, saga::url)
            SAGA_CALL_PRIV_2(update_location, saga::url, saga::url)
            SAGA_CALL_PRIV_0(list_locations)
            SAGA_CALL_PRIV_2(replicate, saga::url, int)
            /// @endcond

        protected:
            /// @cond
            /** These methods are not within API scope */
            TR1::shared_ptr <saga::impl::logical_file> get_impl_sp(void) const;
            saga::impl::logical_file* get_impl (void) const;
            explicit logical_file(saga::impl::logical_file *impl);
            /// @endcond

        public:
            /*! \brief Creates the object.
             *
             *
             */
            logical_file (session const& s, saga::url url, int mode = Read);

            /*! \brief Creates the object.
             *
             *
             */
            explicit logical_file (saga::url url, int mode = Read);

            /*! \brief Creates the object.
             *
             *
             */
            explicit logical_file (saga::object const& o);

            /*! \brief Creates the object.
             *
             *
             */
            logical_file (void);

            /*! \brief Destroys the object. 
             *
             *
             */
            ~logical_file (void);

            /*! \brief Factory function resembling logical_file(session, url, int) 
             *         constructor
             */
            static logical_file create(session const& s, saga::url url, 
                int mode = Read)
            {
                return logical_file(s, url, mode);
            }
            SAGA_CALL_CREATE_3_DEF_1(session const&, saga::url, int, Read)

            /*! \brief Factory function resembling logical_file(url, int) constructor
             */
            static logical_file create(saga::url url, int mode = Read)
            {
                return logical_file(url, mode);
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
            logical_file& operator=(saga::object const& o);

            /*! \brief Add a replica location to the replica set     
             *         
             *  \param url Location to add to set 
             *
             *
             */
            void add_location(saga::url url) 
            {
                saga::task t = add_locationpriv(url, saga::task_base::Sync()); 
                t.get_result ();
            }
            SAGA_CALL_PUB_1_DEF_0(add_location, saga::url)

            /*! \brief Remove a replica location from the replica set     
             *         
             *  \param url Replica to remove from set 
             *
             *
             */
            void remove_location(saga::url url) 
            {
                saga::task t = remove_locationpriv(url, saga::task_base::Sync()); 
                t.get_result ();
            }
            SAGA_CALL_PUB_1_DEF_0(remove_location, saga::url)

            /*! \brief Change a replica location in the replica set      
             *         
             *  \param old_url Replica to be updated
             *  \param new_url Update for the replica
             *
             *
             */
            void update_location(saga::url old_url, saga::url new_url) 
            {
                saga::task t = update_locationpriv(old_url, new_url, saga::task_base::Sync()); 
                t.get_result ();
            }
            SAGA_CALL_PUB_2_DEF_0(update_location, saga::url, saga::url)

            /*! \brief List the locations in the location set     
             *         
             *  \return Array of locations in set 
             *
             *
             */
            std::vector<saga::url> list_locations(void) 
            {
                saga::task t = list_locationspriv(saga::task_base::Sync()); 
                return t.get_result<std::vector<saga::url> >();
            }
            SAGA_CALL_PUB_0_DEF_0(list_locations)

            /*! \brief Brief Replicate a file from any of the known replica locations 
             *               to a new location, and, on success, add the new replica 
             *               location to the set of associated replicas 
             *         
             *  \param url Location to replicate to
             *
             *
             */
            void replicate(saga::url url, 
                           int flags = saga::replica::None) 
            {
                saga::task t = replicatepriv(url, flags, saga::task_base::Sync()); 
                t.get_result ();
            }
            SAGA_CALL_PUB_2_DEF_1(replicate, saga::url, int, None)
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

#endif // !defined(SAGA_PACKAGES_DATA_REPLICA_LOGICAL_FILE_HPP)

