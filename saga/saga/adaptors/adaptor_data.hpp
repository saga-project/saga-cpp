//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005 Stephan Hirmer (stephan.hirmer@gmail.com)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_SAGA_ADAPTORS_ADAPTOR_DATA_HPP
#define SAGA_SAGA_ADAPTORS_ADAPTOR_DATA_HPP

#include <boost/noncopyable.hpp>

#include <saga/saga/adaptors/config.hpp>
#include <saga/impl/engine/cpi.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors {
 
    /**
     * Class implementing the scoped_lock pattern for the adaptor_data
     *
     * @note If you want to use shared data which had been registered
     * in the adaptor_data instance in the session, just create an instance
     * of this class at the beginning of the scope you will use the shared
     * data in by specifying its id.
     * 
     */
    template <typename Adaptor>
    class adaptor_data
    :   public boost::noncopyable
    {
    private:
        typedef saga::impl::v1_0::cpi cpi;

        // this holds the locked data
        TR1::shared_ptr <Adaptor> adaptor_;
        
    public:
        typedef Adaptor adaptor_type;

        /**
          * Constructor: Here we lock the appropriate object identified by id
          * 
          * @param id the identifier for the object you want to lock.
          * @note The object you want to lock has to be registered in the current
          * session before.
          */
        template <typename Cpi>
        adaptor_data(Cpi *cpi_instance)
            : adaptor_ (TR1::static_pointer_cast<Adaptor>(
                cpi_instance->get_adaptor())) 
        {
            adaptor_->lock_data();
        }
        
        /**
          * Destructor: unlocks the object.
          */
        ~adaptor_data (void)
        {
            if (adaptor_) 
                adaptor_->unlock_data();
        }

        // automatic conversion to the actual adaptor data type is provided
        Adaptor * operator->() const
        {
            return boost::get_pointer(adaptor_);
        }
    };

}} // namespace saga::adaptors
///////////////////////////////////////////////////////////////////////////////

#endif  // SAGA_SAGA_ADAPTORS_ADAPTOR_DATA_HPP

