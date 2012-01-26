//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_PERMISSIONS_HPP
#define SAGA_PERMISSIONS_HPP

// include STL
#include <string>
#include <vector>

// include dependent spec sections
#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>

#include <saga/saga/detail/permissions.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable : 4251 4231 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga
{    
    /*! \brief The SAGA permission interface.
     *
     */
    class SAGA_EXPORT permissions
      : public saga::detail::permissions<permissions>
    {
        
    public:
        
        /*! \brief This enum specifies the available permissions in SAGA.        
         *
         */
        enum permission
        {
            None   = 0,
            Query  = 1,
            Read   = 2,
            Write  = 4,
            Exec   = 8,
            Owner  = 0x10,
            All    = 0x1F
        };
    
    protected:
        /// @cond
        /** These methods are not within API scope */
        friend struct saga::detail::permissions<permissions>;  // needs to access get_impl()
        TR1::shared_ptr <saga::impl::object> impl_;

        TR1::shared_ptr<saga::impl::object> get_impl_sp (void) const { return impl_; }
        saga::impl::object* get_impl (void) const { return impl_.get(); }

        bool is_impl_valid() const { return impl_ ? true : false; }
        /// @endcond

    public:
        /*! \brief Brief %description starts here     
         *
         *
         */
        permissions (saga::object rhs);

        /*! \brief Brief %description starts here     
         *
         *
         */
        explicit permissions (saga::impl::object *impl);    // adaptor interface

        /*! \brief Brief %description starts here     
         *
         *
         */        
        ~permissions (void);
    };

///////////////////////////////////////////////////////////////////////////////
} // namespace saga
                                                                             
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // SAGA_PERMISSIONS_HPP

