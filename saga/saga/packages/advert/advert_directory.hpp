#ifndef SAGA_SAGA_PACKAGES_DATA_LOGICAL_FILE_ADVERT_DIRECTORY_HPP
#define SAGA_SAGA_PACKAGES_DATA_LOGICAL_FILE_ADVERT_DIRECTORY_HPP

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/directory.hpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
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
#include <saga/saga/detail/monitorable.hpp>

#include <saga/saga/packages/namespace/namespace_dir.hpp>
#include <saga/saga/packages/advert/advert.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

#ifdef SAGA_DEBUG
#include <saga/saga/packages/filesystem/preprocessed/directory.hpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/directory.hpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga 
{
    namespace advert { 
        
        namespace metrics 
        {
            /*! \brief Brief description here 
             *
             *   Detailed %description here...
             */ 
            char const* const directory_created_entry  = "directory.CreatedEntry";
            
            /*! \brief Brief description here 
             *
             *   Detailed %description here...
             */ 
            char const* const directory_modified_entry = "directory.ModifiedEntry";
            
            /*! \brief Brief description here 
             *
             *   Detailed %description here...
             */ 
            char const* const directory_deleted_entry  = "directory.DeletedEntry";
        }
        
        /*! \brief The %directory class represents a <b>directory</b> within  
         *         a meta data %directory with a hierarchical namespace attached.  
         *         This class inherits all methods from saga::name_space::directoryectory.      
         *
         *  
         */
        class SAGA_ADVERT_PACKAGE_EXPORT directory 
        : public saga::name_space::directory, public saga::detail::attribute<directory>
        {
        protected:
            /// @cond
            /** These types are not within API scope */
            friend class saga::impl::advert_directory;
            
            friend struct saga::detail::attribute<directory>;  
            typedef saga::detail::attribute<directory> attribute_base;
            typedef saga::name_space::directory::monitorable_base monitorable_base;
            /// @endcond

        private:
            // factory
            SAGA_CALL_CREATE_PRIV_3(session const&, saga::url const&, int)

            // overload open methods from namespace_dir
            SAGA_CALL_PRIV_2(open, saga::url, int)
            SAGA_CALL_PRIV_2(open_dir, saga::url, int)
            SAGA_CALL_PRIV_3(find, std::string, std::vector<std::string>, int)

        protected:
            /// @cond
            /** These methods are not within API scope */
            TR1::shared_ptr <saga::impl::advert_directory> get_impl_sp() const;
            saga::impl::advert_directory* get_impl() const;
            explicit directory(saga::impl::advert_directory *impl);
            /// @endcond

        public:
            /*! \brief Constructs an %directory %object that points to %url,  
             *         opens it in mode and associates it with the %session s.
             */   
            directory (saga::session const& s, saga::url url, 
                       int mode = saga::advert::ReadWrite);

            /*! \brief Constructs an %directory %object that points to %url  
             *         and opens it in mode. 
             */
            explicit 
            directory (saga::url url, int mode = saga::advert::ReadWrite);

            /*! \brief Constructs a copy of other.
             */
            explicit directory (saga::object const& other);

            /*! \brief Tries to construct a default %directory %object.
             */
            directory (void);

            /*! \brief Destroys the %directory %object and frees all  
             *         allocated resources.
             */
            ~directory (void);

            /*! \brief Factory function resembling directory(session, url, int) 
             *         constructor
             */
            static directory create(session const& s, saga::url const& url, 
                int mode = Read)
            {
                return directory(s, url, mode);
            }
            /// @cond
            SAGA_CALL_CREATE_3_DEF_1(session const&, saga::url const&, int, Read)
            /// @endcond

            /*! \brief Factory function resembling directory(url, int) constructor
             */
            static directory create(saga::url const& url, int mode = Read)
            {
                return directory(url, mode);
            }
            template <typename Tag>
            static saga::task create(saga::url const& url, int mode = Read)
            {
                return create<Tag>(detail::get_the_session(), url, mode);
            }

            /*! \brief Assigns %object to this %directory and returns 
             *         a reference to it. <b>TASK ONLY</b>
             */        
            directory &operator= (saga::object const& o);

            /*! \brief Tries to open the %advert at the URL url with mode.
             */   
            advert::entry open (saga::url url, int mode = saga::advert::Read) 
            { 
                saga::task t = openpriv (url, mode, saga::task_base::Sync()); 
                return t.get_result<saga::advert::entry>();
            }
            SAGA_CALL_PUB_2_DEF_1(open, saga::url, int, saga::advert::Read)

            /*! \brief Tries to open the %advert_direcory at URL url with mode.
             *
             */   
            advert::directory open_dir (saga::url url, int mode = saga::advert::Read) 
            { 
                saga::task t = open_dirpriv (url, mode, saga::task_base::Sync()); 
                return t.get_result<saga::advert::directory>();
            }
            SAGA_CALL_PUB_2_DEF_1(open_dir, saga::url, int, saga::advert::Read)

            /*! \brief Tries to find %directory entries which match...
             *
             */
            std::vector<saga::url> 
            find (std::string name, std::vector<std::string> pattern, 
                int flags = saga::advert::None)
            { 
                saga::task t = findpriv (name, pattern, flags, saga::task_base::Sync()); 
                return t.get_result<std::vector<saga::url> >();
            }
            SAGA_CALL_PUB_3_DEF_1 (find, std::string, 
                std::vector<std::string>, int, saga::advert::None)

        }; // class directory

}}  // namespace saga::advert

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif // !defined(SAGA_DEBUG)

// re-enable warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // SAGA_SAGA_PACKAGES_DATA_LOGICAL_FILE_ADVERT_DIRECTORY_HPP
