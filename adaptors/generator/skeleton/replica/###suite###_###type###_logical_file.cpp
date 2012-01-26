//  Copyright (c) 2005-2008 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <map>
#include <vector>

#include <boost/assert.hpp>
#include <boost/tokenizer.hpp>

#include <saga/saga/util.hpp>
#include <saga/saga/exception.hpp>
#include <saga/saga/url.hpp>
#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/file.hpp>

#include <saga/impl/config.hpp>
#include <saga/impl/exception_list.hpp>

#include "###suite###_###type###_adaptor.hpp"
#include "###suite###_###type###_logical_file.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace ###suite###_###type###
{

  ///////////////////////////////////////////////////////////////////////////////
  //  constructor
  logical_file_cpi_impl::logical_file_cpi_impl (proxy                * p, 
                                                cpi_info       const & info,
                                                saga::ini::ini const & glob_ini, 
                                                saga::ini::ini const & adap_ini,
                                                TR1::shared_ptr <saga::adaptor> adaptor)
    : base_cpi (p, info, adaptor, cpi::Noflags)
  {
    SAGA_ADAPTOR_THROW("Not Implemented", saga::NotImplemented);
  }

  ///////////////////////////////////////////////////////////////////////////////
  //  destructor
  logical_file_cpi_impl::~logical_file_cpi_impl (void)
  {
    SAGA_ADAPTOR_THROW("Not Implemented", saga::NotImplemented);
  }

//  ///////////////////////////////////////////////////////////////////////////////
//  //  SAGA CPI functions 
//
//  ///////////////////////////////////////////////////////////////////////////////
//  // attribute functions
//  void logical_file_cpi_impl::sync_attribute_exists(bool & ret, std::string key)
//  {
//    SAGA_ADAPTOR_THROW("Not Implemented", saga::NotImplemented);
//  }
//
//  void logical_file_cpi_impl::sync_attribute_is_readonly(bool & ret, std::string key)
//  {
//    SAGA_ADAPTOR_THROW("Not Implemented", saga::NotImplemented);
//  }
//
//  void logical_file_cpi_impl::sync_attribute_is_writable(bool & ret, std::string key)
//  {
//    SAGA_ADAPTOR_THROW("Not Implemented", saga::NotImplemented);
//  }
//
//  ///////////////////////////////////////////////////////////////////////////////
//  void 
//    logical_file_cpi_impl::check_permissions(saga::replica::flags flags,
//                                             char const* name, std::string const & lfn)
//    {
//    }
//
//  void 
//    logical_file_cpi_impl::sync_attribute_is_vector(bool & ret, std::string key)
//    {
//    }
//
//  void 
//    logical_file_cpi_impl::sync_attribute_is_extended(bool & ret, std::string key)
//    {
//    }
//
//  ///////////////////////////////////////////////////////////////////////////////
//  void 
//    logical_file_cpi_impl::sync_get_attribute(std::string & ret, std::string key)
//    {
//    }
//
//  ///////////////////////////////////////////////////////////////////////////////
//  void 
//    logical_file_cpi_impl::sync_get_vector_attribute(std::vector<std::string> & ret, 
//                                                     std::string key)
//    {
//    }
//
//  ///////////////////////////////////////////////////////////////////////////////
//  void 
//    logical_file_cpi_impl::sync_set_attribute(saga::impl::void_t & ret, std::string key, 
//                                              std::string val)
//    {
//    }
//
//  ///////////////////////////////////////////////////////////////////////////////
//  void 
//    logical_file_cpi_impl::sync_set_vector_attribute(saga::impl::void_t & ret, std::string key, 
//                                                     std::vector<std::string> val)
//    {
//    }
//
//  ///////////////////////////////////////////////////////////////////////////////
//  void 
//    logical_file_cpi_impl::sync_remove_attribute(saga::impl::void_t & ret, std::string key)
//    {
//    }
//
//  ///////////////////////////////////////////////////////////////////////////////
//  void 
//    logical_file_cpi_impl::sync_list_attributes(std::vector<std::string> & keys)
//    {
//    }
//
//  ///////////////////////////////////////////////////////////////////////////////
//  void 
//    logical_file_cpi_impl::sync_find_attributes(std::vector<std::string> & keys, 
//                                                std::string pattern)
//    {
//    }
//
//  ///////////////////////////////////////////////////////////////////////////////
//  // namespace_entry functions
//  void logical_file_cpi_impl::sync_get_url(saga::url & url)
//  {
//    SAGA_ADAPTOR_THROW("Not Implemented", saga::NotImplemented);
//  }
//
//  void logical_file_cpi_impl::sync_get_cwd(saga::url & url)
//  {
//    SAGA_ADAPTOR_THROW("Not Implemented", saga::NotImplemented);
//  }
//
//  void logical_file_cpi_impl::sync_get_name(saga::url & url)
//  {
//    SAGA_ADAPTOR_THROW("Not Implemented", saga::NotImplemented);
//  }
//
//  void logical_file_cpi_impl::sync_is_dir(bool & ret)
//  {
//    SAGA_ADAPTOR_THROW("Not Implemented", saga::NotImplemented);
//  }
//
//  void logical_file_cpi_impl::sync_is_entry(bool & ret)
//  {
//    SAGA_ADAPTOR_THROW("Not Implemented", saga::NotImplemented);
//  }
//
//  void logical_file_cpi_impl::sync_is_link(bool & ret)
//  {
//    SAGA_ADAPTOR_THROW("Not Implemented", saga::NotImplemented);
//  }
//
//  void logical_file_cpi_impl::sync_remove(saga::impl::void_t & ret, int flags)
//  {
//    SAGA_ADAPTOR_THROW("Not Implemented", saga::NotImplemented);
//  }
//
//  void logical_file_cpi_impl::sync_close(saga::impl::void_t & ret, double)
//  {
//    SAGA_ADAPTOR_THROW("Not Implemented", saga::NotImplemented);
//  }
//
//
//  ///////////////////////////////////////////////////////////////////////////////
//  // logical_file functions
//  void logical_file_cpi_impl::sync_list_locations(std::vector<saga::url> & locations)
//  {
//    SAGA_ADAPTOR_THROW("Not Implemented", saga::NotImplemented);
//  }
//
//  void logical_file_cpi_impl::sync_add_location(saga::impl::void_t & ret, saga::url location)
//  {
//    SAGA_ADAPTOR_THROW("Not Implemented", saga::NotImplemented);
//  }
//
//  void logical_file_cpi_impl::sync_remove_location(saga::impl::void_t & ret, saga::url location)
//  {
//    SAGA_ADAPTOR_THROW("Not Implemented", saga::NotImplemented);
//  }
//
//  void logical_file_cpi_impl::sync_update_location(saga::impl::void_t & ret, 
//                                                   saga::url oldlocation, saga::url newlocation)
//  {
//    SAGA_ADAPTOR_THROW("Not Implemented", saga::NotImplemented);
//  }
//
//  void logical_file_cpi_impl::sync_replicate(saga::impl::void_t & ret, saga::url location, 
//                                             int mode)
//  {
//    SAGA_ADAPTOR_THROW("Not Implemented", saga::NotImplemented);
//  }

}   // namespace ###suite###_###type###
///////////////////////////////////////////////////////////////////////////////

