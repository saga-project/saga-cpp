#ifndef SAGA_PACKAGES_DATA_NAMESPACEENTRY_HPP
#define SAGA_PACKAGES_DATA_NAMESPACEENTRY_HPP

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/namespace_entry.hpp")
#endif
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 0, output: null)
#endif

// include stl
#include <string>

// include dependent spec sections
#include <saga/saga/call.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/object.hpp>
#include <saga/saga/session.hpp>
#include <saga/saga/task.hpp>
#include <saga/saga/permissions.hpp>
#include <saga/saga/url.hpp>

// include package config
#include <saga/saga/packages/namespace/config.hpp>

#include <saga/saga/detail/monitorable.hpp>
#include <saga/saga/detail/permissions.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

#ifdef SAGA_DEBUG
#include <saga/saga/packages/description/preprocessed/namespace_entry.hpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/namespace_entry.hpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga
{
    /*! \brief The <I>Name Space</I> API package
     *
     *   The Name Space package provides access and manipulation functionality
     *   to an abstract hierarchical name space.  The package does not make any 
     *   assumptions about the entities living in the name space, so does not
     *   look inside them - but instead handles them as opaque entities.
     *
     */   
    namespace name_space {
    
    /*! \brief The flags describe the properties of several operations
     * on namespace entries. 
     *
     *   Packages which inherit from the namespace package use the same
     *   flag semantics unless specified otherwise, but will, in general, 
     *   add additional flags to some operations.
     */
    enum flags
    {
        Unknown         =  -1,
        None            =   0,
        Overwrite       =   1, 
        Recursive       =   2,
        Dereference     =   4,
        Create          =   8,
        Exclusive       =  16,
        Lock            =  32,
        CreateParents   =  64,
        // 128, reserved for Truncate
        // 256,  reserved for Append 
        Read            = 512,
        Write           = 1024,
        ReadWrite       = Read | Write
        // 2048, reserved for Binary
    };
    
    /*! \brief An RNS entry that interconnects a reference to an existing
     * resource into the hierarchical name space.    
     *
     *   Defines methods which serve the inspection of the entry itself,
     *   methods which allows to manage the entry (e.g. to copy, move, or remove
     *   it), and methods to manipulate the entry’s access control lists. 
     */
    class SAGA_NAMESPACE_PACKAGE_EXPORT entry
        : public saga::object,
          public saga::detail::monitorable<entry>,
          public saga::detail::permissions<entry>
    {

    protected:
        /// @cond
        /** These methods are not within API scope */
        TR1::shared_ptr <saga::impl::namespace_entry> get_impl_sp(void) const;
        saga::impl::namespace_entry* get_impl (void) const;

        friend struct saga::detail::monitorable<entry>;  // needs to access get_impl()
        friend struct saga::detail::permissions<entry>;
        friend class saga::impl::namespace_entry;

        typedef saga::detail::monitorable<entry> monitorable_base;
        /// @endcond
          
    private:
        // factory
        SAGA_CALL_CREATE_PRIV_3(session const&, saga::url, int)

        // basic properties
        SAGA_CALL_CONST_PRIV_0(get_url)
        SAGA_CALL_CONST_PRIV_0(get_cwd)
        SAGA_CALL_CONST_PRIV_0(get_name)

        // navigation/query methods
        SAGA_CALL_CONST_PRIV_0(read_link)
        SAGA_CALL_CONST_PRIV_0(is_dir)
        SAGA_CALL_CONST_PRIV_0(is_entry)
        SAGA_CALL_CONST_PRIV_0(is_link)

        // entry management methods 
        SAGA_CALL_PRIV_2(copy, saga::url, int)
        SAGA_CALL_PRIV_2(link, saga::url, int)
        SAGA_CALL_PRIV_2(move, saga::url, int)
        SAGA_CALL_PRIV_1(remove, int)

        SAGA_CALL_PRIV_1(close, double)

        // permissions with flags
        SAGA_CALL_PRIV_3(permissions_allow, std::string, int, int)
        SAGA_CALL_PRIV_3(permissions_deny, std::string, int, int)

    protected:
        /// @cond
        /** These methods are not within API scope */
        explicit entry (saga::impl::namespace_entry * init);
        /// @endcond
     
    public:
        /*! \brief Creates the object.     
         *
         *
         */
        entry (session const & s, saga::url url, int mode = Read, 
            saga::object::type t = saga::object::NSEntry);

        /*! \brief Creates the object.     
         *
         *
         */
        explicit 
        entry (saga::url url, int mode = Read, 
            saga::object::type t = saga::object::NSEntry);

        /*! \brief Creates the object.     
         *
         *
         */
        explicit entry (saga::object const& o);

        /*! \brief Creates the object.     
         *
         *
         */
        entry (void);

        /*! \brief Destroys the object.     
         *
         *
         */
        ~entry (void);

        /*! \brief Factory function resembling entry(session, url, int) 
         *         constructor
         */
        static entry create(session const & s, saga::url url, int mode = Read)
        {
            return entry(s, url, mode);
        }
        SAGA_CALL_CREATE_3_DEF_1(session const&, saga::url, int, Read)

        /*! \brief Factory function resembling entry(url, int) constructor
         */
        static entry create(saga::url url, int mode = Read)
        {
            return entry(url, mode);
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
        entry &operator= (saga::object const& o);

        /*! \brief Obtains the complete url pointing to the entry 
         *         
         *  \return url pointing to the entry
         *
         *
         */
        saga::url get_url() const
        {
            saga::task t = get_urlpriv(saga::task_base::Sync());
            return t.get_result<saga::url>();
        }
        SAGA_CALL_CONST_PUB_0_DEF_0(get_url)

        /*! \brief Obtains the current working directory for the entry 
         *         
         *  \return Current working directory
         *
         *
         */
        saga::url get_cwd() const
        {
            saga::task t = get_cwdpriv(saga::task_base::Sync());
            return t.get_result<saga::url>();
        }
        SAGA_CALL_CONST_PUB_0_DEF_0(get_cwd)

        /*! \brief Obtains the name part of the url     
         *  \return last part of the pathname 
         *         
         *
         *
         */
        saga::url get_name() const
        {
            saga::task t = get_namepriv(saga::task_base::Sync());
            return t.get_result<saga::url>();
        }
        SAGA_CALL_CONST_PUB_0_DEF_0(get_name)

        /*! \brief Returns the name of the link target. 
         *  \return Resolved name
         *         
         *
         *
         */
        saga::url read_link() const
        {
            saga::task t = read_linkpriv(saga::task_base::Sync());
            return t.get_result<saga::url>();
        }
        SAGA_CALL_CONST_PUB_0_DEF_0(read_link)

        /*! \brief Tests entry for being a directory.     
         *         
         *  \return Boolean indicating if entry is a directory
         *
         *
         */
        bool is_dir() const
        {
            saga::task t = is_dirpriv(saga::task_base::Sync());
            return t.get_result<bool>();
        }
        SAGA_CALL_CONST_PUB_0_DEF_0(is_dir)

        /*! \brief Tests entry for being a namespace_entry.     
         *  \return Boolean indicating if entry is a namespace_entry 
         *         
         *
         *
         */
        bool is_entry() const
        {
            saga::task t = is_entrypriv(saga::task_base::Sync());
            return t.get_result<bool>();
        }
        SAGA_CALL_CONST_PUB_0_DEF_0(is_entry)

        /*! \brief Tests the entry for being a link.
        *  \return Boolean indicating if entry is a link
        *
        *
        */
        bool is_link () const
        {
            saga::task t = is_linkpriv(saga::task_base::Sync());
            return t.get_result<bool>();
        }
        SAGA_CALL_CONST_PUB_0_DEF_0(is_link)

        /*! \brief Copy the entry to another part of the namespace      
         *  \param target Name to copy to 
         *  \param flags Flags defining the operation modus
         *         
         *
         *
         */
        void copy(saga::url target, int flags = saga::name_space::None)
        {
            saga::task t = copypriv(target, flags, saga::task_base::Sync());
            t.get_result ();
        }
        SAGA_CALL_PUB_2_DEF_1 (copy, saga::url, int, saga::name_space::None)

        /*! \brief Creates a symbolic link from the entry to the target entry     
         *         
         *  \param target Name to link to
         *  \param flags Flags defining the operation modus
         *
         *
         */
        void link (saga::url target, int flags = saga::name_space::None)
        {
            saga::task t = linkpriv(target, flags, saga::task_base::Sync());
            t.get_result ();
        }
        SAGA_CALL_PUB_2_DEF_1 (link, saga::url, int, saga::name_space::None)

        /*! \brief Rename source to target, or move source to target if target is an directory.    
         *         
         *  \param target Name to move to
         *  \param flags Flags defining the operation modus
         *
         *
         */
        void move (saga::url target, int flags = saga::name_space::None)
        {
            saga::task t = movepriv(target, flags, saga::task_base::Sync());
            t.get_result ();
        }
        SAGA_CALL_PUB_2_DEF_1 (move, saga::url, int, saga::name_space::None)

        /*! \brief Removes this entry and closes it
         *         
         *
         *
         *
         */
        void remove (int flags = saga::name_space::None)
        {
            saga::task t = removepriv(flags, saga::task_base::Sync());
            t.get_result ();
        }
        SAGA_CALL_PUB_1_DEF_1 (remove, int, saga::name_space::None)

        /*! \brief Closes the object.     
         *        
         *
         *
         */
        void close (double timeout = 0.0)
        {
            saga::task t = closepriv(timeout, saga::task_base::Sync());
            t.get_result ();
        }
        SAGA_CALL_PUB_1_DEF_1 (close, double, 0.0)

        // make base class functions available as well
        using saga::detail::permissions<entry>::permissions_allow;
        using saga::detail::permissions<entry>::permissions_deny;

        /*! \brief Enable a permission.     
         *        
         *
         *
         *
         *  \param id     id to set permission for
         *  \param perm   permission to enable
         *  \param flags  mode of operation
         */
        void permissions_allow (std::string id, int perm, int flags)
        {
            saga::task t = permissions_allowpriv(id, perm, flags, saga::task_base::Sync());
            t.get_result ();
        }
        SAGA_CALL_PUB_3_DEF_0 (permissions_allow, std::string, int, int)

        /*! \brief Disable a permission.     
         *        
         *
         *
         *
         *  \param id     id to set permission for
         *  \param perm   permission to disable
         *  \param flags  mode of operation
         */
        void permissions_deny (std::string id, int perm, int flags)
        {
            saga::task t = permissions_denypriv(id, perm, flags, saga::task_base::Sync());
            t.get_result ();
        }
        SAGA_CALL_PUB_3_DEF_0 (permissions_deny, std::string, int, int)

    };  // namespace_entry

}} 

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif
#endif // !defined(SAGA_DEBUG)

// re-enable warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // SAGA_PACKAGES_DATA_NAMESPACEENTRY_HPP

