//  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
//  Copyright (c) 2005-2009 Hartmut Kaiser
//  Copyright (c) 2005 Michel Zandstra (michelzandstra@gmail.com)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef SAGA_IMPL_ENGINE_ENGINE_HPP
#define SAGA_IMPL_ENGINE_ENGINE_HPP

#include <vector>
#include <iostream>
#include <map>
#include <set>

#include <boost/plugin/dll.hpp>
#include <boost/plugin/virtual_constructors.hpp>
#include <boost/noncopyable.hpp>

// filesystem will be in TR2
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>

#include <saga/saga/impl_base.hpp>

#include <saga/impl/config.hpp>
#include <saga/impl/engine/cpi.hpp>
#include <saga/impl/engine/cpi_info.hpp>
#include <saga/impl/engine/adaptor_selector.hpp>

#include <saga/saga/adaptors/adaptor.hpp>

namespace saga
{
  namespace impl
  {
    /**
     * This class makes the various Saga adaptors available to Saga
     */
    class engine : private boost::noncopyable
    {
      // FIXME: add const-ness to the registry hierarchy!

     private:
      typedef std::vector<boost::plugin::dll> module_list_type;

      struct adaptor_instance_data_type
      {
          TR1::shared_ptr<saga::adaptor> adaptor;
          std::string adaptor_name;
          std::string instance_name;
      };

      typedef std::map <saga::uuid, adaptor_instance_data_type>
                                    adaptor_instance_map_type;
      typedef v1_0::preference_type preference_type;
      typedef v1_0::cpi_info        cpi_info;
      typedef v1_0::cpi             cpi;

      // keep infos from ini files
      saga::ini::ini ini;

      /** The global adaptor registry used to create adaptors
       *
       * This factory holds, for each cpi interface_, a list of adaptors (maker
       * function, description table) that implement this class.  See definitions
       * above
       */
      adaptor_selector::adaptor_info_list_type adaptor_infos_; 
      module_list_type                         modules_;     // loaded shared libraries
      adaptor_instance_map_type                adaptors_;    // loaded adaptor instances
      std::set<std::string> modules_to_log_;   // list of modules to generate logging from

      // a selector for finding matching adaptors
      adaptor_selector selector_;

      // tells, whether late_initialize has been called or not
      // CAUTION: the caller has to ensure thread safety of the engine
      bool is_initialized;

      bool is_adaptor_loaded (std::string const& instance_name,
                              std::string const& adaptor_name) const;

      bool load_adaptor (saga::impl::session *current_session, 
                         std::string instance_name, 
                         std::string adaptor_name, 
                         boost::filesystem::path const& lib, 
                         bool load_globally, std::string const& unmangled_name);

      bool load_static_adaptor (saga::impl::session *current_session, 
                                std::string instance_name, 
                                std::string adaptor_name,
                                boost::plugin::get_plugins_list_type get_factory);

      void load_static_adaptors(saga::ini::section_map const& s,
                                saga::impl::session *current_session);

      void load_dynamic_adaptors(saga::ini::section_map const& s,
                                 saga::impl::session *current_session, 
                                 std::string saga_location);

     public:

      /** Constructor of engine, loads all adaptors */
      engine ();

      /** Destructor of engine */
      ~engine (void);

      // engine now gets initialized lazily, i.e. as late as possible (on first 
      // access)
      SAGA_EXPORT void late_initialize(saga::impl::session const *);
      bool has_been_initialized() const { return is_initialized; }

     protected:
      // load all matching shared libraries 
      // throws   saga::exception
      void load (saga::impl::session *);

      /**
       * Reads all shared library names in the file "adaptorlist", and tries to
       * open the defined shared libraries. It uses Boost.Plugin to handle the shared
       * libraries. If it can open a library, it invokes the loader function of
       * this library to register it's maker function and description table with
       * the factory.
       *
       * @see ltdl
       *
       * @throws   saga::exception is thrown if file "adaptorlist" could not be
       *           opened, or does not exist
       */
      void init (void);

      // add this adaptor to the list of modules from which to generate logs, if
      // appropriate
      void register_adaptor_logging(std::string const& adaptor_name);

     public:
      // free all allocated resources
      void tidy (void);

      /**
       * Creates an adaptor object implementing the interface
       *
       * @param cpi_name   a string specifying the interface for which an object 
       *                   is needed
       * @param op_name    a description method wanted from the adaptor
       * @param prefs      a description table describing the adaptor needed
       *
       * @return a pointer of type T* to the created object, or NULL of no suitable 
       *         adaptor could be found
       *
       * @see saga::adaptor_selector::select_adaptor
       */
      SAGA_EXPORT v1_0::cpi* 
      get_adaptor (std::string const& ops_cpi_name, 
                   std::string const& op_name, 
                   preference_type const& prefs, 
                   adaptor_selector::adaptor_info_list_type& no_adaptor_infos,
                   proxy* proxy_, 
                   v1_0::op_info& oi) const;

      /**
       * Tests, whether a given adaptor object implements the required interface
       *
       * @param adaptor    a pointer to the adaptor to test
       * @param cpi_name   a string specifying the interface for which an object 
       *                   is needed
       * @param op_name    a description method wanted from the adaptor
       * @param prefs      a description table describing the adaptor needed
       *
       * @return a pointer of type T* to the created object, or NULL of no suitable 
       *         adaptor could be found
       *
       * @see saga::adaptor_selector::select_adaptor
       */
      SAGA_EXPORT bool test_adaptor (v1_0::cpi const* adaptor,
                                     std::string const& cpi_name, 
                                     std::string const& op_name,
                                     preference_type const& prefs, 
                                     v1_0::op_info& oi) const;

      // return the instance name of the adaptor with the given adaptor id
      std::string get_adaptor_name(saga::uuid id) const
      {
          adaptor_instance_map_type::const_iterator it = adaptors_.find(id);

          if (it != adaptors_.end())
              return (*it).second.instance_name;
              
          SAGA_THROW("saga::engine::get_adaptor: could not retrieve adaptor name "
              "for: " + id.string(), saga::NoSuccess);
          
          return "";
      }

      /////////////////////////////////////////////////////////////////////////
      // logging support
      bool module_needs_logging(char const* modulename) const;

    }; // class engine

  } // namespace impl

} // namespace saga

#endif // SAGA_IMPL_ENGINE_ENGINE_HPP

