#ifndef SAGA_PACKAGES_DATA_NAMESPACEDIR_HPP
#define SAGA_PACKAGES_DATA_NAMESPACEDIR_HPP

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/namespace_dir.hpp")
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
// include stl
#include <string>

// include dependent spec sections
#include <saga/saga/call.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/session.hpp>
#include <saga/saga/task.hpp>
#include <saga/saga/permissions.hpp>
#include <saga/saga/url.hpp>

// include package config
#include <saga/saga/packages/namespace/config.hpp>
#include <saga/saga/packages/namespace/namespace_entry.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

#ifdef SAGA_DEBUG
#include <saga/saga/packages/description/preprocessed/namespace_dir.hpp>
#else

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 1, output: "preprocessed/namespace_dir.hpp")
#endif
///////////////////////////////////////////////////////////////////////////////
namespace saga
{
    namespace name_space {
    
    /*! \brief A "Virtual Directory", an RNS entry that is represented as
     * a non-leaf node in the hierarchical name space tree     
     *
     *   Inherits all navigation and manipulation methods from ns_entry, 
     * but adds some more methods to these sets: instead of dir.copy (target) 
     * they allow, for example, to do dir.copy (source, target). Other methods 
     * added allow to change the cwd of the instance (which changes the values re- 
     * turned by the get_name(), get_cwd() and get_url() inspection methods), 
     *	and others allow to open new ns_entry and ns_directory instances
     * (open() and open_dir()).
     */
    class SAGA_NAMESPACE_PACKAGE_EXPORT directory 
        : public entry
    {
    
    protected:
        /// @cond
        /** These methods are not within API scope */
        TR1::shared_ptr <saga::impl::namespace_dir> get_impl_sp() const;
        saga::impl::namespace_dir* get_impl() const;

        friend class saga::impl::namespace_dir;

        typedef saga::name_space::entry::monitorable_base monitorable_base;
        /// @endcond
     
    private:
        // factories
        SAGA_CALL_CREATE_PRIV_3(session const&, saga::url, int)

        // navigation/query methods
        SAGA_CALL_PRIV_1(change_dir, saga::url)
        SAGA_CALL_CONST_PRIV_2(list, std::string, int)
        SAGA_CALL_CONST_PRIV_2(find, std::string, int)

        SAGA_CALL_CONST_PRIV_1(read_link,  saga::url)
        SAGA_CALL_CONST_PRIV_1(exists, saga::url)
        SAGA_CALL_CONST_PRIV_1(is_dir, saga::url)
        SAGA_CALL_CONST_PRIV_1(is_entry, saga::url)
        SAGA_CALL_CONST_PRIV_1(is_link, saga::url)

        // Deal with entries by entry number
        SAGA_CALL_CONST_PRIV_0(get_num_entries)
        SAGA_CALL_CONST_PRIV_1(get_entry, std::size_t)

        // entry management methods
        SAGA_CALL_PRIV_3(copy, saga::url, saga::url, int)
        SAGA_CALL_PRIV_3(link, saga::url, saga::url, int)
        SAGA_CALL_PRIV_3(move, saga::url, saga::url, int)
        SAGA_CALL_PRIV_2(remove, saga::url, int)
        SAGA_CALL_PRIV_2(make_dir, saga::url, int)

        // open methods
        SAGA_CALL_PRIV_2(open, saga::url, int)
        SAGA_CALL_PRIV_2(open_dir, saga::url, int)

        // management methods - wild card versions
        SAGA_CALL_PRIV_3(copy, std::string, saga::url, int)
        SAGA_CALL_PRIV_3(link, std::string, saga::url, int)
        SAGA_CALL_PRIV_3(move, std::string, saga::url, int)
        SAGA_CALL_PRIV_2(remove, std::string, int)

        // permissions with flags
        SAGA_CALL_PRIV_4(permissions_allow, saga::url, std::string, int, int)
        SAGA_CALL_PRIV_4(permissions_deny, saga::url, std::string, int, int)
        SAGA_CALL_PRIV_4(permissions_allow, std::string, std::string, int, int)
        SAGA_CALL_PRIV_4(permissions_deny, std::string, std::string, int, int)

    protected:
        /// @cond
        /** These methods are not within API scope */
        directory (saga::impl::namespace_dir *);
        /// @endcond
         
    public:
        /*! \brief Creates the object.      
         *
         *
         */
        directory (session const& s, saga::url url, int mode = Read,
                       saga::object::type t = saga::object::NSDirectory);

        /*! \brief Creates the object.      
         *
         *
         */
        explicit directory (saga::url url, int mode = Read,
            saga::object::type t = saga::object::NSDirectory);
            
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

        /*! \brief Factory function resembling entry(session, url, int) 
         *         constructor
         */
        static directory create(session const & s, saga::url url, int mode = Read)
        {
            return directory(s, url, mode);
        }
        SAGA_CALL_CREATE_3_DEF_1(session const&, saga::url, int, Read)

        /*! \brief Factory function resembling entry(url, int) constructor
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

        /*! \brief Change the working directory.   
         *  \param target Directory to change to 
         *         
         *
         *
         */
        void change_dir (saga::url target)
        {
            saga::task t = change_dirpriv(target, saga::task_base::Sync());
            t.get_result ();
        }
        /// @cond
        SAGA_CALL_PUB_1_DEF_0(change_dir, saga::url)
        /// @endcond
        
        /*! \brief List entries in this directory.
         *         
         *  \param pattern Name or pattern to list 
         *  \return Array of names matching the pattern 
         *
         *
         */
        std::vector<saga::url> list (std::string pattern = "*", 
            int flags = saga::name_space::None) const
        {
            saga::task t = listpriv(pattern, flags, saga::task_base::Sync());
            return t.get_result<std::vector<saga::url> >();
        }
        /// @cond
        SAGA_CALL_CONST_PUB_2_DEF_2(list, std::string, 
            "*", int, saga::name_space::None)
        /// @endcond
        
        /*! \brief Find entries in the current directory and below.     
         *         
         *  \param pattern Pattern for names of entries to be found 
         *  \param flags Flags defining the operation modus 
         *  \return Array of names matching the pattern
         *
         *
         */
        std::vector<saga::url> find (std::string pattern, 
            int flags = saga::name_space::Recursive) const
        {
            saga::task t = findpriv(pattern, flags, saga::task_base::Sync());
            return t.get_result<std::vector<saga::url> >();
        }
        /// @cond
        SAGA_CALL_CONST_PUB_2_DEF_1(find, std::string, 
            int, saga::name_space::Recursive)
        /// @endcond

        /*! \brief Returns the name of the link target.
         *         
         *  \param url Name to be resolved
         *  \return Resolved name 
         *
         */
        saga::url read_link (saga::url url) const
        {
            saga::task t = read_linkpriv(url, saga::task_base::Sync());
            return t.get_result<saga::url>();
        }
        /// @cond
        SAGA_CALL_CONST_PUB_1_DEF_0(read_link, saga::url)
        /// @endcond
        
        /*! \brief Returns true if entry exists, false otherwise      
         *         
         *  \param url Name to be tested for existence 
         *  \return Boolean indicating existence of name 
         *
         *
         */
        bool exists (saga::url url) const
        {
            saga::task t = existspriv(url, saga::task_base::Sync());
            return t.get_result<bool>();
        }
        /// @cond
        SAGA_CALL_CONST_PUB_1_DEF_0(exists, saga::url)
        /// @endcond
        
        /*! \brief Tests name for being a directory.
         *         
         *  \param  url Name to be tested 
         *  \return boolean indicating if name is a directory 
         *
         *
         */
        bool is_dir (saga::url url) const
        {
            saga::task t = is_dirpriv(url, saga::task_base::Sync());
            return t.get_result<bool>();
        }
        /// @cond
        SAGA_CALL_CONST_PUB_1_DEF_0(is_dir, saga::url)
        /// @endcond
        
        /*! \brief Tests name for being a namespace_entry     
         *         
         *  \param  url Name to be tested 
         *  \return Boolean indicating if name is a non-directory entry
         * 
         *
         */
        bool is_entry (saga::url url) const
        {
            saga::task t = is_entrypriv(url, saga::task_base::Sync());
            return t.get_result<bool>();
        }
        /// @cond
        SAGA_CALL_CONST_PUB_1_DEF_0(is_entry, saga::url)
        /// @endcond
        
        /*! \brief Tests name for beeing a symbolic link.
         *         
         *  \param  url Name to be tested 
         *  \return Boolean indicating if name is a link
         * 
         *
         *
         */
        bool is_link (saga::url url) const
        {
            saga::task t = is_linkpriv(url, saga::task_base::Sync());
            return t.get_result<bool>();
        }
        /// @cond
        SAGA_CALL_CONST_PUB_1_DEF_0 (is_link, saga::url)
        /// @endcond

        // make base class functions available as well
        using entry::read_link;
        using entry::is_dir;
        using entry::is_entry;
        using entry::is_link;

        /*! \brief Gives the number of entries in the directory     
         *         
         *  \return Number of entries in the directory
         *
         *
         */
        std::size_t get_num_entries (void) const
        {
            saga::task t = get_num_entriespriv(saga::task_base::Sync());
            return t.get_result<std::size_t>();
        }
        /// @cond
        SAGA_CALL_CONST_PUB_0_DEF_0(get_num_entries)
        /// @endcond
        
        /*! \brief Gives the name of an entry in the directory based upon the 
         *         enumeration defined by get_num_entries      
         *  \param entry Index of entry to get
         *  \return Name of entry at index
         *         
         *
         *
         */
        saga::url get_entry (std::size_t entry) const
        {
            saga::task t = get_entrypriv(entry, saga::task_base::Sync());
            return t.get_result<saga::url>();
        }
        /// @cond
        SAGA_CALL_CONST_PUB_1_DEF_0(get_entry, unsigned int)
        /// @endcond

        /*! \brief Copy the entry to another part of the namespace      
         *  \param source_url Name to copy 
         *  \param dest_url Name to copy to 
         *  \param flags Flags defining the operation modus 
         *
         *
         */
        void copy (saga::url source_url, saga::url dest_url, 
            int flags = saga::name_space::None)
        {
            saga::task t = copypriv(source_url, dest_url, flags, saga::task_base::Sync());
            t.get_result ();
        }
        /// @cond
        SAGA_CALL_PUB_3_DEF_1 (copy, saga::url, saga::url, int, 
            saga::name_space::None)
        /// @endcond
        
        /*! \brief Copy the entry to another part of the namespace      
         *  \param source_url Name to copy, can contain wild cards 
         *  \param dest_url Name to copy to 
         *  \param flags Flags defining the operation modus 
         *
         *
         *
         */
        void copy (std::string source_url, saga::url dest_url, 
            int flags = saga::name_space::None)
        {
            saga::task t = copypriv(source_url, dest_url, flags, saga::task_base::Sync());
            t.get_result ();
        }
        /// @cond
        SAGA_CALL_PUB_3_DEF_1 (copy, std::string, saga::url, int, 
            saga::name_space::None)
        /// @endcond
        
        /*! \brief Copy the entry to another part of the namespace
         *  \param source_url Name to copy, can contain wild cards
         *  \param dest_url Name to copy to
         *  \param flags Flags defining the operation modus
         *
         *
         *
         */
        void copy (char const * source_url, saga::url dest_url,
            int flags = saga::name_space::None)
        {
            saga::task t = copypriv(std::string(source_url), dest_url, flags, saga::task_base::Sync());
            t.get_result ();
        }
        /// @cond
        template <class Tag>
        saga::task copy (char const * source_url, saga::url dest_url,
            int flags = saga::name_space::None)
        {
            return copypriv(std::string(source_url), dest_url, flags, Tag());
        }
        /// @endcond

        /*! \brief Create a symbolic link from the source entry to the target 
         *         entry so that any reference to the target refers to the 
         *         source entry      
         *  \param source_url Name to link 
         *  \param dest_url Name to link to 
         *  \param flags Flags defining the operation modus 
         *
         *
         *
         */
        void link (saga::url source_url, saga::url dest_url, 
            int flags = saga::name_space::None)
        {
            saga::task t = linkpriv(source_url, dest_url, flags, saga::task_base::Sync());
            t.get_result ();
        }
        /// @cond
        SAGA_CALL_PUB_3_DEF_1(link, saga::url, saga::url, int, 
            saga::name_space::None)
        /// @endcond
        
        /*! \brief Create a symbolic link from the source entry to the target 
         *         entry so that any reference to the target refers to the 
         *         source entry      
         *  \param source_url Name to link, can contain wild cards  
         *  \param dest_url Name to link to 
         *  \param flags Flags defining the operation modus 
         *
         *
         *
         */
        void link (std::string source_url, saga::url dest_url, 
            int flags = saga::name_space::None)
        {
            saga::task t = linkpriv(source_url, dest_url, flags, saga::task_base::Sync());
            t.get_result ();
        }
        /// @cond
        SAGA_CALL_PUB_3_DEF_1(link, std::string, saga::url, int, 
            saga::name_space::None)
        /// @endcond
        
        /*! \brief Create a symbolic link from the source entry to the target
         *         entry so that any reference to the target refers to the
         *         source entry
         *  \param source_url Name to link, can contain wild cards
         *  \param dest_url Name to link to
         *  \param flags Flags defining the operation modus
         *
         *
         *
         */
        void link (char const * source_url, saga::url dest_url,
            int flags = saga::name_space::None)
        {
            saga::task t = linkpriv(std::string(source_url), dest_url, flags, saga::task_base::Sync());
            t.get_result ();
        }
        /// @cond
        template <class Tag>
        saga::task link (char const * source_url, saga::url dest_url,
            int flags = saga::name_space::None)
        {
            return linkpriv(std::string(source_url), dest_url, flags, Tag());
        }
        /// @endcond

        /*! \brief Rename source to target, or move source to target if target 
         *         is an directory.     
         *  \param source_url Name to move 
         *  \param dest_url Name to move to 
         *  \param flags Flags defining the operation modus 
         *
         *
         *
         *
         */
        void move (saga::url source_url, saga::url dest_url, 
            int flags = saga::name_space::None)
        {
            saga::task t = movepriv(source_url, dest_url, flags, saga::task_base::Sync());
            t.get_result ();
        }
        /// @cond
        SAGA_CALL_PUB_3_DEF_1(move, saga::url, saga::url, int, 
            saga::name_space::None)
        /// @endcond
        
        /*! \brief Rename source to target, or move source to target if target 
         *         is an directory.     
         *  \param source_url Name to move, can contain wild cards  
         *  \param dest_url Name to move to 
         *  \param flags Flags defining the operation modus 
         *
         *
         *
         *
         */
        void move (std::string source_url, saga::url dest_url, 
            int flags = saga::name_space::None)
        {
            saga::task t = movepriv(source_url, dest_url, flags, saga::task_base::Sync());
            t.get_result ();
        }
        /// @cond
        SAGA_CALL_PUB_3_DEF_1(move, std::string, saga::url, int, 
            saga::name_space::None)
        /// @endcond
        
        /*! \brief Rename source to target, or move source to target if target
         *         is an directory.
         *  \param source_url Name to move, can contain wild cards
         *  \param dest_url Name to move to
         *  \param flags Flags defining the operation modus
         *
         *
         *
         *
         */
        void move (char const * source_url, saga::url dest_url,
            int flags = saga::name_space::None)
        {
            saga::task t = movepriv(std::string(source_url), dest_url, flags, saga::task_base::Sync());
            t.get_result ();
        }
        /// @cond
        template <class Tag>
        saga::task move (char const * source_url, saga::url dest_url,
            int flags = saga::name_space::None)
        {
            return movepriv(std::string(source_url), dest_url, flags, Tag());
        }
        /// @endcond

        /*! \brief Removes the entry     
         *  \param url Entry to be removed 
         *
         *
         */
        void remove (saga::url url, int flags = saga::name_space::None)
        {
            saga::task t = removepriv(url, flags, saga::task_base::Sync());
            t.get_result ();
        }
        /// @cond
        SAGA_CALL_PUB_2_DEF_1(remove, saga::url, int, saga::name_space::None)
        /// @endcond

        /*! \brief Removes the entry     
         *  \param url Entry to be removed, can contain wild cards  
         *
         *
         */
        void remove (std::string url, int flags = saga::name_space::None)
        {
            saga::task t = removepriv(url, flags, saga::task_base::Sync());
            t.get_result ();
        }
        /// @cond
        SAGA_CALL_PUB_2_DEF_1(remove, std::string, int, saga::name_space::None)
        /// @endcond

        /*! \brief Removes the entry
         *  \param url Entry to be removed, can contain wild cards
         *
         *
         */
        void remove (char const * url, int flags = saga::name_space::None)
        {
            saga::task t = removepriv(std::string(url), flags, saga::task_base::Sync());
            t.get_result ();
        }
        /// @cond
        template <class Tag>
        saga::task remove (char const * url, int flags = saga::name_space::None)
        {
            return removepriv(std::string(url), flags, Tag());
        }
        /// @endcond

        // make base class functions available as well
        using entry::copy;
        using entry::link;
        using entry::move;
        using entry::remove;

        /*! \brief Creates a new directory      
         *  \param url Directory to create 
         *
         *
         *
         */
        void make_dir (saga::url url, 
            int flags = saga::name_space::None)
        {
            saga::task t = make_dirpriv(url, flags, saga::task_base::Sync());
            t.get_result ();
        }
        /// @cond
        SAGA_CALL_PUB_2_DEF_1(make_dir, saga::url, int, saga::name_space::None)
        /// @endcond

        /*! \brief Creates a new namespace_entry instance      
         *  \param url Entry 
         *  \param flags Flags defining the operation modus 
         *  \return Opened entry instance 
         *         
         *
         *
         */
        saga::name_space::entry open (saga::url url, 
            int flags = saga::name_space::Read)
        {
            saga::task t = openpriv(url, flags, saga::task_base::Sync());
            return t.get_result<saga::name_space::entry>();
        }
        /// @cond
        SAGA_CALL_PUB_2_DEF_1(open, saga::url, int, saga::name_space::Read)
        /// @endcond
        
        /*! \brief Creates a new namespace_directory instance     
         *  \param url Directory to open 
         *  \param flags Flags defining the operation
         *        
         *  \return Opened directory instance
         *
         *
         */
        saga::name_space::directory open_dir (saga::url url, 
                                              int flags = saga::name_space::Read)
        {
            saga::task t = open_dirpriv(url, flags, saga::task_base::Sync());
            return t.get_result<saga::name_space::directory>();
        }
        /// @cond
        SAGA_CALL_PUB_2_DEF_1(open_dir, saga::url, int, saga::name_space::Read)
        /// @endcond

        // make base class functions available as well
        using entry::permissions_allow;
        using entry::permissions_deny;

        /*! \brief Enable a permission.     
         *        
         *
         *
         *
         *  \param target entry to set permissions for
         *  \param id     id to set permission for
         *  \param perm   permission to enable
         *  \param flags  mode of operation
         */
        void permissions_allow (saga::url target, std::string id, 
            int perm, int flags = None)
        {
            saga::task t = permissions_allowpriv(target, id, perm, flags, saga::task_base::Sync());
            t.get_result ();
        }
        /// @cond
        SAGA_CALL_PUB_4_DEF_1 (permissions_allow, saga::url, std::string, 
            int, int, None)
        /// @endcond

        /*! \brief Disable a permission.     
         *         
         *
         *
         *
         *  \param target entry to set permissions for
         *  \param id     id to set permission for
         *  \param perm   permission to disable
         *  \param flags  mode of operation
         */
        void permissions_deny (saga::url target, std::string id, 
            int perm, int flags = None)
        {
            saga::task t = permissions_denypriv(target, id, perm, flags, saga::task_base::Sync());
            t.get_result ();
        }
        /// @cond
        SAGA_CALL_PUB_4_DEF_1 (permissions_deny, saga::url, std::string, 
            int, int, None)
        /// @endcond

        /*! \brief Enable a permission.     
         *         
         *
         *
         *
         *  \param target entry to set permissions for, may contain wild cards
         *  \param id     id to set permission for
         *  \param perm   permission to enable
         *  \param flags  mode of operation
         */
        void permissions_allow (std::string target, std::string id, 
            int perm, int flags = None)
        {
            saga::task t = permissions_allowpriv(target, id, perm, flags, saga::task_base::Sync());
            t.get_result ();
        }
        /// @cond
        SAGA_CALL_PUB_4_DEF_1 (permissions_allow, std::string, std::string, 
            int, int, None)
        /// @endcond

        /*! \brief Disable a permission.     
         *         
         *
         *
         *
         *  \param target entry to set permissions for, may contain wild cards
         *  \param id     id to set permission for
         *  \param perm   permission to disable
         *  \param flags  mode of operation
         */
        void permissions_deny (std::string target, std::string id, 
            int perm, int flags = None)
        {
            saga::task t = permissions_denypriv(target, id, perm, flags, saga::task_base::Sync());
            t.get_result ();
        }
        /// @cond
        SAGA_CALL_PUB_4_DEF_1 (permissions_deny, std::string, std::string, 
            int, int, None)
        /// @endcond

        /*! \brief Enable a permission.
         *         
         *
         *
         *
         *  \param target entry to set permissions for, may contain wild cards
         *  \param id     id to set permission for
         *  \param perm   permission to enable
         *  \param flags  mode of operation
         */
        void permissions_allow (char const * target, std::string id,
                int perm, int flags = None)
        {
            saga::task t = permissions_allowpriv(std::string(target), id, perm, flags,
                  saga::task_base::Sync());
            t.get_result ();
        }
        /// @cond
        template <class Tag>
        saga::task permissions_allow (char const * target, std::string id,
                int perm, int flags = None)
        {
            return permissions_allowpriv(std::string(target), id, perm, flags,
                Tag());
        }
        /// @endcond

        /*! \brief Disable a permission.
         *         
         *
         *
         *
         *  \param target entry to set permissions for, may contain wild cards
         *  \param id     id to set permission for
         *  \param perm   permission to disable
         *  \param flags  mode of operation
         */
        void permissions_deny (char const * target, std::string id,
                int perm, int flags = None)
        {
            saga::task t = permissions_denypriv(std::string(target), id, perm, flags,
                  saga::task_base::Sync());
            t.get_result ();
        }
        /// @cond
        template <class Tag>
        saga::task permissions_deny (char const * target, std::string id,
                int perm, int flags = None)
        {
            return permissions_denypriv(std::string(target), id, perm, flags,
                Tag());
        }
        /// @endcond
    };
    
}} // saga::namespace_dir

#endif // !defined(SAGA_DEBUG)

#if defined(__WAVE__) && defined(SAGA_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif
// re-enable warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // SAGA_PACKAGES_DATA_NAMESPACEDIR_HPP

