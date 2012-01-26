//  Copyright (c) 2005-2006 Stephan Hirmer (shirmer@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_PP_IS_ITERATING)

#if !defined(SAGA_ADAPTOR_BULK_CONTAINER_IMPL_HPP)
#define SAGA_ADAPTOR_BULK_CONTAINER_IMPL_HPP

#include <saga/impl/config.hpp>

#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/comma_if.hpp>

#define BOOST_PP_ITERATION_PARAMS_1                                           \
    (3, (1, SAGA_ARGUMENT_LIMIT,                                              \
    "saga/saga/adaptors/bulk_container_impl.hpp"))                            \
    /**/

#include BOOST_PP_ITERATE()

#endif // SAGA_ADAPTOR_BULK_CONTAINER_IMPL_HPP

///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////
#else // defined(BOOST_PP_IS_ITERATING)

#define O BOOST_PP_ITERATION()

#define GET_PARAM_VEC_REF(z, M, _) BOOST_PP_COMMA_IF(M) std::vector< FuncArg ## M >&
    ///////////////////////////////////////////////////////////////////////////
    /**
      * Helper function, which is called from the adaptor's prepare function.
      * creates a new bulk_container or adds a additional set of parameters
      * to a existing one.
      *
      * used to simplify life for adaptor developers. Hides templates.
      *
      * preprocessor generated function
      */
    template<
        typename Cpi, typename FuncRetVal, 
        BOOST_PP_ENUM_PARAMS (O, typename FuncArg)
    >
    inline void
    create_bulk_container_or_add_params(Cpi* cpi, bulk_container_base *& bc, 
        std::vector<saga::uuid> (Cpi::*exec_bulk)(
                  std::vector<FuncRetVal>&, BOOST_PP_REPEAT(O, GET_PARAM_VEC_REF, _) 
                  , std::vector<saga::uuid>&), 
            FuncRetVal retval, BOOST_PP_ENUM_BINARY_PARAMS (O, FuncArg, func_arg),
            saga::uuid task_uuid
    )
    {
        typedef saga::adaptors::bulk_container<
                Cpi, FuncRetVal, BOOST_PP_ENUM_PARAMS (O, FuncArg)
            > bulk_container_type;

        if (NULL == bc) 
            bc = new bulk_container_type(cpi, exec_bulk, retval, 
                                         BOOST_PP_ENUM_PARAMS (O, func_arg), 
                                         task_uuid);
        else
            static_cast<bulk_container_type *>(bc)->add_params(retval, 
                                         BOOST_PP_ENUM_PARAMS (O, func_arg), 
                                         task_uuid);
    }
    

#define GET_MEMBER_VEC(z, M, _)  std::vector< FuncArg ## M > vec_ ## M ; 
#define GET_PARAM_VEC(z, M, _) BOOST_PP_COMMA_IF(M) std::vector< FuncArg ## M > 
#define GET_PUSHBACK_VEC(z, M, _)  vec_ ## M.push_back(func_arg ## M ); 
#define GET_DEFAULT_ARG(z, M, _) BOOST_PP_COMMA() phoenix::nil_t

    ///////////////////////////////////////////////////////////////////////////
    /**
      * class to store generic function parameters.
      *
      * every cpi object holds one instance of this class, to store
      * the bunch of parameters needed to execute a number of operations
      * in a bulk. Through the create_... function, all template details
      * are hidden from the adaptor programmer.
      *
      * preprocessor generated version for one and more parameters.
      */    
    template<typename Cpi, typename FuncRetVal, 
             BOOST_PP_ENUM_PARAMS (O, typename FuncArg)>
    class bulk_container<Cpi, FuncRetVal, 
            BOOST_PP_ENUM_PARAMS (O, FuncArg)
            BOOST_PP_REPEAT(BOOST_PP_INC(BOOST_PP_SUB(SAGA_ARGUMENT_LIMIT, O)), GET_DEFAULT_ARG, _)
    > 
    :   public saga::adaptors::bulk_container_base
    {
    private:
        std::vector<FuncRetVal> rets;
        std::vector<saga::uuid> uuids;
        BOOST_PP_REPEAT(O, GET_MEMBER_VEC, _)
        
        std::vector<saga::uuid> (Cpi::*exec_bulk)(std::vector<FuncRetVal>&, 
                                    BOOST_PP_REPEAT(O, GET_PARAM_VEC_REF, _) 
                                    , std::vector<saga::uuid>&);
        Cpi* my_adaptor;

    public:
        /**
          * constructor
          *
          * stores the bulk function to execute and the uuids.
          */
        bulk_container (Cpi * cpi_,
                        std::vector <saga::uuid> (Cpi::*exec_bulk_)
                                                (std::vector<FuncRetVal>&, 
                                                 BOOST_PP_REPEAT(O, GET_PARAM_VEC_REF, _) 
                                                 , std::vector<saga::uuid>&), 
                        FuncRetVal retval, 
                        BOOST_PP_ENUM_BINARY_PARAMS (O, FuncArg, func_arg),
                        saga::uuid task_uuid) 
            : exec_bulk  (exec_bulk_),
              my_adaptor (cpi_)
        {
            this->add_params (retval, BOOST_PP_ENUM_PARAMS (O, func_arg), 
                              task_uuid);
        }

        /**
          * default destructor
          */
        ~bulk_container(void) 
        {
        }
        
        /**
          * adds parameters to a existing container.
          */
        bulk_container *add_params(FuncRetVal retval, 
                        BOOST_PP_ENUM_BINARY_PARAMS (O, FuncArg, func_arg),
                        saga::uuid task_uuid)
        {
            rets.push_back(retval);
            uuids.push_back(task_uuid);
            BOOST_PP_REPEAT(O, GET_PUSHBACK_VEC, _)
            return this;
        }

        /**
          * executes the bulk function, and passes the appropriate set
          * of parameters (stored in std::vector's) to it.
          */
        std::vector<saga::uuid> execute_bulk() 
        {
            return ((my_adaptor->*exec_bulk)(rets, 
                                             BOOST_PP_ENUM_PARAMS (O, vec_) ,
                                             uuids));
        }

    }; // class bulk_container

#undef GET_DEFAULT_ARG
#undef GET_PARAM_VEC
#undef GET_MEMBER_VEC
#undef GET_PARAM_VEC
#undef GET_PUSHBACK_VEC
#undef O

#endif // defined(BOOST_PP_IS_ITERATING)

