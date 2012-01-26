//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <utility>
#include <vector>
#include <string>

#include <boost/assert.hpp>

#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/url.hpp>

#include <saga/impl/task.hpp>    // include the task implementation we want to use

#include <saga/saga/detail/task_get_result_impl.hpp>
#include <saga/saga/detail/monitorable_impl.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga 
{
  task::task()
  {
  }

  task::task (saga::task_base::state s)
    : saga::object (new saga::impl::dummy_task(s))
  {
  }

  task::task (saga::impl::object * init)
    : saga::object (init)
  {
  }

  task::task (saga::object const& o)
    : saga::object (o)
  {
  }

  task::~task (void)
  {
  }
  
  task &task::operator= (saga::object const& o)
  {
      return saga::object::operator=(o), *this;
  }

  saga::impl::task_interface* task::get_task_if (void) 
  { 
    return this->saga::object::get_impl()->get_task_interface();
  }
  saga::impl::task_interface const* task::get_task_if (void) const
  { 
    return this->saga::object::get_impl()->get_task_interface();
  }

  /// @cond 
  saga::impl::task_base* task::get_impl (void) const
  {
      return static_cast<saga::impl::task_base*>(this->object::get_impl());
  }

  TR1::shared_ptr <saga::impl::task_base> task::get_impl_sp(void) const
  {
      return TR1::static_pointer_cast<saga::impl::task_base>(
          this->object::get_impl_sp());
  }

  /** Doxygen generates namespace function from friend impls... */
  bool operator== (task const & lhs, task const & rhs)
  {
    return lhs.get_task_if() == rhs.get_task_if();
  }

  /** Doxygen generates namespace function from friend impls... */
  bool operator< (task const & lhs, task const & rhs)
  {
    return lhs.get_id() < rhs.get_id();
  }
  /// @endcond

  void task::run (void)
  {
    get_task_if()->run();
  }

  void task::cancel (void)
  {
    get_task_if()->cancel();
  }

  bool task::wait (double timeout)
  {
    return get_task_if()->wait (timeout);
//     bool result = false;
//     while (!result) {
//       result = get_task_if()->wait (timeout);
// 
//       // we retry on any encountered error
//       saga::task_base::state s = get_task_if()->get_state();
//       if (result && saga::task_base::Failed == s)
//       {
//         // throws if no other adaptors are available
//         if (!get_task_if()->restart()) 
//         {
//           get_task_if()->rethrow();
//         }
//         result = false;           // need to retry
//       }
// 
//       // exit, if this wait should not block
//       if (timeout >= 0)
//         break;
//     }
//     return result;
  }

  task::state task::get_state (void)
  {
    return get_task_if()->get_state();
//     task::state s = get_task_if()->get_state();
//     if (saga::task_base::Failed == s)
//     {
//       get_task_if()->restart();
//       s = get_task_if()->get_state();
//     }
//     return s;
  }

  void task::rethrow() 
  {
    if (saga::task_base::Failed == get_task_if()->get_state())
    {
      get_task_if()->rethrow();                         // must throw
//       // throws if no other adaptors are available
//       get_task_if()->restart();
//       BOOST_ASSERT(saga::task_base::Failed != get_state());
    }
  }

  saga::object task::get_object() const
  {
    return get_task_if()->get_object();
  }

  /////////////////////////////////////////////////////////////////////////////
  //  implement the monitorable functions (we need to explicitly specialize 
  //  the template because the functions are not implemented inline)
  template struct saga::detail::monitorable<task>;

  /////////////////////////////////////////////////////////////////////////////
  //  implement all task::get_result<> functions needed in engine module
  /// @cond
  namespace detail
  {
      hold_any& get_task_result(saga::task t)
      {
          return saga::impl::runtime::get_impl(t)->get_result();
      }
  }
  /// @endcond

  // native types
  template SAGA_EXPORT std::string&                    task::get_result<std::string>();
  template SAGA_EXPORT std::string const&              task::get_result<std::string>() const;
  
  template SAGA_EXPORT std::vector<std::string>&       task::get_result<std::vector<std::string> >();
  template SAGA_EXPORT std::vector<std::string> const& task::get_result<std::vector<std::string> >() const;
  
  template SAGA_EXPORT bool&                           task::get_result<bool>();
  template SAGA_EXPORT bool const&                     task::get_result<bool>() const;
  
  template SAGA_EXPORT int&                            task::get_result<int>();
  template SAGA_EXPORT int const&                      task::get_result<int>() const;
  
#if ! defined (SAGA_TYPE_LONG_IS_INT)
  template SAGA_EXPORT long&                           task::get_result<long>();
  template SAGA_EXPORT long const&                     task::get_result<long>() const;
#endif
  
#if ! defined (SAGA_TYPE_LONGLONG_IS_INT) \
 && ! defined (SAGA_TYPE_LONGLONG_IS_LONG)
  template SAGA_EXPORT long long&                      task::get_result<long long>();
  template SAGA_EXPORT long long const&                task::get_result<long long>() const;
#endif
  
  // saga types
#if ! defined (SAGA_TYPE_SIZE_IS_INT) \
 && ! defined (SAGA_TYPE_SIZE_IS_LONG) \
 && ! defined (SAGA_TYPE_SIZE_IS_LONGLONG)
  template SAGA_EXPORT saga::size_t&                   task::get_result<saga::size_t>();
  template SAGA_EXPORT saga::size_t const&             task::get_result<saga::size_t>() const;
#endif

#if ! defined (SAGA_TYPE_SSIZE_IS_INT)      \
 && ! defined (SAGA_TYPE_SSIZE_IS_LONG)     \
 && ! defined (SAGA_TYPE_SSIZE_IS_LONGLONG) \
 && ! defined (SAGA_TYPE_SSIZE_IS_SIZE)
  template SAGA_EXPORT saga::ssize_t&                  task::get_result<saga::ssize_t>();
  template SAGA_EXPORT saga::ssize_t const&            task::get_result<saga::ssize_t>() const;
#endif
  
#if ! defined (SAGA_TYPE_OFF_IS_INT)      \
 && ! defined (SAGA_TYPE_OFF_IS_LONG)     \
 && ! defined (SAGA_TYPE_OFF_IS_LONGLONG) \
 && ! defined (SAGA_TYPE_OFF_IS_SIZE)     \
 && ! defined (SAGA_TYPE_OFF_IS_SSIZE)
  template SAGA_EXPORT saga::off_t&                    task::get_result<saga::off_t>();
  template SAGA_EXPORT saga::off_t const&              task::get_result<saga::off_t>() const;
#endif


  // saga classes (no packages)
  template SAGA_EXPORT saga::context&                  task::get_result<saga::context>(); 
  template SAGA_EXPORT saga::context const&            task::get_result<saga::context>() const;

  template SAGA_EXPORT saga::url&                      task::get_result<saga::url>(); 
  template SAGA_EXPORT saga::url const&                task::get_result<saga::url>() const;

  template SAGA_EXPORT std::vector<saga::url>&         task::get_result<std::vector<saga::url> >();
  template SAGA_EXPORT std::vector<saga::url> const&   task::get_result<std::vector<saga::url> >() const;

  SAGA_EXPORT void task::get_result()
  {
      if (saga::task_base::Failed == get_task_if()->get_state())
      {
          get_task_if()->rethrow();     // must throw
      }
      get_task_result(*this);   // do synchronization only
  }

  SAGA_EXPORT void task::get_result() const
  {
      if (saga::task_base::Failed == get_task_if()->get_state())
      {
          get_task_if()->rethrow();     // must throw
      }
      get_task_result(*this);   // do synchronization only
  }

  namespace detail 
  {
    ///////////////////////////////////////////////////////////////////////////
    void set_selector_state(saga::task t,  
        TR1::shared_ptr<impl::adaptor_selector_state> state)
    {
        impl::runtime::get_impl(t)->set_selector_state(state);
    }

    // store an exception in a task instance
    saga::task set_task_exception(saga::task t, saga::impl::object const *obj,
        saga::impl::exception_list const& l, saga::error e)
    {
        impl::runtime::get_impl(t)->set_task_exception(obj, l, e);
        return t;
    }

  }

///////////////////////////////////////////////////////////////////////////////
} // namespace saga

