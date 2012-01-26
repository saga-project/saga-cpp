//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_SAGA_ADAPTORS_INSTANCE_DATA_BASE_HPP
#define SAGA_SAGA_ADAPTORS_INSTANCE_DATA_BASE_HPP

#include <boost/thread/recursive_mutex.hpp>

#include <saga/saga/adaptors/config.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga 
{ 
  namespace adaptors 
  {
    /**
     * Base class for all classes, which should be shared for a specific API
     * object instance.
     */
    class instance_data_base
    {
     private:
       /* ! Assigning is not allowed */
       instance_data_base & operator= (instance_data_base const &);

       /* ! Copying is not allowed */
       instance_data_base (instance_data_base const &);

       typedef boost::recursive_mutex mutex_type;
       mutex_type mtx_;

     public:
       /**
        * The constructor
        */
       instance_data_base (void)
       {
       }

       /**
        * The destructor
        */
       virtual ~instance_data_base (void)
       {
       }

       // this mutex is used to lock the adaptor data
       mutex_type& get_mutex (void) { return mtx_; }
    };

  } // namespace adaptors

} // namespace saga
///////////////////////////////////////////////////////////////////////////////

#endif  // SAGA_SAGA_ADAPTORS_INSTANCE_DATA_BASE_HPP

