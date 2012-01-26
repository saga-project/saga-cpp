//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005 Michel Zandstra (michelzandstra@gmail.com)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  job LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_ENGINE_PROXY_HPP
#define SAGA_IMPL_ENGINE_PROXY_HPP

#include <map>

#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>

#include <saga/saga/uuid.hpp>
#include <saga/impl/runtime.hpp>

// include CPI's
#include <saga/impl/config.hpp>
#include <saga/impl/engine/cpi.hpp>
#include <saga/impl/engine/engine.hpp>
#include <saga/impl/engine/session.hpp>
#include <saga/impl/engine/cpi_list.hpp>
#include <saga/impl/exception_list.hpp>

#include <saga/saga/adaptors/instance_data_base.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace impl {

    enum run_mode
    {
        Unknown     = -1,
        Sync_Sync   =  0,
        Sync_Async  =  1,
        Async_Sync  =  2,
        Async_Async =  3
    };

  /**
   * The base class of all proxy classes
   *
   * Contains functions to handle the shared adaptor data objects,
   * and a pointer to the session handle
   */
  class proxy 
    : public saga::impl::object
  {
    protected:
      typedef v1_0::preference_type preference_type;
      typedef v1_0::cpi cpi;

      typedef TR1::shared_ptr<v1_0::cpi> cpi_type;

    private:
      saga::session session_;

    public:
      typedef boost::recursive_mutex mutex_type;
      cpi_list cpis_;

    protected:
      // instance data 
      mutex_type instance_data_mtx_;
      TR1::shared_ptr<adaptors::instance_data_base> instance_data_;

    public:
      /** constructor */
      explicit proxy (saga::object::type type, saga::session const& session) 
      :   object(type), session_(session)
      { 
      }

      /** destructor */
      virtual ~proxy (void) 
      {
          release_cpis();
      }

      void release_cpis()
      {
          mutex_type::scoped_lock lock(mtx_);

          // Clear dangling pointers. We're relinquishing ownership of the cpi.
          cpi_list::iterator end = cpis_.end();

          for (cpi_list::iterator it = cpis_.begin(); it != end; ++it)
          {
              (*it)->proxy_ = 0;
          }

          cpis_.clear();
      }

      saga::session& get_session() { return session_; }
      saga::session const& get_session() const { return session_; }

      std::string get_cpi_name (void) const
      {
        return saga::adaptors::get_cpi_name(this->get_type());
      }

      /** 
       * Asks Saga engine for a new adaptor of type CpiType, that
       * matches the description table. If file was already bound to an
       * adaptor, it is destroyed first. UNLESS this adaptor has fixed
       * itself. In that case, instead of creating a new adaptor, file
       * simply uses this fixed one.
       *
       * @param table a description table describing the adaptor needed
       */
      SAGA_EXPORT bool get_adaptor (std::string     const & cpi_name,
                                    std::string     const & op_name, 
                                    preference_type const & prefs,
                                    adaptor_selector::adaptor_info_list_type & no_no_list, 
                                    v1_0::op_info         & oi);

      /** 
       * Asks the Saga engine, whether the given adaptor supports a certain
       * operation.
       */
      SAGA_EXPORT bool test_adaptor (cpi_type                cpi, 
                                     std::string     const & cpi_name, 
                                     std::string     const & op_name, 
                                     preference_type const & prefs, 
                                     v1_0::op_info         & oi);

      // find out, which execution type a given adaptor prefers to be used
      SAGA_EXPORT run_mode test_adaptor_helper (cpi_type            cpi, 
                                                std::string     const & cpi_name, 
                                                std::string     const & call_name, 
                                                preference_type const & prefs, 
                                                bool                    is_sync, 
                                                v1_0::op_info         & oi);

      /////////////////////////////////////////////////////////////////////////
      //  Helper function to be called from inside the implementation object's 
      //  constructor. It creates and initializes a new CPI instance
      SAGA_EXPORT bool initcpi (std::string  const& cpi_name);

      // create a new CPI instance, and figure out what execution preferences
      // this CPI has for the given function.
      //
      // This function gets called for every CPI function to be called, 
      // whenever the CPI was newly created for this CPI call.
      SAGA_EXPORT run_mode get_adaptor_helper (std::string     const & cpi_name, 
                                               std::string     const & call_name, 
                                               preference_type const & prefs, 
                                               bool                    is_sync, 
                                               adaptor_selector::adaptor_info_list_type & no_no_list, 
                                               v1_0::op_info         & oi);

      // create a new CPI instance, and figure out what execution preferences
      // this CPI has for the given function
      SAGA_EXPORT run_mode get_adaptor_for_init (std::string     const & cpi_name, 
                                                 preference_type const & prefs, 
                                                 adaptor_selector::adaptor_info_list_type & no_no_list, 
                                                 v1_0::op_info         & oi);

      /////////////////////////////////////////////////////////////////////////
      //  This is the main 'API' function for adaptor selection and 
      //  preparation.
      //
      //  select_run_mode() is called for every API function to bind an 
      //  appropriate cpi instance claiming to be able to handle the request.
      SAGA_EXPORT run_mode select_run_mode (std::string     const & cpi_name, 
                                            std::string     const & call_name, 
                                            preference_type const & prefs, 
                                            bool                    is_sync, 
                                            adaptor_selector::adaptor_info_list_type & no_no_list,
                                            v1_0::op_info         & oi);

      /////////////////////////////////////////////////////////////////////////
      //  This returns the type of the first attached CPI instance object
      SAGA_EXPORT saga::object::type get_cpitype (void) const;
      
      /////////////////////////////////////////////////////////////////////////
      // handle API object instance data
      SAGA_EXPORT TR1::shared_ptr<adaptors::instance_data_base> 
          register_instance_data (TR1::shared_ptr <adaptors::instance_data_base> o);

      SAGA_EXPORT void release_instance_data (void);

      SAGA_EXPORT TR1::shared_ptr<adaptors::instance_data_base>
          check_out_instance_data (void);

      SAGA_EXPORT void check_in_instance_data();

      /////////////////////////////////////////////////////////////////////////
      //  helper for enable_shared_from_this support
      SAGA_EXPORT TR1::shared_ptr<proxy      > shared_from_this();
      SAGA_EXPORT TR1::shared_ptr<proxy const> shared_from_this() const;

  }; // class proxy

  /////////////////////////////////////////////////////////////////////////////
}} // namespace saga::impl

#include <saga/impl/engine/run_mode_wrapper.hpp>

#endif // SAGA_IMPL_ENGINE_PROXY_HPP

