
// INSERT YOUR LICENSE BOILER PLATE HERE

#include <saga/saga.hpp>
#include <saga/saga/adaptors/task.hpp>

#include "###suite###_###type###_rpc.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace ###suite###_###type###
{
  /*
   * constructor
   */
  rpc_cpi_impl::rpc_cpi_impl (proxy * p, 
                              cpi_info       const & info, 
                              saga::ini::ini const & glob_ini, 
                              saga::ini::ini const & adap_ini,
                              TR1::shared_ptr <saga::adaptor> adaptor)
    :   base_cpi (p, info, adaptor, cpi::Noflags)
  {
    // first usage of this adaptor
    instance_data data (this);
    saga::url url (data->funcname_);

    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
  }

  /*
   * destructor
   */
  rpc_cpi_impl::~rpc_cpi_impl (void)
  {
  }

//   /*
//    * SAGA API functions
//    */
//   void rpc_cpi_impl::sync_call (saga::impl::void_t & ret, 
//                                 std::vector <saga::rpc::parameter> & args)
//   {
//   }
// 
//   void rpc_cpi_impl::sync_close (saga::impl::void_t & ret, 
//                                  double timeout)
//   {
//   }
// 
// 
// 
//   saga::task rpc_cpi_impl::async_call (std::vector <saga::rpc::parameter> & args)
//   {
//     return saga::adaptors::task ("rpc_cpi_impl::async_call",
//                                  shared_from_this (),
//                                  &rpc_cpi_impl::sync_call, 
//                                  args);
// 
//   }
// 
//   saga::task rpc_cpi_impl::async_close (double timeout)
//   {
//     return saga::adaptors::task ("rpc_cpi_impl::async_close",
//                                  shared_from_this (),
//                                  &rpc_cpi_impl::sync_close, 
//                                  timeout);
//   }

}   // namespace ###suite###_###type###
///////////////////////////////////////////////////////////////////////////////

