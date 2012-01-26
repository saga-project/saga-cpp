
// INSERT YOUR LICENSE BOILER PLATE HERE

#ifndef ADAPTORS_###SUITE###_###TYPE###_RPC_ADAPTOR_HPP
#define ADAPTORS_###SUITE###_###TYPE###_RPC_ADAPTOR_HPP

#include <saga/saga.hpp>
#include <saga/impl/rpc.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace ###suite###_###type###
{

/**
 * This adaptor implements the functionality of the Saga API "rpc".
 * It defines the functions declared in its base class, rpc_cpi.
 */
  class rpc_cpi_impl 
      : public saga::adaptors::v1_0::rpc_cpi <rpc_cpi_impl>
  {
    private:
      typedef saga::adaptors::v1_0::rpc_cpi<rpc_cpi_impl> base_cpi;

    public:
      /*! constructor of the rpc adaptor */
      rpc_cpi_impl (proxy * p, 
                                 cpi_info       const & info, 
                                 saga::ini::ini const & glob_ini, 
                                 saga::ini::ini const & adap_ini,
                                 TR1::shared_ptr <saga::adaptor> adaptor);

      /*! destructor of the rpc adaptor */
      ~rpc_cpi_impl  (void);

//       /*! implementation of SAGA::rpc functions */
//       void sync_call  (saga::impl::void_t & ret, std::vector<saga::rpc::parameter> & args);
//       void sync_close (saga::impl::void_t & ret, double timeout);
// 
//       saga::task async_call  (std::vector <saga::rpc::parameter> & args);
//       saga::task async_close (double timeout);

  }; 

} // namespace ###suite###_###type###
///////////////////////////////////////////////////////////////////////////////

#endif // ADAPTORS_###SUITE###_###TYPE###_RPC_ADAPTOR_HPP

