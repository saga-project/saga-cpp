//  Copyright (c) 2005-2006 Stephan Hirmer (shirmer@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_ADAPTOR_BULK_CONTAINER_BASE_HPP
#define SAGA_ADAPTOR_BULK_CONTAINER_BASE_HPP

#include <saga/saga/uuid.hpp>
#include <saga/saga/exception.hpp>
#include <saga/impl/exception.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors {
        
    /**
      * Bulk container base class
      *
      * used to hide templates, which occur in derived classes.
      */
    class bulk_container_base
    {
    public:
        /**
          * destructor
          */
        virtual ~bulk_container_base(void) {}
        
        
        /**
          * virtual function to execute a given bulk function.
          *
          * throws not implemented.
          */
        virtual std::vector<saga::uuid> execute_bulk() = 0;
    };

///////////////////////////////////////////////////////////////////////////////
}}  // namespace saga::adaptors

#endif // SAGA_ADAPTOR_BULK_CONTAINER_BASE_HPP
