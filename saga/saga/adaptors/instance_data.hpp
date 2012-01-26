//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005 Stephan Hirmer (stephan.hirmer@gmail.com)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_SAGA_ADAPTORS_INSTANCE_DATA_HPP
#define SAGA_SAGA_ADAPTORS_INSTANCE_DATA_HPP

#include <string>

#include <boost/noncopyable.hpp>

#include <saga/saga/util.hpp>
#include <saga/saga/adaptors/config.hpp>
#include <saga/saga/adaptors/instance_data_base.hpp>
#include <saga/saga/adaptors/task.hpp>

#include <saga/impl/engine/cpi.hpp>
#include <saga/impl/engine/proxy.hpp>

///////////////////////////////////////////////////////////////////////////////
// boost::get_pointer doesn't know anything about std::tr1::shared_ptr, 
// which is a pain
#if defined(SAGA_USE_TR1_NAMESPACE)
namespace boost
{
    template<class T> 
    inline T* get_pointer(std::tr1::shared_ptr<T> const& p)
    {
        return p.get();
    }
}
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors { 

    /**
     * Class implementing the scoped_lock pattern for the instance_data
     *
     * @note If you want to use shared data which had been registered
     * in the instance_data instance in the session, just create an instance
     * of this class at the beginning of the scope you will use the shared
     * data in by specifying its id.
     * 
     */
    template <typename T>
    class instance_data
    :   public boost::noncopyable
    {
    private:
        typedef saga::impl::proxy proxy;

        // this holds the locked data
        proxy * proxy_;
        TR1::shared_ptr <T>  data_;

    public:
        typedef T instance_data_type;

        /**
          * Constructor: Here we lock the appropriate object identified by id
          * 
          * @param id the identifier for the object you want to lock.
          * @note The object you want to lock has to be registered in the current
          * session before.
          */
        instance_data (proxy* prxy)
            : proxy_  (prxy), 
              data_   (TR1::static_pointer_cast<T>(
                  proxy_->check_out_instance_data()))
        {
        }
        
        template <typename Cpi>
        instance_data (Cpi *cpi_instance)
            : proxy_  (cpi_instance->get_proxy ()), 
              data_   (TR1::static_pointer_cast<T>(
                  proxy_->check_out_instance_data()))
        {
        }

        //  default constructor.
        instance_data ()
            : proxy_ (NULL)
        {
        }

        //  This will be called by the initializing instance to provide the 
        //  initial data to be stored for the CPI instance
        template <typename Cpi>
        void init_data (Cpi *proxy, TR1::shared_ptr<instance_data_type> d)
        {
            proxy_ = proxy;
            data_  = TR1::static_pointer_cast<instance_data_type> (
                proxy_->register_instance_data(d));
        }

        //  This will be called by the last instance before it goes out of
        //  scope
        template <typename Cpi>
        void release_data (Cpi *proxy)
        {
            proxy_ = proxy;
            proxy_->release_instance_data();
        }

        /**
          * Destructor: unlocks the object.
          */
        ~instance_data (void)
        {
            if ( NULL != proxy_ && data_ )
                proxy_->check_in_instance_data ();
        }

        // automatic conversion to the actual adaptor data type is provided
        T * operator->() const
        {
            return boost::get_pointer (data_);
        }
    };

    /**
     * Class implementing the scoped_lock pattern for the instance_data
     * (same as instance_data, except this is using dynamic_pointer_cast for
     * the stored data, i.e. this should be used whenever a special instance
     * data type is in use).
     *
     * @note If you want to use shared data which had been registered
     * in the instance_data instance in the session, just create an instance
     * of this class at the beginning of the scope you will use the shared
     * data in by specifying its id.
     * 
     */
    template <typename T>
    class dynamic_instance_data
    :   public boost::noncopyable
    {
    private:
        typedef saga::impl::proxy proxy;

        // this holds the locked data
        proxy * proxy_;
        TR1::shared_ptr<adaptors::instance_data_base> base_data_;
        TR1::shared_ptr<T> data_;

        void tidy()
        {
            if (NULL != proxy_ && base_data_) 
            {
                proxy_->check_in_instance_data();
                base_data_.reset();
                data_.reset();
            }
        }

    public:
        typedef T instance_data_type;

        /**
          * Constructor: Here we lock the appropriate object identified by id
          * 
          * @param id the identifier for the object you want to lock.
          * @note The object you want to lock has to be registered in the current
          * session before.
          */
        dynamic_instance_data (proxy* prxy)
          : proxy_(prxy), 
            base_data_(proxy_->check_out_instance_data()),
            data_(TR1::dynamic_pointer_cast<T>(base_data_))
        {
            if (!is_valid()) 
                tidy();
        }

        template <typename Cpi>
        dynamic_instance_data (Cpi *cpi_instance)
          : proxy_  (cpi_instance->get_proxy ()), 
            base_data_(proxy_->check_out_instance_data()),
            data_(TR1::dynamic_pointer_cast<T>(base_data_))
        {
            if (!is_valid()) 
                tidy();
        }

        //  default constructor.
        dynamic_instance_data()
          : proxy_ (NULL)
        {
        }

        //  This will be called by the initializing instance to provide the 
        //  initial data to be stored for the CPI instance
        template <typename Cpi>
        void init_data (Cpi *proxy, TR1::shared_ptr<instance_data_type> d)
        {
            proxy_ = proxy;
            base_data_ = proxy_->register_instance_data(d);
            data_ = TR1::dynamic_pointer_cast<instance_data_type>(base_data_);
            if (!is_valid()) 
                tidy();
        }

        //  This will be called by the last instance before it goes out of
        //  scope
        template <typename Cpi>
        void release_data (Cpi *proxy)
        {
            proxy_ = proxy;
            proxy_->release_instance_data();
        }

        /**
          * Destructor: unlocks the object.
          */
        ~dynamic_instance_data (void)
        {
            tidy();
        }

        // automatic conversion to the actual adaptor data type is provided
        T * operator->() const
        {
            return boost::get_pointer(data_);
        }
        
        // return, if the object got initialized correctly
        bool is_valid() const 
        {
            return data_;
        }
    };

}} // namespace saga::adaptors
///////////////////////////////////////////////////////////////////////////////

#endif  // SAGA_SAGA_ADAPTORS_INSTANCE_DATA_HPP

