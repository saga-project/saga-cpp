//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005 Michel Zandstra (michelzandstra@gmail.com)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_ENGINE_ADAPTOR_SELECTOR_HPP
#define SAGA_IMPL_ENGINE_ADAPTOR_SELECTOR_HPP

#include <vector>
#include <iostream>

#include <saga/impl/config.hpp>

#include <saga/impl/engine/cpi.hpp>
#include <saga/impl/engine/cpi_info.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl {

  /////////////////////////////////////////////////////////////////////////////
  class adaptor_selector
  {
    private:
      typedef v1_0::preference_type preference_type;
      typedef v1_0::cpi_info cpi_info;
      typedef v1_0::op_info op_info;

    protected:
      SAGA_EXPORT static bool 
      match_adaptor (cpi_info        const & info, 
                     std::string     const & cpi_name, 
                     std::string     const & op_name, 
                     preference_type const & prefs, 
                     op_info               & oi);

    public:
      typedef std::vector <v1_0::cpi_info> adaptor_info_list_type;

      // functions to select a suitable adaptor 
      SAGA_EXPORT static cpi_info const &
      select_adaptor (adaptor_info_list_type const & adaptors, 
                      adaptor_info_list_type const & no_adaptors, 
                      std::string            const & cpi_name,
                      std::string            const & ops_cpi_name, 
                      std::string            const & op_name,
                      preference_type        const & prefs, 
                      op_info                      & oi);

      // function to test, whether a given adaptor matches the expectations
      SAGA_EXPORT static bool 
      test_adaptor (adaptor_info_list_type const & adaptors, 
                    v1_0::cpi              const * cpi_instance,
                    const std::string            & cpi_name, 
                    const std::string            & op_name,
                    const preference_type        & pref, 
                    op_info                      & oi);
  };

///////////////////////////////////////////////////////////////////////////////
}} // namespace saga::impl

#endif // SAGA_IMPL_ENGINE_ADAPTOR_SELECTOR_HPP

