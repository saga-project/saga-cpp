#ifndef SAGA_DIRECTORY_HPP
#define SAGA_DIRECTORY_HPP

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/directory.hpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
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
#include <saga/saga/call.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/object.hpp>
#include <saga/saga/session.hpp>

#include <saga/saga/packages/namespace/namespace_dir.hpp>
#include <saga/saga/packages/filesystem/file.hpp>

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
namespace saga { namespace filesystem {
    
    /*! \brief The SAGA %file package %directory API      
     *
     *
     */
    class SAGA_FILESYSTEM_PACKAGE_EXPORT directory 
        : public saga::name_space::directory
    {
    private:
        /// @cond
        // factory
        SAGA_CALL_CREATE_PRIV_3(session const&, saga::url, int)

        /** These types are not within API scope */
        SAGA_CALL_PRIV_2 (get_size, saga::url, int)
        SAGA_CALL_PRIV_1 (is_file, saga::url)

        SAGA_CALL_PRIV_2 (open, saga::url, int)
        SAGA_CALL_PRIV_2 (open_dir, saga::url, int)
        /// @endcond

    protected:
        /// @cond
        /** These types are not within API scope */
        TR1::shared_ptr <saga::impl::directory> get_impl_sp() const;
        saga::impl::directory* get_impl() const;
        friend class saga::impl::directory;
        explicit directory (saga::impl::directory * init);
        /// @endcond

public:
        /*! \brief Open the directory.     
         *
         *
         */
        directory (session const & s, saga::url url, 
                   int mode = ReadWrite);

        /*! \brief Open the directory.      
         *
         *
         */
        explicit directory (saga::url url, int mode = ReadWrite);
     
        /*! \brief Open the directory.      
         *
         *
         */     
        explicit directory (saga::object const& o);

        /*! \brief Open the directory.      
         *
         *
         */
        directory (void);

        /*! \brief Destroy the directory object.      
         *  
         *
         */
        ~directory (void);

        /*! \brief Factory function resembling directory(session, url, int) 
         *         constructor
         */
        static directory create(session const& s, saga::url url, int mode = Read)
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
        directory &operator= (saga::object const& o);

        /*! \brief Returns the number of bytes in the file.
         *  \return    Number of bytes in the file.
         *  \param     url  Name of file to inspect.
         *
         *
         *
         */
        saga::off_t get_size(saga::url url, int flags = None)
        {
            saga::task t = get_sizepriv(url, flags, saga::task_base::Sync());
            return t.get_result<saga::off_t>();
        }
        /// @cond
        SAGA_CALL_PUB_2_DEF_1 (get_size, saga::url, int, None)
        /// @endcond

        /*! \brief Alias for is_entry in saga::ns_directory      
         *
         *
         *
         */     
        bool is_file(saga::url url)
        {
            saga::task t = is_filepriv(url, saga::task_base::Sync());
            return t.get_result<bool>();
        }
        /// @cond
        SAGA_CALL_PUB_1_DEF_0(is_file, saga::url)
        /// @endcond

        /*! \brief Creates a new file instance.
         *  \param url File to be opened 
         *  \param flags: Flags definition operation modus 
         *  \return Opened file instance
         *
         *
         *
         */
        saga::filesystem::file open(saga::url url, 
            int flags = saga::filesystem::Read)
        {
            saga::task t = openpriv(url, flags, saga::task_base::Sync());
            return t.get_result<saga::filesystem::file>();
        }
        /// @cond
        SAGA_CALL_PUB_2_DEF_1(open, saga::url, int, saga::filesystem::Read)
        /// @endcond

        /*! \brief Creates a directory object.     
         *  \return Opened directory instance
         *  \param url Name of directory to open 
         *  \param flags Flags definition operation modus
         *
         *
         *
         */
        saga::filesystem::directory open_dir(saga::url url, 
            int flags = saga::filesystem::Read)
        {
            saga::task t = open_dirpriv(url, flags, saga::task_base::Sync());
            return t.get_result<saga::filesystem::directory>();
        }
        /// @cond
        SAGA_CALL_PUB_2_DEF_1(open_dir, saga::url, int,
                              saga::filesystem::Read)
        /// @endcond
        
    }; // directory

}} // namespace saga::filesystem

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif // !defined(SAGA_DEBUG)

// re-enable warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // SAGA_DIRECTORY_HPP
