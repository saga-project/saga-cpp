//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_SAGA_DETAIL_PERMISSIONS_HPP
#define SAGA_SAGA_DETAIL_PERMISSIONS_HPP

#include <string>

// include dependent spec sections
#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/monitorable.hpp>
#include <saga/saga/call.hpp>
#include <saga/saga/task.hpp>

// For symbol import/export macros
#include <saga/saga-defs.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable : 4251 4231 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
// for modules not belonging to the engine attributes should not be exported
#if !defined (SAGA_CREATE_PREPROCESSED_FILES) && !defined(SAGA_EXPORT_PERMISSIONS)

# if defined(SAGA_ENGINE_EXPORTS) || defined(SAGA_PERMISSIONS_EXPORTS)
#   define SAGA_EXPORT_PERMISSIONS SAGA_SYMBOL_EXPORT
# elif !defined (SAGA_NO_IMPORT_PERMISSIONS) && !defined(BUILD_SAGA_LITE)
#   define SAGA_EXPORT_PERMISSIONS SAGA_SYMBOL_IMPORT
# else
#   define SAGA_EXPORT_PERMISSIONS /* empty */
# endif

#endif // !SAGA_CREATE_PREPROCESSED_FILES

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace detail
{
    /// @cond
    ///////////////////////////////////////////////////////////////////////////
    template <typename Derived, typename Tag>
    struct SAGA_EXPORT_PERMISSIONS permissions_priv
    {
        static saga::task permissions_allow (Derived const& this_, 
            std::string id, int p);
        static saga::task permissions_deny (Derived const& this_, 
            std::string id, int p);
        static saga::task permissions_check (Derived const& this_, 
            std::string id, int p);
        
        static saga::task get_owner (Derived const& this_);
        static saga::task get_group (Derived const& this_);
    };
    /// @endcond
    ///////////////////////////////////////////////////////////////////////////
    
    
    /*! \brief   The SAGA %permissions interface.
     *
     *  \details    TODO .
     */
    template <typename Derived>
    struct SAGA_EXPORT_PERMISSIONS permissions
    {
    private:
        template <typename Tag>
        saga::task permissions_allowpriv (std::string id, int p, Tag) const
        { return permissions_priv<Derived, Tag>::permissions_allow(derived(), id, p); }
        
        template <typename Tag>
        saga::task permissions_denypriv (std::string id, int p, Tag) const
        { return permissions_priv<Derived, Tag>::permissions_deny(derived(), id, p); }
        
        template <typename Tag>
        saga::task permissions_checkpriv (std::string id, int p, Tag) const
        { return permissions_priv<Derived, Tag>::permissions_check(derived(), id, p); }
        
        template <typename Tag>
        saga::task get_ownerpriv (Tag) const
        { return permissions_priv<Derived, Tag>::get_owner(derived()); }
        
        template <typename Tag>
        saga::task get_grouppriv (Tag) const
        { return permissions_priv<Derived, Tag>::get_group(derived()); }
        
    public:
        ///@cond
        impl::permissions_interface* get_perm (void);
        impl::permissions_interface* get_perm (void) const;
        ///@endcond
        
    protected:
        ///@cond
        Derived& derived() 
        { return static_cast<Derived&>(*this); }
        Derived const& derived() const 
        { return static_cast<Derived const&>(*this); }
        ///@endcond
    public: 
        
        /*! \brief     Set a permission flag for this %object instance.
         *  \param     permission_id The permission to set the flag for
         *  \param     flag The flag to set
         *         
         *
         *  \details   TODO .
         */       
        void permissions_allow(std::string permission_id, int flag) 
        {
            permissions_allowpriv (permission_id, flag, 
                saga::task_base::Sync()); 
        }
        ///@cond
        SAGA_CALL_PUB_2_DEF_0(permissions_allow, std::string, int)
        ///@endcond
        
        /*! \brief     Remove a permission flag from this %object instance.
         *  \param     permission_id The permission to remove the flag from
         *  \param     flag The flag to remove
         *         
         *
         *  \details    TODO .
         */       
        void permissions_deny(std::string permission_id, int flag) 
        {
            permissions_denypriv (permission_id, flag, 
                saga::task_base::Sync()); 
        }   
        ///@cond
        SAGA_CALL_PUB_2_DEF_0(permissions_deny, std::string, int)
        ///@endcond
        
        /*! \brief     Check this %object instance for a permission flag.
         *  \param     permission_id The permission to check for a flag 
         *  \param     flag The flag to check for
         *         
         *
         *  \details    TODO .
         */           
        bool permissions_check(std::string permission_id, int flag) const 
        {
            saga::task t = permissions_checkpriv (permission_id, flag, 
                saga::task_base::Sync()); 
            return t.get_result<bool>();
        }   
        ///@cond
        SAGA_CALL_CONST_PUB_2_DEF_0(permissions_check, std::string, int)
        ///@endcond
        
        /*! \brief     Returns the owner of this %object instance.
         *  \return    The owner's name
         *         
         *
         *  \details    TODO .
         */       
        std::string get_owner() const 
        {
            saga::task t = get_ownerpriv (saga::task_base::Sync()); 
            return t.get_result<std::string>();
        }   
        ///@cond
        SAGA_CALL_CONST_PUB_0_DEF_0(get_owner)
        ///@endcond
        
        /*! \brief     Returns the group of the owner of this %object instance.
         *  \return    The group's name
         *         
         *
         *  \details    TODO .
         */       
        std::string get_group() const 
        {
            saga::task t = get_grouppriv (saga::task_base::Sync()); 
            return t.get_result<std::string>();
        }   
        ///@cond        
        SAGA_CALL_CONST_PUB_0_DEF_0(get_group)
        ///@endcond
    };
    
    ///////////////////////////////////////////////////////////////////////////////
}}

#endif


