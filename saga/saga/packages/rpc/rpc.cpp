//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  this is needed in every file including detail/permissions_impl.hpp and not 
//  belonging to the engine
#define SAGA_NO_IMPORT_PERMISSIONS

// include rpc API and implementation
#include <saga/saga/rpc.hpp>
#include <saga/impl/rpc.hpp>
#include <saga/saga/detail/call.hpp>

#include <saga/saga/detail/permissions_impl.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga {
  namespace rpc {

  rpc::rpc (session const& s, saga::url name)
    : saga::object (new saga::impl::rpc (s, name))
  {
    this->saga::object::get_impl()->init();
  }

  rpc::rpc (saga::url name)
    : saga::object (new saga::impl::rpc (detail::get_the_session (), name))
  {
    this->saga::object::get_impl()->init();
  }

  rpc::rpc (saga::impl::rpc *impl)
    : saga::object (impl)
  {
  }

  rpc::rpc (void) 
  {
  }
  
  rpc::~rpc (void)
  {
  }

  saga::impl::rpc* rpc::get_impl() const
  { 
      typedef saga::object base_type;
      return static_cast<saga::impl::rpc*>(this->base_type::get_impl()); 
  }

  TR1::shared_ptr <saga::impl::rpc> rpc::get_impl_sp(void) const
  { 
    // FIXME: this needs documentation
    typedef saga::object base_type;
    return TR1::static_pointer_cast <saga::impl::rpc>(
        this->base_type::get_impl_sp());
  }

  // factory
  SAGA_CALL_CREATE_IMP_2(rpc, impl::rpc, session const&, saga::url)

  SAGA_CALL_IMP_1(rpc, call, std::vector<parameter>&)
  SAGA_CALL_IMP_1(rpc, close, double)
  
  }

  namespace detail
  {
    /////////////////////////////////////////////////////////////////////////////
    //  implement the permissions functions (we need to explicitly specialize 
    //  the template because the functions are not implemented inline)
    template struct SAGA_RPC_PACKAGE_EXPORT permissions<rpc::rpc>;  
  }

///////////////////////////////////////////////////////////////////////////////////
} // namespace saga

