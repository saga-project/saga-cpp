//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
// 
//  Use, modification and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <utility>

#include <saga/saga/adaptors/task_declaration.hpp>
#include <saga/saga/task_container.hpp>
#include <saga/impl/engine/task_container.hpp>

#include <saga/saga/detail/monitorable_impl.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga {
    
  task_container::task_container (void) 
    : saga::object (new saga::impl::task_container())
  {
  }

  task_container::~task_container(void)
  {
  }

  saga::impl::task_container* task_container::get_impl (void) const
  { 
    typedef saga::object base_type;
    return static_cast<saga::impl::task_container*>(this->base_type::get_impl());
  }

  TR1::shared_ptr<saga::impl::task_container> task_container::get_impl_sp(void) const
  { 
    typedef saga::object base_type;
    return TR1::static_pointer_cast<saga::impl::task_container>(
        this->base_type::get_impl_sp());
  }

  void task_container::add_task (task t)
  {
    get_impl()->add_task(t);
  }

  void  task_container::remove_task (task t)
  {
    get_impl()->remove_task(t);
  }

  std::vector <task> task_container::list_tasks(void) const
  {
    return get_impl()->list_tasks();
  }

  std::vector <task_base::state> task_container::get_states(void) const
  {
    return get_impl()->get_states();
  }

  void task_container::cancel(void)
  {
    get_impl()->cancel();
  }

  std::vector <task> task_container::wait (wait_mode mode, float timeout)
  {
    return get_impl()->wait(mode, timeout);
  }

  void task_container::run ()
  {
    get_impl()->run();
  }

  namespace detail
  {
    /////////////////////////////////////////////////////////////////////////////
    //  implement the monitorable functions (we need to explicitly specialize 
    //  the template because the functions are not implemented inline)
    template struct SAGA_EXPORT monitorable<task_container>;
  }

///////////////////////////////////////////////////////////////////////////////
} // namespace saga
