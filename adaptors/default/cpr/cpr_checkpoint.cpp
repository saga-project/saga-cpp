//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <map>
#include <vector>

#include <boost/assert.hpp>
#include <boost/tokenizer.hpp>

#include <saga/saga.hpp>
#include <saga/saga/adaptors/task.hpp>

#include <saga/impl/config.hpp>

#include "config.hpp"
#include "cpr_checkpoint.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace cpr
{

  ///////////////////////////////////////////////////////////////////////////////
  //  constructor
  cpr_checkpoint_cpi_impl::cpr_checkpoint_cpi_impl (proxy* p, 
                                                    cpi_info       const & info,
                                                    saga::ini::ini const & glob_ini, 
                                                    saga::ini::ini const & adap_ini,
                                                    TR1::shared_ptr<saga::adaptor> adaptor)
    :   base_cpi (p, info, adaptor, cpi::Noflags),
    thrd_          (NULL), 
    cond_          (NULL),
    thread_alive_  (false), 
    cancel_thread_ (false)
  {
    // first usage of this adaptor
    instance_data data (this);
    saga::url cpr_url (data->location_);


    // translate 'any' and 'cpr' url schemes to lfn, decline all others
    std::string scheme (cpr_url.get_scheme ());

    if ( ! scheme.empty () && 
           scheme != "cpr" &&
           scheme != "any")
    {
      SAGA_ADAPTOR_THROW (std::string ("cpr::cpr_checkpoint_cpi_impl::init: "
                                       "cannot handle checkpoint name: ") + cpr_url.get_url (), 
                          saga::adaptors::AdaptorDeclined);
    }

    cpr_url.set_scheme ("lfn");


    // create a advert, wich acts as container for the checkpoint files
    ae_ = new saga::advert::entry (cpr_url);

    // FIXME: set some initial meta data (index, ...)

    // FIXME: check how monitoring is forwarded!
  }

  ///////////////////////////////////////////////////////////////////////////////
  void cpr_checkpoint_cpi_impl::check_lf (void)
  {
    if ( ae_ == NULL )
    {
      SAGA_ADAPTOR_THROW ("cpr::cpr_checkpoint_cpi_impl instance is not"
                          "initialized.", saga::IncorrectState);
    }
  }

  ///////////////////////////////////////////////////////////////////////////////
  //  destructor
  cpr_checkpoint_cpi_impl::~cpr_checkpoint_cpi_impl (void)
  {
    if ( ae_ != NULL )
    {
      delete (ae_);
    }
  }

  ///////////////////////////////////////////////////////////////////////////////
  //  SAGA CPI functions 

  ///////////////////////////////////////////////////////////////////////////////
  // attribute functions
  void cpr_checkpoint_cpi_impl::sync_attribute_exists (bool      & ret, 
                                                       std::string key)
  {
    check_lf ();
    ret = ae_->attribute_exists (key);
  }

  void cpr_checkpoint_cpi_impl::sync_attribute_is_readonly (bool      & ret, 
                                                            std::string key)
  {
    check_lf ();
    ret = ae_->attribute_is_readonly (key);
  }

  void cpr_checkpoint_cpi_impl::sync_attribute_is_writable (bool      & ret, 
                                                            std::string key)
  {
    check_lf ();
    ret = ae_->attribute_is_writable (key);
  }

  void cpr_checkpoint_cpi_impl::sync_attribute_is_vector (bool      & ret, 
                                                          std::string key)
  {
    check_lf ();
    ret = ae_->attribute_is_vector (key);
  }

  void cpr_checkpoint_cpi_impl::sync_attribute_is_extended (bool      & ret, 
                                                            std::string key)
  {
    check_lf ();
    // FIXME: that is a private method?!  Do we need in API?
    // ret = ae_->attribute_is_extended (key);
  }

  ///////////////////////////////////////////////////////////////////////////////
  void cpr_checkpoint_cpi_impl::sync_get_attribute (std::string & ret, 
                                                    std::string   key)
  {
    check_lf ();
    ret = ae_->get_attribute (key);
  }

  ///////////////////////////////////////////////////////////////////////////////
  void cpr_checkpoint_cpi_impl::sync_get_vector_attribute (std::vector <std::string> & ret, 
                                                           std::string                 key)
  {
    check_lf ();
    ret = ae_->get_vector_attribute (key);
  }

  ///////////////////////////////////////////////////////////////////////////////
  void cpr_checkpoint_cpi_impl::sync_set_attribute (saga::impl::void_t &,
                                                    std::string key, 
                                                    std::string val)
  {
    check_lf ();
    ae_->set_attribute (key, val);
  }

  ///////////////////////////////////////////////////////////////////////////////
  void cpr_checkpoint_cpi_impl::sync_set_vector_attribute (saga::impl::void_t &, 
                                                           std::string              key, 
                                                           std::vector <std::string> val)
  {
    check_lf ();
    ae_->set_vector_attribute (key, val);
  }

  ///////////////////////////////////////////////////////////////////////////////
  void cpr_checkpoint_cpi_impl::sync_remove_attribute (saga::impl::void_t &, 
                                                       std::string key)
  {
    check_lf ();
    ae_->remove_attribute (key);
  }

  ///////////////////////////////////////////////////////////////////////////////
  void cpr_checkpoint_cpi_impl::sync_list_attributes (std::vector <std::string> & keys)
  {
    check_lf ();
    keys = ae_->list_attributes ();
  }

  ///////////////////////////////////////////////////////////////////////////////
  void cpr_checkpoint_cpi_impl::sync_find_attributes (std::vector <std::string> & keys, 
                                                      std::string                 pattern)
  {
    check_lf ();
    keys = ae_->find_attributes (pattern);
  }


  ///////////////////////////////////////////////////////////////////////////////
  // namespace_entry functions
  void cpr_checkpoint_cpi_impl::sync_get_url (saga::url & url)
  {
    check_lf ();
    url = ae_->get_url ();
  }

  void cpr_checkpoint_cpi_impl::sync_get_cwd (saga::url & url)
  {
    check_lf ();
    url = ae_->get_cwd ();
  }

  void cpr_checkpoint_cpi_impl::sync_get_name (saga::url & url)
  {
    check_lf ();
    url = ae_->get_name ();
  }

  void cpr_checkpoint_cpi_impl::sync_is_dir (bool & ret)
  {
    check_lf ();
    ret = ae_->is_dir ();
  }

  void cpr_checkpoint_cpi_impl::sync_is_entry (bool & ret)
  {
    check_lf ();
    ret = ae_->is_entry ();
  }

  void cpr_checkpoint_cpi_impl::sync_is_link (bool & ret)
  {
    check_lf ();
    ret = ae_->is_link ();
  }

  void cpr_checkpoint_cpi_impl::sync_remove (saga::impl::void_t &, 
                                             int flags)
  {
    check_lf ();
    ae_->remove (flags);
  }

  void cpr_checkpoint_cpi_impl::sync_close (saga::impl::void_t &,
                                            double timeout)
  {
    check_lf ();
    ae_->close (timeout);
  }


  ///////////////////////////////////////////////////////////////////////////////
  // cpr_checkpoint functions
  void cpr_checkpoint_cpi_impl::sync_get_parent (saga::url & ret,
                                                 int         idx)
  {
    saga::url parent;

    if ( idx > 1 )
    {
      // get parent
      check_lf ();
      parent = ae_->get_attribute ("SAGA_CPR_PARENT");
      idx--;
    }


    while ( idx > 1 )
    {
      // get grand...-parent
      saga::cpr::checkpoint cp (parent);
      parent = cp.get_parent ();
      idx--;
    }

    ret = parent;
  }

  void cpr_checkpoint_cpi_impl::sync_get_file (saga::url & ret,
                                               int idx)
  {
    check_lf ();
    std::vector <std::string> listing = ae_->get_vector_attribute ("locations");
    
    // FIXME: range check
    
    ret = listing[idx];
  }

  void cpr_checkpoint_cpi_impl::sync_open_file_idx (saga::filesystem::file & ret,
                                                int idx, int flags)
  {
    check_lf ();
    std::vector <std::string> listing = ae_->get_vector_attribute ("locations");
    
    // FIXME: range check

    saga::url u (listing[idx]);
    saga::filesystem::file f (u);

    ret = f;
  }

  ///////////////////////////////////////////////////////////////////////////////
}   // namespace cpr

