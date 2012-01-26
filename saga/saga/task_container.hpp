//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2007 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef _SAGA_TASK_CONTAINER_HPP
#define _SAGA_TASK_CONTAINER_HPP

// include stl
#include <vector>

// include dependent spec sections
#include <saga/saga/base.hpp>
#include <saga/saga/task.hpp>

#include <saga/saga/detail/monitorable.hpp>

// suppress warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga
{
  /*! \brief  Handles a large number of asynchronous operations.
   *
   *   Managing a large number of tasks can be tedious. The task_container class
   * is designed to help in these situations, and to effectively handle a large number 
   * of asynchronous operations. 
   */
  class SAGA_EXPORT task_container
    : public saga::object,
      public saga::detail::monitorable<task_container>
  {
    private:
      /// @cond
      /** These methods are not within API scope */
      TR1::shared_ptr<saga::impl::task_container> get_impl_sp (void) const;
      saga::impl::task_container* get_impl (void) const;

      friend struct impl::runtime;
      friend struct saga::detail::monitorable <task_container>; // needs to access get_impl()
      /// @endcond
      
    public:
      /*! \brief The wait mode enum specifies the condition on which a wait() 
       *         operation on a saga::task container returns.
       */
      enum wait_mode 
      {
          All = 1,      ///< wait() returns if all tasks in the container 
                        ///< reached a final state
          Any = 2       ///< wait() returns if one or more tasks in the 
                        ///< container reached a final state.
      };

      /*! \brief Creates a task_container     
       *
       *
       */
      task_container (void);

      /*! \brief Destroys a task_container     
       *
       *
       */
      ~task_container (void);

      /*! \brief Start all asynchronous operations in the container.  
       *
       *
       */
      void run ();
      
      /*! \brief Adds a task to a task_container.     
       * \param task Task to add to the task_container.
       *\return Cookie identifying the added task.
       *
       *
       */
      void add_task (task t);
      
      /*! \brief Removes a task from a task_container. 
       * \param task Cookie identifying the task to be removed.
       * \return The removed task.
       *
       *
       */
      void remove_task (task t);

      /*! \brief Lists the tasks in the task_container. 
       * \return Array of cookies for all tasks in task_container.
       *
       *
       */
      std::vector <task> list_tasks  (void) const;
 
      /*! \brief Gets the states of all tasks in the task_container. 
       * \return Array of states for tasks in task_container.
       *
       *
       */
      std::vector <task_base::state> get_states  (void) const; 

      /*! \brief Cancels all the asynchronous operations in the container.      
       * \param timeout Time for freeing resources. 
       * 
       *
       */
      void cancel (void);
      
      /*! \brief Wait for one or more of the tasks to finish. 
       * \param mode Wait for all or any task
       * \param timeout Seconds to wait
       * \return Finished task   
       */
      std::vector <task> wait (wait_mode mode = All, float timeout = -1.0);
  };

  ///////////////////////////////////////////////////////////////////////////////
} // namespace saga

// re-enable warnings about dependent classes not being exported from the dll
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

#endif // _SAGA_TASK_CONTAINER_HPP

