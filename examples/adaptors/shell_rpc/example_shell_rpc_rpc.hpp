
// INSERT YOUR LICENSE BOILER PLATE HERE

#ifndef ADAPTORS_EXAMPLE_SHELL_RPC_RPC_ADAPTOR_HPP
#define ADAPTORS_EXAMPLE_SHELL_RPC_RPC_ADAPTOR_HPP

#include <saga/saga.hpp>
#include <saga/impl/rpc.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace example_shell_rpc
{

/**
 * This adaptor implements the functionality of the Saga API "rpc".
 * It defines the functions declared in its base class, rpc_cpi.
 */
  class rpc_cpi_impl 
      : public saga::adaptors::v1_0::rpc_cpi <rpc_cpi_impl>
  {
    private:
      typedef saga::adaptors::v1_0::rpc_cpi <rpc_cpi_impl> base_cpi;

      bool        closed_;
      saga::url   u_;
      saga::url   rm_;
      std::string exe_;
      std::string host_;

      saga::job::service js_;


    public:
      /*! constructor of the rpc adaptor */
      rpc_cpi_impl (proxy * p, 
                                 cpi_info       const & info, 
                                 saga::ini::ini const & glob_ini, 
                                 saga::ini::ini const & adap_ini,
                                 TR1::shared_ptr <saga::adaptor> adaptor);

      /*! destructor of the rpc adaptor */
      ~rpc_cpi_impl  (void);

      /*! implementation of SAGA::rpc functions */
      void sync_call  (saga::impl::void_t &, std::vector <saga::rpc::parameter> & args);
      void sync_close (saga::impl::void_t &, double timeout);
  }; 

} // namespace example_shell_rpc
///////////////////////////////////////////////////////////////////////////////

#endif // ADAPTORS_EXAMPLE_SHELL_RPC_RPC_ADAPTOR_HPP

