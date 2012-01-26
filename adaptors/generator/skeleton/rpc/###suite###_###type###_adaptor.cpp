
// INSERT YOUR LICENSE BOILER PLATE HERE

#include <saga/saga/adaptors/adaptor.hpp>

#include "###suite###_###type###_adaptor.hpp"
#include "###suite###_###type###_rpc.hpp"

SAGA_ADAPTOR_REGISTER (###suite###_###type###::rpc_adaptor);

///////////////////////////////////////////////////////////////////////////////
namespace ###suite###_###type###
{
  // register function for the SAGA engine
  saga::impl::adaptor_selector::adaptor_info_list_type 
      rpc_adaptor::adaptor_register (saga::impl::session * s)
  {
    // list of implemented cpi's
    saga::impl::adaptor_selector::adaptor_info_list_type list;

    // create preferences
    preference_type prefs; 

    // create rpc cpi infos (each adaptor instance gets its own uuid) 
    // and add cpi_infos to list
    rpc_cpi_impl::register_cpi (list, prefs, adaptor_uuid_);

    // and return list
    return list;
  }

} // namespace ###suite###_###type###
///////////////////////////////////////////////////////////////////////////////

