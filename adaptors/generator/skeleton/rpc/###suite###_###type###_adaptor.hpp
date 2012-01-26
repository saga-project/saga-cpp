
// INSERT YOUR LICENSE BOILER PLATE HERE

#ifndef ADAPTORS_###SUITE###_###TYPE###_ADAPTOR_HPP
#define ADAPTORS_###SUITE###_###TYPE###_ADAPTOR_HPP

#include <saga/saga/adaptors/adaptor.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace ###suite###_###type###
{
  struct rpc_adaptor : public saga::adaptor
  {
    typedef saga::impl::v1_0::cpi_info        cpi_info;
    typedef saga::impl::v1_0::preference_type preference_type;

    /**
     * This functions registers the adaptor with the factory
     *
     * @param factory the factory where the adaptor registers
     *        its maker function and description table
     */
    saga::impl::adaptor_selector::adaptor_info_list_type 
        adaptor_register (saga::impl::session * s);

    std::string get_name (void) const
    { 
      return BOOST_PP_STRINGIZE (SAGA_ADAPTOR_NAME);
    }
  };

}   // namespace ###suite###_###type###
///////////////////////////////////////////////////////////////////////////////

#endif // ADAPTORS_###SUITE###_###TYPE###_ADAPTOR_HPP

