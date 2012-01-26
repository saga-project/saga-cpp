//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga/exception.hpp>
#include <saga/saga/object.hpp>
#include <saga/saga/monitorable.hpp>
#include <saga/saga/metric.hpp>

#include <saga/impl/engine/object.hpp>
#include <saga/impl/engine/monitorable.hpp> 

namespace saga 
{
  monitorable::monitorable (saga::object rhs)
    : impl_ (rhs.get_impl_sp())
  {
  }

  monitorable::monitorable (saga::impl::object *impl)
    : impl_ (impl->shared_from_this())
  {
  }

  monitorable::~monitorable (void)
  {
  }

  saga::impl::monitorable* monitorable::get_monitorable (void) 
  { 
    return impl_->get_monitorable(); 
  }
  saga::impl::monitorable const* monitorable::get_monitorable (void) const 
  { 
    return impl_->get_monitorable(); 
  }

  // introspection
  std::vector<saga::metric> monitorable::list_metrics (void) const
  {
    return get_monitorable()->list_metrics();
  }

  saga::metric monitorable::get_metric (std::string name) const
  {
    return get_monitorable()->get_metric(name);
  }

  // callback handling
  saga::monitorable::cookie_handle 
  monitorable::add_callback(std::string name, saga::callback cb)
  {
    return get_monitorable()->add_callback(name, cb);
  }
  
  void monitorable::remove_callback(std::string name, cookie_handle cookie)
  {
    get_monitorable()->remove_callback(name, cookie);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  steerable::steerable (saga::object rhs)
    : monitorable(rhs)
  {
  }

  steerable::steerable (saga::impl::object *impl)
    : monitorable(impl)
  {
  }

  steerable::~steerable (void)
  {
  }

  saga::impl::steerable* steerable::get_steerable (void) 
  { 
    return impl_->get_steerable(); 
  }
  saga::impl::steerable const* steerable::get_steerable (void) const 
  { 
    return impl_->get_steerable(); 
  }

  // metric handling
  bool steerable::add_metric (saga::metric m)
  {
    return get_steerable()->add_metric(m);
  }
  
  void steerable::remove_metric (std::string name)
  {
    get_steerable()->remove_metric(name);
  }
  
  void steerable::fire_metric (std::string name)
  {
    get_steerable()->fire_metric(name);
  }
  
///////////////////////////////////////////////////////////////////////////////
} // namespace saga

