//  Copyright (c) 2005-2006 Stephan Hirmer (shirmer@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_ADAPTOR_BULK_CONTAINER_HPP
#define SAGA_ADAPTOR_BULK_CONTAINER_HPP


#include <vector>

#include <boost/spirit/phoenix/tuples.hpp>

#include <saga/saga/adaptors/bulk_container_base.hpp>
#include <saga/saga/uuid.hpp>

#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable: 4251 4231 4275 4660)
#endif

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace adaptors {
        
    ///////////////////////////////////////////////////////////////////////////
    //  forward declaration
    
    /**
      * class to store generic function parameters.
      *
      * every cpi object holds one instance of this class, to store
      * the bunch of parameters needed to execute a number of operations
      * in a bulk. Through the create_... function, all template details
      * are hidden from the adaptor programmer.
      */
    template <
        typename Cpi,
        typename FuncRetVal,
        typename FuncArg0 = phoenix::nil_t, 
        typename FuncArg1 = phoenix::nil_t,
        typename FuncArg2 = phoenix::nil_t,
        typename FuncArg3 = phoenix::nil_t,
        typename FuncArg4 = phoenix::nil_t,
        typename FuncArg5 = phoenix::nil_t,
        typename FuncArg6 = phoenix::nil_t>
    class bulk_container;

    ///////////////////////////////////////////////////////////////////////////
    
    /**
      * Helper function, which is called from the adaptor's prepare function.
      * creates a new bulk_container or adds a additional set of parameters
      * to a existing one.
      *
      * used to simplify life for adaptor developers. Hides templates.
      */
    template<typename Cpi, typename FuncRetVal>
    inline void
    create_bulk_container_or_add_params(Cpi* cpi, bulk_container_base *& bc, 
             std::vector<saga::uuid> (Cpi::*exec_bulk)
                         (std::vector<FuncRetVal>&, std::vector<saga::uuid>& ), 
                          FuncRetVal const& retval_, saga::uuid task_uuid_)
    {
        typedef saga::adaptors::bulk_container<Cpi, FuncRetVal> 
            bulk_container_type;
        
        /*boost::fusion::tuple<FuncRetVal, saga::uuid> val (retval_, task_uuid_);*/
            
        if (NULL == bc) 
            bc = new bulk_container_type(cpi, exec_bulk, retval_, task_uuid_);
        else
            static_cast<bulk_container_type *>(bc)->add_params(retval_, task_uuid_);
    }
    
    ///////////////////////////////////////////////////////////////////////////
    /**
      * class to store generic function parameters.
      *
      * every cpi object holds one instance of this class, to store
      * the bunch of parameters needed to execute a number of operations
      * in a bulk. Through the create_... function, all template details
      * are hidden from the adaptor programmer.
      */
    template <typename Cpi, typename FuncRetVal>
    class bulk_container <Cpi, FuncRetVal, 
              phoenix::nil_t, phoenix::nil_t, phoenix::nil_t, phoenix::nil_t, 
              phoenix::nil_t, phoenix::nil_t, phoenix::nil_t> 
        : public saga::adaptors::bulk_container_base
    {
    private:
        typedef phoenix::tuple <FuncRetVal, saga::uuid> tuple_type;
        
        std::vector<saga::uuid> uuids;
        std::vector<FuncRetVal> rets;

        std::vector<saga::uuid> (Cpi::*exec_bulk) (std::vector<FuncRetVal>&,
                                                   std::vector<saga::uuid>&);
        Cpi * my_adaptor;

     public:
        /**
          * constructor
          *
          * stores the bulk function to execute and the uuids.
          */
        bulk_container (Cpi* cpi_,
                       std::vector<saga::uuid> (Cpi::*exec_bulk_) 
                                                      (std::vector<FuncRetVal>&,
                                                      std::vector<saga::uuid>&),
                       FuncRetVal retval, saga::uuid task_uuid) 
           : exec_bulk  (exec_bulk_),
             my_adaptor (cpi_)
             
        {
            this->add_params (retval, task_uuid);
        }

        /**
          * default destructor
          */
        ~bulk_container (void) {}

        /**
          * adds parameters to a existing container.
          */
        bulk_container *add_params (FuncRetVal retval_, saga::uuid task_uuid )
        {
            //ops_args.push_back (val);
            rets.push_back(retval_);
            uuids.push_back(task_uuid);
            return this;
        }
        
        /**
          * executes the bulk function, and passes the appropriate set
          * of parameters (stored in std::vector's) to it.
          */
        std::vector<saga::uuid> execute_bulk (void)
        {
            return (my_adaptor->*exec_bulk) (rets, uuids);
        }

    }; // class bulk_container

    ///////////////////////////////////////////////////////////////////////////
    // bring in higher order functions
#include <saga/saga/adaptors/bulk_container_impl.hpp>

    ///////////////////////////////////////////////////////////////////////////////
}} // namespace saga::adaptors



#endif // SAGA_ADAPTOR_BULK_CONTAINER_HPP

