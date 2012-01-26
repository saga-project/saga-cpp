//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_PACKAGES_COMM_RPC_RPC_HPP
#define SAGA_PACKAGES_COMM_RPC_RPC_HPP

// include stl
#include <string>
#include <vector>

// include dependent spec sections
#include <saga/saga/util.hpp>
#include <saga/saga/base.hpp>
#include <saga/saga/types.hpp>
#include <saga/saga/session.hpp>
#include <saga/saga/call.hpp>
#include <saga/saga/task.hpp>
#include <saga/saga/url.hpp>

#include <saga/saga/detail/permissions.hpp>

#include <saga/saga/packages/rpc/config.hpp>
#include <saga/saga/packages/rpc/parameter.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

namespace saga
{
    /*! \brief The <I>Remote Procedure Call </I> API package
     *
     *   Detailed %description here...
     */   
    namespace rpc {
 
    /*! \brief This class represents a remote function handle, which
     * can be called (repeatedly), and returns the result of the respective
     * remote procedure invocation. 
     *
     *
     */         
     class SAGA_RPC_PACKAGE_EXPORT rpc 
        : public saga::object,
          public saga::detail::permissions<rpc>
    {
    protected:
        /// @cond
        /** These methods are not within API scope */
        TR1::shared_ptr <saga::impl::rpc> get_impl_sp(void) const;
        saga::impl::rpc* get_impl (void) const;
        friend struct saga::detail::permissions<rpc>;
        friend class saga::impl::rpc;
        explicit rpc(saga::impl::rpc *impl);
        /// @endcond

    private:
        // factory
        SAGA_CALL_CREATE_PRIV_2(session const&, saga::url)

        SAGA_CALL_PRIV_1(call, std::vector<parameter>&)
        SAGA_CALL_PRIV_1(close, double)

    public:
        /*! \brief Initializes a remote function handle. 
         *  \return The newly created object.
         *
         *
         */       
        explicit rpc (session const& s, saga::url name = saga::url());

        /*! \brief Brief %description starts here     
         *
         *
         */
        explicit rpc (saga::url name);

        /*! \brief Brief %description starts here
         *
         *
         */
        rpc (void);

        /*! \brief Destroys the object.
         *
         *
         */
        ~rpc (void);

        /*! \brief Factory function resembling rpc(session, url) 
         *         constructor
         */
        static rpc create(session const& s, saga::url name = saga::url())
        {
            return rpc(s, name);
        }
        /// @cond
        SAGA_CALL_CREATE_2_DEF_1(session const&, saga::url, saga::url())
        /// @endcond

        /*! \brief Factory function resembling rpc(url) constructor
         */
        static rpc create(saga::url name = saga::url())
        {
            return rpc(name);
        }
        template <typename Tag>
        static saga::task create(saga::url name = saga::url())
        {
            return create<Tag>(detail::get_the_session(), name);
        }

        /*! \brief Call the remote procedure. 
         *
         *
         */ 
        void call(std::vector<parameter> parameters) 
        {
            saga::task t = callpriv(parameters, saga::task_base::Sync()); 
            t.get_result ();
        }
        /// @cond
        SAGA_CALL_PUB_1_DEF_0(call, std::vector<parameter>&)
        /// @endcond

        /*! \brief Closes the rpc handle instance.     
         *
         *
         */  
        void close(double timeout = 0.0) 
        {
            saga::task t = closepriv(timeout, saga::task_base::Sync()); 
            t.get_result ();
        }
        /// @cond
        SAGA_CALL_PUB_1_DEF_1(close, double, 0.0)
        /// @endcond
    }; 

}}

// re-enable warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // SAGA_PACKAGES_COMM_RPC_RPC_HPP

